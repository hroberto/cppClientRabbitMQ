#ifndef __ODBCCONNECT__H__
#define __ODBCCONNECT__H__

#include <string>
#include <sql.h>
#include <sqlext.h>
#include <memory>


#include "Tools/OdbcStatement.h"


namespace libapp {


class OdbcConnect {
private:
    SQLHENV environment_{0};
    SQLHDBC databaseConn_{0};
    std::string name_;
    bool isTransaction_ { false };

    std::string connectString_;
    std::string serverName_;
    std::string username_;
    std::string password_;

public:
    OdbcConnect();
    OdbcConnect(OdbcConnect& other);
    
    virtual ~OdbcConnect();
    bool connect(const std::string& connectString);
    bool connect(const std::string& serverName, const std::string& username, const std::string& password);
    void close();
    void reconnect();

    OdbcConnect* setName(const std::string& value) { name_ = value; return this; }
    const std::string& getName() const { return name_; }

    bool beginTransaction();
    bool Commit();
    bool Rollback();
    bool isTransaction() const { return isTransaction_; }

    std::string CheckErrorODBC();
    bool isConnectFailure();

    std::shared_ptr<OdbcStatement> createStatement();
};

}

#endif // __ODBCCONNECT__H__
