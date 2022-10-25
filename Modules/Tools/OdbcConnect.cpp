#include "Tools/OdbcConnect.h"

#include <stdexcept>
#include <cassert>
#include <fmt/format.h>


namespace libapp {

OdbcConnect::OdbcConnect()
{
}


OdbcConnect::OdbcConnect(OdbcConnect& other)
: name_{ other.name_ }
, connectString_{ other.connectString_ }
, serverName_{ other.serverName_ }
, username_{ other.username_ }
, password_{ other.password_ }
{
}


OdbcConnect::~OdbcConnect()
{
    this->close();
}

void  OdbcConnect::close() 
{
    if (databaseConn_) {
        SQLDisconnect(databaseConn_);
        SQLFreeHandle(SQL_HANDLE_DBC, databaseConn_);
        SQLFreeConnect(databaseConn_);
        databaseConn_ = 0;
    }
    if (environment_) {
        SQLFreeHandle(SQL_HANDLE_ENV, environment_);
        SQLFreeEnv(environment_);
        environment_ = 0;
    }

    isTransaction_ = false;

    assert( environment_ == 0 );
    assert( databaseConn_ == 0 );    
}


void OdbcConnect::reconnect()
{
    assert(!connectString_.empty() || !serverName_.empty());

    this->close();

    if (!connectString_.empty()) {
        this->connect( connectString_ );
    }
    else if (!connectString_.empty()) {
        this->connect(serverName_, username_, password_);
    }
}


/**
 * @brief Realiza a conexao com o banco de dados.
 * 
 * @param connectString
 *    Eg.: "MSSQLCMDHist"s, "crypto"s, "Plural@2020"s
 * @return true 
 * @return false 
 */
bool OdbcConnect::connect(const std::string& serverName, const std::string& username, const std::string& password)
{
    assert( environment_ == 0 );
    assert( databaseConn_ == 0 );

    connectString_.clear();
    serverName_ = serverName;
    username_ = username;
    password_ = password;

    SQLRETURN retcode{};

    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment_);
    assert( SQL_SUCCEEDED(retcode) );

    retcode = SQLSetEnvAttr(environment_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
    assert( SQL_SUCCEEDED(retcode) );

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, environment_, &databaseConn_);
    assert( SQL_SUCCEEDED(retcode) );

    /* Specify the ODBC Cursor Library is always used then connect. */
    retcode = SQLSetConnectAttr(databaseConn_, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER) SQL_CUR_USE_ODBC, 0);
    assert( SQL_SUCCEEDED(retcode) );

    retcode = SQLConnect(databaseConn_, (SQLCHAR *)serverName.data(), SQL_NTS, (SQLCHAR *) username.data(), SQL_NTS, (SQLCHAR *) password.data(), SQL_NTS);

    isTransaction_ = false;

    return SQL_SUCCEEDED(retcode);
}


/**
 * @brief Realiza a conexao com o banco de dados.
 * 
 * @param connectString
 *    Eg.: "Driver=/usr/local/lib/libtdsodbc.so; Trace=No; TDS_Version=8.0; " \
 *         "Server=10.18.12.27; Port=1433; Database=<database>; UID=<user>; PWD=<senha>"s
 * @return true 
 * @return false 
 */
bool OdbcConnect::connect(const std::string& connectString)
{
    assert( environment_ == 0 );
    assert( databaseConn_ == 0 );

    SQLRETURN retcode;

    connectString_ = connectString;
    serverName_.clear();
    username_.clear();
    password_.clear();

    // char            ConnStrOut [200];
    // SQLSMALLINT     cbConnStrOut;
    
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment_);
    assert( SQL_SUCCEEDED(retcode) );

    retcode = SQLSetEnvAttr(environment_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
    assert( SQL_SUCCEEDED(retcode) );

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, environment_, &databaseConn_);
    assert( SQL_SUCCEEDED(retcode) );

    /* Specify the ODBC Cursor Library is always used then connect. */
    retcode = SQLSetConnectAttr(databaseConn_, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER) SQL_CUR_USE_ODBC, 0);
    assert( SQL_SUCCEEDED(retcode) );

    retcode = SQLDriverConnect( 
            databaseConn_,  
            nullptr, 
            (SQLCHAR *)connectString.c_str(), 
            SQL_NTS , // connectString.size(), 
            NULL, // (SQLCHAR *)ConnStrOut, 
            0, // sizeof(ConnStrOut), 
            NULL, //&cbConnStrOut, 
            SQL_DRIVER_NOPROMPT 
            );

    isTransaction_ = false;

    return SQL_SUCCEEDED(retcode);
}


/**
 * @brief Captura o erro da última execucao da conexao ODBC.
 * 
 * @return std::string 
 */
std::string OdbcConnect::CheckErrorODBC()
{
	SQLCHAR		szError[501];
	SQLCHAR		szSqlState[10];
    SQLINTEGER  nNativeError;
    SQLSMALLINT	nErrorMsg;

	if ( environment_ ) {
		if ( SQLError( environment_, databaseConn_, 0, szSqlState, &nNativeError, szError, 500, &nErrorMsg ) == SQL_SUCCESS ) {
			return std::string((char*)szError);
		}
	}

	return "";    
}

bool OdbcConnect::isConnectFailure()
{
    const std::string msgError = this->CheckErrorODBC();

    if( msgError.find( "Communication link failure" ) != std::string::npos ) {
        return true;
    }

    return false;
}

std::shared_ptr<libapp::OdbcStatement> OdbcConnect::createStatement()
{
    std::shared_ptr<OdbcStatement> ptr = std::shared_ptr<OdbcStatement>( 
        new OdbcStatement(
            std::forward<SQLHENV>(environment_), 
            std::forward<SQLHDBC>(databaseConn_) 
        ) );

    return ptr;
}


bool OdbcConnect::beginTransaction()
{
    auto retcode = SQLSetConnectAttr(databaseConn_, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)false, 0);

    isTransaction_ = true;

    if (! SQL_SUCCEEDED( retcode ) && retcode != SQL_SUCCESS_WITH_INFO ) {
        throw std::runtime_error( fmt::format("OdbcStatement::beginTransaction - \"{} - {}\"", retcode, CheckErrorODBC() ) );
        return false;
    }
    return true;
}

bool OdbcConnect::Commit()
{
    auto retcode = SQLEndTran (SQL_HANDLE_DBC, databaseConn_, SQL_COMMIT);

    isTransaction_ = false;

    if (! SQL_SUCCEEDED( retcode ) && retcode != SQL_SUCCESS_WITH_INFO ) {
        throw std::runtime_error( fmt::format("OdbcStatement::Commit - \"{} - {}\"", retcode, CheckErrorODBC() ) );
        return false;
    }
    return true;
}

bool OdbcConnect::Rollback()
{
    auto retcode = SQLEndTran (SQL_HANDLE_DBC, databaseConn_, SQL_ROLLBACK);

    isTransaction_ = false;
    if (! SQL_SUCCEEDED( retcode ) && retcode != SQL_SUCCESS_WITH_INFO ) {
        throw std::runtime_error( fmt::format("OdbcStatement::Rollback - \"{} - {}\"", retcode, CheckErrorODBC() ) );
        return false;
    }
    return true;
}

}
