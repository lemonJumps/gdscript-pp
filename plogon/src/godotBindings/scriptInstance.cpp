
#include "scriptInstance.hpp"



// following section shamelesly adapted from luau for godot
// honestly it would be nice if these interfaces were provided by godot-cpp

#define COMMON_SELF ((CommonScriptInstance *)p_self)

void CommonScriptInstance::init_script_instance_info_common(GDExtensionScriptInstanceInfo3 &p_info) {
	// Must initialize potentially unused struct fields to nullptr
	// (if not, causes segfault on MSVC).
	p_info.property_can_revert_func = nullptr;
	p_info.property_get_revert_func = nullptr;

	p_info.call_func = nullptr;
	p_info.notification_func = nullptr;

	p_info.to_string_func = nullptr;

	p_info.refcount_incremented_func = nullptr;
	p_info.refcount_decremented_func = nullptr;

	p_info.is_placeholder_func = nullptr;

	p_info.set_fallback_func = nullptr;
	p_info.get_fallback_func = nullptr;

	p_info.set_func = [](void *p_self, GDExtensionConstStringNamePtr p_name, GDExtensionConstVariantPtr p_value) -> GDExtensionBool {
		return COMMON_SELF->set(*(const godot::StringName *)p_name, *(const godot::Variant *)p_value);
	};

	p_info.get_func = [](void *p_self, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) -> GDExtensionBool {
		return COMMON_SELF->get(*(const godot::StringName *)p_name, *(godot::Variant *)r_ret);
	};

	p_info.get_property_list_func = [](void *p_self, uint32_t *r_count) -> const GDExtensionPropertyInfo * {
		return COMMON_SELF->get_property_list(r_count);
	};

	p_info.free_property_list_func = [](void *p_self, const GDExtensionPropertyInfo *p_list, uint32_t p_count) {
		COMMON_SELF->free_property_list(p_list, p_count);
	};

	p_info.validate_property_func = [](void *p_self, GDExtensionPropertyInfo *p_property) -> GDExtensionBool {
		return COMMON_SELF->validate_property(p_property);
	};

	p_info.get_owner_func = [](void *p_self) {
		return COMMON_SELF->get_owner()->_owner;
	};

	p_info.get_property_state_func = [](void *p_self, GDExtensionScriptInstancePropertyStateAdd p_add_func, void *p_userdata) {
		COMMON_SELF->get_property_state(p_add_func, p_userdata);
	};

	p_info.get_method_list_func = [](void *p_self, uint32_t *r_count) -> const GDExtensionMethodInfo * {
		return COMMON_SELF->get_method_list(r_count);
	};

	p_info.free_method_list_func = [](void *p_self, const GDExtensionMethodInfo *p_list, uint32_t p_count) {
		COMMON_SELF->free_method_list(p_list, p_count);
	};

	p_info.get_property_type_func = [](void *p_self, GDExtensionConstStringNamePtr p_name, GDExtensionBool *r_is_valid) -> GDExtensionVariantType {
		return (GDExtensionVariantType)COMMON_SELF->get_property_type(*(const godot::StringName *)p_name, (bool *)r_is_valid);
	};

	p_info.has_method_func = [](void *p_self, GDExtensionConstStringNamePtr p_name) -> GDExtensionBool {
		return COMMON_SELF->has_method(*(const godot::StringName *)p_name);
	};

	p_info.get_script_func = [](void *p_self) {
		return COMMON_SELF->get_script().ptr()->_owner;
	};

	p_info.get_language_func = [](void *p_self) {
		return COMMON_SELF->get_language()->_owner;
	};
}

#define INSTANCE_SELF ((GDscriptppInstance *)p_self)

static GDExtensionScriptInstanceInfo3 init_script_instance_info() {
	GDExtensionScriptInstanceInfo3 info;
	ScriptInstance::init_script_instance_info_common(info);

	info.property_can_revert_func = [](void *p_self, GDExtensionConstStringNamePtr p_name) -> GDExtensionBool {
		return INSTANCE_SELF->property_can_revert(*((godot::StringName *)p_name));
	};

	info.property_get_revert_func = [](void *p_self, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) -> GDExtensionBool {
		return INSTANCE_SELF->property_get_revert(*((godot::StringName *)p_name), (godot::Variant *)r_ret);
	};

	info.call_func = [](void *p_self, GDExtensionConstStringNamePtr p_method, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error) {
		return INSTANCE_SELF->call(*((godot::StringName *)p_method), (const godot::Variant **)p_args, p_argument_count, (godot::Variant *)r_return, r_error);
	};

	info.notification_func = [](void *p_self, int32_t p_what, GDExtensionBool p_reversed) {
		INSTANCE_SELF->notification(p_what);
	};

	info.to_string_func = [](void *p_self, GDExtensionBool *r_is_valid, GDExtensionStringPtr r_out) {
		INSTANCE_SELF->to_string(r_is_valid, (godot::String *)r_out);
	};

	info.free_func = [](void *p_self) {
		godot::memdelete(INSTANCE_SELF);
	};

	info.refcount_decremented_func = [](void *) -> GDExtensionBool {
		// If false (default), object cannot die
		return true;
	};

	return info;
}

const GDExtensionScriptInstanceInfo3 GDscriptppInstance::INSTANCE_INFO = init_script_instance_info();