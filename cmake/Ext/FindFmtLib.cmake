include(ExternalProject)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)


set(libFmtLib fmt)
set(fmtlib_HEADER ${INSTALL_BASE_DIR}/${libFmtLib}/include)
set(fmtlib_BINARY_DIR ${INSTALL_BASE_DIR}/${libFmtLib}/lib)


if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set(fmtlib_LIB   ${fmtlib_BINARY_DIR}/libfmt.a)
endif( CMAKE_BUILD_TYPE STREQUAL "Release" )
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(fmtlib_LIB   ${fmtlib_BINARY_DIR}/libfmtd.a)
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )


message("==> Find Project libFmtLib  ")


ExternalProject_Add(
    ${libFmtLib}
    URL             https://github.com/fmtlib/fmt/archive/refs/tags/8.1.1.tar.gz
    URL_MD5         fe7f1585844b14c647bd332ad5562832
#    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
#    GIT_TAG         8.1.1
    GIT_SHALLOW     1
    SOURCE_DIR      ${FETCHCONTENT_BASE_DIR}/fmtlib-src
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${INSTALL_BASE_DIR}/${libFmtLib}
        -DFMT_DOC=OFF
        -DFMT_TEST=OFF
        -Wno-dev
    TEST_COMMAND ""
  )
include_directories(${fmtlib_HEADER} ${fmtlib_BINARY_DIR})
