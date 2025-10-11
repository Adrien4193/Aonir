function(configure_application TARGET SOURCES)
    add_executable(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${TARGET})
    target_sources(${TARGET} PRIVATE ${SOURCES})
endfunction()

function(install_application TARGET)
    cmake_parse_arguments(arg "" "DESCRIPTION" "DEPENDS" ${ARGN})

    install(
        TARGETS ${TARGET}
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${TARGET}Runtime
    )

    set(DEPENDENCIES "")

    foreach(DEPENDENCY ${arg_DEPENDS})
        list(APPEND DEPENDENCIES ${DEPENDENCY}Runtime)
    endforeach()

    cpack_add_component(
        ${TARGET}Runtime
        DISPLAY_NAME ${TARGET}
        DESCRIPTION ${arg_DESCRIPTION}
        DEPENDS ${DEPENDENCIES}
        INSTALL_TYPES Full Runtime
    )
endfunction()
