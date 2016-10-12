#pragma once
#include "qdialog.h"
#include "GraphicManagerLite.h"
#include "HmiGraphicsView.h"
#include "MyHmiGraphicsScene.h"
#include "QStyleOptionTitleBar"

//虚回路图弹窗类;
class CVirLoopDialog : public QDialog	//继承自QDialog;
{
	Q_OBJECT

public:
	CVirLoopDialog(QWidget *parent = 0);
	CVirLoopDialog(QWidget *parent, const QString masterIEDKey, const QString slaveIEDKey, const int type);
	~CVirLoopDialog(void);

	void DrawVirLoopGraphic(const QString masterIEDKey, const QString slaveIEDKey, const int type);
	bool GetDialogLocked();
	void SetDialogLocked(bool isLock);
	QString GetSubstationName();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	void InitGraphicsView();
	void UpdateGraphicsView();
	void RecoveryProc();

	void splitMasterSlaveKeys(QString msKeys);
	QString GetRealSubstationName(QString subName);

private slots:
	void on_lockButton_clicked();

private:
	CGraphicManagerLite *graphicManager;//图形管理器;
	QHBoxLayout		*hLayout;			//布局;
	QVBoxLayout		*vLayout;			//布局;
	QLabel			*titleLabel;		//窗口标题;
	QPushButton		*lockButton;		//锁定按钮;
	QPushButton		*closeButton;		//关闭按钮;
	MyHmiGraphicsScene *graphicScene;	//场景类;
	HmiGraphicsView	   *graphicsView;	//视图类;
	QString m_MasterIEDKey;				//主IEDKey;
	QString m_SlaveIEDKey;				//从IEDKey;
	int		m_GraphicType;				//图形类型;
	bool	m_IsLocked;					//是否锁定;
	QPoint	lastPoint;					//点坐标;
};
