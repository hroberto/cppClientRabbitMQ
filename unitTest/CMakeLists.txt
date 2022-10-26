cmake_minimum_required(VERSION 3.5)


project(Test)
set (TARGET  unitTest)

include(CTest)
set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

file(GLOB CPP_FILES  *.cpp)

add_executable(${TARGET} ${CPP_FILES})

set(_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}/)
set_target_properties( ${TARGET}  PROPERTIES

RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}/"
RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}/"
RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}/"
)

target_link_libraries(${TARGET} gtest gmock_main rabbitmq::rabbitmq ${LIBCPP_PRJ_LIBRARIES} )
add_test(NAME project_test COMMAND ${TARGET})

include(${CMAKE_MODULE_PATH}/dependencies.cmake)
