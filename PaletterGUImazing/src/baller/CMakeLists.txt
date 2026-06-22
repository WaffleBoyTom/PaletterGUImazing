set(SOURCE
    baller_task.h
    baller_task.cpp
    baller_types.h
)
add_library(baller STATIC ${SOURCE})
target_link_libraries(baller PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC nerd)
target_include_directories(baller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} nerd)
