# project

cmake_minimum_required (VERSION 2.8)

# -- Set project name
set(PROJ_NAME uscheme)

project (${PROJ_NAME})

# -- Set install dirs
if (DEFINED ENV{INSTALL_BASE_DIR})
  set(PROJ_INSTALL_DIR $ENV{INSTALL_BASE_DIR})
else()
  set(PROJ_INSTALL_DIR ${CMAKE_SOURCE_DIR}/install)
endif()

set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_CONTACT )

set(PROJ_LICENSE_FILE COPYING)
set(PROJ_README_FILE  README.txt)

# -- Include the tests in the bin folder?
set(INCLUDE_TEST_IN_BIN FALSE)
set(USE_CPP   TRUE)
set(USE_CPP11 TRUE)

# -- Include the magic ProjTools
include("cmake/ProjTools.cmake")

# -- Add targets
# --- Add uscheme and libuscheme
set(PUBLIC_HDR
  defs.hpp;
  type/type.hpp;
  type/object.hpp;
  stream/stream.hpp;
  exec/exec.hpp
)

set(LIB_SRC
  lib.cpp;
  type/object.cpp;
  stream/stream.cpp;
  exec/exec.cpp
)

set(MAIN_SRC
  scheme.cpp
)

# --- Library version
set(USCHEME_VERSION_MAJOR 0.1)
set(USCHEME_VERSION_MINOR 1)
set(USCHEME_VERSION ${USCHEME_VERSION_MAJOR}.${USCHEME_VERSION_MINOR})

# --- Add libuscheme
add_lib(uscheme SHARED ${PUBLIC_HDR} ${LIB_SRC})
add_lib_build_def(uscheme USCHEME_BUILD)
add_lib_build_def(uscheme "USCHEME_LIB_VERSION=\"${USCHEME_VERSION}\"")
link_libs(uscheme )
set_tgt_ver(uscheme ${USCHEME_VERSION} ${USCHEME_VERSION_MAJOR})

# --- Add scheme
add_exe(scheme ${MAIN_SRC})
link_libs(scheme uscheme)
install_tgt(scheme)

# --- Installation targets
install_hdr(${PUBLIC_HDR})
install_tgt(uscheme)
install_tgt(scheme)
install(FILES README.md DESTINATION .)

# -- End targets

# -- Add the subdirectories
set(PROJ_SUBDIRS  unittest; doc; tools; test)
# add all subdirs
foreach(subdir ${PROJ_SUBDIRS})
  add_subdirectory(${subdir})
endforeach(subdir)
