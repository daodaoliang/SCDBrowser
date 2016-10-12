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

	int						m_nSpace;	//两条虚端子间距;
	QList<QSignalInfo>		m_vtList;	//虚端子列表;
	QList<QSignalInfo>		m_vtDestList;//对端虚端子列表;

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
    int m_iOutSignalCount; // 输出信号总数量，若一个信号对应多个输入则累计

    QList<QSignalInfo>		m_SendSrcList; //输出回路中本侧输出信号列表;
    QList<QSignalInfo>		m_SendDestList; //输出回路中对侧输入信号列表;

    QList<QSignalInfo>		m_ReceiveSrcList; //输入回路中对侧输出信号列表;
    QList<QSignalInfo>		m_ReceiveDestList; //输入回路中本侧输入信号列表;

    IntervalCreator *interval;
    bool m_ShowVirLinkOnly;
    void ShowOutSignalInTerminalGraphic();
    bool IsVirLinkInSameInterval(QString srcSignalKey, QString destSignalKey);
    void DrawNormalTerminalGraphic();
    void DrawDiffTerminalGraphic();
};

