set(METAL_SHADERS
    zoom.metal
)

set(AIR_FILES)

foreach(SHADER ${METAL_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    set(AIR_FILE ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.air)

    add_custom_command(
        OUTPUT ${AIR_FILE}
        COMMAND xcrun metal -c ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} -o ${AIR_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling Metal shader ${SHADER}"
    )

    list(APPEND AIR_FILES ${AIR_FILE})
endforeach()

add_custom_command(
    OUTPUT ${SHADER_OUTPUT_DIR}/shaders.metallib
    COMMAND xcrun metallib ${AIR_FILES} -o ${SHADER_OUTPUT_DIR}/shaders.metallib
    DEPENDS ${AIR_FILES}
    COMMENT "Linking Metal shaders into metallib"
)

add_custom_target(MetalShaders ALL DEPENDS ${SHADER_OUTPUT_DIR}/shaders.metallib)

set_target_properties(MetalShaders PROPERTIES
    METALLIB_PATH ${SHADER_OUTPUT_DIR}/shaders.metallib
)
