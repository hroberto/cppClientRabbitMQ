include(ExternalProject)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)

# set(libQuickfix quickfix)
# set(quickfix_HEADER ${INSTALL_BASE_DIR}/${libQuickfix}/include)
# set(quickfix_BINARY_DIR ${INSTALL_BASE_DIR}/${libQuickfix}/lib)

# set(quickfix_LIB   "libquickfix.a")

message("==> Find Project: QuickFix")

#include(FindPackageHandleStandardArgs.cmake)

find_path( QUICKFIX_INCLUDE_DIR quickfix/FixFields.h )
find_library( QUICKFIX_LIBRARY quickfix )

find_package_handle_standard_args( QuickFix
 DEFAULT_MSG
 QUICKFIX_INCLUDE_DIR
 QUICKFIX_LIBRARY
)

mark_as_advanced( QUICKFIX_LIBRARY   QUICKFIX_INCLUDE_DIR )

if( QUICKFIX_FOUND )
  set( QUICKFIX_LIBRARIES    ${QUICKFIX_LIBRARY} )
  set( QUICKFIX_INCLUDE_DIRS ${QUICKFIX_INCLUDE_DIR} )
endif()


# set(BUILD_SHARED_LIBS  OFF)
# set( quickfix_CMD_CONFIG " ")

# ExternalProject_Add(
#     ${libQuickfix}
#     GIT_REPOSITORY      https://github.com/quickfix/quickfix.git
#     #  GIT_TAG             v1.15.1   ## error: ISO C++17 does not allow dynamic exception specifications
#     GIT_TAG             master
#     PREFIX              ${INSTALL_BASE_DIR}/${libQuickfix}
#     BUILD_IN_SOURCE 1
#     SOURCE_DIR          ${FETCHCONTENT_BASE_DIR}/quickfix-src
#     CONFIGURE_COMMAND   <SOURCE_DIR>/bootstrap &&
#                         [ <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>                        
#     CMAKE_ARGS 
#         -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#         -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_BASE_DIR}/${libQuickfix}
#         -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#         -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
#         -DBUILD_SHARED_LIBS:BOOL=OFF
#         -DHAVE_SSL:BOOL=OFF
#         -Wno-dev
#     TEST_COMMAND        ""    
#     UPDATE_DISCONNECTED 1
# )

# include_directories(${quickfix_HEADER} ${quickfix_BINARY_DIR})
