#include "DbInterface.h"

QVariantMap mapToObject(const  QVariantMap &row_map)
{
	return row_map;
}

DbInterface::DbInterface(QObject *parent /* = 0 */)
	:QObject(parent)
{
	initDb();
	qDebug() << "db init ...";
}

DbInterface::~DbInterface()
{

}

QList<QVariantMap> DbInterface::getSql(QString tableName, QString sqlStr)
{
	QString sql = Sqls::getInstance()->getSql(tableName, sqlStr);
	return DBUtil::selectBeans(mapToObject, sqlStr);
}

void DbInterface::initDb()
{
	ConfigUtil *config = ConfigUtil::getInstance();
	ConnectionPool *cp = ConnectionPool::getInstance();
	qDebug() << config->getDatabaseName();
	qDebug() << config->getHomeName();
	cp->setDatabaseType(config->getDatabaseType());
	cp->setDatabaseName(config->getDatabaseName());
	cp->setHostName(config->getHomeName());
	cp->setUsername(config->getUserName());
	cp->setPassword(config->getPassword());
	cp->setPort(config->getPort());
	cp->setTestOnBorrow(config->getTestOnBorrow());
	cp->setTestOnBorrowSql(config->getTestOnBorrowSql());
	cp->setMaxWaitTime(config->getMaxWaitTime());
	cp->setMaxConnectionCount(config->getMaxConnectCount());
}