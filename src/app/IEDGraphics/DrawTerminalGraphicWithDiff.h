#pragma once
#include "HmiGraphicsScene.h"
#include "hmidll_global.h"
#include "GlobalDefine.h"
#include "PhyPortItem.h"
#include "../../common/IntervalCreator.h"

class DrawTerminalGraphicWithDiff
{
public:
	DrawTerminalGraphicWithDiff(void);
	DrawTerminalGraphicWithDiff(HmiGraphicsScene *scene, const QString IEDKey);
	~DrawTerminalGraphicWithDiff(void);

	void	Draw();

private:
	//void		SortByIOrd();
	void	FillVLinksToList();
	void	PhyPortFilter();

	QVector<int>	GetPortXPos(QString sigKey, VTermType termType);
	QVector<CPhyPortItem*>	GetPortItem(QString sigKey, VTermType termType);
	QString		GetItemIDStr();

	void isDifferGraphic();
	QString getIEDNameInDiff(QString sigKey);

private:
	HmiGraphicsScene		*mainScene;

	int						m_nSpace;	//��������Ӽ��;
	QList<QSignalInfo>		m_vtList;	//������б�;
	QList<QSignalInfo>		m_vtDestList;//�Զ�������б�;

	QString					m_IEDKey;
	QVector<QString>		m_PhyPorts;
	QVector<QVirtualLink>	m_virLinks;
	QMap<QString ,CPhyPortItem*>	m_PortItems;

	bool m_IsDiffGraphic;

    // add by zxc 
private:
    struct QOutSignalInfo
    {
        QString key;
        QString path;
        QString desc;
        QString softTripKey;
        int iOutSignalOrd;
        QList<int> outList;
    };
    QMap<QString, QVector<QOutSignalInfo>> m_OutSignalMap;
    QVector<QString>        m_CBList;
    int m_iOutSignalCount; // ����ź�����������һ���źŶ�Ӧ����������ۼ�

    QList<QSignalInfo>		m_SendSrcList; //�����·�б�������ź��б�;
    QList<QSignalInfo>		m_SendDestList; //�����·�жԲ������ź��б�;

    QList<QSignalInfo>		m_ReceiveSrcList; //�����·�жԲ�����ź��б�;
    QList<QSignalInfo>		m_ReceiveDestList; //�����·�б��������ź��б�;

    IntervalCreator *interval;
    bool m_ShowVirLinkOnly;
    void ShowOutSignalInTerminalGraphic();
    bool IsVirLinkInSameInterval(QString srcSignalKey, QString destSignalKey);
    void DrawNormalTerminalGraphic();
    void DrawDiffTerminalGraphic();
};

