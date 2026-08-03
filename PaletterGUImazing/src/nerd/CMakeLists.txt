add_subdirectory(shaders)

if(USE_CUDA)
    message("Compiling CUDA.")

    add_definitions(-DUSE_CUDA)

    set(SOURCE
        cumeans.cuh
        cumeans.cu
        kmeanifier.h
        kmeanifier.cpp
        nerd_types.h
        palette_generator.h
        palette_generator.cpp
        remapper.h
        remapper.cpp
        zoom.cuh
        zoom.cu
    )

elseif(USE_METAL)
    message("Compiling Metal.")

    add_definitions(-DUSE_METAL)

    set(SOURCE
        kmeanifier.h
        kmeanifier.cpp
        metal_context.h
        metal_context.cpp
        metal_dispatcher.h
        metal_dispatcher.cpp
        metal_implementation.cpp
        metal_remapper.h
        metal_remapper.cpp
        nerd_types.h
        palette_generator.h
        palette_generator.cpp
        remapper.h
        remapper.cpp
    )

else ()
    set(SOURCE
        kmeanifier.h
        kmeanifier.cpp
        nerd_types.h
        palette_generator.h
        palette_generator.cpp
        remapper.h
        remapper.cpp
    )

endif()

add_library(nerd STATIC ${SOURCE})
target_link_libraries(nerd PUBLIC cool)
target_include_directories(nerd PUBLIC ${EXTERNAL_DIR}/metal-cpp cool)
