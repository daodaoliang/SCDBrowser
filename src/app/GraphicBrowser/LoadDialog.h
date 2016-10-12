#pragma once
#include <QDialog> 
#include <QLabel> 
#include <QPainter> 
#include <QMovie>
#include <QTimer>

class CLoadDialog : public QDialog
{
public:
	CLoadDialog(QWidget *parent = 0);
	~CLoadDialog(void);

private:
	QMovie *m_movie; 
	QLabel *m_label;
	QTimer *m_timer;
	QString strWait;
	QString dots;
};

