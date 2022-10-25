#include "Tools/RedisFacade.h"

#include <fmt/format.h>
#include <unordered_set>
#include <iostream>


namespace libapp {
    
/**
 * @brief Construct a new Redis Facade:: Redis Facade object
 * 
 * @param uriRedis 
 */
RedisFacade::RedisFacade(const std::string& uriRedis) 
{
    sw::redis::ConnectionOptions opts(uriRedis);
    redis_ = std::make_unique<sw::redis::Redis>(opts);
}

/**
 * @brief Construct a new Redis Facade:: Redis Facade object
 * 
 * @param host 
 * @param port 
 */
RedisFacade::RedisFacade(const std::string& host, const int port)
{
    const std::string uriRedis = fmt::format("tcp://{}:{}", host, port);

    sw::redis::ConnectionOptions opts(uriRedis);
    redis_ = std::make_unique<sw::redis::Redis>(opts);
}

/**
 * @brief Busca todas as chaves existentes no Redis.
 *        redis-cli 0  COUNT 10000 MATCH "[A-Za-z0-9]*_[A-Za-z0-9]*" TYPE string
 * 
 * @return std::unordered_set<std::string> 
 */
RedisFacade* RedisFacade::scanAll(ARRAY_KEYS& keys)
{
    const auto COUNT { 1000000LL };
    const auto PATTERN { "*" };
    auto cursor = 0LL;

    keys.clear();
    cursor = redis_->scan(cursor, PATTERN, COUNT, std::inserter(keys, keys.begin()));

    return this;
}

/**
 * @brief 
 * 
 * @param keys 
 * @param keys_str 
 * @param keys_hash 
 * @param keys_zset 
 * @return RedisFacade* 
 */
RedisFacade* RedisFacade::separete_for_types(const ARRAY_KEYS& keys, ARRAY_KEYS& keys_str, ARRAY_KEYS& keys_hash, ARRAY_KEYS& keys_zset)
{
    for (auto it_key : keys) {
        const auto type_key = redis_->type(it_key);

        if (type_key == "string")       keys_str.emplace(it_key);
        else if (type_key == "hash")    keys_hash.emplace(it_key);
        else if (type_key == "zset")    keys_zset.emplace(it_key);
    }

    return this;
}

bool RedisFacade::get(const std::string& key, std::string& value)
{
    sw::redis::OptionalString ret = redis_->get(key);

    if (ret.has_value()) value.assign( *ret );
    else value.clear();

    return (ret.has_value());
}


bool RedisFacade::set(const std::string& key, const std::string& value)
{
    bool sucess = redis_->set(key, value);
    return sucess;
}

bool RedisFacade::setEx(const std::string& key, const std::string& value, const std::chrono::milliseconds &ttl, sw::redis::UpdateType type)
{
    bool sucess = redis_->set(key, value, ttl, type);
    return sucess;
}


bool RedisFacade::hgetall(const std::string& key, std::unordered_map<std::string, std::string>& output)
{
    output.clear();
    redis_->hgetall(key, std::inserter(output, output.begin()));

    return (!output.empty());
}


bool RedisFacade::zgetall(const std::string& key, std::vector<std::pair<std::string, double>>& output)
{
    output.clear();

    std::vector<std::pair<std::string, double>> zset_result;
    redis_->zrangebyscore(key,
                        sw::redis::UnboundedInterval<double>{},  // (-inf, +inf)
                        std::back_inserter(output));

    return (!output.empty());
}


/**
 * @brief 
 * 
 * 
 * @param key 
 * @param first 
 * @param second 
 * @param utype 
 * @return true 
 * @return false 
 * 
 * @note redis_->zadd( keyRedis, scores.begin(), scores.end(), sw::redis::UpdateType::NOT_EXIST );
 */
bool RedisFacade::zadd(const std::string& key, const std::unordered_map<std::string, double>::iterator first, const std::unordered_map<std::string, double>::iterator last, sw::redis::UpdateType  utype) 
{
    bool sucess = redis_->zadd( key, first, last, utype );

    return sucess;
}



bool RedisFacade::hset(const std::string& keyName, const std::string& fieldName, const std::string& value )
{
    bool sucess = redis_->hset(keyName, HASH_MAP::value_type(fieldName, value));
    return sucess;
}


bool RedisFacade::hset(const std::string& keyName, const HASH_MAP::iterator first, const HASH_MAP::iterator last )
{
    bool sucess { true };

    std::for_each(first, last, [&](const HASH_MAP::value_type& value) {
        sucess = sucess && redis_->hset(keyName, value.first, value.second);
    });

    return sucess;
}


/**
 * @brief 
 * 
 * @param systemname 
 * @return RedisFacade* 
 */
RedisFacade*  RedisFacade::setName(const std::string& systemname)
{
    systemName_ = systemname;

    if( redis_.get() != nullptr ) {
        libapp::RedisClientStorage client(systemname, redis_.get());
    }

    return this;
}

void RedisFacade::print_reply(sw::redis::ReplyUPtr::element_type* ptr, const std::string& ident) 
{
    std::cout << ident
              << "Elements:= " << ptr->elements
              << " / integer := " << ptr->integer
              << " / len := " << ptr->len
              << " / type := " << ptr->type
              << " / is_array := " << sw::redis::reply::is_array(*ptr)
              << " / is_string := " << sw::redis::reply::is_string(*ptr)
              << " / is_integer := " << sw::redis::reply::is_integer(*ptr)
              << " / is_nil := " << sw::redis::reply::is_nil(*ptr);

    if (ptr->str != nullptr) {
        std::cout << " / str(p) := " << sw::redis::reply::parse<std::string>(*ptr);
    }
    else if (ptr->integer) {
        std::cout << " / integer := " << ptr->integer;
    }
    
    std::cout << std::endl;

    if (sw::redis::reply::is_array(*ptr)) {
        for (size_t i = 0; i < ptr->elements; ++i) {
            RedisFacade::print_reply(ptr->element[i], ident + "  ");
        }
    } 
    else if (ptr->element != nullptr) {
        RedisFacade::print_reply(ptr->element[0], ident + "  ");
    }
}


/**
 * @brief 
 * 
 * @param ptr 
 * @return const std::vector< std::unordered_map< std::string, std::any >>
 */
void RedisFacade::parse_xinfo(sw::redis::ReplyUPtr::element_type* ptr, RedisFacade::XINFO_GROUP_TYPE& result_xinfo) 
{
    result_xinfo.clear();

    assert( ptr != nullptr );
    assert( ptr->type == REDIS_REPLY_ARRAY );

    for (size_t idxMap{}; idxMap < ptr->elements; ++idxMap ) {
        sw::redis::ReplyUPtr::element_type* map_ptr = ptr->element[idxMap];

        assert( map_ptr != nullptr );
        assert( map_ptr->type == REDIS_REPLY_ARRAY );

        std::unordered_map< std::string, std::any > list;

        for (size_t idx{}; idx < map_ptr->elements; ++idx ) {
            sw::redis::ReplyUPtr::element_type* item_ptr_key = map_ptr->element[idx];
            sw::redis::ReplyUPtr::element_type* item_ptr_value = map_ptr->element[++idx];

            assert( item_ptr_key != nullptr );
            assert( item_ptr_value != nullptr );
            assert( item_ptr_key->type == REDIS_REPLY_STRING );
            assert( item_ptr_value->type == REDIS_REPLY_STRING || item_ptr_value->type == REDIS_REPLY_INTEGER );

            if( item_ptr_value->type == REDIS_REPLY_STRING ) {
                list.emplace(  item_ptr_key->str ,  std::string( item_ptr_value->str ) );
            }
            else if( item_ptr_value->type == REDIS_REPLY_INTEGER ) {
                list.emplace(  item_ptr_key->str ,  item_ptr_value->integer );
            }
        }

        result_xinfo.emplace_back( std::move( list ));
    }
}



/**
 * @brief 
 * 
 * @param ptr 
 * @return const std::vector< std::unordered_map< std::string, std::any >>
 */
void RedisFacade::parse_xinfo(sw::redis::ReplyUPtr::element_type* ptr, RedisFacade::XINFO_STREAM_TYPE& result_xinfo) 
{
    result_xinfo.clear();

    assert( ptr != nullptr );
    assert( ptr->type == REDIS_REPLY_ARRAY );

    for (size_t idx{}; idx < ptr->elements; ++idx ) {
        sw::redis::ReplyUPtr::element_type* item_ptr_key = ptr->element[idx];
        sw::redis::ReplyUPtr::element_type* item_ptr_value = ptr->element[++idx];

        assert( item_ptr_key != nullptr );
        assert( item_ptr_value != nullptr );
        assert( item_ptr_key->type == REDIS_REPLY_STRING );
        assert( item_ptr_value->type == REDIS_REPLY_STRING || item_ptr_value->type == REDIS_REPLY_ARRAY || item_ptr_value->type == REDIS_REPLY_INTEGER || item_ptr_value->type == REDIS_REPLY_NIL);

        if( item_ptr_value->type == REDIS_REPLY_STRING ) {
            result_xinfo.emplace(  item_ptr_key->str ,  std::string( item_ptr_value->str ) );
        }
        else if( item_ptr_value->type == REDIS_REPLY_INTEGER ) {
            result_xinfo.emplace(  item_ptr_key->str ,  item_ptr_value->integer );
        }
    }
}


bool xinfo(sw::redis::Redis* redis_ptr, const std::initializer_list<const char*>& CMD, auto& output)
{
    assert( redis_ptr != nullptr );

    sw::redis::ReplyUPtr reply;
    try {
        reply = redis_ptr->command(CMD.begin(), CMD.end());
        assert( reply );
    }
    catch(...) {}
    
    if (!reply) {
        return false;
    }

    // libapp::RedisFacade::print_reply( reply );

    libapp::RedisFacade::parse_xinfo( reply.get(), output );
    return true;
}

/**
 * @brief   Obtem as informacoes do Stream..
 *          Uso:
 *               std::any_cast<std::string>( result[0]["name"] )
 * 
 * @param keyName 
 * @param output  std::vector< std::unordered_map< std::string, std::any >>
 * @return true 
 * @return false 
 */
bool RedisFacade::xinfoGroups(const std::string& keyName, XINFO_GROUP_TYPE& output)
{
    assert( redis_ != nullptr );
    assert(redis_->type(keyName) == "stream" || redis_->type(keyName) == "none" );

    const std::initializer_list<const char*> CMD = {"XINFO", "GROUPS", keyName.c_str() };

    return xinfo( redis_.get(), CMD, output );
}

bool RedisFacade::xinfoStream(const std::string& keyName, XINFO_STREAM_TYPE& output)
{
    assert( redis_ != nullptr );
    assert(redis_->type(keyName) == "stream" || redis_->type(keyName) == "none" );

    const std::initializer_list<const char*> CMD = {"XINFO", "STREAM", keyName.c_str() };

    return xinfo( redis_.get(), CMD, output );
}

/**
 * @brief 
 * 
 * @param keyName 
 * @param strategy 
 * @param approx 
 * @param threshold 
 * @return true 
 * @return false 
 */
bool RedisFacade::xtrim(const std::string& keyName, const XTrimStrategyEnum& strategy, const bool& approx, const std::string& threshold) {
    assert( redis_ != nullptr );
    assert(redis_->type(keyName) == "stream" || redis_->type(keyName) == "none" );

    const char* strStrategy = ( strategy == eXTrimStrategyMINID ? "MINID" : "MAXLEN" );

    const std::initializer_list<const char*> CMD = {"XTRIM", keyName.c_str(), strStrategy, (approx ? "~" : "=" ), threshold.c_str() };

    sw::redis::ReplyUPtr reply;
    try {
        reply = redis_->command(CMD.begin(), CMD.end());
        assert( reply );
    }
    catch(...) {}
    
    if (!reply) {
        return false;
    }

    return true;
}
}
