#include "stdafx.h"
#include "RMysql.h"

using R::RMysql;
using R::RESULT;


RMysql::RMysql(
    const char* ip,
    const char* username,
    const char* password,
    const char* database
)
    : m_ip(ip)
    , m_name(username)
    , m_password(password)
    , m_database(database)
{
    init();
}

void RMysql::init()
{
    mysql_init(&m_sql);

    if (!mysql_real_connect(&m_sql, m_ip, m_name, m_password, m_database, 3306, NULL, 0))
    {
        return;
    }
}

bool RMysql::RQuery(const std::string& query,RESULT& result, bool select /*= false*/)
{
    if (mysql_query(&m_sql, query.c_str()))
    {
        return false;
    }

    if (!select)
    {
        return true;
    }

    MYSQL_RES* res = mysql_store_result(&m_sql);
    if (!res)
    {
        return false;
    }

    uint64_t rows = mysql_affected_rows(&m_sql);
    size_t   columns = mysql_num_fields(res);
    
    result.resize(columns);
    
    /*for (size_t i = 0; i < columns; ++i)
    {
        result.at(i).push_back(mysql_fetch_field(res)->name);
    }*/

    MYSQL_ROW column;
    while (column = mysql_fetch_row(res))
    {
        for (size_t i = 0; i < columns; i++)
        {
            char* arr = column[i];
            if (arr == NULL)
            {
                result.at(i).push_back("NULL");
            }
            else
            {
                result.at(i).push_back(arr);
            }
        }
    }

    mysql_free_result(res);

    return true;
}

