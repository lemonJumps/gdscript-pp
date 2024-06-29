
#include "register_types.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// #include "scannur.hpp"
// #include "oraculum/oraculum.hpp"
// #include "miscObjects/vectorWrapper.hpp"
// #include "miscObjects/projector.hpp"
// #include "miscObjects/opencvHelper.hpp"
// #include "miscObjects/camWrapper.hpp"
// #include "miscObjects/imgManip.hpp"

#include "cv2Interface/modules.hpp"

// #include "godot_std_wrap/vector_array.hpp"

using namespace godot;

void initialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ocv_modules::register_classes();
	// ClassDB::register_class<VectorArray<int>>();
	// ClassDB::register_class<ImgManip>();

	// ClassDB::register_class<Vector_int>();
	// ClassDB::register_class<Vector_float>();
	// ClassDB::register_class<Vector_vec2x2>();

	// ClassDB::register_class<Scannur>();
	// ClassDB::register_class<projector>();
	// ClassDB::register_class<Opencv>();

	// ClassDB::register_class<Oraculum>();
	// ClassDB::register_class<CamWrapper>();

	
}

void uninitialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_module);
	init_obj.register_terminator(uninitialize_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
