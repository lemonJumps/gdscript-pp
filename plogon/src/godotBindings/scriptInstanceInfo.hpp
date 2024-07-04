/**
 * @file scriptInstanceInfo.hpp
 * @author Lemon Jumps (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <gdextension_interface.h>


/**
 * @brief class implements and generalizes GDExtensionScriptInstanceInfo
 * 
 */
class ScriptInstanceInfo
{
	virtual GDExtensionBool set_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        GDExtensionConstStringNamePtr p_name, 
        GDExtensionConstVariantPtr p_value) = 0;

	virtual GDExtensionBool get_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        GDExtensionConstStringNamePtr p_name, 
        GDExtensionVariantPtr r_ret) = 0;

	virtual const GDExtensionPropertyInfo * get_property_list_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        uint32_t *r_count) = 0;

	virtual void free_property_list_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        const GDExtensionPropertyInfo *p_list) = 0;
    
    // Optional. Set to NULL for the default behavior.
    virtual GDExtensionBool get_class_category_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        GDExtensionPropertyInfo *p_class_category) = 0;
	

    virtual GDExtensionBool property_can_revert_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        GDExtensionConstStringNamePtr p_name) = 0;

    virtual GDExtensionBool property_get_revert_func(
        GDExtensionScriptInstanceDataPtr p_instance, 
        GDExtensionConstStringNamePtr p_name, 
        GDExtensionVariantPtr r_ret) = 0;


	virtual GDExtensionObjectPtr get_owner_func(GDExtensionScriptInstanceDataPtr p_instance) = 0;
	virtual void get_property_state_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionScriptInstancePropertyStateAdd p_add_func, void *p_userdata) = 0;

	virtual const GDExtensionMethodInfo * get_method_list_func(GDExtensionScriptInstanceDataPtr p_instance, uint32_t *r_count) = 0;
	virtual void free_method_list_func(GDExtensionScriptInstanceDataPtr p_instance, const GDExtensionMethodInfo *p_list, uint32_t p_count) = 0;
	virtual GDExtensionVariantType get_property_type_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionBool *r_is_valid) = 0;
	virtual GDExtensionBool validate_property_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionPropertyInfo *p_property) = 0;

	virtual GDExtensionBool has_method_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name) = 0;

	virtual GDExtensionInt get_method_argument_count_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionBool *r_is_valid) = 0;

	virtual void call_func(GDExtensionScriptInstanceDataPtr p_self, GDExtensionConstStringNamePtr p_method, const GDExtensionConstVariantPtr *p_args, GDExtensionInt p_argument_count, GDExtensionVariantPtr r_return, GDExtensionCallError *r_error) = 0;
	virtual void notification_func(GDExtensionScriptInstanceDataPtr p_instance, int32_t p_what, GDExtensionBool p_reversed) = 0;

	virtual void to_string_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionBool *r_is_valid, GDExtensionStringPtr r_out) = 0;

	virtual void refcount_incremented_func(GDExtensionScriptInstanceDataPtr p_instance) = 0;
	virtual GDExtensionBool refcount_decremented_func(GDExtensionScriptInstanceDataPtr p_instance) = 0;

	virtual GDExtensionObjectPtr get_script_func(GDExtensionScriptInstanceDataPtr p_instance) = 0;

	virtual GDExtensionBool is_placeholder_func(GDExtensionScriptInstanceDataPtr p_instance) = 0;

	virtual GDExtensionBool set_fallback_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionConstVariantPtr p_value) = 0;
	virtual GDExtensionBool get_fallback_func(GDExtensionScriptInstanceDataPtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) = 0;

	virtual GDExtensionScriptLanguagePtr get_language_func(GDExtensionScriptInstanceDataPtr p_instance) = 0;

	virtual void free_func(GDExtensionScriptInstanceDataPtr p_instance);
};