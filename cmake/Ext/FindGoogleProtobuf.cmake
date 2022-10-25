# include(ExternalProject)

# set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
# set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)



# INCLUDE(  ${CMAKE_ROOT}/Modules/Genial/FindZLib.cmake )

# message("==> Find Project GOOGLE PROTOBUF ")


# set(PROTOBUF_TARGET Protobuf)
# # set(PROTOBUF_VERSION "3.19.4" )

# # set( protobuf_PREFIX     ${CMAKE_BINARY_DIR} )
# set( protobuf_PREFIX     ${INSTALL_BASE_DIR}/${PROTOBUF_TARGET} )
# set( protobuf_HEADER         ${protobuf_PREFIX}/include )
# set( protobuf_BINARY_DIR     ${protobuf_PREFIX}/lib )

# # set ( Protobuf_USE_STATIC_LIBS ON )
# set( PROTOBUF_DIR            ${protobuf_PREFIX} )
# set( ${PROTOBUF_TARGET}_DIR  ${protobuf_PREFIX} )


# if ( NOT PROTOBUF_FOUND )
#     ExternalProject_Add(
#         ${PROTOBUF_TARGET}
#         GIT_SHALLOW     1
#         URL             https://github.com/protocolbuffers/protobuf/releases/download/v3.20.0/protobuf-cpp-3.20.0.tar.gz
#         URL_MD5         1db6ee0c403452514fd87e5c1a0f8405
#         TIMEOUT         600
#         PREFIX          ${PROTOBUF_TARGET}-build
#         SOURCE_DIR      ${FETCHCONTENT_BASE_DIR}/${PROTOBUF_TARGET}-src
#         CONFIGURE_COMMAND ${FETCHCONTENT_BASE_DIR}/${PROTOBUF_TARGET}-src/configure --prefix=${protobuf_PREFIX}  
#         BUILD_COMMAND       make -j8 -s --no-print-directory
#         CMAKE_ARGS 
#             -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#             -DCMAKE_INSTALL_PREFIX=${protobuf_PREFIX}
#             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#             -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
#             -DBUILD_SHARED_LIBS:BOOL=OFF
#             -DBUILD_PYTHON=OFF 
#             -DBUILD_EXAMPLES=OFF
#             -Dprotobuf_BUILD_TESTS=OFF            
#             -DCMAKE_SKIP_RPATH:BOOL=OFF
#             -DPARAVIEW_USE_VTKM:BOOL=OFF
#             -DPARAVIEW_USE_QT:BOOL=ON
#             -DPARAVIEW_USE_PYTHON:BOOL=ON
#             -DPARAVIEW_PYTHON_SITE_PACKAGES_SUFFIX=%{_lib}/python%{py3_ver}/site-packages/paraview
#             -DPARAVIEW_ENABLE_WEB:BOOL=ON
#             -DVTK_WRAP_PYTHON:BOOL=ON
#             -DVTK_OPENGL_HAS_OSMESA:BOOL=OFF
#             -DBUILD_TESTING:BOOL=OFF
#             -DQtTesting_INSTALL_NO_DEVELOPMENT:BOOL=ON
#             -DVTK_BUILD_QT_DESIGNER_PLUGIN:BOOL=OFF
#             -DPARAVIEW_INSTALL_DEVELOPMENT_FILES:BOOL=ON
#             -DPARAVIEW_BUILD_WITH_EXTERNAL:BOOL=ON
#             -DVTK_MODULE_USE_EXTERNAL_VTK_gl2ps=%{?with_gl2ps:ON}%{!?with_gl2ps:OFF}
#             -DVTK_MODULE_USE_EXTERNAL_VTK_libharu=%{?with_haru:ON}%{!?with_haru:OFF}
#             -DVTK_MODULE_USE_EXTERNAL_VTK_pugixml=%{?with_pugixml:ON}%{!?with_pugixml:OFF}
#             -Wno-dev
#             -Dprotobuf_WITH_ZLIB:BOOL=ON
#             -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}
#             -DZLIB_LIB=${ZLIB_LIB}     
#         TEST_COMMAND ""
#     )
# # else()
# #     include_directories( ${INSTALL_BASE_DIR} )
# #     find_package( PROTOBUF REQUIRED )
# #     if( PROTOBUF_FOUND )
# #         add_definitions(${PROTOBUF_LIB_DIAGNOSTIC_DEFINITIONS})
# #     endif()
# #     target_link_libraries( ${TARGET} ${PROTOBUF_LIBRARIES} )
# endif()

# include_directories( ${protobuf_HEADER} ${protobuf_BINARY_DIR} )
# include_directories( ${protobuf_PREFIX} )


# set( Protobuf_DIR  ${protobuf_PREFIX}  )
# set( Protobuf_INCLUDE_DIR  ${protobuf_HEADER} )

# set( Protobuf_SRC_ROOT_FOLDER ${FETCHCONTENT_BASE_DIR}/${PROTOBUF_TARGET}-src )


# set(PROTOBUF_FOUND TRUE)
# set(PROTOBUF_GENERATE_CPP_APPEND_PATH TRUE)
# set(PROTOBUF_BINARY_DIR ${protobuf_BINARY_DIR})
# set(PROTOBUF_INCLUDE_DIR ${protobuf_HEADER})
# set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIR})
# set(PROTOBUF_LIBRARY   "${PROTOBUF_BINARY_DIR}/libprotobuf.a")
# set(PROTOBUF_LIBRARIES  "${PROTOBUF_LIBRARY};-pthread" )
# set(PROTOBUF_LITE_LIBRARY   "${PROTOBUF_BINARY_DIR}/libprotobuf-lite.a")
# set(PROTOBUF_LITE_LIBRARIES  "${PROTOBUF_LIBRARY};-pthread" )
# set(PROTOBUF_PROTOC_EXECUTABLE   "${protobuf_PREFIX}/bin/protoc")
# set(PROTOBUF_PROTOC_LIBRARY   "${PROTOBUF_BINARY_DIR}/libprotoc.a")
# set(PROTOBUF_PROTOC_LIBRARIES   "${PROTOBUF_PROTOC_LIBRARY};-pthread" )

# set(Protobuf_FOUND TRUE)
# set(Protobuf__GENERATE_CPP_APPEND_PATH TRUE)
# set(Protobuf_BINARY_DIR ${protobuf_PREFIX}/lib)
# set(Protobuf_INCLUDE_DIR ${protobuf_PREFIX}/include)
# set(Protobuf_INCLUDE_DIRS ${Protobuf_INCLUDE_DIR})
# set(Protobuf_PROTOC_EXECUTABLE   "${protobuf_PREFIX}/bin/protoc")
# set(Protobuf_LIBRARY   "${Protobuf_BINARY_DIR}/libprotobuf.a")
# set(Protobuf_LIBRARIES  "${Protobuf_LIBRARY};-pthread" )
# set(Protobuf_LITE_LIBRARY   "${Protobuf_BINARY_DIR}/libprotobuf-lite.a")
# set(Protobuf_LITE_LIBRARIES  "${Protobuf_LIBRARY};-pthread" )
# set(Protobuf_PROTOC_LIBRARY   "${Protobuf_BINARY_DIR}/libprotoc.a")
# set(Protobuf_PROTOC_LIBRARIES   "${Protobuf_PROTOC_LIBRARY};-pthread" )


# if( CMAKE_BUILD_TYPE STREQUAL "Release" )
#     set(PROTOBUF_LIBRARY_RELEASE  ${PROTOBUF_LIBRARY} )
#     set(PROTOBUF_LITE_LIBRARY_RELEASE  ${PROTOBUF_LITE_LIBRARY} )
#     set(PROTOBUF_PROTOC_LIBRARY_RELEASE  ${PROTOBUF_LIBRARY} )
#     set(Protobuf_LIBRARY_RELEASE  ${Protobuf_LIBRARY} )
#     set(Protobuf_LITE_LIBRARY_RELEASE  ${Protobuf_LITE_LIBRARY} )
#     set(Protobuf_PROTOC_LIBRARY_RELEASE  ${Protobuf_LIBRARY} )
# endif( CMAKE_BUILD_TYPE STREQUAL "Release" )
# if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
#     set(PROTOBUF_LIBRARY_DEBUG  ${PROTOBUF_LIBRARY} )
#     set(PROTOBUF_LITE_LIBRARY_DEBUG  ${PROTOBUF_LITE_LIBRARY} )
#     set(PROTOBUF_PROTOC_LIBRARY_DEBUG  ${PROTOBUF_LIBRARY} )
#     set(Protobuf_LIBRARY_DEBUG  ${Protobuf_LIBRARY} )
#     set(Protobuf_LITE_LIBRARY_DEBUG  ${Protobuf_LITE_LIBRARY} )
#     set(Protobuf_PROTOC_LIBRARY_DEBUG  ${Protobuf_LIBRARY} )
# endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )


# # message( "Protobuf_SRC_ROOT_FOLDER  ==  ${Protobuf_SRC_ROOT_FOLDER} " )
# # INCLUDE (${FETCHCONTENT_BASE_DIR}/${PROTOBUF_TARGET}-src/cmake/protobuf-module.cmake.in )
