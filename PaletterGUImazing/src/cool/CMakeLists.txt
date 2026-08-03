set(SOURCE
    cool_widget_debug.h
    cool_widget_debug.cpp
    cool_fileio.h
    cool_fileio.cpp
    cool_logger.h
    cool_logger.cpp
)

add_library(cool STATIC ${SOURCE})
target_link_libraries(cool PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(cool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
