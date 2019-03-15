#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H
#include "util/Singleton.h"
#include <QSqlQuery>

class QSqlDatabase;
class ConnectionPoolPrivate;

class ConnectionPool
{
protected:
    ConnectionPool();
    static ConnectionPool *createInstance();
public:
    ~ConnectionPool();
    static ConnectionPool *getInstance();

public:
    void destory();
    QSqlDatabase openConnection();
    void closeConnection(const QSqlDatabase &connection);
    void clearConnection();
    void setHostName(const QString &host_name);
    void setDatabaseName(const QString &database_name);
    void setUsername(const QString &userName);
    void setPassword(const QString &password);
    void setDatabaseType(const QString &database_type);
    void setPort(int port);
    void setTestOnBorrow(bool test_on_borrow);
    void setTestOnBorrowSql(const QString &test_on_borrow_sql);
    void setMaxWaitTime(int max_wait_time);
    void setMaxConnectionCount(int max_conn_count);

private:
    QSqlDatabase createConnection(const QString &connectionName);
    ConnectionPoolPrivate *d;
};

class  ConnectionGuard
{
public:
     ConnectionGuard();
    ~ConnectionGuard();
     QSqlDatabase connection();
     QSqlQuery query();

private:
     QSqlDatabase m_db;
};

#endif
