#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FreeGLUT::freeglut_static" for configuration "RelWithDebInfo"
set_property(TARGET FreeGLUT::freeglut_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(FreeGLUT::freeglut_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C;RC"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/freeglut_static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS FreeGLUT::freeglut_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_FreeGLUT::freeglut_static "${_IMPORT_PREFIX}/lib/freeglut_static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
