#pragma once
#include <QObject>

#include "db/Sqls.h"
#include "db/DBUtil.h"
#include "db/ConnectionPool.h"
#include "util/TypeDefines.h"
#include "util/ConfigUtil.h"
#include "interface/AkPanelService.h"
#include <QDebug>
#include <QVariantMap>

class DbInterface : public QObject, public ak::AkPluginOperateInterface
{
	Q_OBJECT
	Q_INTERFACES(ak::AkPluginOperateInterface)
public:
	DbInterface(QObject *parent = 0);
	~DbInterface();
	virtual QList<QVariantMap> getSql(QString tableName, QString sqlStr);
private:
	void initDb();
};