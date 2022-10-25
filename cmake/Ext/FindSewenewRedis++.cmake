include(ExternalProject)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)

set(libRedisCPP redis_plus_plus)

set(redis_plus_plus_HEADER ${INSTALL_BASE_DIR}/${libRedisCPP}/include)
set(redis_plus_plus_BINARY_DIR ${INSTALL_BASE_DIR}/${libRedisCPP}/lib)

set(redis_plus_plus_LIB ${redis_plus_plus_BINARY_DIR}/libredis++.so)  

message("==> Find Project REDIS-PLUS-PLUS ")


ExternalProject_Add(
    ${libRedisCPP}
    GIT_REPOSITORY      https://github.com/sewenew/redis-plus-plus.git
    GIT_TAG             1.3.3
    PREFIX              ${libRedisCPP}
    SOURCE_DIR          ${FETCHCONTENT_BASE_DIR}/${libRedisCPP}-src
    CMAKE_ARGS 
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCMAKE_INSTALL_PREFIX=${INSTALL_BASE_DIR}/${libRedisCPP}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -Wno-dev
    TEST_COMMAND ""                      
    BUILD_COMMAND       make -j8 -s
    UPDATE_DISCONNECTED 1
  )

include_directories(${redis_plus_plus_HEADER} ${redis_plus_plus_BINARY_DIR})
