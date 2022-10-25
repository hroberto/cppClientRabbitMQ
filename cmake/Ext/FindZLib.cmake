# include(ExternalProject)

# set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
# set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)


# set(libZLib zlib)

# # https://github.com/madler/zlib/tree/v1.2.12/examples
# ExternalProject_Add(${libZLib}
#     URL             https://github.com/madler/zlib/archive/v1.2.12.tar.gz
#     URL_MD5         db5b7326d4e0dbcbd1981b640d495c9b
#     TIMEOUT         600
#     SOURCE_DIR      ${FETCHCONTENT_BASE_DIR}/zlib-src
#     BINARY_DIR      zlib-build
#     INSTALL_DIR     ${INSTALL_BASE_DIR}/zlib
#     CMAKE_ARGS
#         -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#         -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#         -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#         -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
#         -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_BASE_DIR}/${libZLib}
#         -DZLIB_MANGLE_PREFIX:STRING=""
#   )
  
# set(zlib_DIR "${INSTALL_BASE_DIR}/zlib" CACHE PATH "zlib dir" FORCE)
# set(ZLIB_ROOT ${zlib_DIR} CACHE PATH "zlib root" FORCE)
# set(ZLIB_INCLUDE_DIR ${zlib_DIR}/include CACHE PATH "zlib include dir" FORCE)

# if(WIN32)
#     set(ZLIB_LIBRARY ${zlib_DIR}/lib/zlib.lib CACHE FILEPATH "zlib library" FORCE)
# else()
#     set(ZLIB_LIBRARY ${zlib_DIR}/lib/libz.a CACHE FILEPATH "zlib library" FORCE)
# endif()
# set( ZLIB_LIB  ${ZLIB_LIBRARY} )

# include_directories( ${FETCHCONTENT_BASE_DIR}/zlib-src ${FETCHCONTENT_BASE_DIR}/zlib-src/include ) 
