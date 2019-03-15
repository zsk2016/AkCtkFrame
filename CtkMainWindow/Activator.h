#ifndef ACTIVATOR_H
#define ACTIVATOR_H

#include <QObject>
#include <ctkPluginActivator.h>
#include "widgetmanager.h"

class WidgetManager;

class Activator : public QObject , public ctkPluginActivator
{
    Q_OBJECT
    Q_INTERFACES(ctkPluginActivator)
    Q_PLUGIN_METADATA(IID "ak.CtkMainWindow")

public:
    explicit Activator(QObject *parent = 0);
    ~Activator();

    void start(ctkPluginContext* context);
    void stop(ctkPluginContext* context);

public:
    static ctkPluginContext* PLUGIN_CONTEXT;
    static ctkPluginContext* getPluginContext() { return PLUGIN_CONTEXT; }

private:
    QScopedPointer<WidgetManager> m_widgetManager;
};

#endif // ACTIVATOR_H
