#pragma once
#include "HmiGraphicsScene.h"
#include "CtrlBlockItem.h"
#include "GraphicTitleTable.h"
#include "iedtypeanalysor.h"

class HMIDLL_EXPORT DrawInfoFlowGraphicWithDiff
{
public:
	DrawInfoFlowGraphicWithDiff(void);
	DrawInfoFlowGraphicWithDiff(HmiGraphicsScene *scene, const QString &iedKey);
	~DrawInfoFlowGraphicWithDiff(void);

	void DrawGraph();			//����ͼ��,���ⲿ����;

private:
	void InitGraphicData();		//��ʼ������;
	void FillSlaveIEDVec();		//����IED�б�;
	void GetAllSlaveHeight();	//�������д�IED�߶�;
	void isDifferGraphic();		//�ж��Ƿ�Ϊ���컯״̬;

	void DrawMasterIED();		//������IED�Լ����ƿ�;
	void DrawSlaveIEDs();		//���ƴ�IED�Լ����ƿ���ź���;
	int  DrawTitleTable(IEDTitleInfo& titleInfo);	//�������½Ǳ�����;
	void ResizeSceneRect(const int titleTableWidth);//����ͼ������;

	QString DrawMasterCB(const QCtrlBlockInfo& cbInfo, const int xPos, const int yPos);

	IEDTitleInfo FillTitleInfo_Simple();	//������������ -- ��;
	IEDTitleInfo FillTitleInfo_Full();		//������������ -- ȫ��(��ʱ����,��ɾ);
	
	QString GetItemIDStr();					//��ȡͼԪID;
	
	//����Report���ƿ�;
	void reportCBFilter(QVector<QString> OrigenVec, QVector<QString> *noReVec);

	//��ȡ���ƿ�ToolTip�ַ���;
	QString GetCBToolTipStr(const QCtrlBlockInfo &ctrlBlockInfo);

private:
	HmiGraphicsScene *mainScene;			//scene;
	QString m_MasterIEDKey;					//��IED��key;
	QIEDInfo m_MasterIEDInfo;				//��IED��Info;
	IEDType m_IedType;						//��IED������;
	QVector<CtrlBlockItem*> m_MasterCBs;	//��IED�Ŀ��ƿ��б�;
	QVector<QString> m_SlaveIEDKeys;		//��IEDKey�б�;
	QVector<int> m_SlaveIEDCBCount;			//��IED�������ƿ�����;
	
	int m_AllSlaveIEDHeight;				//���д�IED�ĸ߶Ⱥ�(�����);
	int m_LeftSlaveIEDHeight;				//����IED�ĸ߶Ⱥ�(�����);
	
	int m_MasterIEDHeight;					//��IED�߶�;
	int m_MasterIEDWidth;					//��IED���;
	int m_IEDVerticalDis;					//��IEDͼԪ֮�䴹ֱ����;
	int m_IEDCBBorder;						//��IEDͼԪ����ƿ�ͼԪ���¿�϶;
	
	int m_CBHeight;							//���ƿ�ͼԪ�߶�;
	int m_CBWidth;							//���ƿ�ͼԪ���;
	int m_CBVerticalDis;					//���ƿ�ͼԪ֮�䴹ֱ����;

	QColor m_GooseCBRegularClr;				//Goose���ƿ鳣����ɫ;
	QColor m_SVCBRegularClr;				//SV���ƿ鳣����ɫ;
	QColor m_CBHighLightClr;				//���ƿ������ɫ;

	bool m_IsDiffGraphic;					//�Ƿ�Ϊ���컯ģʽ;
};