
message("==> Find Project rapidjson")


SET(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "...")
SET(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "...")
SET(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "...")
SET(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "...")

FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY      https://github.com/Tencent/rapidjson.git
    GIT_TAG             v1.1.0
)
FetchContent_MakeAvailable(rapidjson)

include_directories( ${rapidjson_SOURCE_DIR}/include/ )
