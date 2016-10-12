/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      GraphicTitleTable.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ��Ϣ��ͼ�������Ļ���
  */ 
#ifndef CGRAPHICTITLETABLE_H
#define CGRAPHICTITLETABLE_H

#include "HmiGraphicsScene.h"

//������������Ϣ;
typedef struct IEDTitleInfo 
{
	QString IEDName;	//װ������;
	QString IEDModel;	//װ���ͺ�;
	QString confVer;	//װ�ð汾;
	QString manufac;	//װ�ó���;
	QString desc;		//װ������;
	QString IEDTypeStr;	//װ������;
	QString CRCCode;	//CRCУ����;
	QString smv;		//������;
	QString GooseInOut;	//GOOSE�������;
	QVector<QString> subNetName;	//��������;
} IEDTITLEINFO;

class CGraphicTitleTable
{
public:
	CGraphicTitleTable(HmiGraphicsScene* mScene);
	~CGraphicTitleTable(void);

	void setOrigenPt(const int xVal, const int yVal);	//����ԭ��;
	void setOrigenPt(const QPoint& origenPt);
	void setRowHeight(const int rHeight);				//�����и�;
	void setColumnWidth(const int cWidth);				//�����п�;
	void setRowNum(const int rNum);						//��������;
	void setFontPointSize(const int fSize);				//�����ֺ�;

	int  getRowHeight();								//��ȡ�и�;
	int  getColumnWidth();								//��ȡ�п�;

	//���Ʊ��������;
	void DrawTitleInfo(const QString& masterIEDKey, const IEDTitleInfo& masterIEDInfo);
private:
	HmiGraphicsScene *mainScene;	//������;
	QFont *textFont;		//����;
	QPen  *linePen;			//�߻���;
	QPen  *textPen;			//���ֻ���;

	int xPtValue;			//������ԭ�������ֵ;
	int yPtValue;			//������ԭ��������ֵ;
	int rowHeight;			//���и߶�;
	int columnWidth;		//���п��;
	int rowNum;				//����;
	int fontPtSize;			//�ֺ�;
};

#endif //CGRAPHICTITLETABLE_H