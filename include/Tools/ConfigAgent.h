#ifndef __CONFIG_AGENT_H__
#define __CONFIG_AGENT_H__


#include <string>
#include <functional>
#include <vector>
#include <filesystem>
#include <rapidjson/document.h>


namespace libapp {


class IResourceAgent {
public:
    virtual void reload(rapidjson::Document*) = 0;
};


class ConfigAgent
{
public:
    ConfigAgent(rapidjson::Document* properties, const std::string& path_file);

    // ConfigAgent* bind( IResourceAgent* ptr ) { resources.emplace_back( ptr ); return this; }
    ConfigAgent* bindConfig( IResourceAgent* ptr );
    ConfigAgent* bindScheduler( IResourceAgent* ptr );

    void run();
    void refresh();

private:

    bool loadConfig();

    rapidjson::Document*     ptr_config_ { nullptr };
    const std::string               path_file_;

    std::vector<IResourceAgent* >           resources;
    IResourceAgent*                         resource_scheduler_ { nullptr };
    std::filesystem::file_time_type         last_refresh_ {};
};

}

#endif // __CONFIGRELOADAGENT_H__
