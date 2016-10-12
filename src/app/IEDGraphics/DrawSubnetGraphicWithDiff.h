#ifndef CDRAWSUBNETGRAPHICWITHDIFF_H
#define CDRAWSUBNETGRAPHICWITHDIFF_H

#include "HmiGraphicsScene.h"
#include "SCLNameTranslator.h"

//����ͼ������(��ʽ����);
class CDrawSubnetGraphicWithDiff
{
public:
	CDrawSubnetGraphicWithDiff(void);
	CDrawSubnetGraphicWithDiff(HmiGraphicsScene *scene, const QString &subNetKey);
	~CDrawSubnetGraphicWithDiff(void);

	void DrawSubnetGraphic();	//��������ͼ;

	void setRowDistance(int rowDis);		//�����м��;
	void setColumnDistance(int columnDis);	//�����м��;

private:
	//���ݵ�ѹ�ȼ�����;
	void sortSubIEDsByVoltageLevel(QMap<int, QIEDInfo>& iedMap);
	void sortSubIEDs();						//������������;

	//��������ͼ��������;
	void DrawSubnetLine();
	//��������IED;
	void DrawSubnetIEDs(QVector<int>& IEDsVec, int vltName, int& xPos, int& yPos);
	//�������߿�;
	void DrawDotLineRect(const int typeIEDSum, int rectWidth, IEDType iedType, int xPos, int yPos);
	//���Ƶ���IEDͼԪ;
	IEDType DrawOneIEDItem(const QString& iedKey, int xPos, int yPos);

	void GetColumnCount();			//��ȡ�������;
	void GetMaxRowCount();			//��ȡ�������;

	QVector<int>* GetVectorPoint(int index);	//������������ȡ��Ӧ������;

private:
	HmiGraphicsScene* mainScene;	//������;
	int m_rowDis;					//�м��(IEDͼԪ�ײ�����һ��IEDͼԪ�����Ĵ�ֱ����);
	int m_columnDis;				//�м��(IEDͼԪ�Ҳ�����һ��IEDͼԪ���Ĵ�ֱ����);
	int m_IEDItemWidth;				//IEDͼԪ���;
	int m_IEDItemHeight;			//IEDͼԪ�߶�;
	int m_VtlItemWidth;				//��ѹ�ȼ�ͼԪ���;
	int m_VtlItemHeight;			//��ѹ�ȼ�ͼԪ�߶�;
	int leftBorder;					//��߿�;
	int topBorder;					//���߿�;
	int m_columnCount;				//����;
	int m_maxRowCount;				//�������;
	int m_rectMargin;				//���߿��Ե;
	QString	m_subNetKey;			//����Key;
	QVector<QString> m_subIEDs;		//װ���б�;
	SCLNameTranslator *sclNameTranslator;	//������Keyת����;

	//����ѹ�ȼ�����,QVector�д洢IED��m_subIEDs�е����;
	QVector<int> mainTransIEDs;		//����IED;
	QVector<int> otherTransIEDs;	//����IED;
	QVector<int> IEDs_6kV;			//6ǧ��IED;
	QVector<int> IEDs_10kV;			//10ǧ��IED;
	QVector<int> IEDs_35kV;			//35ǧ��IED;
	QVector<int> IEDs_66kV;			//66ǧ��IED;
	QVector<int> IEDs_110kV;		//110ǧ��IED;
	QVector<int> IEDs_220kV;		//220ǧ��IED;
	QVector<int> IEDs_330kV;		//330ǧ��IED;
	QVector<int> IEDs_500kV;		//500ǧ��IED;
	QVector<int> IEDs_750kV;		//750ǧ��IED;
	QVector<int> IEDs_1000kV;		//1000ǧ��IED;
};

#endif	//CDRAWSUBNETGRAPHICWITHDIFF_H;