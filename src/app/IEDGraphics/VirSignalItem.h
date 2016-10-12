/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VirSignalItem.h
  * Author:         nzm
  * Description:   ���ƶ�����ͼ�����·ͼ���źſ�������
  */ 
#pragma once
#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

class CVirSignalItem : public CMyGraphItem
{
public:
	CVirSignalItem(HmiGraphicsScene *hmiScene);
	~CVirSignalItem(void);

	//���л�����;
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	//��궯����Ӧ����;
	void	mousePressEvent(QGraphicsSceneMouseEvent *event);
	void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void	hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void	hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	//��ѡ��ʱ������ȡ������;
	void	highLight();
	void	alwaysHighLight();
	void	highLightCancel();

	void	setVirSignalName(QString virsignalName);		//�������ź�����;
	void	setHeight(int height);	//���ø߶�;
	void	setWidth(int width);	//���ø߶�;
	void	setChangeType(ItemChangeType changeType);		//���ñ������;
	void	setToolTipPos(int xPos, int yPos);
	void	appendAssoID(QString assoID);
	QVector<QString> getAssoIDs();
	int		getHeight();
	void	setRightImage(bool isRight);
	void	setDescEmpty(bool isDEmpty);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;
	QRectF m_Rect;
	QString m_VirSignalName;
	int m_Height;
	int m_Width;
	ItemChangeType m_ChangeType;
	QVector<QString> m_assoIDs;
	bool m_IsInRect;
	bool middleWay;//���з�ʽ
	bool isRightImage;
	bool isDescEmpty;

    // add by zxc
public:
    void SetSignalOrd(int ord);
    void SetSignalPath(QString path);

private:
    int m_Ord;
    QString m_SignalPath;
    bool m_iVirLink; //�ж��ź��Ƿ�Ϊ���·
    QColor m_BrushColor;
    QString ChangeSignalPathToNormal(QString path);
};

