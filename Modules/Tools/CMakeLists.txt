cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project( Library )
set ( TARGET  LibTRTools )

project(Library  VERSION  1.0.0   LANGUAGES C CXX )
message("|-- Project name ${CMAKE_PROJECT_NAME}")

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBCPP_PRJ_BINARY_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBCPP_PRJ_BINARY_DIR} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )


file(GLOB  SOURCE_CPP_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

#add_executable(${TARGET}  ${MAIN_SOURCE_CPP_FILES} )
add_library( ${TARGET} STATIC ${SOURCE_CPP_FILES} )

set(BUILD_SHARED_LIBS  ON)
include( ${CMAKE_MODULE_PATH}/dependencies.cmake )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


# ##############
# ## CONFIG
# execute_process( COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/config/${TARGET}/ )
# execute_process( COMMAND mkdir -p ${_OUTPUT_DIRECTORY} )

# file(GLOB CONFIG_FILES  ${CMAKE_SOURCE_DIR}/config/${TARGET}/*)
# foreach(config_file IN ITEMS  ${CONFIG_FILES} )
#     execute_process( COMMAND ln -sf ${config_file}  ${_OUTPUT_DIRECTORY} )
# endforeach()

# include(${CMAKE_HOME_DIRECTORY}/cmake/PrintEnv.cmake)
# print_env_cmake()