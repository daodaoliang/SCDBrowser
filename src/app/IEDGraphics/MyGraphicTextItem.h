/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      MyGraphicTextItem.h
  * Author:         nzm
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ����������������Ϣ�Ļ��Ƽ���ת����
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "HmiGraphicsScene.h"

//��Ϣ��ͼ�б�������������ʾ��ͼԪ;
class CMyGraphicTextItem : public QGraphicsItem
{
public:
	CMyGraphicTextItem(void);
	CMyGraphicTextItem(HmiGraphicsScene *hmiScene);
	~CMyGraphicTextItem(void);

	//���л�����;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);

	//��궯����Ӧ����;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void setDisplayText(const QString& disText);	//������ʾ����;
	void setSubnetKey(const QString& subnetKey);	//��������Key;
	void setFontSize(const int fontSize);			//�����ֺ�;

	int  getDisplayTextLength();					//��ȡ��ʾ�ַ������;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene* mainScene;	//������;
	QString m_Text;			//��ʾ����;
	QString m_SubnetKey;	//����Key;
	int		m_FontSize;		//�ֺ�;
	QFont textFont;			//����;
	QPen  textPen;			//����;
};

