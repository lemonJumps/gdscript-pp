# SPDX-License-Identifier: Unlicense

target_sources( ${PROJECT_NAME}
    PRIVATE
        register_types.h
        register_types.cpp

        scannur.cpp
        scannur.hpp

        miscObjects/opencvHelper.cpp
        miscObjects/opencvHelper.hpp
        miscObjects/projector.cpp
        miscObjects/projector.hpp
        miscObjects/vectorWrapper.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)