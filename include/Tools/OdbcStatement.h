#ifndef __ODBCSTATEMENT__H__
#define __ODBCSTATEMENT__H__

#include <string>
#include <any>
#include <vector>
#include <sql.h>
#include <sqlext.h>
#include <memory>




namespace libapp {


class OdbcStatement {
private:
    const SQLHENV& environment_{0};
    const SQLHDBC& databaseConn_{0};
    SQLHSTMT stmt_{0};
    SQLLEN  numRowsFetched_{-1};
    SQLUSMALLINT  columnsBind_{ 0 };
    SQLLEN  fetchRowsSize_{1};
    std::size_t  sizeModel_{0};

    
    std::vector<std::shared_ptr<int>> param_int_;
    std::vector<std::shared_ptr<double>> param_double_;
    std::vector< std::shared_ptr<char[]> > param_str_;
    std::vector<std::string> param_values_;

public:

    struct SQLParameter {
        std::any    value{};
        SQLSMALLINT sqlType { SQL_CHAR };
        SQLULEN     colDef {0};
        SQLSMALLINT colScale{0};

        ~SQLParameter() {
            value.reset();
        }
    };


    friend class OdbcConnect;
    virtual ~OdbcStatement();

    std::string CheckErrorODBC();
    OdbcStatement* setBindParameter( const std::vector<SQLParameter>& parameters );

    bool executeDirect(const std::string& expression);
    SQLLEN getRowsFetched() const { return numRowsFetched_; }
    OdbcStatement* setFetchRowsSize(const SQLLEN& length, const std::size_t& sizeModel);
    bool fetchNext();

    OdbcStatement*  addBindColumn(int* ptrValue);
    OdbcStatement*  addBindColumn(double* ptrValue);
    OdbcStatement*  addBindColumn(char* ptrValue, const int& length);
    OdbcStatement*  addBindColumn(char* ptrValue, const int& length, SQLLEN* strLenOrInd);

    OdbcStatement* clearParameters();
    const auto& getParameters() const { return param_values_; }

    OdbcStatement* addParameter(const int& ivalue, const SQLSMALLINT& sqlType = SQL_INTEGER, SQLULEN colDef = 0, SQLSMALLINT colScale = 0 );
    OdbcStatement* addParameter(const float& dvalue, const SQLSMALLINT& sqlType = SQL_FLOAT, SQLULEN colDef = 0, SQLSMALLINT colScale = 0 );
    OdbcStatement* addParameter(const double& dvalue, const SQLSMALLINT& sqlType = SQL_DOUBLE, SQLULEN colDef = 0, SQLSMALLINT colScale = 0 );
    OdbcStatement* addParameter(char const* szvalue, const SQLSMALLINT& sqlType = SQL_CHAR, SQLULEN colDef = 0, SQLSMALLINT colScale = 0 );
    OdbcStatement* addParameter(std::string const& svalue, const SQLSMALLINT& sqlType = SQL_VARCHAR, SQLULEN colDef = 0, SQLSMALLINT colScale = 0 );

protected:
    OdbcStatement(SQLHENV&& environment, SQLHDBC&& databaseConn);

    void prepareFetch();

};

}

#endif // __ODBCSTATEMENT__H__
