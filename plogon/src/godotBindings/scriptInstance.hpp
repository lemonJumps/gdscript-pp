/**
 * @file scriptInstance.hpp
 * @author Lemon Jumps (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <godot_cpp/classes/script_extension.hpp>
#include <godot_cpp/templates/pair.hpp>

#include "scriptInstanceInfo.hpp"
#include "scriptExtension.hpp"

/**
 * @brief this contains functions and data that isn't changed between different instances
 * 
 */
class CommonScriptInstance : ScriptInstanceInfo
{

	enum PropertySetGetError {
		PROP_OK,
		PROP_NOT_FOUND,
		PROP_WRONG_TYPE,
		PROP_READ_ONLY,
		PROP_WRITE_ONLY,
		PROP_GET_FAILED,
		PROP_SET_FAILED
	};

	public:
	static void init_script_instance_info_common(GDExtensionScriptInstanceInfo3 &p_info);

	virtual bool set(const godot::StringName &p_name, const godot::Variant &p_value, PropertySetGetError *r_err = nullptr) = 0;
	virtual bool get(const godot::StringName &p_name, godot::Variant &r_ret, PropertySetGetError *r_err = nullptr) = 0;

	void get_property_state(GDExtensionScriptInstancePropertyStateAdd p_add_func, void *p_userdata);
	void get_property_state(godot::List<godot::Pair<godot::StringName, godot::Variant>> &p_list);

	virtual GDExtensionPropertyInfo *get_property_list(uint32_t *r_count) = 0;
	void free_property_list(const GDExtensionPropertyInfo *p_list, uint32_t p_count) const;
	virtual bool validate_property(GDExtensionPropertyInfo *p_property) const { return false; }

	virtual godot::Variant::Type get_property_type(const godot::StringName &p_name, bool *r_is_valid) const = 0;

	virtual GDExtensionMethodInfo *get_method_list(uint32_t *r_count) const;
	void free_method_list(const GDExtensionMethodInfo *p_list, uint32_t p_count) const;

	virtual bool has_method(const godot::StringName &p_name) const = 0;

	virtual godot::Object *get_owner() const = 0;
	virtual godot::Ref<GDscriptpp> get_script() const = 0;
	ScriptLanguage *get_language() const;
};

class GDscriptppInstance : CommonScriptInstance
{

public:

#pragma region script info
    
	bool property_can_revert(const godot::StringName &p_name);
	bool property_get_revert(const godot::StringName &p_name, godot::Variant *r_ret);

	void call(const godot::StringName &p_method, const godot::Variant *const *p_args, const GDExtensionInt p_argument_count, Variant *r_return, GDExtensionCallError *r_error);

	void notification(int32_t p_what);
	void to_string(GDExtensionBool *r_is_valid, godot::String *r_out);

	static const GDExtensionScriptInstanceInfo3 INSTANCE_INFO;

#pragma endregion

};