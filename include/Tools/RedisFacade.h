#ifndef __Tools_REDISFACADE_H__
#define __Tools_REDISFACADE_H__

#include <string>
#include <unordered_set>
#include <any>
#include <vector>

#include <sw/redis++/redis++.h>
#include "Tools/RedisClientStorage.h"


namespace libapp {
    
class RedisFacade {
private:
    std::unique_ptr<sw::redis::Redis> redis_;
    std::string systemName_;

public:
    enum XTrimStrategyEnum {
        eXTrimStrategyMAXLEN,
        eXTrimStrategyMINID
    };

    typedef std::unordered_set<std::string>  ARRAY_KEYS;
    typedef std::unordered_map<std::string, std::string>  HASH_MAP;
    typedef std::vector< std::unordered_map< std::string, std::any >>  XINFO_GROUP_TYPE;
    typedef std::unordered_map< std::string, std::any >  XINFO_STREAM_TYPE;

    static void print_reply(const sw::redis::ReplyUPtr& ptr) { print_reply( ptr.get() ); }
    static void print_reply(sw::redis::ReplyUPtr::element_type* ptr, const std::string& ident = "");
    static void parse_xinfo(sw::redis::ReplyUPtr::element_type* ptr, XINFO_GROUP_TYPE& output);
    static void parse_xinfo(sw::redis::ReplyUPtr::element_type* ptr, XINFO_STREAM_TYPE& output);


    RedisFacade(const std::string& uriRedis);
    RedisFacade(const std::string& host, const int port);

    sw::redis::Redis* operator->() const noexcept { return redis_.get() ; }
    sw::redis::Redis* get() const noexcept { return redis_.get() ; }
    
    RedisFacade* setName(const std::string& systemname);


    RedisFacade* scanAll(ARRAY_KEYS& keys);
    RedisFacade* separete_for_types(const ARRAY_KEYS& keys, ARRAY_KEYS& keys_str, ARRAY_KEYS& keys_hash, ARRAY_KEYS& keys_zset);

    bool get(const std::string& key, std::string& value);
    bool set(const std::string& key, const std::string& value);
    bool setEx(const std::string& key, const std::string& value, const std::chrono::milliseconds &ttl, sw::redis::UpdateType type);
    bool hgetall(const std::string& key, std::unordered_map<std::string, std::string>& output);
    bool zgetall(const std::string& key, std::vector<std::pair<std::string, double>>& output);

    bool zadd(const std::string& key, const std::unordered_map<std::string, double>::iterator first, const std::unordered_map<std::string, double>::iterator  second, sw::redis::UpdateType  utype) ;

    bool hset(const std::string& keyName, const std::string& fieldName, const std::string& value );
    bool hset(const std::string& keyName, const HASH_MAP::iterator first, const HASH_MAP::iterator last );
    bool xinfoGroups(const std::string& keyName, XINFO_GROUP_TYPE& output);
    bool xinfoStream(const std::string& keyName, XINFO_STREAM_TYPE& output);
    bool xtrim(const std::string& keyName, const XTrimStrategyEnum& strategy, const bool& approx, const std::string& threshold);
};

}

#endif // __REDISFACADE_H__