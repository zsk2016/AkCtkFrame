#pragma once

#include <QString>
#include <QStringList>
#include "util/Singleton.h"

class JsonUtil;

class ConfigUtil
{
protected:
    ConfigUtil();
    static ConfigUtil *createInstance();
public:
    ~ConfigUtil();
    static ConfigUtil *getInstance();

    QString getDatabaseType() const;
    QString getHomeName() const;
    QString getDatabaseName() const;
    QString getUserName() const;
    QString getPassword() const;
    QString getTestOnBorrowSql() const;
    bool getTestOnBorrow();
    int getPort();
    int getMaxWaitTime();
    int getMaxConnectCount();
    bool getDatabaseDebug();
    QStringList getDatabaseSqlFiles() const;

private:
    JsonUtil *m_config;
    bool mutable m_error;
};
