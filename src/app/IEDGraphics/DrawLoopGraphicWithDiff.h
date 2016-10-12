#pragma once
#include "HmiGraphicsScene.h"
#include "hmidll_global.h"
#include "GlobalDefine.h"
#include "VirSignalItem.h"
#include "csofttripitem.h"

class HMIDLL_EXPORT DrawLoopGraphicWithDiff
{
public:
	DrawLoopGraphicWithDiff(void);
	DrawLoopGraphicWithDiff(HmiGraphicsScene *scene, const QString masterIEDKey, const QString slaveIEDKey, 
		const QString CtrlBlockKey, const QSignalTypeEnum sigType, int sigArrow);
	~DrawLoopGraphicWithDiff(void);

	//��������IED�ı���ͷ;
	void DrawIedHeaders();

	//����һ�����·�������������--������--�������;
	void DrawOneVloop(QSignalInfo srcVT, QSignalInfo descVT, int yPos, int linkIndex, bool isSame = false);

	//�����������·;
	void DrawAllVloops();

private:
	void splitMasterSlaveKeys(QString msKeys);

	//�����·��������;
	void SortVirtualLoops();

	void FillDestSignalInfos();

	QString GetItemIDStr();

	void isDifferGraphic();

	//void FillSigInfos(const QVector<QVirtualLink> vLinks);

private:
	HmiGraphicsScene *mainScene;
	CVirSignalItem *m_LastSigItem;
	CSoftTripItem *m_LastSoftTripItem;
	QString m_MasterIEDKey;
	QString m_SlaveIEDKey;
	QString m_CtrlBlockKey;

	QVector<QVirtualLink> m_virLinks;
	QList<QSignalInfo>	m_vtListOfMaster;	//������б�;
	QList<QSignalInfo>	m_vtListOfSlave;

	QVector<QString> m_SrcSigKeys;
	QVector<QString> m_DesSigKeys;

	int m_SigArrow;				//��ͷ����:0Ϊ����,1Ϊ����;
	bool m_IsDiffGraphic;
};

