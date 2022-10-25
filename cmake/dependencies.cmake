include(FetchContent)
#include(${CMAKE_HOME_DIRECTORY}/cmake/DownloadProject.cmake)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)

set(FETCHCONTENT_QUIET ON)


INCLUDE( Ext/FindGoogleTest )
INCLUDE( Ext/FindRapidJSon )


########################

message("######################")
message("-- Project Threads")
message("----------------------")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries( ${TARGET} Threads::Threads )


# <------------ add library FMT --------------->
find_package(fmt CONFIG)


# <------------ add library OpenSSL --------------->
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    add_definitions(${OPENSSL_LIB_DIAGNOSTIC_DEFINITIONS})
endif()


########################
# ##  HIREDIS
find_package(HIREDIS REQUIRED)
if(HIREDIS_FOUND)
    add_definitions(${HIREDIS_LIB_DIAGNOSTIC_DEFINITIONS})
endif()


# <------------ add Boost dependency --------------->
message("######################")
message("-- Project BOOST ")
find_package(Boost REQUIRED COMPONENTS regex program_options )
if(Boost_FOUND)
    add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
endif()


# <------------ add spdlog dependency --------------->
message("######################")
message("-- Project spdlog ")
find_package(spdlog REQUIRED)
if(spdlog_FOUND)
    add_definitions(${spdlog_LIB_DIAGNOSTIC_DEFINITIONS})
endif()

include_directories( ${redis_plus_plus_HEADER} )



# # # <------------ add QuickFix dependency --------------->
# find_package(QuickFix REQUIRED)
# if(QUICKFIX_FOUND)
#     add_definitions(${QUICKFIX_LIB_DIAGNOSTIC_DEFINITIONS})
# endif()


target_link_libraries( ${TARGET} 
        Threads::Threads
        ${OPENSSL_LIBRARIES}
        ${rapidjson}
        ${spdlog_LIB}
        ${fmtlib_LIB}
        ${Boost_LIBRARIES}
        ${pocobase_LIB}
        ${QUICKFIX_LIBRARIES}
        ${Protobuf_LIBRARIES}
        ${redis_plus_plus_LIB}  ${HIREDIS_LIBRARIES} 
        ${googletest}
)


# include(${CMAKE_HOME_DIRECTORY}/cmake/PrintEnv.cmake)
# print_env_cmake()
