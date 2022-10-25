#include "Tools/Logger.h"

#include <stdio.h>
#include <sstream>
#include <execinfo.h>
#include <signal.h>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h> // support for basic file logging
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h> // support for rotating file logging
#include <spdlog/sinks/stdout_color_sinks.h>

#include <rapidjson/document.h>



namespace libapp {


const std::string NAME_LOGGER { "daily_logger" };
std::string FILENAME_LOGGER { "logs/daily.txt" };

// std::shared_ptr<spdlog::logger> _log = spdlog::daily_logger_mt<spdlog::async_factory>(NAME_LOGGER, "logs/daily.txt",1, 30);
bool __initializedLog { false };
std::shared_ptr<spdlog::logger> _log = spdlog::stdout_color_mt(NAME_LOGGER);


/**
 * @brief Set the Level Logger object
 * 
 * @param strLevel Valores aceitos: trace, debug, info, warn, err, critical
 */
void SetLevelLogger(const char* strLevel)
{
    _log->set_level( spdlog::level::from_str(strLevel) );
}

/**
 * @brief Set the Max Size Logger object
 * 
 * @param size_max_mb 
 */
void SetRotateLogger(const size_t& size_max_mb, const size_t& size_max_files)
{
    constexpr const size_t FATOR_CONVERSAO_MB{1024 * 1024};

    const size_t max_size = size_max_mb * FATOR_CONVERSAO_MB;

    using namespace std::string_literals;
    
    if ( spdlog::get(NAME_LOGGER) != nullptr ) {
        spdlog::details::registry::instance().drop(NAME_LOGGER);
    }
    std::shared_ptr<spdlog::logger> logger;
    
    if( size_max_files != 0 ) {
        logger = spdlog::rotating_logger_mt(NAME_LOGGER, FILENAME_LOGGER, max_size, size_max_files, true);
    }
    else {
        logger = spdlog::daily_logger_mt<spdlog::async_factory>(NAME_LOGGER, FILENAME_LOGGER,1, 30);
    }
   
    logger->set_level(_log->level());
    
    // _log->swap(*logger);
    _log = logger;
    __initializedLog = true;
}

/**
 * @brief 
 * 
 * @param config 
 */
void InitLogger()
{
    if( ! __initializedLog ) {
        SetRotateLogger( 0, 0 );
    }
    spdlog::flush_every(std::chrono::seconds(1));
    
    signal(SIGSEGV, print_trace);   /* 11 - Invalid access to storage.  */
    // signal(SIGINT, print_trace);    /* 2 - Interactive attention signal.  */
    signal(SIGILL, print_trace);    /* 4 - Illegal instruction.  */
    signal(SIGABRT, print_trace);   /* 6 - Abnormal termination.  */
    signal(SIGFPE, print_trace);    /* 8 - Erroneous arithmetic operation.  */

}

void print_trace(int sig)
{
    char** strings;
    size_t i, size;
    enum Constexpr { MAX_SIZE = 1024 };
    void* array[MAX_SIZE];

    size = backtrace(array, MAX_SIZE);
    strings = backtrace_symbols(array, size);

    std::stringstream ss("");

    for (i = 0; i < size; i++)
        ss << strings[i] << std::endl;

    _log->critical(fmt::format("** - Received Signal {} - {}\n{}", sig, strsignal(sig), ss.str()));

    puts("");
    free(strings);

    exit(EXIT_FAILURE);
}

// /**
//  * @brief Carrega o arquivo de configuracao (formato JSON).
//  * 
//  * @param config 
//  * @param FILE_NAME 
//  * @return size_t 
//  */
// size_t loadConfigFromJson(rapidjson::Document& config, const std::string& FILE_NAME )
// {
//     return loadConfigFromJson(&config, FILE_NAME);
// }

}
