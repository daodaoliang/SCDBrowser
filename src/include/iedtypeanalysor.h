/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      iedtypeanalysor.h
  * Author:         nzm
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   根据IED名称及描述处理IED类型
  */ 
#ifndef IEDTYPEANALYSOR_H
#define IEDTYPEANALYSOR_H

#include <QtGui/QMainWindow>
#include "ui_iedtypeanalysor.h"
#include "GlobalDef.h"

class IEDTypeAnalysor : public QMainWindow
{
	Q_OBJECT

public:
	IEDTypeAnalysor(QWidget *parent = 0, Qt::WFlags flags = 0);
	IEDType parseIEDType(QString name,QString desc);
	void setParsePrefix();
	~IEDTypeAnalysor();

private:
	void setInitalVal();
	void setLineVal();
	Ui::IEDTypeAnalysorClass ui;
	QString filename;
	QString prefixP;
	QString prefixC;
	QString prefixM;
	QString prefixI;
	QString prefixBC_1,prefixBC_2;
	QString prefixHZ_1,prefixHZ_2;

	QString prefixPKey_1,prefixPKey_2;
	QString prefixCKey_1,prefixCKey_2;
	QString prefixMKey_1,prefixMKey_2;
	QString prefixIKey_1,prefixIKey_2;
	QString prefixBCKey_1,prefixBCKey_2;
	QString prefixHZKey_1,prefixHZKey_2;


private slots:

	void  on_pushButton_clicked();
	void  on_pushButton_2_clicked();
	
};

#endif // IEDTYPEANALYSOR_H
