/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      SearchDialog.h
  * Author:         ţ����;
  * Version:        1.0.0
  * Date:           2015/1/22
  * Description:   ���ҶԻ�����,�����ȡ�������;
  */ 
#ifndef CSEARCHDIALOG_H
#define CSEARCHDIALOG_H

#include <QDialog>
#include "ui_QSearchDialog.h"

class CSearchDialog : public QDialog
{
	Q_OBJECT

public:
	CSearchDialog(void);
	~CSearchDialog(void);

	QString getSearchText();
	bool getIsSensitive();
	bool getIsBackward();

	void setTextEditFocus();//����������ý���;

signals:
	void SearchText();		//���Ҹ����ַ���;

private slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();

private:
	Ui::SearchDialog ui;	//�Ի���UI���;
};

#endif
