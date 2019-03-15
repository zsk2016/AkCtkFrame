#pragma once
#include "GeneratedFiles/ui_WidgetShow.h"
#include <QWidget>
#include <QObject>
class WidgetShow : public QWidget
{
	Q_OBJECT
public:
	WidgetShow(QObject *parent = 0);
	void initTableWidget(QStringList sl);
private slots:
	void onStartPlugins();
private:
	Ui::Form * m_uiForm;
};
