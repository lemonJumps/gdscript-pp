#pragma once

/**
 * @file scriptLanguage.hpp
 * @author Lemon Jumps (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include <godot_cpp/classes/script_language_extension.hpp>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/mutex.hpp>

#include "scriptExtension.hpp"


class GDscriptppLanguage : public godot::ScriptLanguageExtension
{
    // after reading godot luau implementation,
    // none of us know how the fuck we're supposed to actually use this.
    // Like It would be lovely to know what's happening here.
    // and about all the other things, but everyone is literally just copying everyone else.
    // This is hell, this is literally like working with legacy code written in the 90s
    // an ounce of documentation, a single page even, would help so much

    godot::Ref<godot::Mutex> lock;

    static GDscriptppLanguage *singleton;
    GDscriptpp *gdscriptpp = nullptr;
    // LuauCache *cache = nullptr;
    // LuauInterface *interface = nullptr;
    // TaskScheduler task_scheduler;

    uint64_t ticks_usec = 0;

    // godot::SelfList<godot::LuauScript>::List script_list;

    // godot::HashMap<godot::StringName, godot::Variant> global_constants;

    struct DebugInfo {
        struct StackInfo {
            godot::String source;
            godot::String name;
            int line;

            operator godot::Dictionary() const;
        };

        godot::Ref<godot::Mutex> call_lock;
        godot::Vector<StackInfo> call_stack;
    } debug;

    // void debug_init();
    // static void lua_interrupt(lua_State *L, int p_gc);

    // static bool ar_to_si(lua_Debug &p_ar, DebugInfo::StackInfo &p_si);

    bool finalized = false;
    void finalize();

#ifdef TOOLS_ENABLED
    List<Ref<LuauScript>> get_scripts() const;
#endif // TOOLS_ENABLED

protected:
    static void _bind_methods() {}

public:
    static GDscriptppLanguage *get_singleton() { return singleton; }

    void _init() override;
    void _finish() override;

    /* LANGUAGE INFO */
    godot::String _get_name() const override;
    godot::String _get_type() const override;

    godot::String _get_extension() const override;
    godot::PackedStringArray _get_recognized_extensions() const override;

    godot::PackedStringArray _get_reserved_words() const override;
    bool _is_control_flow_keyword(const godot::String &p_keyword) const override;
    godot::String _validate_path(const godot::String &p_path) const override { return ""; }

    godot::PackedStringArray _get_comment_delimiters() const override;
    godot::PackedStringArray _get_string_delimiters() const override;

    bool _supports_builtin_mode() const override;
    bool _can_inherit_from_file() const override;

    /* ... */
    Object *_create_script() const override;

    bool _is_using_templates() override { return true; }
    godot::TypedArray<godot::Dictionary> _get_built_in_templates(const godot::StringName &p_object) const override;
    godot::Ref<godot::Script> _make_template(const godot::String &p_template, const godot::String &p_class_name, const godot::String &p_base_class_name) const override;

    void _add_global_constant(const godot::StringName &p_name, const godot::Variant &p_value) override;
    void _add_named_global_constant(const godot::StringName &p_name, const godot::Variant &p_value) override;
    void _remove_named_global_constant(const godot::StringName &p_name) override;

    void _frame() override;

    /* EDITOR */
    void _reload_all_scripts() override;
    void _reload_tool_script(const godot::Ref<godot::Script> &p_script, bool p_soft_reload) override;

    bool _handles_global_class_type(const godot::String &p_type) const override;
    godot::Dictionary _get_global_class_name(const godot::String &p_path) const override;

    /* DEBUGGER */
    // String _debug_get_error() const;
    // int64_t _debug_get_stack_level_count() const;
    // int64_t _debug_get_stack_level_line(int64_t level) const;
    // String _debug_get_stack_level_function(int64_t level) const;
    // Dictionary _debug_get_stack_level_locals(int64_t level, int64_t max_subitems, int64_t max_depth);
    // Dictionary _debug_get_stack_level_members(int64_t level, int64_t max_subitems, int64_t max_depth);
    // void *_debug_get_stack_level_instance(int64_t level);
    // Dictionary _debug_get_globals(int64_t max_subitems, int64_t max_depth);
    // String _debug_parse_stack_level_expression(int64_t level, const String &expression, int64_t max_subitems, int64_t max_depth);
    // void set_call_stack(lua_State *L);
    // void clear_call_stack();
    godot::TypedArray<godot::Dictionary> _debug_get_current_stack_info() override;

    /* ???: pure virtual functions which have no clear purpose */
    bool _has_named_classes() const override;

    /* UNNEEDED */
    void _thread_enter() override {}
    void _thread_exit() override {}

    bool _overrides_external_editor() override { return false; }
    // Error _open_in_external_editor(const Ref<Script> &script, int64_t line, int64_t column);

    /* TO IMPLEMENT */
    godot::Dictionary _validate(const godot::String &p_script, const godot::String &p_path, bool p_validate_functions, bool p_validate_errors, bool p_validate_warnings, bool p_validate_safe_lines) const override {
        godot::Dictionary output;

        output["valid"] = true;

        return output;
    }

    godot::Dictionary _complete_code(const godot::String &p_code, const godot::String &p_path, godot::Object *p_owner) const override { return godot::Dictionary(); }
    godot::Dictionary _lookup_code(const godot::String &p_code, const godot::String &p_symbol, const godot::String &p_path, godot::Object *p_owner) const override { return godot::Dictionary(); }
    godot::String _auto_indent_code(const godot::String &p_code, int32_t p_from_line, int32_t p_to_line) const override { return p_code; }

    int32_t _find_function(const godot::String &p_class_name, const godot::String &p_function_name) const override { return -1; }
    godot::String _make_function(const godot::String &p_class_name, const godot::String &p_function_name, const godot::PackedStringArray &p_function_args) const override { return godot::String(); }

    bool _supports_documentation() const override { return false; }
    godot::TypedArray<godot::Dictionary> _get_public_functions() const override { return godot::TypedArray<godot::Dictionary>(); }
    godot::Dictionary _get_public_constants() const override { return godot::Dictionary(); }
    godot::TypedArray<godot::Dictionary> _get_public_annotations() const override { return godot::TypedArray<godot::Dictionary>(); }

};