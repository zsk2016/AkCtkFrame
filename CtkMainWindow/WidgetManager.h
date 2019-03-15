#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H
#include <QWidget>
#include <QObject>
#include <ctkServiceTracker.h>
#include "WidgetShow.h"
#include "LoginWidget.h"
#include "interface/AkPanelService.h"
#include <service/event/ctkEvent.h>

class WidgetManager : public QWidget, public ctkServiceTracker<ak::AkPluginOperateInterface *>
{
	Q_OBJECT
public:
	WidgetManager(QWidget *parent = 0);
	~WidgetManager();
	WidgetShow *getWidgetShowPoint() { return m_widgetShow.data(); }
public slots:
	void handleEvent(const ctkEvent &event);
protected:
	virtual ak::AkPluginOperateInterface * addingService(const ctkServiceReference& reference);
	virtual void modifiedService(const ctkServiceReference& reference, ak::AkPluginOperateInterface * service);
	virtual void removedService(const ctkServiceReference& reference, ak::AkPluginOperateInterface * service);
private:
	void startWidgetShow();
private:
	QScopedPointer<WidgetShow> m_widgetShow;

};

#endif // WIDGETMANAGER_H
