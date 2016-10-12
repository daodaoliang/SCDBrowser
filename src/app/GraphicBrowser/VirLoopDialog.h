#pragma once
#include "qdialog.h"
#include "GraphicManagerLite.h"
#include "HmiGraphicsView.h"
#include "MyHmiGraphicsScene.h"
#include "QStyleOptionTitleBar"

//���·ͼ������;
class CVirLoopDialog : public QDialog	//�̳���QDialog;
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
	CGraphicManagerLite *graphicManager;//ͼ�ι�����;
	QHBoxLayout		*hLayout;			//����;
	QVBoxLayout		*vLayout;			//����;
	QLabel			*titleLabel;		//���ڱ���;
	QPushButton		*lockButton;		//������ť;
	QPushButton		*closeButton;		//�رհ�ť;
	MyHmiGraphicsScene *graphicScene;	//������;
	HmiGraphicsView	   *graphicsView;	//��ͼ��;
	QString m_MasterIEDKey;				//��IEDKey;
	QString m_SlaveIEDKey;				//��IEDKey;
	int		m_GraphicType;				//ͼ������;
	bool	m_IsLocked;					//�Ƿ�����;
	QPoint	lastPoint;					//������;
};
