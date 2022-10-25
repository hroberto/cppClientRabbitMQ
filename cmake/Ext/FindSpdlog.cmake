include(ExternalProject)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)



set(libSpdLog spdlog)


set(spdlog_HEADER ${INSTALL_BASE_DIR}/${libSpdLog}/include)
set(spdlog_BINARY_DIR ${INSTALL_BASE_DIR}/${libSpdLog}/lib)


if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set(spdlog_LIB   ${spdlog_BINARY_DIR}/libspdlog.a)
endif( CMAKE_BUILD_TYPE STREQUAL "Release" )
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(spdlog_LIB   ${spdlog_BINARY_DIR}/libspdlogd.a)
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )



message("==> Find Project spdlog  ")

ExternalProject_Add(
    ${libSpdLog}
    URL             https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.tar.gz
    URL_MD5         cee7f3d31178a00791d7a22c6738df6d
#    GIT_REPOSITORY      https://github.com/gabime/spdlog.git
#    GIT_TAG             v1.9.2
    GIT_SHALLOW 1
    PREFIX      ${libSpdLog}-build
    SOURCE_DIR          ${FETCHCONTENT_BASE_DIR}/spdlog-src
    CMAKE_ARGS  
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${INSTALL_BASE_DIR}/${libSpdLog}
        -DSPDLOG_BUILD_SHARED=OFF
        -DSPDLOG_BUILD_EXAMPLES=OFF
        -DSPDLOG_BUILD_BENCH=OFF
        -DSPDLOG_BUILD_TESTS=OFF
        -Wno-dev
    TEST_COMMAND ""
  )
include_directories(${spdlog_HEADER} ${spdlog_BINARY_DIR})
