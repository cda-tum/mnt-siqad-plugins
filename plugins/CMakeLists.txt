# Gather all cpp source files (assuming each file is one plugin)
file(GLOB_RECURSE FILENAMES *.cpp)

# For each plugin
foreach (filename ${FILENAMES})
    # Create an executable
    get_filename_component(plugin ${filename} NAME_WE)
    add_executable(${plugin} ${filename})
    target_link_libraries(${plugin} PRIVATE mnt-siqad-plugins-include mnt-siqad-plugins-dependencies)

    # SiQAD installation path
    if (DEFINED SIQAD_PLUGINS_ROOT)
        set(PLUGIN_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/${plugin}")
    elseif (CMAKE_BUILD_TYPE MATCHES Debug)
        set(PLUGIN_INSTALL_DIR "debug")
    else ()
        set(PLUGIN_INSTALL_DIR "release")
    endif ()

    # Copy the physeng file to the build directory
    add_custom_command(
            TARGET ${plugin} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/${plugin}.physeng
            ${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/logo-mnt.png
            ${CMAKE_CURRENT_BINARY_DIR})

    # Install the executable and the physeng file
    install(TARGETS ${plugin} RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR})
    install(FILES ${plugin}/${plugin}.physeng ${plugin}/logo-mnt.png DESTINATION ${PLUGIN_INSTALL_DIR})

endforeach ()
