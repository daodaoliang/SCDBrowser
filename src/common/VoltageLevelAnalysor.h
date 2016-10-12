/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VoltageLevelAnalysor.h
  * Author:         nzm
  * Description:   根据IED名称和描述对电压等级进行分类
  */ 
#ifndef CVOLTAGELEVELANALYSOR_H
#define CVOLTAGELEVELANALYSOR_H

#include <QString>

#define VOLTAGELEVEL_MAINTRANS      1		//主变;
#define VOLTAGELEVEL_OTHERS		    0		//其他;
#define VOLTAGELEVEL_6KV			6		//6kV
#define VOLTAGELEVEL_10KV			10		//10kV
#define VOLTAGELEVEL_35KV			35		//35kV
#define VOLTAGELEVEL_66KV			66		//66kV
#define VOLTAGELEVEL_110KV			110		//110kV
#define VOLTAGELEVEL_220KV			220		//220kV
#define VOLTAGELEVEL_330KV			330		//330kV
#define VOLTAGELEVEL_500KV			500		//500kV
#define VOLTAGELEVEL_750KV			750		//750kV
#define VOLTAGELEVEL_1000KV			1000	//1000kV

class CVoltageLevelAnalysor
{
public:
	CVoltageLevelAnalysor(void);
	~CVoltageLevelAnalysor(void);

	int analysisVoltageLevelByIED(const QString& IEDName, const QString& IEDDesc);
private:
};

#endif