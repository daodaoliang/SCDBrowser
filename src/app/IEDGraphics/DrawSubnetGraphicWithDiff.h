#ifndef CDRAWSUBNETGRAPHICWITHDIFF_H
#define CDRAWSUBNETGRAPHICWITHDIFF_H

#include "HmiGraphicsScene.h"
#include "SCLNameTranslator.h"

//子网图布局类(帘式布局);
class CDrawSubnetGraphicWithDiff
{
public:
	CDrawSubnetGraphicWithDiff(void);
	CDrawSubnetGraphicWithDiff(HmiGraphicsScene *scene, const QString &subNetKey);
	~CDrawSubnetGraphicWithDiff(void);

	void DrawSubnetGraphic();	//绘制子网图;

	void setRowDistance(int rowDis);		//设置行间距;
	void setColumnDistance(int columnDis);	//设置列间距;

private:
	//根据电压等级排序;
	void sortSubIEDsByVoltageLevel(QMap<int, QIEDInfo>& iedMap);
	void sortSubIEDs();						//根据类型排序;

	//绘制子网图各横竖线;
	void DrawSubnetLine();
	//绘制所有IED;
	void DrawSubnetIEDs(QVector<int>& IEDsVec, int vltName, int& xPos, int& yPos);
	//绘制虚线框;
	void DrawDotLineRect(const int typeIEDSum, int rectWidth, IEDType iedType, int xPos, int yPos);
	//绘制单个IED图元;
	IEDType DrawOneIEDItem(const QString& iedKey, int xPos, int yPos);

	void GetColumnCount();			//获取最大列数;
	void GetMaxRowCount();			//获取最大行数;

	QVector<int>* GetVectorPoint(int index);	//根据列索引获取相应的数组;

private:
	HmiGraphicsScene* mainScene;	//主场景;
	int m_rowDis;					//行间距(IED图元底部与下一行IED图元顶部的垂直距离);
	int m_columnDis;				//列间距(IED图元右侧与下一列IED图元左侧的垂直距离);
	int m_IEDItemWidth;				//IED图元宽度;
	int m_IEDItemHeight;			//IED图元高度;
	int m_VtlItemWidth;				//电压等级图元宽度;
	int m_VtlItemHeight;			//电压等级图元高度;
	int leftBorder;					//左边框;
	int topBorder;					//顶边框;
	int m_columnCount;				//列数;
	int m_maxRowCount;				//最大行数;
	int m_rectMargin;				//虚线框边缘;
	QString	m_subNetKey;			//子网Key;
	QVector<QString> m_subIEDs;		//装置列表;
	SCLNameTranslator *sclNameTranslator;	//名称与Key转换器;

	//按电压等级分类,QVector中存储IED在m_subIEDs中的序号;
	QVector<int> mainTransIEDs;		//主变IED;
	QVector<int> otherTransIEDs;	//其他IED;
	QVector<int> IEDs_6kV;			//6千伏IED;
	QVector<int> IEDs_10kV;			//10千伏IED;
	QVector<int> IEDs_35kV;			//35千伏IED;
	QVector<int> IEDs_66kV;			//66千伏IED;
	QVector<int> IEDs_110kV;		//110千伏IED;
	QVector<int> IEDs_220kV;		//220千伏IED;
	QVector<int> IEDs_330kV;		//330千伏IED;
	QVector<int> IEDs_500kV;		//500千伏IED;
	QVector<int> IEDs_750kV;		//750千伏IED;
	QVector<int> IEDs_1000kV;		//1000千伏IED;
};

#endif	//CDRAWSUBNETGRAPHICWITHDIFF_H;