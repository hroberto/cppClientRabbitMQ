#include "Tools/RedisClientStorage.h"

#include <fmt/format.h>
#include <limits.h>
#include <unistd.h>


#include "Tools/StringUtils.h"


namespace libapp {


using namespace std::string_literals;

RedisClientStorage::RedisClientStorage()
    : system_name_(""s), redis_{nullptr} 
{}

RedisClientStorage::RedisClientStorage(const std::string & system_name)
    : RedisClientStorage(system_name, nullptr) 
{}

RedisClientStorage::RedisClientStorage(const std::string & system_name, sw::redis::Redis* ptr)
    : system_name_(system_name), redis_{ptr} 
{
    char NAME_SERVER[HOST_NAME_MAX];
    gethostname(NAME_SERVER, HOST_NAME_MAX);

    const std::string ID_PROCESS = fmt::format("{}_{}", system_name, NAME_SERVER);
    this->setName(ID_PROCESS);
}


RedisClientStorage* RedisClientStorage::setRedis(sw::redis::Redis* ptr)
{
    assert(ptr != nullptr);

    redis_ = ptr;
    return this;
}

/**
 * @brief Retorna o nome da conexao no Redis, vide( redis-cli client  getname )
 * 
 * @return std::string 
 */
std::string RedisClientStorage::getName()
{
    assert(redis_ != nullptr);

    const auto CMD = {"client", "getname" };
    auto val = redis_->command<sw::redis::OptionalString>(CMD.begin(), CMD.end());

    if( !val ) return "";

    return *val;
}

/**
 * @brief Retorna o numero de identificacao da conexao no Redis.
 * 
 * @return long long 
 */
long long RedisClientStorage::getID()
{
    assert(redis_ != nullptr);

    const auto CMD = {"client", "id" };
    auto val = redis_->command<sw::redis::OptionalLongLong>(CMD.begin(), CMD.end());

    if (!val) return 0;

    return *val;
}


/**
 * @brief 
 * 
 * @param client_name 
 * @return true 
 * @return false 
 */
bool RedisClientStorage::setName(const std::string& client_name) 
{
    assert(redis_ != nullptr);
    assert(!client_name.empty());

    const auto CMD = {"client", "setname", client_name.c_str()};
    auto val = redis_->command<sw::redis::OptionalString>(CMD.begin(), CMD.end());

    assert(val);
    assert(*val == "OK");

    return (val && *val == "OK");
}




/**
 * @brief 
 * 
 * @return std::vector<RedisClientStorage::client_st > 
 */
std::vector<RedisClientStorage::client_st > RedisClientStorage::list()
{
    return list("");
}
std::vector<RedisClientStorage::client_st > RedisClientStorage::list(const std::string& name)
{
    assert(redis_ != nullptr);
    std::vector< RedisClientStorage::client_st > results;

    const auto CMD = {"client", "list"};
    auto val = redis_->command<sw::redis::OptionalString>(CMD.begin(), CMD.end());

    if( val ) {
        std::vector<std::string> input = split(*val, '\n');

        for (const auto& line : input) {
            if ( line.empty() ) continue;

            std::vector<std::string> fields = split(line, ' ', 21);

            RedisClientStorage::client_st   cl;

            for (const auto& fld : fields) {
                auto t = split(fld, '=', 2);
                if (t.at(0) == "id") cl.id = atoui64(t.at(1));
                else if (t.at(0) == "addr") cl.addr = t.at(1);
                else if (t.at(0) == "fd") cl.fd = t.at(1);
                else if (t.at(0) == "name") cl.name = t.at(1);
                else if (t.at(0) == "age") cl.age = atoui64(t.at(1));
                else if (t.at(0) == "idle") cl.idle = atoui64(t.at(1));
                else if (t.at(0) == "flags") cl.flags = t.at(1);
                else if (t.at(0) == "db") cl.db = t.at(1);
                else if (t.at(0) == "sub") cl.sub = std::atol(t.at(1).c_str());
                else if (t.at(0) == "psub") cl.psub = std::atol(t.at(1).c_str());
                else if (t.at(0) == "multi") cl.multi = std::atol(t.at(1).c_str());
                else if (t.at(0) == "qbuf") cl.qbuf = std::atol(t.at(1).c_str());
                else if (t.at(0) == "qbuf_free") cl.qbuf_free = std::atol(t.at(1).c_str());
                else if (t.at(0) == "argv_mem") cl.argv_mem = std::atol(t.at(1).c_str());
                else if (t.at(0) == "obl") cl.obl = std::atol(t.at(1).c_str());
                else if (t.at(0) == "oll") cl.oll = std::atol(t.at(1).c_str());
                else if (t.at(0) == "omem") cl.omem = std::atol(t.at(1).c_str());
                else if (t.at(0) == "tot_mem") cl.tot_mem = std::atol(t.at(1).c_str());
                else if (t.at(0) == "events") cl.events = t.at(1);
                else if (t.at(0) == "cmd") cl.cmd = t.at(1);
                else if (t.at(0) == "user") cl.user = t.at(1);
            }
            assert( cl.id != 0 );
            assert( ! cl.addr.empty() );

            if ( cl.name.find(name) == std::string::npos)   continue;
            
            results.emplace_back( cl );
        }
    }

    return results;
}

/**
 * @brief Limpa a estrutura das conexao no servidor Redis.
 * 
 */
void RedisClientStorage::client_st::clear()
{
    id = 0x00000000;
    addr.clear();
    fd.clear();
    name.clear();
    age = 0x00000000;
    idle = 0x00000000;
    flags.clear();
    db.clear();
    sub = 0;
    psub = 0;
    multi = 0;
    qbuf = 0;
    qbuf_free = 0;
    argv_mem = 0;
    obl = 0;
    oll = 0;
    omem = 0;
    tot_mem = 0;
    events.clear();
    cmd.clear();
    user.clear();
}

std::string  RedisClientStorage::client_st::toString()
{
    std::string json;
    if (name.empty()) {
        json = to_string_json(
            {{"id", id}, {"addr", addr}, {"name", nullptr}, {"age", age}, {"idle", idle}, {"flags", flags}, {"db", db}, {"sub", sub}, {"psub", psub}, {"multi", multi}, {"qbuf", qbuf}, {"qbuf_free", qbuf_free}, {"argv_mem", argv_mem}, {"obl", obl}, {"oll", oll}, {"omem", omem}, {"tot_mem", tot_mem}, {"events", events}, {"cmd", cmd}, {"user", user}});
    } else {
        json = to_string_json(
            {{"id", id}, {"addr", addr}, {"name", name}, {"age", age}, {"idle", idle}, {"flags", flags}, {"db", db}, {"sub", sub}, {"psub", psub}, {"multi", multi}, {"qbuf", qbuf}, {"qbuf_free", qbuf_free}, {"argv_mem", argv_mem}, {"obl", obl}, {"oll", oll}, {"omem", omem}, {"tot_mem", tot_mem}, {"events", events}, {"cmd", cmd}, {"user", user}});
    }

    return json;
}

std::string  RedisClientStorage::client_st::toStringResume()
{
    std::string json;
    if (name.empty()) {
        json = to_string_json(
            {{"id", id}, {"addr", addr}, {"name", nullptr}, {"age", age}, {"idle", idle}, {"user", user}});
    } else {
        json = to_string_json(
            {{"id", id}, {"addr", addr}, {"name", name}, {"age", age}, {"idle", idle}, {"user", user}});
    }

    return json;
}

/**
 * @brief 
 * 
 * @param resource 
 * @param id 
 * @return true 
 * @return false 
 */
bool RedisClientStorage::sysmaster_lock()
{
    return this->sysmaster_lock(system_name_, getName());
}

/**
 * @brief 
 * 
 * @param resource 
 * @param id 
 * @return true 
 * @return false 
 */
bool RedisClientStorage::sysmaster_lock(const std::string& resource, const std::string& id)
{
    const std::string systemLock = fmt::format("sys.{}.master.lock", resource);
    const std::string systemMaster = fmt::format("sys.{}.master", resource);

    sw::redis::OptionalString idMaster = redis_->get(systemLock);

    if (!idMaster) {
        bool retLock = redis_->setnx(systemLock, id);

        if (retLock) {
            using namespace std::chrono_literals;
            redis_->expire(systemLock, 10s);
            redis_->set(systemMaster, id);
        }

        do {
            idMaster = redis_->get(systemLock);
        } while (!idMaster);
    }

    return (*idMaster == id);
}

/**
 * @brief Verifica se a Identificacao da Conexao é unica no Redis.
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool RedisClientStorage::check_unique_name()
{
    const std::string id = this->getName();
    
    if (id.empty()) return false;

    auto results = this->list(id);
    return (results.size() == 1);
}

/**
 * @brief Apaga o controle de lock do master.
 * 
 */
void RedisClientStorage::drop_unique_name()
{
    assert(redis_ != nullptr);
    assert( check_unique_name() );

    if ( check_unique_name() ) {
        redis_->del( this->getName() );
    }
}



}
