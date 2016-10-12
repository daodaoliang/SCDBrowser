/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VoltageLevelRectItem.h
  * Author:         牛子明
  * Description:   绘制子网图中的电压等级框及其内部文字
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "HmiGraphicsScene.h"

//电压等级图元 (用于子网图);
class CVoltageLevelRectItem :
	public QGraphicsItem
{
public:
	CVoltageLevelRectItem(HmiGraphicsScene *hmiScene);
	~CVoltageLevelRectItem(void);

	void setVoltageLevel(int voltageLevel);	//设置电压等级;
	void setItemWidth(int width);			//设置图元宽度;
	void setItemHeight(int height);			//设置图元高度;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);

private:
	void setBrushColorByVoltageLevel();			//根据电压等级设置画刷颜色;
	void setVoltageLevelNameByVoltageLevel();	//根据电压等级设置显示文字;

private:
	HmiGraphicsScene *mainScene;	//主场景;
	int m_VoltageLevel;				//电压等级;
	int m_Width;					//图元宽度;
	int m_Height;					//图元高度;
	QFont textFont;					//文字字体;
	QColor m_BrushClr;				//画刷颜色;
	QColor m_TextClr;				//字体颜色;
	QString m_VoltageLevelName;		//显示文字;
};

