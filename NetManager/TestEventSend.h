#pragma once
#include <QWidget>

class TestEventSend : public QWidget
{
	Q_OBJECT
public:
	TestEventSend(QWidget *parent = 0);

public slots:
	void onBtnSend();
};