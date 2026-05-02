if (USE_CUDA)
    message( "compiling zoom")
    add_definitions(-DUSE_CUDA)
    set(SOURCE
        kmeanifier.h
        kmeanifier.cpp
        quantizer.h
        quantizer.cpp
        remapper.h
        remapper.cpp
        zoom.cuh
        zoom.cu
    )
else()
    set(SOURCE
        kmeanifier.h
        kmeanifier.cpp
        quantizer.h
        quantizer.cpp
        remapper.h
        remapper.cpp
    )
endif()
add_library(nerd STATIC ${SOURCE})
# this shouldn't include sick .......
target_link_libraries(nerd PRIVATE Qt${QT_VERSION_MAJOR}::Widgets sick)
target_include_directories(nerd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
