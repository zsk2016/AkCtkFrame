#include "ConfigUtil.h"
#include "Singleton.h"
#include "JsonUtil.h"

ConfigUtil::ConfigUtil()
{
    m_config = new JsonUtil(":/resource/SqlConfig.json");
}

ConfigUtil *ConfigUtil::ConfigUtil::createInstance()
{
    return new ConfigUtil;
}

ConfigUtil::~ConfigUtil()
{
    delete m_config;
}

ConfigUtil *ConfigUtil::getInstance()
{
    return Singleton<ConfigUtil>::instance(ConfigUtil::createInstance);
}

QString ConfigUtil::getDatabaseType() const
{
    return m_config->getString("database.type");
}

QString ConfigUtil::getHomeName() const
{
    return m_config->getString("database.host");
}

QString ConfigUtil::getDatabaseName() const
{
    return m_config->getString("database.database_name");
}
QString ConfigUtil::getUserName() const
{
    return m_config->getString("database.username");
}
QString ConfigUtil::getPassword() const
{
    return m_config->getString("database.password");
}
QString ConfigUtil::getTestOnBorrowSql() const
{
    return m_config->getString("database.test_on_borrow_sql");
}
bool ConfigUtil::getTestOnBorrow()
{
    return m_config->getBool("database.test_on_borrow", false);
}

int ConfigUtil::getPort()
{
    return m_config->getInt("database.port", 3306);
}

int ConfigUtil::getMaxWaitTime()
{
    return m_config->getInt("database.max_wait_time", 5000);
}

int ConfigUtil::getMaxConnectCount()
{
    return m_config->getInt("database.max_connection_count", false);
}

bool ConfigUtil::getDatabaseDebug()
{
    return m_config->getBool("database.debug", false);
}
QStringList ConfigUtil::getDatabaseSqlFiles() const
{
    return m_config->getStringList("database.sql_files");
}
