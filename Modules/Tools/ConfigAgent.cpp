#include "Tools/ConfigAgent.h"


#include <thread>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <rapidjson/error/en.h>

#include "Tools/Logger.h"
#include "Tools/JSonUtils.h"
#include "Tools/SystemUtils.h"


using namespace  std::chrono_literals;


namespace libapp {


ConfigAgent::ConfigAgent(rapidjson::Document* ptr, const std::string& path_file)
: ptr_config_{ ptr }, path_file_{ path_file }
{
    assert( ptr_config_ != nullptr );
    loadConfig();
}


void ConfigAgent::run()
{
    while( 1 ) {
        this->refresh();
        std::this_thread::sleep_for(20s);
    }
}



bool ConfigAgent::loadConfig() 
{
    if ( ! std::filesystem::exists(path_file_)) {
        return false;
    }

    libapp::loadConfigFromJson(ptr_config_, path_file_);

    if ( ptr_config_->HasParseError() ) {
        _log->error("ConfigAgent | Error parser JSON: \"{}:= {}\"", (unsigned)ptr_config_->GetErrorOffset(), rapidjson::GetParseError_En(ptr_config_->GetParseError()) );
        return false;
    }

    if (libapp::HAS_CONFIG(*ptr_config_, "logger", "level")) {
        libapp::SetLevelLogger( libapp::GET_CONFIG_STRING(*ptr_config_,"logger","level", "info") );
        libapp::SetRotateLogger( 
                libapp::GET_CONFIG_INT(*ptr_config_, "logger", "max_size", 250 ), 
                libapp::GET_CONFIG_INT(*ptr_config_, "logger", "max_files", 4 ));
    }
    
    return true;
}



void ConfigAgent::refresh()
{
    if ( ! std::filesystem::exists(path_file_)) {
        return;
    }

    auto last_write_file = std::filesystem::last_write_time(path_file_);

    if ( last_write_file != last_refresh_ ) {

        if ( ! this->loadConfig() ) {
            return;
        }

        for( const auto it : resources ) {
            it->reload(ptr_config_);
        }

        last_refresh_ = last_write_file;
        _log->info("ConfigAgent | Configuracao carregada. " );

        if( resource_scheduler_ != nullptr ) {
            resource_scheduler_->reload(ptr_config_);
            resource_scheduler_ = nullptr;
        }
    }
}


ConfigAgent* ConfigAgent::bindConfig(IResourceAgent* ptr)
{
    assert(ptr != nullptr);

    resources.emplace_back(ptr);    
    if (ptr_config_ != nullptr ) ptr->reload(ptr_config_);

    return this;
}


ConfigAgent* ConfigAgent::bindScheduler( IResourceAgent* ptr )
{
    assert(ptr != nullptr);
    assert(resource_scheduler_ == nullptr);

    resource_scheduler_ = ptr;

    if (std::filesystem::last_write_time(path_file_) == last_refresh_ ) {
        resource_scheduler_->reload(ptr_config_);
        resource_scheduler_ = nullptr;
    }

    return this;
}

}
