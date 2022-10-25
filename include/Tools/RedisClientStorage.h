#ifndef __REDISCLIENTSTORAGE_H__
#define __REDISCLIENTSTORAGE_H__



#include <string>
#include <chrono>

#include <sw/redis++/redis++.h>


namespace libapp {


class RedisClientStorage {
private:
    std::string system_name_;
    sw::redis::Redis* redis_;
    // std::chrono::milliseconds   timeSleep_;

public:
    struct client_st {
        std::uint64_t id{};     // a unique 64-bit client ID
        std::string addr;       // address/port of the client
        std::string fd;         // file descriptor corresponding to the socket
        std::string name;       // the name set by the client with CLIENT SETNAME
        std::uint64_t age{};    // total duration of the connection in seconds
        std::uint64_t idle{};   // idle time of the connection in seconds
        std::string flags;      // client flags (see below)
        std::string db;         // current database ID
        int sub{};              // number of channel subscriptions
        int psub{};             // number of pattern matching subscriptions
        int multi{};            // number of commands in a MULTI/EXEC context
        int qbuf{};             // query buffer length (0 means no query pending)
        int qbuf_free{};        // free space of the query buffer (0 means the buffer is full)
        int argv_mem{};         // incomplete arguments for the next command (already extracted from query buffer)
        int obl{};              // output buffer length
        int oll{};              // output list length (replies are queued in this list when the buffer is full)
        int omem{};             // output buffer memory usage
        int tot_mem{};          // total memory consumed by this client in its various buffers
        std::string events;     // file descriptor events (see below)
        std::string cmd;        // last command played
        std::string user;       // the authenticated username of the client

        void clear();
        std::string toString();
        std::string toStringResume();
    };

    RedisClientStorage();
    RedisClientStorage(const std::string& system_name);
    RedisClientStorage(const std::string& system_name, sw::redis::Redis* ptr);

    RedisClientStorage* setRedis(sw::redis::Redis* ptr);
    // RedisClientStorage* setTimeSleep(const std::chrono::milliseconds&& value) { timeSleep_ = value; return this; }

    std::string getName();
    long long getID();

    bool setName(const std::string& client_name);
    std::vector< client_st > list();
    std::vector< client_st > list(const std::string& name);

    bool sysmaster_lock(const std::string& resource, const std::string& id);
    bool sysmaster_lock();
    bool check_unique_name();
    void drop_unique_name();

};

}

#endif // __REDISCLIENTSTORAGE_H__