#include <QCoreApplication>

#include "db/Sqls.h"
#include "db/DBUtil.h"
#include "db/ConnectionPool.h"
#include "util/TypeDefines.h"
#include "util/ConfigUtil.h"
#include <QDebug>

struct plan_info
{
    INTEGER PLAN_ID;
    REAL    PLAN_NUMBER;
    TEXT    PLAN_NAME;
};

plan_info mapToObject(const  QVariantMap &row_map)
{
    plan_info obj;
    obj.PLAN_ID = row_map.value("PLAN_ID").toLongLong();
    obj.PLAN_NUMBER = row_map.value("PLAN_NUMBER").toDouble();
    obj.PLAN_NAME = row_map.value("PLAN_NAM").toString();
    return obj;
}

void initDatabase()
{
    ConfigUtil *config = ConfigUtil::getInstance();
    ConnectionPool *cp = ConnectionPool::getInstance();
    qDebug()<<config->getDatabaseName();
    qDebug()<<config->getHomeName();
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

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	initDatabase();

	QString sql = Sqls::getInstance()->getSql("PLAN_INFO", "queryAll");

	QList<plan_info> pi = DBUtil::selectBeans(mapToObject, sql);
	qDebug() << pi.at(0).PLAN_ID;

	return a.exec();
}
