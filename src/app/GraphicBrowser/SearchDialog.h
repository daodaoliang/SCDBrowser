/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      SearchDialog.h
  * Author:         牛子明;
  * Version:        1.0.0
  * Date:           2015/1/22
  * Description:   查找对话框类,负责获取输出数据;
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

	void setTextEditFocus();//设置输入框获得焦点;

signals:
	void SearchText();		//查找给定字符串;

private slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();

private:
	Ui::SearchDialog ui;	//对话框UI设计;
};

#endif
