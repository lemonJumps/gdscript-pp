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
#include "scriptInstanceInfo.hpp"


class GDscriptppInstance : scriptInstanceInfo
{

public:
	// static void init_script_instance_info_common(GDExtensionScriptInstanceInfo2 &p_info);

#pragma region script info
    
	bool property_can_revert(const godot::StringName &p_name);
	bool property_get_revert(const godot::StringName &p_name, godot::Variant *r_ret);

	void call(const godot::StringName &p_method, const godot::Variant *const *p_args, const GDExtensionInt p_argument_count, Variant *r_return, GDExtensionCallError *r_error);

	static const GDExtensionScriptInstanceInfo3 INSTANCE_INFO;

#pragma endregion

};