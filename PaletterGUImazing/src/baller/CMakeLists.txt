set(SOURCE
    image_processor.h
    image_processor.cpp
    processor_device.h
)
add_library(baller STATIC ${SOURCE})
target_link_libraries(baller PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC nerd)
target_include_directories(baller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} nerd)
