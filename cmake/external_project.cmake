include(ExternalProject)

set(FETCHCONTENT_BASE_DIR  ${CMAKE_BINARY_DIR}/_deps)
set(INSTALL_BASE_DIR  ${CMAKE_BINARY_DIR}/_install)


########################

INCLUDE( ./Ext/FindFmtLib )
INCLUDE( ./Ext/FindSpdlog )
INCLUDE( ./Ext/FindSewenewRedis++ )
INCLUDE( ./Ext/FindPocoProject )

# Mostra as variaveis de ambiente geradas pelo CMake
# include(cmake/PrintEnv.cmake)
# print_env_cmake()
