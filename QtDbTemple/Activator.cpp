#include "Activator.h"
#include <QDebug>
#include <ctkPlugin.h>
#include "interface/AkPanelService.h"

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
	m_dbInerface.reset(new DbInterface());

	ctkDictionary properties;
	properties.insert(ctkPluginConstants::SERVICE_VENDOR, "ak");
	properties.insert(ctkPluginConstants::SERVICE_DESCRIPTION, "Db Inerface");
	properties.insert(ctkPluginConstants::SERVICE_PID, "QtDbTemple");

	dbRegistration = PLUGIN_CONTEXT->registerService<ak::AkPluginOperateInterface>(m_dbInerface.data(), properties);
}

void Activator::stop(ctkPluginContext *context)
{
  Q_UNUSED(context);
  dbRegistration.unregister();
  m_dbInerface.reset(0);
  PLUGIN_CONTEXT = nullptr;
}
