#ifndef __LIB_MISC_LOGGER_H__
#define __LIB_MISC_LOGGER_H__

#include <spdlog/spdlog.h>


namespace libapp {

extern std::shared_ptr<spdlog::logger> _log;

extern std::string FILENAME_LOGGER;


void InitLogger();
void SetLevelLogger(const char* strLevel);
void SetRotateLogger(const size_t& size_max_mb, const size_t& size_max_files);
void print_trace(int sig);
// size_t loadConfigFromJson(rapidjson::Document& config, const std::string& FILE_NAME );
// size_t loadConfigFromJson(rapidjson::Document* ptrConfig, const std::string& FILE_NAME );


}

#endif //__LIB_MISC_LOGGER_H__