/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      GraphicTitleTable.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   信息流图标题栏的绘制
  */ 
#ifndef CGRAPHICTITLETABLE_H
#define CGRAPHICTITLETABLE_H

#include "HmiGraphicsScene.h"

//标题栏所需信息;
typedef struct IEDTitleInfo 
{
	QString IEDName;	//装置名称;
	QString IEDModel;	//装置型号;
	QString confVer;	//装置版本;
	QString manufac;	//装置厂商;
	QString desc;		//装置描述;
	QString IEDTypeStr;	//装置类型;
	QString CRCCode;	//CRC校验码;
	QString smv;		//采样率;
	QString GooseInOut;	//GOOSE输入输出;
	QVector<QString> subNetName;	//接入子网;
} IEDTITLEINFO;

class CGraphicTitleTable
{
public:
	CGraphicTitleTable(HmiGraphicsScene* mScene);
	~CGraphicTitleTable(void);

	void setOrigenPt(const int xVal, const int yVal);	//设置原点;
	void setOrigenPt(const QPoint& origenPt);
	void setRowHeight(const int rHeight);				//设置行高;
	void setColumnWidth(const int cWidth);				//设置列宽;
	void setRowNum(const int rNum);						//设置行数;
	void setFontPointSize(const int fSize);				//设置字号;

	int  getRowHeight();								//获取行高;
	int  getColumnWidth();								//获取列宽;

	//绘制标题栏表格;
	void DrawTitleInfo(const QString& masterIEDKey, const IEDTitleInfo& masterIEDInfo);
private:
	HmiGraphicsScene *mainScene;	//主场景;
	QFont *textFont;		//字体;
	QPen  *linePen;			//线画笔;
	QPen  *textPen;			//文字画笔;

	int xPtValue;			//标题栏原点横坐标值;
	int yPtValue;			//标题栏原点纵坐标值;
	int rowHeight;			//单行高度;
	int columnWidth;		//单列宽度;
	int rowNum;				//行数;
	int fontPtSize;			//字号;
};

#endif //CGRAPHICTITLETABLE_H