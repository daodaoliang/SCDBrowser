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

	//绘制主从IED的标题头;
	void DrawIedHeaders();

	//绘制一条虚回路，包括主虚端子--连接线--从虚端子;
	void DrawOneVloop(QSignalInfo srcVT, QSignalInfo descVT, int yPos, int linkIndex, bool isSame = false);

	//绘制所有虚回路;
	void DrawAllVloops();

private:
	void splitMasterSlaveKeys(QString msKeys);

	//对虚回路进行排序;
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
	QList<QSignalInfo>	m_vtListOfMaster;	//虚端子列表;
	QList<QSignalInfo>	m_vtListOfSlave;

	QVector<QString> m_SrcSigKeys;
	QVector<QString> m_DesSigKeys;

	int m_SigArrow;				//箭头方向:0为向右,1为向左;
	bool m_IsDiffGraphic;
};

