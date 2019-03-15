#include "ConnectionPool.h"

#include <QDebug>
#include <QtSql>
#include <QStack>
#include <QString>
#include <QMutex>
#include <QSemaphore>

class ConnectionPoolPrivate
{
public:
    ConnectionPoolPrivate();
    ~ConnectionPoolPrivate();

    QStack<QString> used_conn_names;
    QStack<QString> unused_conn_names;

    QString host_name;
    QString database_name;
    QString username;
    QString password;
    QString database_type;
    int port;

    bool test_on_borrow;
    QString test_on_borrow_sql;
    int max_wait_time;
    int max_connection_count;

    QSemaphore *sem;

    static QMutex mutex;
    static int last_key;
};

QMutex ConnectionPoolPrivate::mutex;
int ConnectionPoolPrivate::last_key = 0;

ConnectionPoolPrivate::ConnectionPoolPrivate()
{
    sem = NULL;
    test_on_borrow = false;
    max_wait_time = 5000;
    max_connection_count = 5;
}

ConnectionPoolPrivate::~ConnectionPoolPrivate()
{
    foreach (QString conn_name, used_conn_names) {
        QSqlDatabase::removeDatabase(conn_name);
    }

    foreach (QString conn_name, unused_conn_names) {
        QSqlDatabase::removeDatabase(conn_name);
    }
}

ConnectionPool::ConnectionPool()
    :d(new ConnectionPoolPrivate)
{

}

ConnectionPool *ConnectionPool::createInstance()
{
    return new ConnectionPool();
}

ConnectionPool::~ConnectionPool()
{
    delete d;
}

ConnectionPool *ConnectionPool::getInstance()
{
    return Singleton<ConnectionPool>::instance(ConnectionPool::createInstance);
}

void ConnectionPool::destory()
{
    Q_ASSERT(NULL != d);
    if(NULL != d)
    {
        QMutexLocker locker(&ConnectionPoolPrivate::mutex);
        delete d;
        d = NULL;
    }
}

QSqlDatabase ConnectionPool::openConnection()
{
    Q_ASSERT(NULL != d);
    if(d->sem->tryAcquire(1, d->max_wait_time))
    {
        ConnectionPoolPrivate::mutex.lock();
        QString conn_name = d->unused_conn_names.size() > 0 ? d->unused_conn_names.pop() : QString("C%1").arg(++ConnectionPoolPrivate::last_key);
        d->used_conn_names.push(conn_name);
        ConnectionPoolPrivate::mutex.unlock();

        QSqlDatabase db = createConnection(conn_name);

        if(!db.isOpen())
        {
            ConnectionPoolPrivate::mutex.lock();
            d->used_conn_names.removeOne(conn_name);
            ConnectionPoolPrivate::mutex.unlock();
            d->sem->release();
        }
        return db;
    }
    else
    {
        qDebug()<<"time out to create connection.";
        return QSqlDatabase();
    }
}

void ConnectionPool::closeConnection(const QSqlDatabase &connection)
{
    Q_ASSERT(NULL != d);
    QString conn_name = connection.connectionName();

    if(d->used_conn_names.contains(conn_name))
    {
        QMutexLocker locker(&ConnectionPoolPrivate::mutex);
        d->used_conn_names.removeOne(conn_name);
        d->unused_conn_names.push(conn_name);
        d->sem->release();
    }
}

void ConnectionPool::clearConnection()
{
    while(!d->unused_conn_names.isEmpty())
    {
        QString name = d->unused_conn_names.pop();
        QSqlDatabase::removeDatabase(name);
    }
}

void ConnectionPool::setHostName(const QString &host_name)
{
    d->host_name = host_name;
}

void ConnectionPool::setDatabaseName(const QString &database_name)
{
    d->database_name = database_name;
}

void ConnectionPool::setUsername(const QString &userName)
{
    d->username = userName;
}

void ConnectionPool::setPassword(const QString &password)
{
    d->password = password;
}

void ConnectionPool::setDatabaseType(const QString &database_type)
{
    d->database_type  = database_type;
}

void ConnectionPool::setPort(int port)
{
    d->port = port;
}

void ConnectionPool::setTestOnBorrow(bool test_on_borrow)
{
    d->test_on_borrow = test_on_borrow;
}

void ConnectionPool::setTestOnBorrowSql(const QString &test_on_borrow_sql)
{
    d->test_on_borrow_sql = test_on_borrow_sql;
}

void ConnectionPool::setMaxWaitTime(int max_wait_time)
{
    d->max_wait_time = max_wait_time;
}

void ConnectionPool::setMaxConnectionCount(int max_conn_count)
{
    d->max_connection_count = max_conn_count;
    if(d->sem)
        delete d->sem;
    d->sem = new QSemaphore(max_conn_count);
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName)
{
    Q_ASSERT(NULL != d);
    if(QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);
        if(d->test_on_borrow)
        {
            qDebug()<<__FUNCTION__<<QString("test connection on borrow, execute: %1, for connection %2")
                      .arg(d->test_on_borrow_sql).arg(connectionName);
            QSqlQuery query(d->test_on_borrow_sql, db1);
            if(query.lastError().type() != QSqlError::NoError && !db1.open())
            {
                qDebug()<<__FUNCTION__<<"open database error: "<<db1.lastError().text();
                return QSqlDatabase();
            }
        }
        return db1;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(d->database_type, connectionName);
    db.setHostName(d->host_name);
    db.setDatabaseName(d->database_name);
    db.setUserName(d->username);
    db.setPassword(d->password);
    if(d->port != 0)
    {
        db.setPort(d->port);
    }
    if(!db.open())
    {
        qDebug()<<__FUNCTION__<<"Open database error: "<<db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}

ConnectionGuard::ConnectionGuard()
{
    m_db = ConnectionPool::getInstance()->openConnection();
}

ConnectionGuard::~ConnectionGuard()
{
    ConnectionPool::getInstance()->closeConnection(m_db);
}

QSqlDatabase ConnectionGuard::connection()
{
    return m_db;
}

QSqlQuery ConnectionGuard::query()
{
    return QSqlQuery(m_db);
}
