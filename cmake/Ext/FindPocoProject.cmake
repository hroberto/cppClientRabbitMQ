include(ExternalProject)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)


message("==> Find Project POCO PROJECT ")


set(libPocoProject pocoproject)
set(pocobase_HEADER ${INSTALL_BASE_DIR}/${libPocoProject}/include)
set(pocobase_BINARY_DIR ${INSTALL_BASE_DIR}/${libPocoProject}/lib)


if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set(pocobase_LIB  ${pocobase_BINARY_DIR}/libPocoCrypto.a; ${pocobase_BINARY_DIR}/libPocoEncodings.a; ${pocobase_BINARY_DIR}/libPocoFoundation.a;
                      ${pocobase_BINARY_DIR}/libPocoJSON.a; ${pocobase_BINARY_DIR}/libPocoJWT.a; ${pocobase_BINARY_DIR}/libPocoNet.a;
                      ${pocobase_BINARY_DIR}/libPocoSevenZip.a; ${pocobase_BINARY_DIR}/libPocoUtil.a; ${pocobase_BINARY_DIR}/libPocoXML.a;
                      ${pocobase_BINARY_DIR}/libPocoZip.a; ${pocobase_BINARY_DIR}/libPocoFoundation.a )
endif( CMAKE_BUILD_TYPE STREQUAL "Release" )
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(pocobase_LIB  ${pocobase_BINARY_DIR}/libPocoCryptod.a; ${pocobase_BINARY_DIR}/libPocoEncodingsd.a; ${pocobase_BINARY_DIR}/libPocoFoundationd.a;
                      ${pocobase_BINARY_DIR}/libPocoJSONd.a; ${pocobase_BINARY_DIR}/libPocoJWTd.a; ${pocobase_BINARY_DIR}/libPocoNetd.a;
                      ${pocobase_BINARY_DIR}/libPocoSevenZipd.a; ${pocobase_BINARY_DIR}/libPocoUtild.a; ${pocobase_BINARY_DIR}/libPocoXMLd.a;
                      ${pocobase_BINARY_DIR}/libPocoZipd.a; ${pocobase_BINARY_DIR}/libPocoFoundationd.a )
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )



# https://github.com/ros2/poco_vendor/blob/master/CMakeLists.txt

ExternalProject_Add(
    ${libPocoProject}
    GIT_SHALLOW     1
    PREFIX          ${libPocoProject}-build
    SOURCE_DIR      ${FETCHCONTENT_BASE_DIR}/${libPocoProject}-src
    URL             https://github.com/pocoproject/poco/archive/poco-1.11.1-release.tar.gz
    URL_MD5         a96210b60a675c4a8183ad1f2099d549
    TIMEOUT         600
    CMAKE_ARGS 
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCMAKE_INSTALL_PREFIX=${INSTALL_BASE_DIR}/${libPocoProject}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DBUILD_SHARED_LIBS:BOOL=OFF
        # -DPOCO_MT:BOOL=OFF
        -DENABLE_XML:BOOL=OFF
        -DENABLE_JSON:BOOL=OFF
        -DENABLE_MONGODB:BOOL=OFF
        -DENABLE_REDIS:BOOL=OFF
        -DENABLE_PDF:BOOL=OFF
        -DENABLE_FOUNDATION:BOOL=ON
        -DENABLE_UTIL:BOOL=ON
        -DENABLE_NET:BOOL=ON
        -DENABLE_NETSSL:BOOL=OFF
        -DENABLE_NETSSL_WIN:BOOL=OFF
        -DENABLE_CRYPTO:BOOL=ON
        -DENABLE_ACTIVERECORD:BOOL=OFF
        -DENABLE_ACTIVERECORD_COMPILER:BOOL=OFF
        -DENABLE_DATA:BOOL=OFF
        -DENABLE_DATA_SQLITE:BOOL=OFF
        -DENABLE_DATA_MYSQL:BOOL=OFF
        -DENABLE_DATA_ODBC:BOOL=OFF
        -DENABLE_SEVENZIP:BOOL=ON
        -DENABLE_ZIP:BOOL=ON
        -DENABLE_APACHECONNECTOR:BOOL=OFF
        -DENABLE_CPPPARSER:BOOL=OFF
        -DENABLE_POCODOC:BOOL=OFF
        -DENABLE_PAGECOMPILER:BOOL=OFF
        -DENABLE_PAGECOMPILER_FILE2PAGE:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DENABLE_PROGRAMS:BOOL=OFF
        -DENABLE_TESTING:BOOL=OFF
    TEST_COMMAND ""
)

include_directories( ${pocobase_HEADER} )
