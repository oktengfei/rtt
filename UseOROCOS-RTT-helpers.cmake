#
# Parses the manifest.xml file and stores the dependencies in RESULT.
# Relies on xpath.
#
# Usage: orocos_get_manifest_deps DEPS)
#
macro( orocos_get_manifest_deps RESULT)
  find_program(XPATH_EXE xpath )
  if (NOT XPATH_EXE)
    message("Warning: xpath not found. Can't read dependencies in manifest.xml file.")
  else(NOT XPATH_EXE)
    execute_process(COMMAND ${XPATH_EXE} -q -e "package/depend/@package" ${CMAKE_CURRENT_SOURCE_DIR}/manifest.xml RESULT_VARIABLE RES OUTPUT_VARIABLE DEPS)
    if (NOT RES EQUAL 0)
      message(SEND_ERROR "Error: xpath found but returned non-zero:${DEPS}")
    endif (NOT RES EQUAL 0)
    
    string(REGEX REPLACE " package=\"([^\"]+)\"\n" "\\1;" RR_RESULT ${DEPS})

    #message("Dependencies are: ${RR_RESULT}")
    set(${RESULT} ${RR_RESULT})
  endif (NOT XPATH_EXE)

endmacro( orocos_get_manifest_deps RESULT)

#
# Find a package, pick up its include dirs and link with its libraries.
# It does this by locating and reading the .pc file generated by that package.
# In case no such .pc file exists (or is not found), tt is assumed that no
# flags are necessary.
#
# This macro is called for you by UseOrocos-RTT.cmake
# for each dependency listed in your manifest.xml file. 
#
# sets these variables:
# ${PACKAGE}_LIBRARIES            The fully resolved link libraries for this package.
# ${PACKAGE}_<LIB>_LIBRARIES      Each fully resolved link library <LIB> in the above list.
#
# Usage: orocos_use_package( myrobot )
#
function( orocos_use_package PACKAGE )
  if (PACKAGE STREQUAL "rtt")
    return()
  endif (PACKAGE STREQUAL "rtt")
  if (ROS_ROOT)
    if (NOT USE_FOUND_${PACKAGE}_PACKAGE_PATH)
      # use rospack to find package directories of *all* dependencies.
      # We need these because a .pc file may depend on another .pc file in another package.
      # This package + the packages this package depends on:
      rosbuild_invoke_rospack(${PACKAGE} ${PACKAGE}_prefix DEPS depends)
      string(REGEX REPLACE "\n" ";" ${PACKAGE}_prefix_DEPS2 "${${PACKAGE}_prefix_DEPS}" )
      foreach(ROSDEP ${${PACKAGE}_prefix_DEPS2} ${PACKAGE})
        # Skip previously found packages
        if (NOT USE_FOUND_${ROSDEP}_PACKAGE_PATH)
          rosbuild_find_ros_package( ${ROSDEP} )
          set( ENV{PKG_CONFIG_PATH} "${${ROSDEP}_PACKAGE_PATH}:${${ROSDEP}_PACKAGE_PATH}/install/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}" )
	  set( USE_FOUND_${ROSDEP}_PACKAGE_PATH 1 ) # mark we don't need to find it again.
        endif (NOT USE_FOUND_${ROSDEP}_PACKAGE_PATH)
      endforeach(ROSDEP ${${PACKAGE}_prefix_DEPS2} ${PACKAGE})

      #message("Searching for ${PACKAGE} in ${${ROSDEP}_PACKAGE_PATH}.")
      endif (NOT USE_FOUND_${PACKAGE}_PACKAGE_PATH)
  else(ROS_ROOT)
    #Use default pkg-config path
    #message("Searching for ${PACKAGE} in env PKG_CONFIG_PATH.")
  endif(ROS_ROOT)

  # Now we are ready to get the flags from the .pc files:
  #pkg_check_modules(${PACKAGE}_COMP ${PACKAGE}-${OROCOS_TARGET})
  pkg_search_module(${PACKAGE}_COMP ${PACKAGE} ${PACKAGE}-${OROCOS_TARGET})
  if (${PACKAGE}_COMP_FOUND)
    include_directories(${${PACKAGE}_COMP_INCLUDE_DIRS})

    # Use find_libraries to find each library:
    foreach(COMP_LIB ${${PACKAGE}_COMP_LIBRARIES})
        find_library(${PACKAGE}_${COMP_LIB}_LIBRARY NAMES ${COMP_LIB} HINTS ${${PACKAGE}_COMP_LIBRARY_DIRS})
        if(${PACKAGE}_${COMP_LIB}_LIBRARY)
        else(${PACKAGE}_${COMP_LIB}_LIBRARY)
            message(SEND_ERROR "Could not find library ${COMP_LIB} in ${${PACKAGE}_COMP_LIBRARY_DIRS}, although its .pc file says it should be there.")
        endif(${PACKAGE}_${COMP_LIB}_LIBRARY)
	list(APPEND ${PACKAGE}_LIBRARIES ${${PACKAGE}_${COMP_LIB}_LIBRARY})
    endforeach(COMP_LIB ${${PACKAGE}_COMP_LIBRARIES})

    # Only link in case there is something *and* the user didn't opt-out:
    if (NOT OROCOS_NO_AUTO_LINKING AND ${PACKAGE}_COMP_LIBRARIES)
      link_libraries( ${${PACKAGE}_LIBRARIES} )
      message("[UseOrocos] Linking all targets with libraries from package '${PACKAGE}'.")
      #message("Linking with ${PACKAGE}: ${${PACKAGE}_LIBRARIES}")
    endif (NOT OROCOS_NO_AUTO_LINKING AND ${PACKAGE}_COMP_LIBRARIES)

  else (${PACKAGE}_COMP_FOUND)
    if (VERBOSE)
      message("[UseOrocos] ${PACKAGE} does not provide a .pc file for exporting its build/link flags (or one of it 'Requires' dependencies was not found).")
    endif (VERBOSE)
  endif (${PACKAGE}_COMP_FOUND)
    
endfunction( orocos_use_package PACKAGE )

