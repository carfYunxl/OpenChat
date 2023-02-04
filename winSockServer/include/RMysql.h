#ifndef RMYSQL_H_
#define RMYSQL_H_
#include <mysql.h>
#include <string>
#include <vector>
#include <tuple>

namespace R
{
    using std::vector;
    using std::string;
    using std::tuple;

    using RESULT = vector<vector<string>>;
    class RMysql
    {
    public:
        RMysql(
            const char* ip,
            const char* username,
            const char* password,
            const char* database
            );

        bool RQuery(const std::string& query, RESULT& result,bool select = false);
        ~RMysql()
        {
            mysql_close(&m_sql);
        };
    private:
        MYSQL m_sql;
        const char* m_ip;
        const char* m_name;
        const char* m_password;
        const char* m_database;

    private:
        void init();
    };
}

#endif //RMYSQL_H_
