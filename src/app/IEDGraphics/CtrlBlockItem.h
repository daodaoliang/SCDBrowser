/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      CtrlBlockItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ���ƿ��ͼԴ��������Ϣ��ͼ�����·ͼ��
  */ 
#ifndef CTRLBLOCK_H
#define CTRLBLOCK_H

#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

enum CtrlBlockSigType
{
	gooseType = 0,
	svType,
	inputType
};

class CtrlBlockItem : public CMyGraphItem
{
public:
	CtrlBlockItem(HmiGraphicsScene *hmiScene);
	~CtrlBlockItem(void);

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

	void	setCBName(QString cbName);	//����Name
	void	setAPPID(QString appid);	//����APPID
	void	setSigType(CtrlBlockSigType sigType);	//�����ź����� 0:Goose   1:SV   2:����;

	void	setRegularClr(const QColor clr);	//���û�����ɫ;
	void	setHighLightClr(const QColor clr);	//���ø�����ɫ;
	void	setChangeType(ItemChangeType changeType);		//���ñ������;
	void	setToolTipPos(int xPos, int yPos);

	void	setWidth(int width);
	void	setIsRight(bool isRight);

	void	appendAssoID(QString assoID);
	QVector<QString> getAssoIDs();

	QString getCBName();
	QString getAPPID();
	int		getSigType();
	QPoint	getLeftPoint();			//���������ӵ�;
	QPoint	getRightPoint();		//����Ҳ����ӵ�;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;
	QString m_Name;				//name
	QString m_APPID;			//appid
	CtrlBlockSigType m_SigType;	//���ƿ�����goose��sv�����;
	QColor m_RegularClr;		//������ɫ;
	QColor m_HighLightClr;		//������ɫ;
	QVector<QString> m_AssoItemIDs;
	int m_Width;
	QRectF m_Rect;
	ItemChangeType m_ChangeType;
	bool m_IsInRect;
	bool m_IsRight;

public:
    void SetHeight(int height);

private:
    int m_Height;

};
#endif //#ifndef CTRLBLOCK_H