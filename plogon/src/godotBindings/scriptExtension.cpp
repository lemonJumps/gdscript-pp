
#include "scriptExtension.hpp"
#include <godot_cpp/godot.hpp>

using namespace godot;

bool GDscriptpp::_editor_can_reload_from_file()
{
    return true;
}

// void _placeholder_erased(void *p_placeholder)
// {

// }

// bool _can_instantiate()
// {

// }

godot::Ref<godot::Script> GDscriptpp::_get_base_script() const
{
    return base;
}

/**
 * @brief get name of the script?
 * 
 * @return godot::StringName 
 */
godot::StringName GDscriptpp::_get_global_name() const
{
    return name;
}

/**
 * @brief go trough script hierarchy and check if any script in p_script inherits from this
 * 
 * @param p_script 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_inherits_script(const godot::Ref<godot::Script> &p_script) const
{
    Ref<GDscriptpp> script = p_script;
    if (script.is_null())
        return false; // script is null

    const GDscriptpp *s = this;

    // repeat until s is invalid
    while (s) {
        if (s == script.ptr())
            return true;

        // go to parent
        s = s->base.ptr();
    }

    return false; // no relation
}

godot::StringName GDscriptpp::_get_instance_base_type() const
{
    /// @todo if native class exists use that instead,
    // if (native.is_valid()) {
    //     return native->get_name();
    // }
    if (base.is_valid() && base->is_valid()) {
        return base->get_instance_base_type();
    }
    return StringName();
}

void * GDscriptpp::_instance_create(godot::Object *p_for_object) const
{
 	LuauScriptInstance *internal = memnew(LuauScriptInstance(Ref<Script>(this), p_for_object, type));
	return internal::gdextension_interface_script_instance_create3(&LuauScriptInstance::INSTANCE_INFO, internal);
}

void * GDscriptpp::_placeholder_instance_create(godot::Object *p_for_object) const
{
#ifdef TOOLS_ENABLED
    PlaceHolderScriptInstance *si = memnew(PlaceHolderScriptInstance(GDScriptLanguage::get_singleton(), Ref<Script>(this), p_this));
    placeholders.insert(si);
    _update_exports(nullptr, false, si);
    return si;
#else
    return nullptr;
#endif
}

bool GDscriptpp::_instance_has(godot::Object *p_object) const
{
    MutexLock lock(GDScriptLanguage::singleton->mutex);

    return instances.has((Object *)p_this);
}

bool GDscriptpp::_has_source_code() const
{
    return !source.is_empty();
}

godot::String GDscriptpp::_get_source_code() const
{
    return source;
}

void GDscriptpp::_set_source_code(const godot::String &p_code)
{
    if (source == p_code) {
        return;
    }
    source = p_code;
#ifdef TOOLS_ENABLED
    source_changed_cache = true;
#endif
}

godot::Error GDscriptpp::_reload(bool p_keep_state)
{
    if (reloading) {
        return OK;
    }
    reloading = true;

    bool has_instances;
    {
        MutexLock lock(GDScriptLanguage::singleton->mutex);

        has_instances = instances.size();
    }

    ERR_FAIL_COND_V(!p_keep_state && has_instances, ERR_ALREADY_IN_USE);

    String basedir = path;

    if (basedir.is_empty()) {
        basedir = get_path();
    }

    if (!basedir.is_empty()) {
        basedir = basedir.get_base_dir();
    }

    // Loading a template, don't parse.
#ifdef TOOLS_ENABLED
    if (EditorPaths::get_singleton() && basedir.begins_with(EditorPaths::get_singleton()->get_project_script_templates_dir())) {
        reloading = false;
        return OK;
    }
#endif

    {
        String source_path = path;
        if (source_path.is_empty()) {
            source_path = get_path();
        }
        Ref<GDScript> cached_script = GDScriptCache::get_cached_script(source_path);
        if (!source_path.is_empty() && cached_script.is_null()) {
            MutexLock lock(GDScriptCache::singleton->mutex);
            GDScriptCache::singleton->shallow_gdscript_cache[source_path] = Ref<GDScript>(this);
        }
    }

    bool can_run = ScriptServer::is_scripting_enabled() || is_tool();

#ifdef TOOLS_ENABLED
    if (p_keep_state && can_run && is_valid()) {
        _save_old_static_data();
    }
#endif

    valid = false;
    GDScriptParser parser;
    Error err = parser.parse(source, path, false);
    if (err) {
        if (EngineDebugger::is_active()) {
            GDScriptLanguage::get_singleton()->debug_break_parse(_get_debug_path(), parser.get_errors().front()->get().line, "Parser Error: " + parser.get_errors().front()->get().message);
        }
        // TODO: Show all error messages.
        _err_print_error("GDScript::reload", path.is_empty() ? "built-in" : (const char *)path.utf8().get_data(), parser.get_errors().front()->get().line, ("Parse Error: " + parser.get_errors().front()->get().message).utf8().get_data(), false, ERR_HANDLER_SCRIPT);
        reloading = false;
        return ERR_PARSE_ERROR;
    }

    GDScriptAnalyzer analyzer(&parser);
    err = analyzer.analyze();

    if (err) {
        if (EngineDebugger::is_active()) {
            GDScriptLanguage::get_singleton()->debug_break_parse(_get_debug_path(), parser.get_errors().front()->get().line, "Parser Error: " + parser.get_errors().front()->get().message);
        }

        const List<GDScriptParser::ParserError>::Element *e = parser.get_errors().front();
        while (e != nullptr) {
            _err_print_error("GDScript::reload", path.is_empty() ? "built-in" : (const char *)path.utf8().get_data(), e->get().line, ("Parse Error: " + e->get().message).utf8().get_data(), false, ERR_HANDLER_SCRIPT);
            e = e->next();
        }
        reloading = false;
        return ERR_PARSE_ERROR;
    }

    can_run = ScriptServer::is_scripting_enabled() || parser.is_tool();

    GDScriptCompiler compiler;
    err = compiler.compile(&parser, this, p_keep_state);

    if (err) {
        _err_print_error("GDScript::reload", path.is_empty() ? "built-in" : (const char *)path.utf8().get_data(), compiler.get_error_line(), ("Compile Error: " + compiler.get_error()).utf8().get_data(), false, ERR_HANDLER_SCRIPT);
        if (can_run) {
            if (EngineDebugger::is_active()) {
                GDScriptLanguage::get_singleton()->debug_break_parse(_get_debug_path(), compiler.get_error_line(), "Parser Error: " + compiler.get_error());
            }
            reloading = false;
            return ERR_COMPILATION_FAILED;
        } else {
            reloading = false;
            return err;
        }
    }

#ifdef TOOLS_ENABLED
    // Done after compilation because it needs the GDScript object's inner class GDScript objects,
    // which are made by calling make_scripts() within compiler.compile() above.
    GDScriptDocGen::generate_docs(this, parser.get_tree());
#endif

#ifdef DEBUG_ENABLED
    for (const GDScriptWarning &warning : parser.get_warnings()) {
        if (EngineDebugger::is_active()) {
            Vector<ScriptLanguage::StackInfo> si;
            EngineDebugger::get_script_debugger()->send_error("", get_script_path(), warning.start_line, warning.get_name(), warning.get_message(), false, ERR_HANDLER_WARNING, si);
        }
    }
#endif

    if (can_run) {
        err = _static_init();
        if (err) {
            return err;
        }
    }

#ifdef TOOLS_ENABLED
    if (can_run && p_keep_state) {
        _restore_old_static_data();
    }
#endif

    reloading = false;
    return OK;
}

/**
 * @brief 
 * @todo figure out in which manner to return the data
 * 
 * @note is used only in editor
 * 
 * @return godot::TypedArray<godot::Dictionary> 
 */
godot::TypedArray<godot::Dictionary> GDscriptpp::_get_documentation() const
{

}

/**
 * @brief 
 * @note is used only in editor
 * @return godot::String 
 */
godot::String GDscriptpp::_get_class_icon_path() const
{

}

/**
 * @brief check if script has method?
 * 
 * @param p_method 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_has_method(const godot::StringName &p_method) const
{
	// return member_functions.has(p_method);
}

/**
 * @brief check if script has method?
 * 
 * @param p_method 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_has_static_method(const godot::StringName &p_method) const
{

}

/**
 * @brief 
 * 
 * @param p_method 
 * @return godot::Variant 
 */
godot::Variant GDscriptpp::_get_script_method_argument_count(const godot::StringName &p_method) const
{

}

/**
 * @brief 
 * 
 * @todo figure out in which manner to return the data
 * 
 * @param p_method 
 * @return godot::Dictionary 
 */
godot::Dictionary GDscriptpp::_get_method_info(const godot::StringName &p_method) const
{

}

/**
 * @brief returns wether script runs as tool or not
 * 
 * @todo check when this is read out
 * 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_is_tool() const
{

}

/**
 * @brief returns wether the script is valid
 * 
 * @todo check when this is read out
 * 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_is_valid() const
{

}

/**
 * @brief returns wether the script is abstract
 * 
 * @todo check when this is read out
 * @todo what is abstract in this case
 * 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_is_abstract() const
{

}

/**
 * @brief return the script language singleton
 * 
 * @todo figure out how this singleton is used and what it needs
 * 
 * @return godot::ScriptLanguage* 
 */
godot::ScriptLanguage * GDscriptpp::_get_language() const
{

}

/**
 * @brief check if script contains signal
 * 
 * @param p_signal 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_has_script_signal(const godot::StringName &p_signal) const
{

}

/**
 * @brief get list of signals
 * 
 * @return godot::TypedArray<godot::Dictionary> 
 */
godot::TypedArray<godot::Dictionary> GDscriptpp::_get_script_signal_list() const
{

}

/**
 * @brief 
 * 
 * @todo figure out what this does
 * 
 * @param p_property 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_has_property_default_value(const godot::StringName &p_property) const
{

}

/**
 * @brief 
 * 
 * @todo differs from implementation
 * @todo check what value needs to be returned
 * 
 * @param p_property 
 * @return godot::Variant 
 */
godot::Variant GDscriptpp::_get_property_default_value(const godot::StringName &p_property) const
{

}

/**
 * @brief gets called when exports are supposed to be updated?
 * 
 * @todo figure out what needs to be called
 * 
 */
void GDscriptpp::_update_exports()
{

}

/**
 * @brief return methods
 * @todo figure out output format
 * 
 * @return godot::TypedArray<godot::Dictionary> 
 */
godot::TypedArray<godot::Dictionary> GDscriptpp::_get_script_method_list() const
{

}

/**
 * @brief return prperties
 * @todo figure out output format
 * 
 * @return godot::TypedArray<godot::Dictionary> 
 */
godot::TypedArray<godot::Dictionary> GDscriptpp::_get_script_property_list() const
{

}

/**
 * @brief return at which line the member is ?
 * 
 * @param p_member 
 * @return int32_t 
 */
int32_t GDscriptpp::_get_member_line(const godot::StringName &p_member) const
{

}

/**
 * @brief return a list of constants
 * @todo figure out output format
 * 
 * @return godot::Dictionary 
 */
godot::Dictionary GDscriptpp::_get_constants() const
{

}

/**
 * @brief return a list of member names?
 * @todo figure out output format
 * 
 * @return godot::Dictionary 
 */
godot::TypedArray<godot::StringName> GDscriptpp::_get_members() const
{

}

/**
 * @brief 
 * 
 * @todo figure out, idk what this even is
 * 
 * @return true 
 * @return false 
 */
bool GDscriptpp::_is_placeholder_fallback_enabled() const
{

}

/**
 * @brief Remote procedure call config
 * @todo figure out output format
 * 
 * @todo what is rpc config???
 * 
 * @return godot::Variant 
 */
godot::Variant GDscriptpp::_get_rpc_config() const
{

}