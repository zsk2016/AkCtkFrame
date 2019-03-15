#include "Activator.h"
#include <QDebug>
#include "widgetmanager.h"
#include <ctkPlugin.h>
#include <service/event/ctkEventConstants.h>
#include <service/event/ctkEventAdmin.h>

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

	m_widgetManager.reset(new WidgetManager);
	m_widgetManager->open();

	QList<QSharedPointer<ctkPlugin> > lp = Activator::getPluginContext()->getPlugins();
	QStringList sl;
	for (int i = 0; i < lp.size(); i++)
		sl << lp.at(i)->getSymbolicName();
	m_widgetManager->getWidgetShowPoint()->initTableWidget(sl);


	ctkServiceTracker<ctkEventAdmin *> tracker(context);
	tracker.open();

	ctkEventAdmin *event_admin = tracker.getService();
	Q_ASSERT(event_admin);

	ctkDictionary props;
	props[ctkEventConstants::EVENT_TOPIC] = "testSend";

	event_admin->subscribeSlot(m_widgetManager.data(), SLOT(handleEvent(ctkEvent)), props);
}

void Activator::stop(ctkPluginContext *context)
{
  Q_UNUSED(context);
  m_widgetManager.reset(0);
  PLUGIN_CONTEXT = nullptr;
}
