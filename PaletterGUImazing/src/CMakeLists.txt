set(PROJECT_SOURCES
    main.cpp
    paletter_window.cpp
    paletter_window.h
    palette_viewer.cpp
    palette_viewer.h
    palette_row.cpp
    palette_row.h
    src_image_viewer.cpp
    src_image_viewer.h
    dst_image_viewer.cpp
    dst_image_viewer.h
)

add_subdirectory("nerd")
add_subdirectory("baller")
add_subdirectory("sick")

# Create the executable target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PaletterGUImazing
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PaletterGUImazing APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PaletterGUImazing SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PaletterGUImazing
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PaletterGUImazing PUBLIC baller sick)

target_link_libraries(PaletterGUImazing PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(USE_CUDA)
    target_link_libraries(PaletterGUImazing PRIVATE CUDA::cudart)
    set_target_properties(PaletterGUImazing PROPERTIES CUDA_ARCHITECTURES native)
    add_definitions(-DUSE_CUDA)
elseif(USE_METAL)
    target_link_libraries(PaletterGUImazing PRIVATE ${METAL_LIB} ${FOUNDATION_LIB})
    add_definitions(-DUSE_METAL)
endif()

if(APPLE)
    target_link_libraries(PaletterGUImazing PRIVATE OpenGL::GL)
    target_compile_options(PaletterGUImazing PRIVATE -Wno-deprecated-declarations)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PaletterGUImazing)
endif()

set_target_properties(PaletterGUImazing PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PaletterGUImazing
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PaletterGUImazing)
endif()
