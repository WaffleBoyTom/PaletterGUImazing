set(SOURCE
    sick_debug.h
    sick_dropdown.h
    sick_dropdown.cpp
    sick_fileio.h
    sick_fileio.cpp
    sick_file_line_edit.h
    sick_file_line_edit.cpp
    sick_graphics_view.h
    sick_graphics_view.cpp
    sick_image_viewer.h
    sick_image_viewer.cpp
    sick_imageholder.h
    sick_imageholder.cpp
    sick_inspector.h
    sick_inspector.cpp
    sick_log_viewer.h
    sick_log_viewer.cpp
    sick_export_opts.h
    sick_export_opts.cpp
    sick_logger.h
    sick_logger.cpp
    sick_slider.h
    sick_slider.cpp
    sick_utils.h
    sick_utils.cpp
)
add_library(sick STATIC ${SOURCE})
target_link_libraries(sick PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC baller)
target_include_directories(sick PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} baller)
