#include "WidgetShow.h"
#include <QPushButton>
#include "Activator.h"

WidgetShow::WidgetShow(QObject *parent /* = 0 */)
	:m_uiForm(new Ui::Form)
{
	m_uiForm->setupUi(this);
}

void WidgetShow::initTableWidget(QStringList sl)
{
	m_uiForm->tableWidget->setRowCount(sl.size());
	for (int i = 0; i < sl.size(); i++)
	{
		QTableWidgetItem *newItem = new QTableWidgetItem(sl.at(i));
		m_uiForm->tableWidget->setItem(i, 0, newItem);
		QPushButton *btn = new QPushButton(sl.at(i), this);
		connect(btn, SIGNAL(clicked()), this, SLOT(onStartPlugins()));
		btn->setObjectName(QString(sl.at(i)));
		m_uiForm->tableWidget->setCellWidget(i, 1, btn);
	}
}

void WidgetShow::onStartPlugins()
{
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	QString plugName = button->objectName();
	QList<QSharedPointer<ctkPlugin> > lp = Activator::getPluginContext()->getPlugins();
	for (int i = 0; i < lp.size(); i++)
	{
		if (lp.at(i)->getSymbolicName() == plugName)
		{
			lp.at(i)->start(ctkPlugin::START_TRANSIENT);
		}	
	}
}