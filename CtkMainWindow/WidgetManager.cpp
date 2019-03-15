#include "WidgetManager.h"
#include "LoginWidget.h"
#include "WidgetShow.h"
#include "Activator.h"

WidgetManager::WidgetManager(QWidget *parent)
	:QWidget(parent)
	,ctkServiceTracker<ak::AkPluginOperateInterface *>(Activator::PLUGIN_CONTEXT)
{
	startWidgetShow();
}

WidgetManager::~WidgetManager()
{

}

void WidgetManager::startWidgetShow()
{
	m_widgetShow.reset(new WidgetShow(this));
	m_widgetShow->show();
}

ak::AkPluginOperateInterface * WidgetManager::addingService(const ctkServiceReference &reference)
{
	ctkPluginContext *context = Activator::getPluginContext();
	ak::AkPluginOperateInterface *interface = context->getService<ak::AkPluginOperateInterface>(reference);
	
	return interface;
}

void WidgetManager::modifiedService(const ctkServiceReference &reference, ak::AkPluginOperateInterface *service)
{

}

void WidgetManager::removedService(const ctkServiceReference &reference, ak::AkPluginOperateInterface *service)
{
	
}

void WidgetManager::handleEvent(const ctkEvent &event)
{
	qDebug() << Q_FUNC_INFO << endl
		<< "Topic: " << event.getTopic() << endl
		<< "Property[content]" << event.getProperty("content").toString() << endl;
}