cmake_minimum_required(VERSION 3.0.0)

# Policies
cmake_policy(SET CMP0076 NEW)  # Tell cmake to convert target_sources input from relative path to absolute path
cmake_policy(SET CMP0077 NEW)  # Tell cmake to allow variables set through "set" and "option" to be compatible with each other
cmake_policy(SET CMP0048 NEW)  # Tell cmake to use VERSION parameter in project() for all PROJECT_VERSION_* variables
cmake_policy(SET CMP0092 NEW)  # Tell cmake not to include warning parameters by default in CMAKE_<LANG>_FLAGS variables

## Default Policies
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW) # Tell CMake to use the new policy for CMP0048. (Due to zlib not defining it)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) # Tell CMake to use the new policy for CMP0077. (Due to fftw not defining it)


# # 
# # Project options
# # 
#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")


set(DEFAULT_PROJECT_OPTIONS
    DEBUG_POSTFIX             "d"
    CXX_STANDARD              20 # Not available before CMake 3.1; see below for manual command line argument addition
    LINKER_LANGUAGE           "CXX"
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET     "hidden"
    CXX_EXTENSIONS            Off
)


set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra -g")
#set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -Wall -Wextra -Werror -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2 -g -Wall -Wextra" )

project(${project_NAME}  TRobot )
message("-- Project name ${CMAKE_PROJECT_NAME}")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")



set(LIBCPP_PRJ_INCLUDE    ${CMAKE_SOURCE_DIR}/include)
include_directories( ${LIBCPP_PRJ_INCLUDE} )


include(CTest)
enable_testing()

set(BUILD_SHARED_LIBS  ON)
include(${CMAKE_MODULE_PATH}/external_project.cmake)

set (LIBCPP_PRJ_BINARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set (LIBCPP_PRJ_LIBRARIES  LibTRTools LibTRWeb LibBroker )
include_directories(${LIBCPP_PRJ_BINARY_DIR})

add_subdirectory(Modules)
add_subdirectory(src)
add_subdirectory(tmp-src)
add_subdirectory(unitTest)

#add_subdirectory(samples)

