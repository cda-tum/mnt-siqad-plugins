# Gather all cpp source files (assuming each file is one plugin)
file(GLOB_RECURSE FILENAMES *.cpp)

# For each plugin
foreach (filename ${FILENAMES})
    # Create an executable
    get_filename_component(plugin ${filename} NAME_WE)
    add_executable(${plugin} ${filename})
    target_link_libraries(${plugin} PRIVATE cda-tum-siqad-plugins-include cda-tum-siqad-plugins-dependencies)

    # SiQAD installation path
    if (DEFINED SIQAD_PLUGINS_ROOT)
        set(PLUGIN_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/${plugin}")
    elseif (CMAKE_BUILD_TYPE MATCHES Debug)
        set(PLUGIN_INSTALL_DIR "debug")
    else ()
        set(PLUGIN_INSTALL_DIR "release")
    endif ()

    if (WIN32)
        # Modify the physeng file (add .exe to the bin_path)
        set(physeng_file "${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/${plugin}.physeng")

        # Check if the file exists
        if (EXISTS ${physeng_file})
            file(READ ${physeng_file} physeng_content)

            # Replace <bin_path>plugin_name</bin_path> with <bin_path>plugin_name.exe</bin_path>
            string(REPLACE "<bin_path>${plugin}</bin_path>" "<bin_path>${plugin}.exe</bin_path>" physeng_content ${physeng_content})

            # Write the updated content back to the file
            file(WRITE ${physeng_file} ${physeng_content})
        else ()
            message(WARNING "Physeng file not found for plugin: ${plugin}")
        endif ()
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
    install(FILES ${plugin}/${plugin}.physeng DESTINATION ${PLUGIN_INSTALL_DIR})
endforeach ()
