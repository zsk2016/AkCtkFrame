#include "TestEventSend.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <service/event/ctkEventAdmin.h>
#include <ctkServiceTracker.h>
#include "Activator.h"

TestEventSend::TestEventSend(QWidget *parent /* = 0 */)
	:QWidget(parent)
{
	QPushButton *btn = new QPushButton("testSend");
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(btn);
	setLayout(hbox);
	resize(300, 300);
	connect(btn, SIGNAL(clicked()), this, SLOT(onBtnSend()));
}

void TestEventSend::onBtnSend()
{
	/*ctkPluginContext *context = Activator::getPluginContext();
	ctkServiceTracker<ctkEventAdmin *> tracker(context);
	tracker.open();

	ctkEventAdmin *event_admin = tracker.getService();

	if (0 == event_admin)
		return;

	ctkDictionary event_props;
	event_props.insert("content", "test.....");
	ctkEvent event("testSend", event_props);
	event_admin->sendEvent(event);*/

	ctkPluginContext *context = Activator::getPluginContext();
	ctkServiceTracker<ctkEventAdmin *> tracker(context);
	tracker.open();

	ctkEventAdmin *event_admin = tracker.getService();

	if (0 == event_admin)
		return;

	ctkDictionary event_props;
	event_props.insert("content", "ctkEventAdmin->postEvent");
	ctkEvent event("testSend", event_props);
	event_admin->postEvent(event);
}