#pragma once
#include <QObject>
#include "interface/AkPanelService.h"
#include <QVariantMap>

class NetManager : public QObject, public ak::AkPluginOperateInterface
{
	Q_OBJECT
	Q_INTERFACES(ak::AkPluginOperateInterface)
public:
	NetManager(QObject *parent = 0);

};
