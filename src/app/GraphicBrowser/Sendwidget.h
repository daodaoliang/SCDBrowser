#ifndef GOOSESENDWIDGET_H
#define GOOSESENDWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "TableSCLModelImpl.h"
#include "SCDDiffCommon.h"
#include "TableModelDiffImpl.h"
#include "SCLNameTranslator.h"
#include "TabInfoDataSetter.h"
#include "fixedprocessbar.h"

// goose send 
class GooseSendWidget : public QWidget
{
	Q_OBJECT

public:
	GooseSendWidget(QWidget *parent=0);
	~GooseSendWidget();
	void GetGooseCtrIndex(QVector<QString> ctrlBlockList,int type);
	void GetDate(GOOSECBStru gooseinfo,QList <QString> &data);
	void GetDateDiff(GOOSECBStru gooseInfo,QList <QTableWidgetItem *> &data);
	//void fillSingleCell(int row,int col,);
	void fillGsCrlBlockListTab(QString ctrlName,int row,int type);
	void clearCell();
	void setIEDKey(QString IEDkey);
	void clearData();

private:
	QTableWidget *gsCrlBlockListWidget;
	QTableWidget *ChannelListTabWidget;
	QTableWidget *virtualTerminalTabWidget;
	QList<QTableWidgetItem *> ItemList;
    QStringList gsTabHead;
	QVector<QString> m_ctrlBlockList;
	int rowSelect;
	QString IEDKEY;
	int currnetType;
private slots:
	void fillChannelListTab();
};


//sv send;
class SvSendWidget:public QWidget
{
	Q_OBJECT

public:
	SvSendWidget(QWidget *parent=0);
	void GetSvCtrIndex(QVector<QString> ctrlBlockList,int type);
	void fillSvCrlBlockListTab(QString ctrlName,int row,int type);
	void GetDate(SVCBStru gooseinfo,QList <QString> &data);
    void GetDateDiff(SVCBStru svInfo,QList <QTableWidgetItem *> &data);
	void setIEDKey(QString IEDkey);
	void clearData();
	~SvSendWidget();
private:
	QTableWidget *svCrlBlockListWidget;
	QTabWidget *virtualTerminalTab;
	QTableWidget *virtualTerminalTabWidget;
	QVector<QString> m_ctrlBlockList;
	QList <QTableWidgetItem*>ItemList;
	QStringList gsTabHead;
	int rowSelect;
	int currentType;
	QString IEDKEY;
	void clearCell();
private slots:
	void fillChannelListTab();
};

//信息点表;
class ReportSendWidget:public QWidget
{
	Q_OBJECT
public:
	ReportSendWidget(QWidget *parent=0);
	~ReportSendWidget();
	void GetReportCtrIndex(QVector<QString> ctrlBlockList,int type);
	void fillReportCrlBlockListTab(QString ctrlName,int row,int type);
	void InitVirtualTerminalTabWidgetHead();
	void GetDate(ReportCBStru gooseinfo,QList <QString> &data);
	void GetDateDiff(ReportCBStru svInfo,QList <QTableWidgetItem *> &data);
	void setIEDKey(QString IEDkey);
	void clearData();
private:
	void getMainData(QMap<QString,QStringList>* mainMap);
	void getSubsidiaryData(QMap<QString,QVector<QStringList>>* subsidaryMap);
private:
	QTableWidget *reportCrlBlockListWidget;
	QTabWidget *virtualTerminalTab;
	QTableWidget *virtualTerminalTabWidget;
	QPushButton *ExportButton;
	QVector<QString> m_ctrlBlockList;
	QList <QTableWidgetItem*>ItemList;
	QStringList gsTabHead;
	int rowSelect;
	int currentType;
	QString IEDKEY;
	void clearCell();
	QMap<QString, QString> m_ReportCBNames;
	void InitReportCBName();
	SCLNameTranslator m_NameTransLator;
	TabInfoDataSetter *m_DataSetter;
	int m_DiffType;
private slots:
	void fillChannelListTab();
	void exportXLSFile();

private:
	FixedProcessBar *m_ProcessBar;
};

//定值条目;
class DsSettingWidget:public QWidget
{
	Q_OBJECT
public:
	DsSettingWidget(QWidget *parent=0);
	~DsSettingWidget();
	void GetDsSettingIndex(QVector<QDataSetInfo> dataSetList,int type);
	void fillDsSettingListTab(QDataSetInfo dataSetlName,int row,int type);
	void GetDate(QDataSetInfo gooseinfo,QList <QString> &data);
	void GetDateDiff(QDataSetInfo svInfo,QList <QTableWidgetItem *> &data);
	void setIEDKey(QString IEDkey);
	void clearData();

private:
	void getMainData(QMap<QString,QStringList>* mainMap);
	void getSubsidiaryData(QMap<QString,QVector<QStringList>>* subsidaryMap);
private:
	QTableWidget *DsSettingListWidget;
	QTabWidget *virtualTerminalTab;
	QPushButton *ExportButton;
	QTableWidget *virtualTerminalTabWidget;
	QVector<QDataSetInfo> m_DsSettingList;
	QList <QTableWidgetItem*>ItemList;
	QStringList gsTabHead;
	int rowSelect;
	int currentType;
	QString IEDKEY;
	void clearCell();
	SCLNameTranslator m_NameTransLator;
	TabInfoDataSetter *m_DataSetter;
	int m_DiffType;

private slots:
	void fillChannelListTab();
	void exportXLSFile();
};

#endif // GOOSESENDWIDGET_H
