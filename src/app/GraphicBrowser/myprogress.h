#ifndef MYPROGRESS_H
#define MYPROGRESS_H

#include <QDialog>
#include "ui_myprogress.h"
#include <QMovie>
#include <QCloseEvent> 

class Myprogress : public QDialog
{
	Q_OBJECT

public:
	Myprogress(QWidget *parent = 0);
	~Myprogress();
	void setPorgressValue(int value);
	void setFileText(QString text,int current,int total);
	void setLabeCurrentText(QString text);
private:
	QMovie *moive;
	Ui::Myprogress ui;
signals:
	void sendHideState();

private slots:
	void backRunButton();

};

#endif // MYPROGRESS_H
