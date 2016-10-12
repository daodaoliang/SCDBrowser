#include "VirLoopDialog.h"

CVirLoopDialog::CVirLoopDialog(QWidget *parent) : QDialog(parent)
{
	graphicManager = new CGraphicManagerLite();
	m_IsLocked = false;
	setModal(false);
	setWindowFlags(/*Qt::WindowStaysOnTopHint | */Qt::Tool | Qt::FramelessWindowHint);
	setMinimumSize(800, 600);
	setWindowTitle("虚回路图");
	resize(800, 600);

	hLayout = new QHBoxLayout();
	vLayout = new QVBoxLayout();
	InitGraphicsView();
	setLayout(vLayout);
}

CVirLoopDialog::CVirLoopDialog(QWidget *parent, const QString masterIEDKey,const QString slaveIEDKey,const int type) : QDialog(parent)
{
	graphicManager = new CGraphicManagerLite();
	m_MasterIEDKey = masterIEDKey;
	m_SlaveIEDKey = slaveIEDKey;
	m_GraphicType = type;
	m_IsLocked = false;
	setModal(false);
	setWindowFlags(/*Qt::WindowStaysOnTopHint | */Qt::Tool | Qt::FramelessWindowHint);
	setMinimumSize(800, 600);
	setWindowTitle("虚回路图");
	resize(800, 600);

	hLayout = new QHBoxLayout();
	vLayout = new QVBoxLayout();
	InitGraphicsView();
	setLayout(vLayout);
	DrawVirLoopGraphic(m_MasterIEDKey, m_SlaveIEDKey, m_GraphicType);
}

CVirLoopDialog::~CVirLoopDialog(void)
{
	//delete graphicsView;
	//delete graphicScene;
	delete graphicManager;
}

void CVirLoopDialog::InitGraphicsView()
{
	lockButton = new QPushButton();
	closeButton = new QPushButton();
	lockButton->setFixedSize(18,18);
	closeButton->setFixedSize(18,18);
	lockButton->setFlat(true);
	closeButton->setFlat(true);
	lockButton->setToolTip("未锁定");
	closeButton->setToolTip("关闭");
	
	lockButton->setStyleSheet("QPushButton{background-image: url(images/pin_unlock.png);}" 
		"QPushButton:hover{background-image: url(images/pin_unlock_h.png);}" 
		"QPushButton:pressed{background-image: url(images/pin_unlock_h.png);}"
		"QPushButton:focus{padding: -1");
	closeButton->setStyleSheet("QPushButton{background-image: url(images/vl_close.png);}" 
		"QPushButton:hover{background-image: url(images/vl_close_h.png);}" 
		"QPushButton:pressed{background-image: url(images/vl_close_h.png);}"
		"QPushButton:focus{padding: -1");
	
	connect(lockButton, SIGNAL(clicked()), this, SLOT(on_lockButton_clicked()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	titleLabel = new QLabel("虚回路图");
	hLayout->addWidget(titleLabel, 0, Qt::AlignVCenter);
	hLayout->addStretch();
	hLayout->addWidget(lockButton, 0, Qt::AlignTop);
	hLayout->addWidget(closeButton, 0, Qt::AlignTop);

	QPrinter *printer = new QPrinter;
	QSizeF size = printer->paperSize(QPrinter::DevicePixel);
	graphicScene = new MyHmiGraphicsScene(NULL,QRectF(0, 0, 800, 600),this);
	graphicScene->setBackgroundBrush(QBrush(QColor(255,255,255)));
	graphicScene->setDrag(false);
	delete printer;

	graphicsView = new HmiGraphicsView(this);
	graphicsView->setMinimumWidth(780);
	graphicsView->setScene(graphicScene);
	graphicsView->setDragMode(QGraphicsView::NoDrag);
	graphicsView->setContextMenuPolicy(Qt::ActionsContextMenu);
	graphicsView->setBackgroundBrush(QBrush(QColor(255,255,255)));
	vLayout->addLayout(hLayout);
	vLayout->addWidget(graphicsView);
}

void CVirLoopDialog::DrawVirLoopGraphic(const QString masterIEDKey, const QString slaveIEDKey, const int type)
{
	splitMasterSlaveKeys(masterIEDKey);
	m_GraphicType = type;

	graphicScene->clear();
	switch (m_GraphicType)
	{
	case 2:
		graphicManager->LoadGraphicData(masterIEDKey, slaveIEDKey, GRAPHIC_TYPE_NO_VIRTUAL_LINK_GOOSE);
		break;
	case 3:
		graphicManager->LoadGraphicData(masterIEDKey, slaveIEDKey, GRAPHIC_TYPE_NO_VIRTUAL_LINK_SV);
		break;
	default:
		break;
	}

	UpdateGraphicsView();
	RecoveryProc();

	QString panelKey = graphicManager->GetSCLModel()->GetParentKey(m_MasterIEDKey);
	QString subKey = graphicManager->GetSCLModel()->GetParentKey(panelKey);
	QSubStationInfo subInfo;
	graphicManager->GetSCLModel()->GetSubstationInfo(subKey, subInfo);
	titleLabel->setText("站名：【" + GetRealSubstationName(subInfo.Name) + "】 文件名：【" + subInfo.fileName + "】");
}

void CVirLoopDialog::UpdateGraphicsView()
{
	if(graphicManager->IsGraphicExists() != 0)
	{
		QByteArray graphicData = graphicManager->GetGraphicData();
		QDataStream stream(&graphicData, QIODevice::ReadWrite);
		graphicScene->clear();
		QRectF rect = graphicScene->load(stream);
		graphicScene->setSceneRect(rect);

		double newScale = graphicsView->width() / rect.width() * graphicManager->GetGraphicScale();
		double newScale1 = graphicsView->height() / rect.width() * graphicManager->GetGraphicScale();
		QMatrix oldMatrix = graphicsView->matrix();
		graphicsView->resetMatrix();
		graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
		graphicsView->scale(qMin(newScale, newScale1),qMin(newScale, newScale1));
	}
}

void CVirLoopDialog::RecoveryProc()
{
	if (graphicsView == NULL)
	{
		return;
	}

	QRectF rect = ((HmiGraphicsView *)graphicsView)->sceneRect();
	double newScale = graphicsView->width() / rect.width() * 0.9;

	QMatrix oldMatrix = graphicsView->matrix();
	graphicsView->resetMatrix();
	graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
	graphicsView->scale(newScale,newScale);
	graphicsView->verticalScrollBar()->setSliderPosition(0);
}

bool CVirLoopDialog::GetDialogLocked()
{
	return m_IsLocked;
}

void CVirLoopDialog::SetDialogLocked(bool isLock)
{
	m_IsLocked = isLock;
	if (m_IsLocked)
	{
		lockButton->setStyleSheet("QPushButton{background-image: url(images/pin_lock.png);}" 
			"QPushButton:hover{background-image: url(images/pin_lock_h.png);}" 
			"QPushButton:pressed{background-image: url(images/pin_lock_h.png);}"
			"QPushButton:focus{padding: -1");
		lockButton->setToolTip("锁定");
	}
	else
	{
		lockButton->setStyleSheet("QPushButton{background-image: url(images/pin_unlock.png);}" 
			"QPushButton:hover{background-image: url(images/pin_unlock_h.png);}" 
			"QPushButton:pressed{background-image: url(images/pin_unlock_h.png);}"
			"QPushButton:focus{padding: -1");
		lockButton->setToolTip("未锁定");
	}
}

QString CVirLoopDialog::GetSubstationName()
{
	QString panelKey = graphicManager->GetSCLModel()->GetParentKey(m_MasterIEDKey);
	QString subName = graphicManager->GetSCLModel()->GetParentName(panelKey);
	return subName;
}

void CVirLoopDialog::on_lockButton_clicked()
{
	m_IsLocked = !m_IsLocked;
	if (m_IsLocked)
	{
		lockButton->setStyleSheet("QPushButton{background-image: url(images/pin_lock.png);}" 
			"QPushButton:hover{background-image: url(images/pin_lock_h.png);}" 
			"QPushButton:pressed{background-image: url(images/pin_lock_h.png);}"
			"QPushButton:focus{padding: -1");
		lockButton->setToolTip("锁定");
	}
	else
	{
		lockButton->setStyleSheet("QPushButton{background-image: url(images/pin_unlock.png);}" 
			"QPushButton:hover{background-image: url(images/pin_unlock_h.png);}" 
			"QPushButton:pressed{background-image: url(images/pin_unlock_h.png);}"
			"QPushButton:focus{padding: -1");
		lockButton->setToolTip("未锁定");
	}
}

void CVirLoopDialog::mousePressEvent(QMouseEvent *e)
{
	lastPoint = e->globalPos();
}

void CVirLoopDialog::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->globalX() - lastPoint.x();
	int dy = e->globalY() - lastPoint.y();

	lastPoint = e->globalPos();
	move(x()+dx, y()+dy);
}

void CVirLoopDialog::mouseReleaseEvent(QMouseEvent *e)
{
	int dx = e->globalX() - lastPoint.x();
	int dy = e->globalY() - lastPoint.y();

	move(x()+dx, y()+dy);
}

void CVirLoopDialog::splitMasterSlaveKeys(QString msKeys)
{
	QList<QString> mAndsKey = msKeys.split("$");
	m_MasterIEDKey= mAndsKey.at(0);
	m_SlaveIEDKey = mAndsKey.at(1);
}

QString CVirLoopDialog::GetRealSubstationName(QString subName)
{
	QList<QString> subNames = subName.split("#");
	if (subNames.size() == 0)
	{
		return "";
	}
	else
	{
		return subNames.at(0);
	}
}
