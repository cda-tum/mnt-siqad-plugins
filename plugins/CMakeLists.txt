# Gather all cpp source files (assuming each file is one plugin)
file(GLOB_RECURSE FILENAMES *.cpp)

# For each plugin
foreach(filename ${FILENAMES})
  # Create an executable
  get_filename_component(plugin ${filename} NAME_WE)
  add_executable(${plugin} ${filename})
  target_link_libraries(${plugin} PRIVATE mnt-siqad-plugins-include
                                          mnt-siqad-plugins-dependencies)

  # SiQAD installation path
  if(DEFINED SIQAD_PLUGINS_ROOT)
    set(PLUGIN_INSTALL_DIR "${SIQAD_PLUGINS_ROOT}/${plugin}")
  elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    set(PLUGIN_INSTALL_DIR "debug")
  else()
    set(PLUGIN_INSTALL_DIR "release")
  endif()

  # Prepare plugin metadata in the build tree so configuring leaves sources
  # unchanged.
  set(physeng_file
      "${CMAKE_CURRENT_BINARY_DIR}/${plugin}-files/${plugin}.physeng")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/${plugin}.physeng"
                 "${physeng_file}" COPYONLY)
  if(WIN32)
    file(READ "${physeng_file}" physeng_content)
    string(REPLACE "<bin_path>${plugin}</bin_path>"
                   "<bin_path>${plugin}.exe</bin_path>" physeng_content
                   "${physeng_content}")
    file(WRITE "${physeng_file}" "${physeng_content}")
  endif()

  # Copy the logo into a per-plugin subdirectory of the build tree. Every
  # plugin's logo is named `logo-mnt.png`, so copying them all straight into the
  # shared ${CMAKE_CURRENT_BINARY_DIR} would have every plugin's POST_BUILD step
  # race to write the same destination path. The subdirectory is suffixed with
  # "-files" so it can't collide with the plugin's own executable, which on
  # non-Windows platforms is an extension-less file literally named
  # ${CMAKE_CURRENT_BINARY_DIR}/${plugin}.
  add_custom_command(
    TARGET ${plugin}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_CURRENT_BINARY_DIR}/${plugin}-files
    COMMAND
      ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/${plugin}/logo-mnt.png
      ${CMAKE_CURRENT_BINARY_DIR}/${plugin}-files)

  # Install the executable and the physeng file
  install(TARGETS ${plugin} RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR})
  install(FILES "${physeng_file}" ${plugin}/logo-mnt.png
          DESTINATION ${PLUGIN_INSTALL_DIR})

endforeach()
