#include "Activator.h"
#include <QDebug>
#include <ctkPlugin.h>


ctkPluginContext* Activator::PLUGIN_CONTEXT = NULL;

Activator::Activator(QObject *parent) :
    QObject(parent)
{
  // TODO: Init the resources that used by this plugin
  qDebug() << "AsuMainActivator" << "loading now";
}

Activator::~Activator()
{
}

void Activator::start(ctkPluginContext *context)
{
    PLUGIN_CONTEXT = context;
  // TODO: Prepare the resources that used by this plugin

	m_netManager.reset(new NetManager());

	ctkDictionary properties;
	properties.insert(ctkPluginConstants::SERVICE_VENDOR, "ak");
	properties.insert(ctkPluginConstants::SERVICE_DESCRIPTION, "netmanager");
	properties.insert(ctkPluginConstants::SERVICE_PID, "NetManager");
	dbRegistration = PLUGIN_CONTEXT->registerService<ak::AkPluginOperateInterface>(m_netManager.data(), properties);

	m_testEventSend.reset(new TestEventSend());	
	m_testEventSend.data()->show();
}

void Activator::stop(ctkPluginContext *context)
{
  Q_UNUSED(context);
  PLUGIN_CONTEXT = nullptr;
}
