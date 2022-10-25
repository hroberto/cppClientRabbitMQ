#include "Tools/OdbcStatement.h"

#include <typeindex>
#include <fmt/format.h>
#include <cassert>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <sstream>
#include <map>
#include <unistd.h>

using namespace std::string_literals;



const std::map< SQLSMALLINT, std::string > DESC_SQL_TYPES
{ { SQL_UNKNOWN_TYPE, "SQL_UNKNOWN_TYPE"s }
, { SQL_CHAR, "SQL_CHAR"s }
, { SQL_NUMERIC, "SQL_NUMERIC"s }
, { SQL_DECIMAL, "SQL_DECIMAL"s }
, { SQL_INTEGER, "SQL_INTEGER"s }
, { SQL_SMALLINT, "SQL_SMALLINT"s }
, { SQL_FLOAT, "SQL_FLOAT"s }
, { SQL_REAL, "SQL_REAL"s }
, { SQL_DOUBLE, "SQL_DOUBLE"s }
, { SQL_DATETIME, "SQL_DATETIME"s }
, { SQL_VARCHAR, "SQL_VARCHAR"s }
, { SQL_TYPE_DATE, "SQL_TYPE_DATE"s }
, { SQL_TYPE_TIME, "SQL_TYPE_TIME"s }
, { SQL_TYPE_TIMESTAMP, "SQL_TYPE_TIMESTAMP"s }
, { SQL_BIGINT, "SQL_BIGINT"s }
};

const auto fn_desc_SQL_TYPES = [](const SQLSMALLINT& value) -> std::string {
    const auto& it = DESC_SQL_TYPES.find(value);
    
    if ( it == DESC_SQL_TYPES.end() )
        return ""s;

    return it->second;
};



namespace libapp {

OdbcStatement::OdbcStatement(SQLHENV&& environment, SQLHDBC&& databaseConn)
: environment_{environment}, databaseConn_{databaseConn}
{
    assert( stmt_ == 0 );
    
    auto ret = SQLAllocHandle (SQL_HANDLE_STMT, databaseConn, &stmt_);
    if (! SQL_SUCCEEDED( ret )) {
        throw std::runtime_error( fmt::format("OdbcStatemen | SQLAllocHandle [ SQL_HANDLE_STMT ] - \"{} - {}\"", ret, CheckErrorODBC() ) );
    }     
}

OdbcStatement::~OdbcStatement()
{
    if (stmt_ ) {
        SQLCloseCursor(stmt_);
        // SQLFreeStmt(stmt_, SQL_DROP);        
  		SQLFreeHandle(SQL_HANDLE_STMT, stmt_);
    }
    stmt_ = 0;
}


/**
 * @brief Captura o erro da última execucao da conexao ODBC.
 * 
 * @return std::string 
 */
std::string OdbcStatement::CheckErrorODBC()
{
	SQLCHAR		szError[501];
	SQLCHAR		szSqlState[10];
    SQLINTEGER  nNativeError;
    SQLSMALLINT	nErrorMsg;

    memset(szError, 0, 500 );

    std::stringstream ss("");

    if( SQL_SUCCEEDED( SQLGetDiagRec(SQL_HANDLE_STMT, stmt_, 1, szSqlState, &nNativeError, szError, 500, &nErrorMsg) ))
        ss << std::string((char*)szError);

    else if( SQL_SUCCEEDED( SQLGetDiagRec(SQL_HANDLE_DBC, databaseConn_, 1, szSqlState, &nNativeError, szError, 500, &nErrorMsg) ))
        ss << std::string((char*)szError);

    else if( SQL_SUCCEEDED( SQLGetDiagRec(SQL_HANDLE_ENV, environment_, 1, szSqlState, &nNativeError, szError, 500, &nErrorMsg) ))
        ss << std::string((char*)szError);

    return ss.str();
}


OdbcStatement* OdbcStatement::addParameter(const int& ivalue, const SQLSMALLINT& sqlType, SQLULEN colDef, SQLSMALLINT colScale) 
{
    param_int_.emplace_back( std::make_shared<int>( ivalue) );
    
    SQLPOINTER ParameterValuePtr = param_int_.back().get();
    SQLLEN BufferLength = sizeof(ivalue);

    SQLRETURN ret{};

    if (ivalue == 0 && colDef == 0) {        
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": null}"s );

        static SQLLEN IndPtrNULL{SQL_NULL_DATA};
        ret = SQLBindParameter(stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_LONG, sqlType, 0, 0, NULL, 0, &IndPtrNULL);
    }
    else {        
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": "s+std::to_string(ivalue)+"}"s );
        ret = SQLBindParameter (stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_LONG, sqlType, colDef, colScale, ParameterValuePtr, BufferLength, NULL);
    }

    if (! SQL_SUCCEEDED( ret )) {
        throw std::runtime_error( fmt::format("OdbcStatement::addParameter [int] - \"{} - {}\"", ret, CheckErrorODBC() ) );
    }    
    return this;

}



OdbcStatement* OdbcStatement::addParameter(const float& dvalue, const SQLSMALLINT& sqlType, SQLULEN colDef, SQLSMALLINT colScale) 
{
    param_double_.emplace_back( std::make_shared<double>( dvalue) );
    
    SQLPOINTER ParameterValuePtr = param_double_.back().get();
    SQLLEN BufferLength = sizeof(dvalue);
    SQLLEN IndPtr = SQL_NTS;

    SQLRETURN ret{};

    if (dvalue == 0.0 && colDef == 0) {
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": null}"s );
        static SQLLEN IndPtrNULL{SQL_NULL_DATA};
        ret = SQLBindParameter(stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_FLOAT, sqlType, 0, 0, NULL, 0, &IndPtrNULL);
    }
    else {
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": "s+std::to_string(dvalue)+"}"s );
        ret = SQLBindParameter (stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_FLOAT, sqlType, colDef, colScale, ParameterValuePtr, BufferLength, &IndPtr);
    }

    if (! SQL_SUCCEEDED( ret )) {
        throw std::runtime_error( fmt::format("OdbcStatement::addParameter [float] - \"{} - {}\"", ret, CheckErrorODBC() ) );
    }
    return this;
}



OdbcStatement* OdbcStatement::addParameter(const double& dvalue, const SQLSMALLINT& sqlType, SQLULEN colDef, SQLSMALLINT colScale) 
{
    param_double_.emplace_back( std::make_shared<double>( dvalue) );
    
    SQLPOINTER ParameterValuePtr = param_double_.back().get();
    SQLLEN BufferLength = sizeof(dvalue);
    SQLLEN IndPtr = SQL_NTS;

    SQLRETURN ret{};

    if (dvalue == 0.0 && colDef == 0) {
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": null}"s );
        static SQLLEN IndPtrNULL{SQL_NULL_DATA};
        ret = SQLBindParameter(stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_DOUBLE, sqlType, 0, 0, NULL, 0, &IndPtrNULL);
    }
    else {
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": "s+std::to_string(dvalue)+"}"s );
        ret = SQLBindParameter (stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_DOUBLE, sqlType, colDef, colScale, ParameterValuePtr, BufferLength, &IndPtr);
    }

    if (! SQL_SUCCEEDED( ret )) {
        throw std::runtime_error( fmt::format("OdbcStatement::addParameter [double] - \"{} - {}\"", ret, CheckErrorODBC() ) );
    }
    return this;
}



OdbcStatement* OdbcStatement::addParameter(char const* szvalue, const SQLSMALLINT& sqlType, SQLULEN colDef, SQLSMALLINT colScale) 
{
    const size_t MYSIZE{(szvalue == nullptr ? 0 : strlen(szvalue) + 1)};

    SQLRETURN ret{};

    if (szvalue == nullptr) {
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": null}"s );

        static SQLLEN IndPtrNULL{SQL_NULL_DATA};
        ret = SQLBindParameter(stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_CHAR, sqlType, 0, 0, NULL, 0, &IndPtrNULL);
    }
    else {
        param_values_.emplace_back( "{"s+ fn_desc_SQL_TYPES(sqlType)+": "s+std::string(szvalue)+"}"s );

        std::shared_ptr< char[] > value( new char[ MYSIZE ] );
        strcpy( value.get(), szvalue );
        param_str_.emplace_back( value );

        if ( colDef == 0 ) colDef = MYSIZE;

        SQLPOINTER ParameterValuePtr = value.get();
        SQLLEN BufferLength =  MYSIZE -1;

        ret = SQLBindParameter (stmt_, param_values_.size(), SQL_PARAM_INPUT, SQL_C_CHAR, sqlType, colDef, colScale, ParameterValuePtr, BufferLength, NULL );
    }
    // auto ret = SQLBindParameter (stmt_, param_size_.size(), SQL_PARAM_INPUT, SQL_C_CHAR, sqlType, colDef, colScale, ParameterValuePtr, BufferLength, &IndPtr );

    if (! SQL_SUCCEEDED( ret ) && ret != SQL_SUCCESS_WITH_INFO ) {
        throw std::runtime_error( fmt::format("OdbcStatement::addParameter [char*] - param {} -- \"{} - {}\"", param_values_.size(), ret, CheckErrorODBC() ) );
    }
    return this;
}

OdbcStatement* OdbcStatement::addParameter(std::string const& svalue, const SQLSMALLINT& sqlType, SQLULEN colDef, SQLSMALLINT colScale) 
{
    if ( colDef == 0 ) colDef = svalue.size();

    if (!colDef && sqlType == SQL_INTEGER)       return this->addParameter(0, sqlType, 0, colScale);
    else if (!colDef && sqlType == SQL_FLOAT)   return this->addParameter(0.0, sqlType, 0, colScale);
    else if (!colDef && sqlType == SQL_DOUBLE)   return this->addParameter(0.0, sqlType, 0, colScale);
    else if (!colDef && sqlType == SQL_NUMERIC)  return this->addParameter(nullptr, sqlType, 0, colScale);
    else if (!colDef && sqlType == SQL_DECIMAL)  return this->addParameter(nullptr, sqlType, 0, colScale);
    else if (!colDef && sqlType == SQL_VARCHAR)  return this->addParameter(nullptr, sqlType, 0, colScale);

    return this->addParameter(svalue.c_str(), sqlType, colDef, colScale);
}



OdbcStatement* OdbcStatement::setBindParameter( const std::vector<OdbcStatement::SQLParameter>& parameters )
{
    for( const auto& it : parameters) {
        const auto TYPE_VALUE = std::type_index(it.value.type());

        if ( std::type_index(typeid(int)) ==  TYPE_VALUE ) {
            auto sqlType = (it.sqlType == SQL_UNKNOWN_TYPE ? SQL_INTEGER : it.sqlType );
            addParameter( std::any_cast<int>(it.value), sqlType, it.colDef, it.colScale);
        }
        else
        if ( std::type_index(typeid(float)) ==  TYPE_VALUE ) {
            auto sqlType = (it.sqlType == SQL_UNKNOWN_TYPE ? SQL_FLOAT : it.sqlType );
            addParameter( std::any_cast<float>(it.value), sqlType, it.colDef, it.colScale);
        }
        else
        if ( std::type_index(typeid(double)) ==  TYPE_VALUE ) {
            auto sqlType = (it.sqlType == SQL_UNKNOWN_TYPE ? SQL_DOUBLE : it.sqlType );
            addParameter( std::any_cast<double>(it.value), sqlType, it.colDef, it.colScale);
        }
        else
        if ( std::type_index(typeid(const char*)) ==  TYPE_VALUE ) {
            addParameter( std::any_cast<const char*>(it.value), it.sqlType, it.colDef, it.colScale);
        }
        else
        if ( std::type_index(typeid(std::string)) ==  TYPE_VALUE ) {
            addParameter( std::any_cast<std::string>(it.value), it.sqlType, it.colDef, it.colScale);
        }
        else {
            throw std::runtime_error( fmt::format("OdbcStatement::setBindParameter | Tipo de dado desconhecido \"{}\" - idx = {}",  it.value.type().name() , param_values_.size() ) );
        }
    }

    return this;
}

/**
 * @brief 
 * 
 * @param expression 
 * @return true 
 * @return false 
 */
bool OdbcStatement::executeDirect(const std::string& expression)
{
    SQLRETURN retcode{};
    assert( stmt_ != 0 );

    // Bind to an integer reporting on the number of rows fetched.
    SQLSetStmtAttr(stmt_, SQL_ATTR_ROWS_FETCHED_PTR, &numRowsFetched_, 0);

    retcode = SQLExecDirect(stmt_, (SQLCHAR*)expression.data(), SQL_NTS);

    if (retcode!=SQL_NO_DATA && ! SQL_SUCCEEDED(retcode))  {        
        return false;
    }

    bool sucess = (retcode == SQL_SUCCESS_WITH_INFO || SQL_SUCCEEDED(retcode));

    if (sucess && columnsBind_) {        
        prepareFetch();
    }

    return sucess;
}

/**
 * @brief 
 * 
 * @return OdbcStatement* 
 */
OdbcStatement*  OdbcStatement::clearParameters()
{
    param_int_.clear();
    param_double_.clear();
    param_values_.clear();
    param_str_.clear();
    numRowsFetched_ = -1;
    sizeModel_ = 0;
    fetchRowsSize_ = 1;

    return this;
}

/**
 * @brief Configura as propriedades da sessao.
 * 
 */
void OdbcStatement::prepareFetch() 
{
    if (sizeModel_ == 0) {
        SQLSetStmtAttr(stmt_, SQL_ATTR_ROW_BIND_TYPE, SQL_BIND_BY_COLUMN, 0);
    } else {
        SQLSetStmtAttr(stmt_, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeModel_, 0);
    }

    SQLSetStmtAttr(stmt_, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)fetchRowsSize_, 0);
}

/**
 * @brief Realiza a leitura do grupo de registros ( fetchRowsSize_ ) da conexao atual.
 * 
 * @return true 
 * @return false 
 */
bool OdbcStatement::fetchNext() 
{
    RETCODE ret;

    ret = SQLFetchScroll(stmt_, SQL_FETCH_NEXT, 0);

    if (ret == SQL_NO_DATA_FOUND)
        return false;

    if (!SQL_SUCCEEDED(ret))
        return false;

    return true;
}

/**
 * @brief Configura a quantidade de registro a serem lidos por cada Fetch e se a estrutura de carregamento 
 *      é por matriz para cada coluna ( sizeModel == 0 ) ou matriz por struct por registro.
 * 
 * @param fetchRowsSize 
 * @param sizeModel 
 * @return OdbcStatement* 
 */
OdbcStatement* OdbcStatement::setFetchRowsSize(const SQLLEN& fetchRowsSize, const std::size_t& sizeModel) 
{
    fetchRowsSize_ = fetchRowsSize;
    sizeModel_ = sizeModel;

    return this;
}

/**
 * @brief Referencia a variavel para leitura de retorno.
 * 
 * @param ptrValue 
 * @return OdbcStatement* 
 */
OdbcStatement* OdbcStatement::addBindColumn(int* ptrValue) 
{
    if (numRowsFetched_ >= 0) prepareFetch();

    static SQLLEN orind = SQL_NTS;
    ++columnsBind_;
    SQLBindCol(stmt_, columnsBind_, SQL_INTEGER, ptrValue, sizeof(*ptrValue), &orind);
    return this;
}

/**
 * @brief Referencia a variavel para leitura de retorno.
 * 
 * @param ptrValue 
 * @return OdbcStatement* 
 */
OdbcStatement* OdbcStatement::addBindColumn(double* ptrValue) 
{
    if (numRowsFetched_ >= 0) prepareFetch();

    static SQLLEN orind = SQL_NTS;
    ++columnsBind_;
    SQLBindCol(stmt_, columnsBind_, SQL_DOUBLE, ptrValue, sizeof(*ptrValue), &orind);
    return this;
}

/**
 * @brief Referencia a variavel para leitura de retorno.
 * 
 * @param ptrValue 
 * @param length 
 * @return OdbcStatement* 
 */
OdbcStatement* OdbcStatement::addBindColumn(char* ptrValue, const int& length)
{
    if (numRowsFetched_ >= 0) prepareFetch();

    static SQLLEN orind = SQL_NTS;
    ++columnsBind_;
    SQLBindCol(stmt_, columnsBind_, SQL_C_CHAR, ptrValue, length, &orind);
    return this;
}

/**
 * @brief Referencia a variavel para leitura de retorno.
 * 
 * @param ptrValue 
 * @param length 
 * @return OdbcStatement* 
 */
OdbcStatement* OdbcStatement::addBindColumn(char* ptrValue, const int& length, SQLLEN* strLenOrInd)
{
    if (numRowsFetched_ >= 0) prepareFetch();

    ++columnsBind_;
    SQLBindCol(stmt_, columnsBind_, SQL_C_CHAR, ptrValue, length, strLenOrInd);
    return this;
}

}  // namespace libapp
