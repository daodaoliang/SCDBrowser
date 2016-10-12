#ifndef GOOSERECEIVEWIDGET_H
#define GOOSERECEIVEWIDGET_H
#include <QTableWidgetItem>
#include <QWidget>

#include "TableSCLModelImpl.h"
#include "TableModelDiffImpl.h"

// goose receive 
class GooseReceiveWidget : public QWidget
{
	Q_OBJECT

public:
	GooseReceiveWidget(QWidget *parent=0);
	void GetExtRefStruInfo(QVector<ExtRefStru> ExtRefs,int i,int index=0);
	void GetSignalList(QVector<QString> inSignalKey,int i);
	~GooseReceiveWidget();
	void fillGooseExternalCtllListWidget(GOOSECBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i);
	void fillGooseExternalCtllListWidget_1(GOOSECBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i);
	void fiilReceiveTerminal(QVector<QString> inSignalKey);
	void clearData();

	
private:
	QTableWidget *gooseRecvListWidget;
	QTableWidget *gooseExternalCtllListWidget;
	QTableWidget *gooseExternalChanListWidget;
	QStringList   gooseExternalCtlHead;
	QStringList   gooseRecvHead;

	QList <QString> ctrList;
	QHash <int, QString> CtrIndex; // 控制块索引
	QHash <QString,QList<int>> SiganlToCtrl; //区域选中索引

    QHash <int, QHash<QString,QString>> mCtrlList; 
	QHash <QString,QString> crlAppIed;
	int currentIndex;
	
	void  recvListSelection(QList <int> mm_row);

signals:
	void enbleTanFouce(bool);
private slots:
	void setSelecttion(QTableWidgetItem*);
	void setReverse(QTableWidgetItem*);
};

//sv receive
class SvReceiveWidget : public QWidget
{
	Q_OBJECT

public:
	SvReceiveWidget(QWidget *parent=0);
	void GetExtRefStruInfo(QVector<ExtRefStru> ExtRefs,int i);
	void fillGooseExternalCtllListWidget(SVCBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i);
	void fillGooseExternalCtllListWidget_1(SVCBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i);
	void clearData();
	~SvReceiveWidget();

private:
	QList <QString> ctrList;
	QTableWidget *svRecvListWidget;
	QTableWidget *svExternalCtllListWidget;
    QStringList   svRecvHead;
    QStringList   svExternalCtlHead;
	QHash <int, QString>CrtIndex; //控制块索引

	QHash <QString,QList<int>> SiganlToCtrl;


	QHash <int, QHash<QString,QString>> mCtrlList; 
	QHash <QString,QString> crlAppIed;

	void recvListSelection(QList <int> mm_row);

signals:
	void enbleTanFouce(bool);

private slots:
	void setSelection(QTableWidgetItem*);
	void setReverse(QTableWidgetItem*);

};

#endif // GOOSERECEIVEWIDGET_H
