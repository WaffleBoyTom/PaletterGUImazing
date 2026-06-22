add_subdirectory(shaders)

if(USE_CUDA)
    message("Compiling CUDA.")

    add_definitions(-DUSE_CUDA)

    set(SOURCE
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
# this shouldn't include sick .......
target_link_libraries(nerd PRIVATE Qt${QT_VERSION_MAJOR}::Widgets sick)
target_include_directories(nerd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${EXTERNAL_DIR}/metal-cpp)
