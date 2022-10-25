# Copyright 2007 - 2021, Alan Antonuk and the rabbitmq-c contributors.
# SPDX-License-Identifier: mit


project(sampleRPC)
set( TARGET   rpc-server )

set(LIBRABBITMQ_INCLUDE    ${CMAKE_SOURCE_DIR}/include/librabbitmq)



# add_library(examples-common OBJECT)
# target_sources(examples-common PRIVATE
#   utils.h
#   utils.c)
# target_sources(examples-common PRIVATE unix/platform_utils.c)


set(_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}/)


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
# target_link_libraries(examples-common PRIVATE rabbitmq::rabbitmq)


file(GLOB  MAIN_SOURCE_CPP_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable( ${TARGET}  ${MAIN_SOURCE_CPP_FILES}  )

target_link_libraries( ${TARGET} PRIVATE  rabbitmq::rabbitmq ${LIBCPP_PRJ_LIBRARIES} )

set_target_properties( ${TARGET}  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${CMAKE_SOURCE_DIR}/bin/Debug/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin/Release/${PROJECT_NAME}/"
)
