set(SOURCE
    coolutil_logger.h
    coolutil_logger.cpp
)
add_library(coolutil STATIC ${SOURCE})
target_link_libraries(coolutil PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(coolutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
