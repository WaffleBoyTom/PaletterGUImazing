if (USE_CUDA)
    set(SOURCE
        quantizer.h
        quantizer.cpp
        remapper.h
        remapper.cpp
        zoom.cuh
        zoom.cu
    )
else()
    set(SOURCE
        quantizer.h
        quantizer.cpp
        remapper.h
        remapper.cpp
    )
endif()
add_library(nerd STATIC ${SOURCE})
target_link_libraries(nerd PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(nerd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
