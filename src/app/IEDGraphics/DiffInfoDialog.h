/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      DiffInfoDialog.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ���컯�ȽϺ������컯ͼ��ʱ����Ӧ�Ի���
  */ 
#pragma once
#include "qdialog.h"
#include "QLabel"
#include "QTextEdit"
#include "QVBoxLayout"

class CDiffInfoDialog :	public QDialog
{
	Q_OBJECT
public:
	CDiffInfoDialog(void);
	CDiffInfoDialog(QString diffInfo);
	~CDiffInfoDialog(void);
	bool compareInfo(QString compareinfo);
	void setDiffInfo(const QString diffInfo);

protected:
	void closeEvent(QCloseEvent * event);

signals:
	void CLose();

private:
	QString m_DiffInfo;
	QTextEdit *m_Label;
	QVBoxLayout *m_VLayout;

};

