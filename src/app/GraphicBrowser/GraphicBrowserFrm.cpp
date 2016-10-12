#include <QtGui>
#include "GraphicBrowserFrm.h"
#include "TransSubsAddDlg.h"
#include "SCDManger.h"
#include "SCDValidator.h"
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDrag>
#include <QDragEnterEvent>
#include <QSplitter>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidgetItem>
#include "XLCreat.h"
#include "myitemdelegate.h"
#include "ExportDoc.h"
#include "iedtypeanalysor.h"
#include "VoltageLevelAnalysor.h"
#include <QDebug>
#include <Windows.h>
#include "ExportICDInfo.h"
#include "SCLModelAdapterForClient.h"//chenxin add 2014.10.26
#include "ExportValidateExcel.h"
#include "fixedprocessbar.h"
#include "regmanager.h"
#include "../../common/IntervalCreator.h"
#include <QActionGroup>

#define  FRAME_TITLE "ͼ�������"

#define NODE_TYPE_NOTHING			0  //δѡ���κνڵ�
#define NODE_TYPE_VERSION			1  //�汾+
#define NODE_TYPE_SUBNET_ROOT		2  //������Ŀ¼
#define NODE_TYPE_SUBNET			3  //����
#define NODE_TYPE_IED_ROOT			4  //����װ�ø�Ŀ¼
#define NODE_TYPE_IED				5  //����װ��
#define BASIC_INFO					6  // ������Ϣ
#define NODE_TYPE_COMPARE_IED		9  //
#define NODE_TYPE_XMLFILE			7  //XMLԭ��

// ���У������
#define NODE_TYPE_VALIDATOR_SCHEMA			110  //У��
#define NODE_TYPE_VALIDATOR_PROPERTIES		111  //У��
#define NODE_TYPE_VALIDATOR_UNIQUENESS		112  //У��
#define NODE_TYPE_VALIDATOR_MODELINTEGRITY	113  //У��
#define NODE_TYPE_VALIDATOR_REFERENCE		114  //У��
#define NODE_TYPE_VALIDATOR_STRUCT			115  //У��

#define NODE_TYPE_VALIDATOR_SCHEMA_WARN			116  //У��
#define NODE_TYPE_VALIDATOR_PROPERTIES_WARN		117  //У��
#define NODE_TYPE_VALIDATOR_UNIQUENESS_WARN		118  //У��
#define NODE_TYPE_VALIDATOR_MODELINTEGRITY_WARN	119  //У��
#define NODE_TYPE_VALIDATOR_REFERENCE_WARN		120  //У��
#define NODE_TYPE_VALIDATOR_STRUCT_WARN			121  //У��


#define SHOW_ToTal_IED  0 
#define SHOW_INFO_TABLE  1

#define NODE_TYPE_WIDGETADD			10  //δѡ���κνڵ�;
#define NODE_TYPE_WIDGETUP			11  //δѡ���κνڵ�;
#define NODE_TYPE_WIDGETDEL	     	12  //δѡ���κνڵ�;
#define NODE_TYPE_WIDGETEQL			13  //δѡ���κνڵ�;
#define NODE_TYPE_WIDGETTOTAL       20
#define NODE_TYPE_WIDGETVer         21  //�汾����;


GraphicBrowserFrm::GraphicBrowserFrm(QWidget *parent)
    :QMainWindow(parent)
{
#ifndef GRAND_DOG
#ifdef SFBM
	TrivalInfo = "[ͼ����] ";
#else
	TrivalInfo = "[���ð�] ";
#endif
#endif
    raise();
	logsrv_scoped_output(logsrv, "����GraphicBrowserFrm���캯��", "�˳�GraphicBrowserFrm", "GraphicBrowserFrm::GraphicBrowserFrm");
	setWindowIcon(QIcon("./images/Icon.ico"));
	setStatusTip("     ɽ���ݸ�����Ϣ�Ƽ����޹�˾   �绰:0531-88872188   ��ַ:http://www.tech-roof.com");
	setAcceptDrops(true);
	m_IsAutoCompare = false;
	connect(this,SIGNAL(sihf()),this,SLOT(Myinitcon()));
	graphicsManger    = GraphicsManger::GetInstance();
	scdManger         = SCDManger::GetInstance();
	scdComparer		  = SCDComparer::GetInstance();
	scdDiffCompare	  = SCDDiffCompare::GetInstance();
	tableModelImpl	  = new CTableSCLModelImpl();
	tableModelDiffImpl= new TableModelDiffImpl();
	tableModel		  = tableModelImpl;
	Query             = new QLabel();
	DiffQuery         = new QLabel();
	QueryHLayout      = new QHBoxLayout();
	labelVotageLevel  = new QLabel();
	labelSubstation   = new QLabel();
	comboVoltageLevel = new QComboBox();
	comboSubStation   = new QComboBox();
	IEDsLoader        = new IEDLoader();
	IEDQueryBar		  = new QLineEdit(this);
	DiffQueryHLayout  = new QHBoxLayout();
	DiffIEDQueryBar	  = new QLineEdit(this);
	IEDsTreeWidget    = new QTreeWidget(this);
	compareModel	  = new QTreeWidget(this);
	subInfoTreeWidget = new QTreeWidget(this);
	subnetTreeWidget  = new QTreeWidget(this);
	scdValidatorTreeWidget = new QTreeWidget(this);
	treeVLayout		  = new QVBoxLayout();
	DiffTreeVLayout	  = new QVBoxLayout();
	infoTreeVLayout   = new QVBoxLayout();
	subnetTreeVLayout = new QVBoxLayout();
	scdValidatorTreeVLayout = new QVBoxLayout();
	treeTabWidget	  = new QTabWidget();
	treeContainerWidget = new QWidget(treeTabWidget);
	DiffTreeContainerWidget = new QWidget(treeTabWidget);
	infoTreeContainerWidget = new QWidget(treeTabWidget);
	subnetTreeContainerWidget = new QWidget(treeTabWidget);
	scdValidatorWidget = new QWidget(treeTabWidget);
    xmlViewer = new QXmlViewer(this);

    m_SwitchProcessBar = new FixedProcessBar(this);
    connect(IEDsLoader->checkThread, SIGNAL(checkFinish()), this, SLOT(ImportValidateFinished()));
    m_SSDInfo = new CSSDLoader();
    InitSSDUiWidget();

	// logsrv_output_info(logsrv, L"��ʼ��ָ�����", L"GraphicBrowserFrm::GraphicBrowserFrm");

	IEDsTreeDockWidget = new QDockWidget("",this);
	IEDsTreeDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
	// IEDsTreeDockWidget->setMaximumWidth(500);
	IEDsTreeDockWidget->setWidget(treeTabWidget);

	// logsrv_output_info(logsrv, L"IEDsTreeDockWidget��ʼ�����", L"GraphicBrowserFrm::GraphicBrowserFrm");

	graphicsViewDockWidget = new QDockWidget("��ǰװ��  �� ��", this);
	graphicsViewDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
	graphicsViewDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	setCentralWidget(graphicsViewDockWidget);

	// logsrv_output_info(logsrv, L"graphicsViewDockWidget��ʼ�����", L"GraphicBrowserFrm::GraphicBrowserFrm");

	porgressFlg=false;
	isComparing = false;

	// logsrv_output_info(logsrv, L"��ʼ��porgressFlg��isComparing���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	vDialog = new CVirLoopDialog(this);
	lockedDialogVec.clear();

	// logsrv_output_info(logsrv, L"new CVirLoopDialog���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	IEDsTreeDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	IEDQueryBar->setPlaceholderText("����IED");
	connect(IEDQueryBar, SIGNAL(textChanged (const QString&)), this, SLOT(queryIEDProc(const QString&)));
	connect(DiffIEDQueryBar, SIGNAL(textChanged (const QString&)), this, SLOT(queryDiffIEDProc(const QString&)));

	// logsrv_output_info(logsrv, L"�������", L"GraphicBrowserFrm::GraphicBrowserFrm");

	DiffTreeContainerWidget->hide();
	analysisProgressBar = new QProgressBar(this);
	analysisProgressBar->setStyleSheet(
		"QProgressBar {"
		"border: 2px solid grey;"
		"border-radius: 5px;}");

	filePath = "";

	PreFlag=false;
	NextFlag=false;
	timer = new QTimer(this);
	connect(timer,SIGNAL(timeout()),this,SLOT(timePro()));
	timer->start(100);
	InitIEDTotalTab();
	//loadingThread = new CMyLoadingThread();

	//�����������dock
	addDockWidget(Qt::LeftDockWidgetArea, IEDsTreeDockWidget);
	// logsrv_output_info(logsrv, L"�����������dock���", L"GraphicBrowserFrm::GraphicBrowserFrm");

    subSetting = new QSettings("../cfg/SubStationInfo.ini", QSettings::IniFormat);
	QString substationID = "";
#ifndef WORK_STATION
	substationID = subSetting->value("config/CurrentSubstationID").toString();
	IEDsLoader->setShowStationOnStart(true);
#else
	IEDsLoader->setShowStationOnStart(false);
#endif


	IEDsLoader->LoadVoltageLevelNameList();
	IEDsLoader->SetCurrentVoltageLevelIndex(0);
	IEDsLoader->LoadSubstationNameList();

	// logsrv_output_info(logsrv, L"LoadSubstationNameList���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	//chenxin modify 2014.10.26 --->
	IEDsLoader->SetCurrentSubstationID(substationID);
    m_SSDInfo->SetCurrentSubstation(substationID);
	//chenxin modify 2014.10.26 <---
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadHItemList();
	IEDsLoader->LoadIEDInfoList();

	// logsrv_output_info(logsrv, L"LoadIEDInfoList���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	if(IEDsLoader->GetVoltageLevelNameList()->size()>0&&IEDsLoader->GetSubstationNameList()->size()>0)
	{
		InitVoltageLevelCombBox();
		InitSubstationCombBox();
	}

	// logsrv_output_info(logsrv, L"IEDsLoader���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	//��ʼ�� �����߳�
	loadingThread = new CMyLoadingThread();
	connect(loadingThread, SIGNAL(addSubstationFinish(int)), this, SLOT(LoadSCDProc1(int))/*, Qt::BlockingQueuedConnection*/);

	// logsrv_output_info(logsrv, L"new CMyLoadingThread���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	// �����������ʼ��
	proGressCount=0;
	progresStyle=new Myprogress; 
	timerProgress=new QTimer(this);
	connect(timerProgress,SIGNAL(timeout()),this,SLOT(timerProgressOut()));
	averageSpeed=0;
	Endcompare=true;  // δ�Ƚ�

	// ����������ʼ��
	m_SCLValid = CSCLValid::GetInstance();
	progresCheckStyle=new Myprogress;
	timerCheckProgress=new QTimer(this);
	connect(timerCheckProgress,SIGNAL(timeout()),this,SLOT(timerProgressCheckOut()));
	proCheckFlg=false;  // δ�Ƚ�	

	// �ȽϽ�������ʼ��
    progresCompareStyle=new Myprogress;
	timerCompare=new QTimer(this);
	connect(timerCompare,SIGNAL(timeout()),this,SLOT(timerCompareOut()));
	proCompareFlg=false;

	InitSubStationTableData();

	// logsrv_output_info(logsrv, L"InitSubStationTableData���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	versionView = new QTreeView(this);
	scdValidatorView = new QTreeView(this);
	deviceView = new QTreeView(this);
	substationView = new QTreeView(this);
	itemdele=new qComItemDelegate(this);
	substationView->setItemDelegate(itemdele); // �Ƚ�combox ����
	
	// logsrv_output_info(logsrv, L"substationView->setItemDelegate(itemdele)���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	substationManageView = new QTreeView(this);
	versionModel = new QStandardItemModel(0,7,this);
	deviceModel = new QStandardItemModel(0,1,this);
	substationModel = new QStandardItemModel(0,4,this);
	substationModelCompare = new QStandardItemModel(0,5,this);
	scdValidatorModel = new QStandardItemModel(0,7,this);
	comparethread=new CompareThread(scdDiffCompare);   // �Ƚ��߳�
	
	// logsrv_output_info(logsrv, L"comparethread=new CompareThread(scdDiffCompare)���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	
	connect(comparethread,SIGNAL(compareFinish(bool)),this,SLOT(FinishCompareThread(bool)));
	connect(substationModelCompare,SIGNAL(itemChanged( QStandardItem *)),SLOT(modelChange( QStandardItem *)));
	connect(this,SIGNAL(SendItemDelegateFlag(bool,bool)),itemdele,SLOT(GetStrCount(bool,bool)));
	
	checkThread = new CheckThread(m_SCLValid);
	connect(checkThread, SIGNAL(checkFinish()), this, SLOT(FinishCheckThread()));

	// logsrv_output_info(logsrv, L"new CheckThread(m_SCLValid)���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	// �Ƚ�����������
	substationCompareConfigView = new QTreeView(this);
	substationCompareConfigModel = new QStandardItemModel(this);
	connect(substationCompareConfigModel ,SIGNAL(itemChanged(QStandardItem*)), this, SLOT(treeItemChanged(QStandardItem*)));  

	substationManageModel = new QStandardItemModel(0,4,this);

	// logsrv_output_info(logsrv, L"new QStandardItemModel(0,4,this)���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	connect(substationManageView, SIGNAL(customContextMenuRequested(const QPoint&)), 
		this, SLOT(on_substationManageView_customContextMenuRequested(const QPoint&)));

	CreateAction();

	// logsrv_output_info(logsrv, L"CreateAction���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	CreateMenus();

	// logsrv_output_info(logsrv, L"CreateMenus���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	CreateToolbars();

	// logsrv_output_info(logsrv, L"CreateToolbars���", L"GraphicBrowserFrm::GraphicBrowserFrm");

    // ������
    group = new QActionGroup(this);
    CheckConfigDlg = new CCheckConfigDlg();
    connect(CheckConfigDlg, SIGNAL(DialogClose(QString)), this, SLOT(UpdateConfigFileMenu(QString)));
    UpdateConfigFileMenu("Ĭ�Ϸ���");

	graphicOpersEnable(false);
	InitGraphicsView();

	// logsrv_output_info(logsrv, L"InitGraphicsView���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	InitVersionView();

	// logsrv_output_info(logsrv, L"InitVersionView���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	// InitSCDValidatorTreeWidget();
	// InitSCDValidatorView();
	InitDeviceView();

	// logsrv_output_info(logsrv, L"InitDeviceView���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	//InitSubstationView();
	InitIEDsTreeWidget();
    InitSSDTreeWidget();

	// logsrv_output_info(logsrv, L"InitIEDsTreeWidget���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	InitSubInfoTreeWidget();
	// logsrv_output_info(logsrv, L"InitSubInfoTreeWidget���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	InitSubnetTreeWidget();
	// logsrv_output_info(logsrv, L"InitSubnetTreeWidget���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	
	connect(IEDsTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		this,SLOT(SelectTreeProc(QTreeWidgetItem*)));
	connect(subInfoTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		this,SLOT(SelectSubInfoTreePorc(QTreeWidgetItem*)));
	connect(subnetTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		this,SLOT(SelectSubNetTreeProc(QTreeWidgetItem*)));
	connect(scdValidatorTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		this,SLOT(SelectSCDValidatorTreeProc(QTreeWidgetItem*)));
	connect(scdValidatorView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(SelectValidatorProc(const QModelIndex&)));

	Query->setText("���ٲ��ң�");
	QueryHLayout->addWidget(Query);
	QueryHLayout->addWidget(IEDQueryBar);

	treeVLayout->addLayout(QueryHLayout);
	treeVLayout->addWidget(IEDsTreeWidget);
	treeContainerWidget->setLayout(treeVLayout);

	DiffQuery->setText("���ٲ��ң�");
	DiffQueryHLayout->addWidget(DiffQuery);
	DiffQueryHLayout->addWidget(DiffIEDQueryBar);
	DiffTreeVLayout->addLayout(DiffQueryHLayout);
	// DiffTreeVLayout->addWidget(DiffIEDQueryBar);
	DiffTreeVLayout->addWidget(compareModel);
	DiffTreeContainerWidget->setLayout(DiffTreeVLayout);

	infoTreeVLayout->addWidget(subInfoTreeWidget);
	infoTreeContainerWidget->setLayout(infoTreeVLayout);

	subnetTreeVLayout->addWidget(subnetTreeWidget);
	subnetTreeContainerWidget->setLayout(subnetTreeVLayout);

	ExportValidateExcelButton = new QPushButton(tr("���������"), this);
	connect(ExportValidateExcelButton, SIGNAL(clicked()), this, SLOT(ExportValidateExcel()));
	scdValidatorTreeVLayout->addWidget(ExportValidateExcelButton);
	scdValidatorTreeVLayout->addWidget(scdValidatorTreeWidget);
	scdValidatorTreeWidget->hide();
	ExportValidateExcelButton->hide();
	scdValidatorWidget->setLayout(scdValidatorTreeVLayout);

	//treeTabWidget->addTab(treeContainerWidget,tr("�ۺ����"));
	//treeTabWidget->addTab(deviceView,tr("����װ����Ϣ"));
	treeTabWidget->addTab(infoTreeContainerWidget,tr("������Ϣ"));
	// treeTabWidget->addTab(scdValidatorWidget,tr("У����"));
	treeTabWidget->addTab(treeContainerWidget,tr("IED�б�"));
	treeTabWidget->addTab(deviceView,tr("��վ����װ���ͺ�"));
	treeTabWidget->addTab(subnetTreeContainerWidget,tr("ͨ������"));
    treeTabWidget->addTab(ssdTreeContainerWidget,tr("һ���豸"));
	treeTabWidget->setCurrentIndex(1);
	treeTabWidget->setMinimumWidth(1);
	treeTabWidget->setTabPosition(QTabWidget::West);

	versionWin = new QDialog(this);
	hLayout = new QVBoxLayout();
	hLayout->addWidget(versionView);
	versionWin->setLayout(hLayout);
	versionWin->setWindowTitle(tr("�汾��Ϣһ��"));
	//versionWin->setFixedSize(1500, 600);
	versionWin->move(200,100);

	scdValidatorWin = new QDialog(this);
	hScdValidatorLayout = new QVBoxLayout();
	hScdValidatorLayout->addWidget(scdValidatorView);
	scdValidatorWin->setLayout(hScdValidatorLayout);
	scdValidatorWin->setWindowTitle(tr("У����"));
	scdValidatorWin->move(200,100);

	substationWin = new QDialog(this);
	substationCancelBtn = new QPushButton(tr("ȡ��"),substationWin);
	substationCompareBtn = new QPushButton(tr("�Ƚ�"),substationWin);
	substationConfigBtn = new QPushButton(tr("����"),substationWin);
	substationCancelBtn->setMaximumWidth(80);
	substationCompareBtn->setMaximumWidth(80);
	substationConfigBtn->setMaximumWidth(80);
	connect(substationCancelBtn, SIGNAL(clicked()), substationWin, SLOT(hide()));
	connect(substationCompareBtn, SIGNAL(clicked()), this, SLOT(SubStationCompareProc()));
	connect(substationConfigBtn, SIGNAL(clicked()), this, SLOT(ShowSubStationCompareConfigProc()));
	substationHLayout = new QHBoxLayout();
	substationHLayout->addWidget(substationCompareBtn);
	substationHLayout->addWidget(substationCancelBtn);
	substationHLayout->addWidget(substationConfigBtn);
	substationVLayout = new QVBoxLayout();
	substationView->hide();
	substationVLayout->addWidget(tablesubstation);
	substationVLayout->addLayout(substationHLayout);

	substationWin->setLayout(substationVLayout);
	substationWin->setWindowTitle(tr("SCD�Ƚ�"));
	substationWin->setMinimumWidth(600);
	substationWin->move(200,100);

	// ���ý�������
	SetConfigureInfo();
	SubstationCompareConfigWin = new QDialog(this);
	SubstationCompareConfigWin->setModal(true);
	substationConfigSureBtn = new QPushButton(tr("ȷ��"), SubstationCompareConfigWin);
	substationConfigCancelBtn = new QPushButton(tr("ȡ��"), SubstationCompareConfigWin);
	substationConfigSureBtn->setMaximumHeight(20);
	substationConfigCancelBtn->setMaximumHeight(20);
	connect(substationConfigCancelBtn, SIGNAL(clicked()), SubstationCompareConfigWin, SLOT(hide()));
	connect(substationConfigSureBtn, SIGNAL(clicked()), this, SLOT(SubStationCompareConfigSetProc()));
	SubstationCompareConfigHLayout = new QHBoxLayout();
	SubstationCompareConfigHLayout->addStretch();
	SubstationCompareConfigHLayout->addWidget(substationConfigSureBtn);
	SubstationCompareConfigHLayout->addWidget(substationConfigCancelBtn);
	SubstationCompareConfigVLayout = new QVBoxLayout();
	SubstationCompareConfigVLayout->addWidget(substationCompareConfigView);
	SubstationCompareConfigVLayout->addLayout(SubstationCompareConfigHLayout);

	SubstationCompareConfigWin->setLayout(SubstationCompareConfigVLayout);
	SubstationCompareConfigWin->setWindowTitle(tr("SCD�Ƚ�������"));
	SubstationCompareConfigWin->setMinimumHeight(400);
	SubstationCompareConfigWin->move(250,150);

	ConfigItem = new QStandardItem[ITEMCOUNT];
	CreatCompareConfTree();

	substationManageWin = new QDialog(this);
	substationManageWin->setModal(true);
	surebutton=new QPushButton(tr("ȷ��"),substationManageWin);
	cancelButton=new QPushButton(tr("ȡ��"),substationManageWin);
	surebutton->setMaximumWidth(80);
	cancelButton->setMaximumWidth(80);
	connect(cancelButton, SIGNAL(clicked()), substationManageWin, SLOT(hide()));
	substationManageHLayout = new QHBoxLayout;
	substationManageHLayout->addWidget(surebutton);
	substationManageHLayout->addWidget(cancelButton);

	substationManageVLayout = new QVBoxLayout();
	substationManageVLayout->addWidget(substationManageView);
	substationManageVLayout->addLayout(substationManageHLayout);
	substationManageWin->setLayout(substationManageVLayout);
	substationManageWin->setWindowTitle(tr("���վ����"));
	substationManageWin->setMinimumWidth(600);
	substationManageWin->move(200,100);

	lDialog = new CLoadDialog(this);
	lDialog->setModal(true);
	CreateStatusBar();
	setCenterLayer(SHOW_ToTal_IED);
	QVector<QString> *subList = IEDsLoader->GetSubstationNameList();
	QString subName = "";
	if (subList->size() == 0)
	{
		setWindowTitle(TrivalInfo + "SCD���ӻ� - []");
		IEDsTreeDockWidget->setWindowTitle(getDockTitleName());
	} 
	else
	{
		
		bool bFindStation = false;
		for (int i = 0; i < subList->size(); i++)
		{
			if(subList->at(i) ==  IEDsLoader->GetCurrentSubstationID())
			{
				bFindStation = true;
			}
		}
		if (!bFindStation)
		{
			setWindowTitle(TrivalInfo + "SCD���ӻ� - []");
			IEDsTreeDockWidget->setWindowTitle(getDockTitleName());
		}
		else
		{
            QString subKey = "220kV." + IEDsLoader->GetCurrentSubstationID();
			QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo(subKey);

			//chenxin modify 2014.10.17 --->
			setWindowTitle(TrivalInfo + "SCD���ӻ� - " + subInfo->fileName);
			IEDsTreeDockWidget->setWindowTitle(getDockTitleName() + subInfo->Name);
			//chenxin modify 2014.10.17 <---
		}
	}
    showMinimized();
    setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
	showMaximized();
	analysisProgressBar->setRange(0, 100);
	ReadSettings();

	mainToolbar->setStyleSheet("QToolBar {spacing: 16px;padding:2px;}");

#ifndef WORK_STATION
	// ���������
	m_ListenThread = new ListenThread(this);
	connect(m_ListenThread, SIGNAL(EmitNewProcess(QString)), this, SLOT(ThreadRead(QString)), Qt::BlockingQueuedConnection);
	m_ListenThread->start();
	hwnd = CreateWindowA("QWidget", "{617278E1-A464-40A5-A87A-4AF62D5421F2}", WS_POPUP, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	if (hwnd)
	{
		qDebug() << "New Window    Created!";
	}
	else
	{
		qDebug() << "New Window    Not Created!";
	}
#else 
	// ���������
	m_ListenThread = new ListenThread(this);
	connect(m_ListenThread, SIGNAL(EmitNewProcess(QString)), this, SLOT(ThreadRead(QString)), Qt::BlockingQueuedConnection);
	m_ListenThread->start();
	hwnd = CreateWindowA("QWidget", "{3A8D23B6-F91F-4624-9829-0263E2BD74BD}", WS_POPUP, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	if (hwnd)
	{
		qDebug() << "New Window    Created!";
	}
	else
	{
		qDebug() << "New Window    Not Created!";
	}
#endif
}

void GraphicBrowserFrm::ThreadRead(QString arg)
{
#ifndef WORK_STATION
	qDebug() << "GraphicBrowserFrm::ThreadRead !!!";
	showMinimized();
	setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
	showMaximized();
	RegManager reg;
	if (QMessageBox::Ok == QMessageBox::warning(this, "Warning", "���µĶ��������Դ򿪱������Ƿ�رյ�ǰ����", QMessageBox::Ok, QMessageBox::No))
	{
		QString arg = reg.ReadRegValue();
		arg = arg.replace("$", " ");
		reg.DeleteRegValue();
		if (SetWindowTextA(hwnd, "{61B90ACC-81AE-49FC-98CB-EC0E8F438ACD}"))
		{
			qDebug() << "GraphicBrowserFrm::ThreadRead     New Window   Change Title Success!!!";
		}
		else
		{
			qDebug() << "GraphicBrowserFrm::ThreadRead     New Window   Change Title Faild!!!";
		}

		QProcess *newProcss = new QProcess();
		newProcss->start(arg);
		exit(0);
	}
	reg.DeleteRegValue();
#else
	qDebug() << "GraphicBrowserFrm::ThreadRead !!!";
	showMinimized();
	setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
	showMaximized();
	RegManager reg;
	QString NewArg = reg.ReadRegValue();
	reg.DeleteRegValue();
	if (SetWindowTextA(hwnd, "{61B90ACC-81AE-49FC-98CB-EC0E8F438ACD}"))
	{
		qDebug() << "GraphicBrowserFrm::ThreadRead     New Window   Change Title Success!!!";
	}
	else
	{
		qDebug() << "GraphicBrowserFrm::ThreadRead     New Window   Change Title Faild!!!";
	}
	QProcess *newProcss = new QProcess();
	newProcss->start(NewArg);
	exit(0);
#endif
}

void   GraphicBrowserFrm::InitSubStationTableData()
{
	tablesubstation=new QTableWidget;  //�Ƚ��б�;
	tablesubstation->setSelectionBehavior(QAbstractItemView::SelectRows); 
	tablesubstation->setEditTriggers(QAbstractItemView::NoEditTriggers); 
	tablesubstation->setShowGrid(false);
}

GraphicBrowserFrm::~GraphicBrowserFrm()
{
	delete graphicPopupMenu;
	delete labelVotageLevel;
	delete labelSubstation;
	delete comboVoltageLevel;
	delete comboSubStation;
	delete IEDsTreeWidget;
	delete IEDsLoader;
	delete timer;
	delete analysisProgressBar;
	delete deviceView;
	delete versionView;
	delete scdValidatorView;
	delete treeTabWidget;
	delete hLayout;
	delete versionWin;
	delete loadingThread;
	delete lDialog;

	delete widgetIEDTable;
	delete widgetDiffEditTable;
	delete widgetDiffDelTable;
	delete widgetDIffAddTable;
	delete widgetDiffEqlTable;
	delete widgetDiffVerTable;
	delete progresStyle;
	delete progresCompareStyle;
}
void GraphicBrowserFrm::clickdobleCell(int row,int col)
{
   if(sender()==widgetIEDTable)
   {
	   QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_IED);
	   IEDItem->setData(0,Qt::AccessibleTextRole, widgetIEDTable->item(row,0)->data(100));
	   SelectTreeProc(IEDItem);

	   //QModelIndex index=IEDsTreeWidget->model()->index(2,0,IEDsTreeWidget->rootIndex());
	   //IEDsTreeWidget->expand(index);
	   //QModelIndex index_chid=IEDsTreeWidget->model()->index(row,0,index);
	   //QItemSelectionModel *sel=IEDsTreeWidget->selectionModel();
	   //sel->select(index_chid,QItemSelectionModel::SelectCurrent);
	   //IEDsTreeWidget->scrollTo(index_chid);
   }
   else if(sender()==widgetDIffAddTable)
   {
	   QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_WIDGETTOTAL);
	   IEDItem->setData(0,Qt::AccessibleTextRole, widgetDIffAddTable->item(row,0)->data(100));
	   SelectCompareTreeProc(IEDItem);

   }
   else if(sender()==widgetDiffDelTable)
   {
	   QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_WIDGETTOTAL);
	   IEDItem->setData(0,Qt::AccessibleTextRole, widgetDiffDelTable->item(row,0)->data(100));
	   SelectCompareTreeProc(IEDItem);

   }
   else if(sender()==widgetDiffEditTable)
   {
	   QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_WIDGETTOTAL);
	   IEDItem->setData(0,Qt::AccessibleTextRole, widgetDiffEditTable->item(row,0)->data(100));
	   SelectCompareTreeProc(IEDItem);

   }
   else if(sender()==widgetDiffEqlTable)
   {
	   QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_WIDGETTOTAL);
	   IEDItem->setData(0,Qt::AccessibleTextRole, widgetDiffEqlTable->item(row,0)->data(100));
	   SelectCompareTreeProc(IEDItem);
   }
   // SSD��ת �������
   else if(sender()==ceLNodeTable)
   {
       QString iedName = ceLNodeTable->item(row,0)->data(Qt::DisplayRole).toString();
       QString iedKey = GetSCDIEDKeyBySSDLnIED(iedName);
       if (iedKey.isEmpty())
       {
           QMessageBox::warning(NULL, "�����豸��ת", "��Ӧ�Ķ����豸������");
           return;
       }
       
       QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_IED);
       IEDItem->setData(0,Qt::AccessibleTextRole, iedKey);
       SelectTreeProc(IEDItem);

       //QModelIndex index=IEDsTreeWidget->model()->index(2,0,IEDsTreeWidget->rootIndex());
       //IEDsTreeWidget->expand(index);
       //QModelIndex index_chid=IEDsTreeWidget->model()->index(row,0,index);
       //QItemSelectionModel *sel=IEDsTreeWidget->selectionModel();
       //sel->select(index_chid,QItemSelectionModel::SelectCurrent);
       //IEDsTreeWidget->scrollTo(index_chid);
   }
   else if(sender()==ptlnodeTableWidget)
    { 
        QString iedName = ptlnodeTableWidget->item(row,0)->data(Qt::DisplayRole).toString();
        QString iedKey = GetSCDIEDKeyBySSDLnIED(iedName);
        if (iedKey.size() == 0)
        {
            QMessageBox::warning(NULL, "�����豸��ת", "��Ӧ�Ķ����豸������");
            return;
        }

        QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_IED);
        IEDItem->setData(0,Qt::AccessibleTextRole, iedKey);
        SelectTreeProc(IEDItem);

        //QModelIndex index=IEDsTreeWidget->model()->index(2,0,IEDsTreeWidget->rootIndex());
        //IEDsTreeWidget->expand(index);
        //QModelIndex index_chid=IEDsTreeWidget->model()->index(row,0,index);
        //QItemSelectionModel *sel=IEDsTreeWidget->selectionModel();
        //sel->select(index_chid,QItemSelectionModel::SelectCurrent);
        //IEDsTreeWidget->scrollTo(index_chid);
    }
   else if(sender()==ptTWLNodeTable)
    {
        QString iedName = ptTWLNodeTable->item(row,0)->data(Qt::DisplayRole).toString();
        QString iedKey = GetSCDIEDKeyBySSDLnIED(iedName);
        if (iedKey.size() == 0)
        {
            QMessageBox::warning(NULL, "�����豸��ת", "��Ӧ�Ķ����豸������");
            return;
        }

        QTreeWidgetItem  *IEDItem=new QTreeWidgetItem(NODE_TYPE_IED);
        IEDItem->setData(0,Qt::AccessibleTextRole, iedKey);
        SelectTreeProc(IEDItem);

        //QModelIndex index=IEDsTreeWidget->model()->index(2,0,IEDsTreeWidget->rootIndex());
        //IEDsTreeWidget->expand(index);
        //QModelIndex index_chid=IEDsTreeWidget->model()->index(row,0,index);
        //QItemSelectionModel *sel=IEDsTreeWidget->selectionModel();
        //sel->select(index_chid,QItemSelectionModel::SelectCurrent);
        //IEDsTreeWidget->scrollTo(index_chid);
    }
   else if(sender()==widgetIEDSSDTable)
    {
        QString vol = widgetIEDSSDTable->item(row,2)->data(Qt::DisplayRole).toString();
        QString bay = widgetIEDSSDTable->item(row,3)->data(Qt::DisplayRole).toString();
        fillSSDBayContainer(vol, bay);

        totalTab->clear();
        totalTab->tabbarhide();
        totalTab->addTab(bayContainer,"SSD");
        return;        
    }    

   totalTab->clear();
   totalTab->tabBarShow();
   totalTab->addTab(graphicsView,stringlist.at(0));
   totalTab->addTab(svSendWidget,stringlist.at(1));
   totalTab->addTab(svRecvWidget,stringlist.at(2));
   totalTab->addTab(gooseSendWidget,stringlist.at(3));
   totalTab->addTab(gooseRecvWidget,stringlist.at(4));
   totalTab->addTab(reportSendWidget, stringlist.at(5));
   totalTab->addTab(dsSettingWidget, stringlist.at(6));
   totalTab->addTab(xmlViewer, stringlist.at(7));
   QString iedKey = totalTab->getIEDKey();
   QString iedName = iedKey.mid(iedKey.lastIndexOf(".") + 1);
   if (m_SSDInfo->IsIEDHaveEquipment(iedName))
   {
       totalTab->addTab(widgetIEDSSDTable, stringlist.at(8));
   }
}

// �ж�SSD LN��Ӧ��IED�Ƿ����
QString GraphicBrowserFrm::GetSCDIEDKeyBySSDLnIED(QString iedName)
{
    for (int i = 0; i < IEDsLoader->GetIEDList()->size(); i++)
    {
        if (IEDsLoader->GetIEDList()->at(i).IEDName == iedName)
        {
            return IEDsLoader->GetIEDList()->at(i).IEDKey;
        }
    }
    return "";
}

void GraphicBrowserFrm::InitIEDTotalTab()
{
	 widgetIEDTable=new QTableWidget;
	 connect(widgetIEDTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));


	 widgetDiffEditTable=new QTableWidget;
	 connect(widgetDiffEditTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

	 widgetDiffDelTable=new QTableWidget;
	 connect(widgetDiffDelTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

	 widgetDIffAddTable=new QTableWidget;
	 connect(widgetDIffAddTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

	 widgetDiffEqlTable=new QTableWidget;
     connect(widgetDiffEqlTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));


	 widgetDiffVerTable=new QTableWidget;


	// connect(widgetDiffVerTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

}
void    GraphicBrowserFrm::ReInitTabDataClear()
{
	widgetIEDTable->clear();
	widgetDiffEditTable->clear();
	widgetDiffDelTable->clear();
	widgetDIffAddTable->clear();
	widgetDiffEqlTable->clear();
	widgetDiffVerTable->clear();

	QStringList stringlist;
	stringlist<<"IED"<<"IED����"<<"��������"<<"�豸�ͺ�";

	widgetIEDTable->setColumnCount(4);
	widgetIEDTable->setRowCount(0);
	widgetIEDTable->setHorizontalHeaderLabels(stringlist);
	widgetIEDTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	widgetIEDTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭

	widgetDiffEditTable->setColumnCount(4);
	widgetDiffEditTable->setRowCount(0);
	widgetDiffEditTable->setHorizontalHeaderLabels(stringlist);
	widgetDiffEditTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	widgetDiffEditTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭

	widgetDiffDelTable->setColumnCount(4);
	widgetDiffDelTable->setRowCount(0);
	widgetDiffDelTable->setHorizontalHeaderLabels(stringlist);
	widgetDiffDelTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	widgetDiffDelTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭

	widgetDIffAddTable->setColumnCount(4);
	widgetDIffAddTable->setRowCount(0);
	widgetDIffAddTable->setHorizontalHeaderLabels(stringlist);
	widgetDIffAddTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	widgetDIffAddTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭


	widgetDiffEqlTable->setColumnCount(4);
	widgetDiffEqlTable->setRowCount(0);
	widgetDiffEqlTable->setHorizontalHeaderLabels(stringlist);
	widgetDiffEqlTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	widgetDiffEqlTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭


	QStringList  str;
	str<<"�ļ�����"<<"����ʱ��"<<"�汾"<<"�޶��汾"<<"CRCУ����";
	widgetDiffVerTable->setColumnCount(5);
	widgetDiffVerTable->setRowCount(0);
	widgetDiffVerTable->setHorizontalHeaderLabels(str);
	widgetDiffVerTable->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ����Ϊʱÿ��ѡ��һ��
	widgetDiffVerTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭

}

void GraphicBrowserFrm::on_IEDsTreeDockWidget_closed()
{
	actionShowIEDsTree->setCheckable(false);
}
void GraphicBrowserFrm::ShowSwithDialog()
{
	substationManageWin->setWindowTitle(tr("�л�SCD�ļ�"));
	disconnect(surebutton, 0, 0, 0);
	connect(surebutton, SIGNAL(clicked()), this, SLOT(SubStationChangeProc()));
	ShowSubstationManageProc();
	substationManageWin->show();
}

void GraphicBrowserFrm::StartCheckSCD()
{
	qDebug()<<::GetCurrentProcessId()<<"StartCheckSCD			Start";
	if(!proCheckFlg)
	{
        
        QString appPath = QApplication::applicationDirPath();
        filePath = appPath + "/../data/" + IEDsLoader->GetCurrentSubstationID() + "/stored.rfscd";
        if (!QFile::exists(filePath))
        {
            QMessageBox::warning(NULL, "SCL���", "��Ӧ��SCD�ļ������ڣ�");
            return;
        }
        QString subKey = "220kV." + IEDsLoader->GetCurrentSubstationID();
        QSubStationInfo *subInfo = IEDsLoader->GetSubstationInfo(subKey);

        // ��ȡ�����ļ�
        QString configFileName = "Ĭ�Ϸ���";
        foreach(QAction* temp, group->actions())
        {
            if (temp->isChecked())
            {
                configFileName = temp->text();
            }           
        }
        QString configFilePath = appPath + "/../cfg/private/" + configFileName + ".xml";
        if (!QFile::exists(configFilePath))
        {
            QMessageBox::warning(NULL, "SCL���", "��Ӧ�������ļ������ڣ�");
            return;
        }

        // ���exe�ļ�
        QString scdCheckExe = appPath + "/SCLCheck/ScdChecker.exe"; 

		substationManageWin->hide();

		progresCheckStyle->setPorgressValue(0);    // ��������ʼ��
		progresCheckStyle->setLabeCurrentText("���ڼ��.......");
		proCheckFlg=true;
		proGressCount=0;
		
		
		QFileInfo scdInfo(filePath);
		int ms= scdInfo.size()/1024/500;  //Ԥ����average m ����ɵ���
		averageSpeed=100/(ms+1);
		QString text="  "+subInfo->fileName;
		progresCheckStyle->setFileText(text,currentImportFile,fileNameList.count());
		progresCheckStyle->show();
		ShowEvent();
		// checkThread->setFilePath(fileName);
		

		
		m_SCLValid->SetFilePath(filePath, scdCheckExe, configFilePath);
		checkThread->start();
		timerCheckProgress->start(250);
		qValidateDialog.SetFilePath(filePath);
	}
	else
		progresCheckStyle->show();


}

void GraphicBrowserFrm::ShowDeleDialog()
{
	substationManageWin->setWindowTitle(tr("ɾ��SCD"));
	disconnect(surebutton, 0, 0, 0);
	connect(surebutton, SIGNAL(clicked()), this, SLOT(SubStationDeleteProc()));
	ShowSubstationManageProc();
	substationManageWin->show();
}
void GraphicBrowserFrm::CreateAction(void)
{

	//__________________________________________
	actionImportSCD=new QAction( QIcon("./images/scdImp.png"),tr("����SCD�ļ�"), this);
	actionImportSCD->setToolTip(tr("����SCD�ļ�"));
	actionImportSCD->setIconText(tr("����"));
	connect(actionImportSCD, SIGNAL(triggered()),
		this, SLOT(LoadSCDProc()));

	actionCheck=new QAction( tr("У��"), this); 
	actionCheck->setIcon(QIcon("./images/search.png"));
	actionCheck->setToolTip(tr("SCD�ļ��Ϸ���У��"));
	actionCheck->setIconText(tr("У��"));
	connect(actionCheck, SIGNAL(triggered()),
		this, SLOT(StartCheckSCD()));

	actionDelete=new QAction( tr("ɾ��SCD�ļ�"), this); 
	actionDelete->setIcon(QIcon("./images/scdDel.png"));
	actionDelete->setToolTip(tr("ɾ��SCD�ļ�"));
	actionDelete->setIconText(tr("ɾ��"));
	connect(actionDelete, SIGNAL(triggered()),
		this, SLOT(ShowDeleDialog()));

	actionSwitch=new QAction( tr("�л�"), this); 
	actionSwitch->setIcon(QIcon("./images/scdChange.png"));
	actionSwitch->setToolTip(tr("�л����վ"));
	actionSwitch->setIconText(tr("�л�"));
	connect(actionSwitch, SIGNAL(triggered()),
		this, SLOT(ShowSwithDialog()));

	actionCompare=new QAction( tr("�Ƚ�"), this);
	actionCompare->setIcon(QIcon("./images/scdCom.png"));
	actionCompare->setToolTip(tr("�Ƚ�SCD�ļ�"));
	actionCompare->setIconText(tr("�Ƚ�"));
	connect(actionCompare, SIGNAL(triggered()),
		this, SLOT(ShowSubstationChangeProc()));


	//actionShowIEDsTree = new QAction(QIcon(":/images/icon-m-toolbar-list-white.png"),
	//	tr("������ͼ��ʾ/����ʾ"), this);
	actionShowIEDsTree = new QAction(QIcon(":/images/icon-m-toolbar-list-white.png"),
		tr("����"), this);
	actionShowIEDsTree->setShortcut(tr("Ctrl+T"));
	actionShowIEDsTree->setStatusTip(tr("��ʾ/��������ͼ"));
	actionShowIEDsTree->setToolTip(tr("������ͼ��ʾ/����ʾ"));
	QPixmap pixmap1("./images/icon-m-toolbar-list-white.png");
	actionShowIEDsTree->setIcon(QIcon(pixmap1));
	actionShowIEDsTree->setCheckable(true);
	actionShowIEDsTree->setChecked(true);
	connect(actionShowIEDsTree, SIGNAL(triggered()),
		this, SLOT(ShowIEDsTreeProc()));

	actionShowgraphicsView = new QAction(QIcon(":/images/icon-m-picasa.png"),
		tr("ͼ����ͼ"), this);
	//actionShowgraphicsView->setShortcut(tr("Ctrl+A"));
	actionShowgraphicsView->setStatusTip(tr("��ʾ/����ͼ����ͼ"));
	QPixmap pixmap2("./images/icon-m-picasa.png");
	actionShowgraphicsView->setIcon(QIcon(pixmap2));
	actionShowgraphicsView->setCheckable(true);
	connect(actionShowgraphicsView, SIGNAL(triggered()),
		this, SLOT(ShowgraphicsViewProc()));

	actionSubStationChange = new QAction(QIcon(":/images/ConvertTableToText.png"),
		tr("SCD�Ƚ�"), this);

	actionLoadSCD = new QAction(QIcon("images/ControlsGallery.png"),
		tr("���վ����"), this);

	actionClearDB = new QAction(QIcon("images/ClearGrid.png"),tr("ж�ص�ǰվ"),this);
	actionClearDB->setStatusTip(tr("ж�ص�ǰվ"));
	connect(actionClearDB, SIGNAL(triggered()), this, SLOT(ClearDBProc()));

	actionCheckSCD = new QAction(QIcon("images/AdpDiagramTableModesMenu.png"),tr("���SCD"),this);
	actionCheckSCD->setStatusTip(tr("У��SCD�ļ��Ϸ���"));
	connect(actionCheckSCD, SIGNAL(triggered()),this, SLOT(CheckSCDProc()));

	actionPre = new QAction(QIcon("images/previous.png"),
		tr("����"), this);
	actionPre->setShortcut(tr("Ctrl+P"));
	QPixmap pixmap3("./images/previous.png");
	actionPre->setIcon(QIcon(pixmap3));
	actionPre->setStatusTip(tr("����"));
	actionPre->setIconText(tr("����"));
	//actionPre->setStatusTip(tr(""));
	connect(actionPre, SIGNAL(triggered()),
		this, SLOT(PreProc()));
	actionPre->setEnabled(false);

    // ��������� �˵���
    actionEditConfig = new QAction("���������", this);
    actionEditConfig->setStatusTip(tr("���������"));
    actionEditConfig->setEnabled(true);
    connect(actionEditConfig, SIGNAL(triggered()), this, SLOT(OnEditConfigTriggered()));

	actionNext = new QAction(QIcon("images/next.png"),
		tr("ǰ��"), this);
	actionNext->setShortcut(tr("Ctrl+N"));
	actionNext->setStatusTip(tr("ǰ��"));
	actionNext->setIconText(tr("ǰ��"));
	QPixmap pixmap4("./images/next.png");
	actionNext->setIcon(QIcon(pixmap4));
	connect(actionNext, SIGNAL(triggered()),
		this, SLOT(NextProc()));
	actionNext->setEnabled(false);

	actionZoomIn = new QAction(QIcon(":/images/zoomin.png"),
		tr("�Ŵ�"), this);
	actionZoomIn->setShortcut(tr("Ctrl+I"));
	actionZoomIn->setStatusTip(tr("�Ŵ���ͼ"));
	QPixmap pixmap5("./images/zoomin.png");
	actionZoomIn->setIcon(QIcon(pixmap5));
	connect(actionZoomIn, SIGNAL(triggered()),
		this, SLOT(ZoomInProc()));

	actionZoomOut = new QAction(QIcon(":/images/zoomout.png"),
		tr("��С"), this);
	actionZoomOut->setShortcut(tr("Ctrl+O"));
	actionZoomOut->setStatusTip(tr("��С��ͼ"));
	QPixmap pixmap6("./images/zoomout.png");
	actionZoomOut->setIcon(QIcon(pixmap6));
	connect(actionZoomOut, SIGNAL(triggered()),
		this, SLOT(ZoomOutProc()));

	actionRoam = new QAction(QIcon(":/images/roam.png"),
		tr("����"), this);
	actionRoam->setShortcut(tr("Ctrl+M"));
	actionRoam->setStatusTip(tr("����"));
	QPixmap pixmap7("./images/roam.png");
	actionRoam->setIcon(QIcon(pixmap7));
	actionRoam->setCheckable(true);
	connect(actionRoam, SIGNAL(triggered()),
		this, SLOT(RoamProc()));

	actionRecovery = new QAction(QIcon(":/images/ZoomClassic.png"),
		tr("�ָ�"), this);
	actionRecovery->setShortcut(tr("Ctrl+R"));
	actionRecovery->setStatusTip(tr("�ָ���ͼ"));
	QPixmap pixmap8("./images/ZoomClassic.png");
	actionRecovery->setIcon(QIcon(pixmap8));
	connect(actionRecovery, SIGNAL(triggered()),
		this, SLOT(RecoveryProc()));

	actionShowToolBar = new QAction(QIcon(":/images/zoomout.png"),
		tr("������"), this);
	//actionShowToolbar->setShortcut(tr("Ctrl+W"));
	actionShowToolBar->setStatusTip(tr("��ʾ/���ع�����"));
	QPixmap pixmap15("./images/exit.png");
	actionShowToolBar->setIcon(QIcon(pixmap15));
	connect(actionShowToolBar, SIGNAL(triggered()),
		this, SLOT(ShowToolbarProc()));
	actionShowToolBar->setCheckable(true);
	actionShowToolBar->setChecked(true);

	actionShowStatusBar = new QAction(QIcon(":/images/zoomout.png"),
		tr("״̬��"), this);
	actionShowStatusBar->setStatusTip(tr("��ʾ/����״̬��"));
	QPixmap pixmap16("./images/exit.png");
	actionShowStatusBar->setIcon(QIcon(pixmap16));
	connect(actionShowStatusBar, SIGNAL(triggered()),
		this, SLOT(ShowStatusbarProc()));
	actionShowStatusBar->setCheckable(true);
	actionShowStatusBar->setChecked(true);

    actionShowOutSignal = new QAction(tr("չʾ������ͼδ��������ź�"), this);
    actionShowOutSignal->setCheckable(true);
    QSettings *setting = new QSettings("../cfg/SubStationInfo.ini", QSettings::IniFormat);
    bool isShowOutSignalChecked = setting->value("config/ShowOutSignalInTerminalGraphic").toBool();
    actionShowOutSignal->setChecked(isShowOutSignalChecked);
    connect(actionShowOutSignal, SIGNAL(triggered()), this, SLOT(ShowOutSignalInTerminalGraphic()));

	actionPrintPreview = new QAction(QIcon("images/AdpDiagramTableModesMenu.png"),
		tr("��ӡԤ��"), this);
	actionPrintPreview->setShortcut(tr("Ctrl+F"));
	actionPrintPreview->setStatusTip(tr("��ӡԤ����ǰͼ��"));
	connect(actionPrintPreview, SIGNAL(triggered()),
		this, SLOT(PrintPreviewProc()));

	actionSaveAs = new QAction(QIcon("images/Save32.png"),
		tr("���ΪͼƬ"), this);
	actionSaveAs->setShortcut(tr("Ctrl+S"));
	actionSaveAs->setStatusTip(tr("���ΪͼƬ"));
	connect(actionSaveAs, SIGNAL(triggered()),
		this, SLOT(SaveAsProc()));

	actionExportICDInfo = new QAction(QIcon("images/Save32.png"),
		tr("����ICD�汾��Ϣ"), this);
	actionExportICDInfo->setStatusTip(tr("����ICD�汾��Ϣ"));
	connect(actionExportICDInfo, SIGNAL(triggered()),
		this, SLOT(ExportICDInfoProc()));

	actionExit = new QAction(QIcon(":/images/zoomout.png"),
		tr("�˳�"), this);
	actionExit->setShortcut(tr("Ctrl+E"));
	actionExit->setStatusTip(tr("�˳�"));
	connect(actionExit, SIGNAL(triggered()),
		qApp, SLOT(quit()));

	actionAbout = new QAction(QIcon("images/roof_history.png"),tr("����..."), this);
	actionAbout->setShortcut(tr("Ctrl+B"));
	actionAbout->setStatusTip(tr("����..."));
	connect(actionAbout, SIGNAL(triggered()),
		this, SLOT(AboutProc()));

	actionCompareEnd = new QAction(QIcon("images/DeclineTask.png"),tr("�����Ƚ�"),this);
	actionCompareEnd->setStatusTip(tr("�����Ƚ�"));
	actionCompareEnd->setEnabled(false);
	actionCompareEnd->setVisible(false);
	connect(actionCompareEnd, SIGNAL(triggered()), this, SLOT(SubStationCompareEndProc()));

	actionExportFile = new QAction(QIcon("images/excel.png"),tr("����excel����"),this);
	actionExportFile->setStatusTip(tr("����excel����"));
	actionExportFile->setVisible(false);
	connect(actionExportFile, SIGNAL(triggered()), this, SLOT(SubStationCompareExportProc()));

	actionExportWordFile =new QAction(QIcon("images/word.png"),tr("����word����"),this);
	actionExportWordFile->setStatusTip(tr("����word����"));
	actionExportWordFile->setVisible(false);
	connect(actionExportWordFile, SIGNAL(triggered()), this, SLOT(SubStationCompareExportWordProc()));
	//act


}

void GraphicBrowserFrm::CreateMenus(void)
{
	fileMenu = menuBar()->addMenu(tr("�ļ�(&S)"));
	fileMenu->addAction(actionSaveAs);
	fileMenu->addAction(actionExportICDInfo);
	fileMenu->addSeparator();
	fileMenu->addAction(actionExit);

	operatorMenu = menuBar()->addMenu(tr("����(&O)"));
	operatorMenu->addAction(actionPre);
	operatorMenu->addAction(actionNext);
	operatorMenu->addAction(actionZoomIn);
	operatorMenu->addAction(actionZoomOut);
	operatorMenu->addAction(actionRecovery);

	visMenu = menuBar()->addMenu(tr("��ͼ(&V)"));
	visMenu->addAction(actionShowIEDsTree);
	visMenu->addAction(actionShowToolBar);
	visMenu->addAction(actionShowStatusBar);
    visMenu->addSeparator();
    visMenu->addAction(actionShowOutSignal);

    checkMenu = menuBar()->addMenu(tr("���(&C)"));
    checkMenu->addAction(actionEditConfig);
    checkMenu->addSeparator();


	helpMenu = menuBar()->addMenu(tr("����(&H)"));
	helpMenu->addAction(actionAbout);
}

void GraphicBrowserFrm::CreateToolbars(void)
{
	labelVotageLevel->setText("��ѹ�ȼ� ");
	labelSubstation->setText("���վ ");


	mainToolbar = addToolBar(tr("��������"));
	mainToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	mainToolbar->addAction(actionShowIEDsTree);
	mainToolbar->addSeparator();
		
	mainToolbar->addAction(actionImportSCD);
	mainToolbar->addAction(actionDelete);
	mainToolbar->addAction(actionSwitch);
	mainToolbar->addAction(actionCompare);
	mainToolbar->addSeparator();

	mainToolbar->addAction(actionCheck);
	mainToolbar->addSeparator();

	mainToolbar->addAction(actionPre);
	mainToolbar->addAction(actionNext);
	mainToolbar->addAction(actionZoomIn);
	mainToolbar->addAction(actionZoomOut);
	mainToolbar->addAction(actionRecovery);

	
	mainToolbar->addSeparator();
	mainToolbar->addAction(actionCompareEnd);
	mainToolbar->addAction(actionExportFile);
	mainToolbar->addAction(actionExportWordFile);
	mainToolbar->addWidget(analysisProgressBar);
	//____________________________________________
}

void GraphicBrowserFrm::CreateStatusBar()
{
    statusBar()->addWidget(analysisProgressBar);
}

void GraphicBrowserFrm::ReadSettings()
{
	QSettings settings("tech-roof Inc.", "GraphicBrowser");

	settings.beginGroup("SecurityMeasuresEditor");
	restoreGeometry(settings.value("geometry").toByteArray());

	//vertSplitter->restoreState(
	//	settings.value("mainSplitter").toByteArray());
	//horzSplitter->restoreState(
	//	settings.value("rightSplitter").toByteArray());

	settings.endGroup();
}

void GraphicBrowserFrm::WriteSettings()
{
	QSettings settings("tech-roof Inc.", "GraphicBrowser");

	settings.beginGroup("SecurityMeasuresEditor");
	settings.setValue("geometry", saveGeometry());

	//settings.setValue("mainSplitter", vertSplitter->saveState());
	//settings.setValue("rightSplitter", horzSplitter->saveState());
	settings.endGroup();
}

void GraphicBrowserFrm::InitIEDsTreeWidget()
{
	IEDsTreeWidget->clear();
	widgetIEDTable->clear();
	QTreeWidgetItem *nothingItem,*versionItem,*substationRootItem,*substationItem,*subNetRootItem,*subNetItem,*IEDRootItem,*IEDItem,*deviceItem;
	QTreeWidgetItem *mainTransIEDItem, *otherIEDItem, *IEDItem6kV, *IEDItem10kV, *IEDItem35kV, 
		*IEDItem66kV, *IEDItem110kV, *IEDItem220kV, *IEDItem330kV, *IEDItem500kV, *IEDItem750kV, *IEDItem1000kV;
	QTableWidgetItem *itemTotalIedName,*itemTotalIedDesc,*itemTotalIedtype,*itemTotalIedmanu;
	QStringList lists;
	lists<<"����ͼ";
	IEDsTreeWidget->setHeaderLabels(lists);
	IEDsTreeWidget->header()->hide();

	if (IEDsTreeWidget->topLevelItemCount() == 0)
	{
		nothingItem = new QTreeWidgetItem(IEDsTreeWidget,NODE_TYPE_NOTHING);
		nothingItem->setHidden(true);
		IEDsTreeWidget->addTopLevelItem(nothingItem);


		//subNetRootItem = new  QTreeWidgetItem(IEDsTreeWidget,NODE_TYPE_SUBNET_ROOT);
		//subNetRootItem->setText(0,"����");
		//subNetRootItem->setIcon(0, QIcon("images/greenStone.png"));   
		//IEDsTreeWidget->addTopLevelItem(subNetRootItem);  
		//for(int i=0;i<IEDsLoader->GetSubNetList()->size();i++)
		//{
		//	subNetItem = new  QTreeWidgetItem(subNetRootItem,NODE_TYPE_SUBNET);
		//	subNetItem->setText(0,IEDsLoader->GetSubNetList()->at(i).subNetName);
		//	subNetItem->setIcon(0, QIcon("images/redStone.png"));   
		//	IEDsTreeWidget->setItemExpanded(subNetItem,false);
		//	subNetRootItem->addChild(subNetItem); 
		//}

		QVector<QString> *substationList = IEDsLoader->GetSubstationNameList();

		if (substationList->size() == 0 || IEDsLoader->GetCurrentSubstationID().size() == 0)
		{
			if (actionCheck != NULL)
			{
				actionCheck->setDisabled(true);
			}
			return;
		}
		actionCheck->setEnabled(true);
		//QString subKey = "220kV." + substationList->at(IEDsLoader->GetCurrentSubstationIndex());
		//QSubStationInfo *subInfo = IEDsLoader->GetSubstationInfo(subKey);

		IEDRootItem = new  QTreeWidgetItem(IEDsTreeWidget,NODE_TYPE_IED_ROOT);
		//QList<QString> nameList = subInfo->Name.split("#");
		//QString subName = "";
		//if (nameList.size() != 0)
		//{
		//	subName = nameList.at(0);
		//}
		IEDRootItem->setText(0,"����װ�� ��" + QString::number(IEDsLoader->GetIEDList()->size()) + "̨��");
		IEDRootItem->setToolTip(0, "����װ�� ��" + QString::number(IEDsLoader->GetIEDList()->size()) + "̨��");
		IEDRootItem->setIcon(0, QIcon("images/greenStone.png"));   
		IEDsTreeWidget->addTopLevelItem(IEDRootItem);

		//����;
		mainTransIEDItem = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		mainTransIEDItem->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(mainTransIEDItem);
		addChildItem(mainTransIEDItem);

		//1000ǧ��;
		IEDItem1000kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem1000kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem1000kV);
		addChildItem(IEDItem1000kV);

		//750ǧ��;
		IEDItem750kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem750kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem750kV);
		addChildItem(IEDItem750kV);

		//500ǧ��;
		IEDItem500kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem500kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem500kV);
		addChildItem(IEDItem500kV);

		//330ǧ��;
		IEDItem330kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem330kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem330kV);
		addChildItem(IEDItem330kV);

		//220ǧ��;
		IEDItem220kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem220kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem220kV);
		addChildItem(IEDItem220kV);

		//110ǧ��;
		IEDItem110kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem110kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem110kV);		
		addChildItem(IEDItem110kV);

		//66ǧ��;
		IEDItem66kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem66kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem66kV);
		addChildItem(IEDItem66kV);

		//35ǧ��;
		IEDItem35kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem35kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem35kV);
		addChildItem(IEDItem35kV);

		//10ǧ��;
		IEDItem10kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem10kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem10kV);
		addChildItem(IEDItem10kV);

		//6ǧ��;
		IEDItem6kV = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		IEDItem6kV->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(IEDItem6kV);
		addChildItem(IEDItem6kV);

		//����;
		otherIEDItem = new QTreeWidgetItem(IEDRootItem, NODE_TYPE_NOTHING);
		otherIEDItem->setIcon(0, QIcon("images/yellowStone.png"));
		IEDRootItem->addChild(otherIEDItem);
		addChildItem(otherIEDItem);

		//______________________________����ȫ���豸�б�;
		widgetIEDTable->setRowCount(IEDsLoader->GetIEDList()->size());
		QStringList stringlist;
		stringlist<<"IED"<<"IED����"<<"��������"<<"�豸�ͺ�";
		widgetIEDTable->setColumnCount(4);
		widgetIEDTable->setHorizontalHeaderLabels(stringlist);

		int mSum = 0;
		int oSum = 0;
		int Sum6kV = 0;
		int Sum10kV = 0;
		int Sum35kV = 0;
		int Sum66kV = 0;
		int Sum110kV = 0;
		int Sum220kV = 0;
		int Sum330kV = 0;
		int Sum500kV = 0;
		int Sum750kV = 0;
		int Sum1000kV = 0;
		for(int j=0;j<IEDsLoader->GetIEDList()->size();j++)
		{
			//QString IEDName = IEDsLoader->GetIEDList()->at(j).IEDName;//ԭ
			QString IEDKey = IEDsLoader->GetIEDList()->at(j).IEDKey;
			QString srcIedName = IEDsLoader->GetIEDList()->at(j).IEDName;
			QString IEDName = "[" + IEDsLoader->GetIEDList()->at(j).IEDName + "]" + IEDsLoader->GetIEDInfoList()->at(j).desc;

			CVoltageLevelAnalysor vtAnalysor;
			int voltageLevelType = vtAnalysor.analysisVoltageLevelByIED(IEDsLoader->GetIEDList()->at(j).IEDName, IEDsLoader->GetIEDInfoList()->at(j).desc);

			IEDTypeAnalysor typeAnalysor;
			IEDType iedType = typeAnalysor.parseIEDType(IEDsLoader->GetIEDList()->at(j).IEDName, IEDsLoader->GetIEDInfoList()->at(j).desc);
			switch (voltageLevelType)
			{
			case VOLTAGELEVEL_MAINTRANS:
				IEDItem = addChildItemByType(mainTransIEDItem, iedType,srcIedName);
				mSum++;
				break;
			case VOLTAGELEVEL_OTHERS:
				IEDItem = addChildItemByType(otherIEDItem, iedType,srcIedName);
				oSum++;
				break;
			case VOLTAGELEVEL_6KV:
				IEDItem = addChildItemByType(IEDItem6kV, iedType,srcIedName);
				Sum6kV++;
				break;
			case VOLTAGELEVEL_10KV:
				IEDItem = addChildItemByType(IEDItem10kV, iedType,srcIedName);
				Sum10kV++;
				break;
			case VOLTAGELEVEL_35KV:
				IEDItem = addChildItemByType(IEDItem35kV, iedType,srcIedName);
				Sum35kV++;
				break;
			case VOLTAGELEVEL_66KV:
				IEDItem = addChildItemByType(IEDItem66kV, iedType,srcIedName);
				Sum66kV++;
				break;
			case VOLTAGELEVEL_110KV:
				IEDItem = addChildItemByType(IEDItem110kV, iedType,srcIedName);
				Sum110kV++;
				break;
			case VOLTAGELEVEL_220KV:
				IEDItem = addChildItemByType(IEDItem220kV, iedType,srcIedName);
				Sum220kV++;
				break;
			case VOLTAGELEVEL_330KV:
				IEDItem = addChildItemByType(IEDItem330kV, iedType,srcIedName);
				Sum330kV++;
				break;
			case VOLTAGELEVEL_500KV:
				IEDItem = addChildItemByType(IEDItem500kV, iedType,srcIedName);
				Sum500kV++;
				break;
			case VOLTAGELEVEL_750KV:
				IEDItem = addChildItemByType(IEDItem750kV, iedType,srcIedName);
				Sum750kV++;
				break;
			case VOLTAGELEVEL_1000KV:
				IEDItem = addChildItemByType(IEDItem1000kV, iedType,srcIedName);
				Sum1000kV++;
				break;
			}
			IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
			IEDItem->setText(0,IEDName);
			IEDItem->setToolTip(0,IEDName);
			IEDItem->setIcon(0,QIcon("images/graphic_two.png"));

			//add widgetTatal
			itemTotalIedName=new QTableWidgetItem(IEDsLoader->GetIEDList()->at(j).IEDName);
			itemTotalIedName->setData(100,IEDKey);
		    itemTotalIedDesc=new QTableWidgetItem(IEDsLoader->GetIEDInfoList()->at(j).desc);
			itemTotalIedmanu=new QTableWidgetItem(IEDsLoader->GetIEDInfoList()->at(j).manu);
			itemTotalIedtype=new QTableWidgetItem(IEDsLoader->GetIEDInfoList()->at(j).type);
			widgetIEDTable->setItem(j,0,itemTotalIedName);
			widgetIEDTable->setItem(j,1,itemTotalIedDesc);
			widgetIEDTable->setItem(j,2,itemTotalIedmanu);
			widgetIEDTable->setItem(j,3,itemTotalIedtype);
		}

		if (mSum == 0)
		{
			mainTransIEDItem->setHidden(true);
			delete mainTransIEDItem;
		}
		else
		{
			mainTransIEDItem->setText(0, "���� ��" + QString::number(mSum) + "̨��");
			hideEmptyChildItem(mainTransIEDItem);
		}

		if (oSum == 0)
		{
			otherIEDItem->setHidden(true);
			delete otherIEDItem;
		}
		else
		{
			otherIEDItem->setText(0, "���� ��" + QString::number(oSum) + "̨��");
			hideEmptyChildItem(otherIEDItem);
		}

		if (Sum6kV == 0)
		{
			IEDItem6kV->setHidden(true);
			delete IEDItem6kV;
		}
		else
		{
			IEDItem6kV->setText(0, "6kV ��" + QString::number(Sum6kV) + "̨��");
			hideEmptyChildItem(IEDItem6kV);
		}

		if (Sum10kV == 0)
		{
			IEDItem10kV->setHidden(true);
			delete IEDItem10kV;
		}
		else
		{
			IEDItem10kV->setText(0, "10kV ��" + QString::number(Sum10kV) + "̨��");
			hideEmptyChildItem(IEDItem10kV);
		}

		if (Sum35kV == 0)
		{
			IEDItem35kV->setHidden(true);
			delete IEDItem35kV;
		}
		else
		{
			IEDItem35kV->setText(0, "35kV ��" + QString::number(Sum35kV) + "̨��");
			hideEmptyChildItem(IEDItem35kV);
		}

		if (Sum66kV == 0)
		{
			IEDItem66kV->setHidden(true);
			delete IEDItem66kV;
		}
		else
		{
			IEDItem66kV->setText(0, "66kV ��" + QString::number(Sum66kV) + "̨��");
			hideEmptyChildItem(IEDItem66kV);
		}

		if (Sum110kV == 0)
		{
			IEDItem110kV->setHidden(true);
			delete IEDItem110kV;
		}
		else
		{
			IEDItem110kV->setText(0, "110kV ��" + QString::number(Sum110kV) + "̨��");
			hideEmptyChildItem(IEDItem110kV);
		}

		if (Sum220kV == 0)
		{
			IEDItem220kV->setHidden(true);
			delete IEDItem220kV;
		}
		else
		{
			IEDItem220kV->setText(0, "220kV ��" + QString::number(Sum220kV) + "̨��");
			hideEmptyChildItem(IEDItem220kV);
		}

		if (Sum330kV == 0)
		{
			IEDItem330kV->setHidden(true);
			delete IEDItem330kV;
		}
		else
		{
			IEDItem330kV->setText(0, "330kV ��" + QString::number(Sum330kV) + "̨��");
			hideEmptyChildItem(IEDItem330kV);
		}

		if (Sum500kV == 0)
		{
			IEDItem500kV->setHidden(true);
			delete IEDItem500kV;
		}
		else
		{
			IEDItem500kV->setText(0, "500kV ��" + QString::number(Sum500kV) + "̨��");
			hideEmptyChildItem(IEDItem500kV);
		}

		if (Sum750kV == 0)
		{
			IEDItem750kV->setHidden(true);
			delete IEDItem750kV;
		}
		else
		{
			IEDItem750kV->setText(0, "750kV ��" + QString::number(Sum750kV) + "̨��");
			hideEmptyChildItem(IEDItem750kV);
		}

		if (Sum1000kV == 0)
		{
			IEDItem1000kV->setHidden(true);
			delete IEDItem1000kV;
		}
		else
		{
			IEDItem1000kV->setText(0, "1000kV ��" + QString::number(Sum1000kV) + "̨��");
			hideEmptyChildItem(IEDItem1000kV);
		}

		IEDRootItem->setExpanded(true);

		//versionItem = new  QTreeWidgetItem(IEDsTreeWidget,NODE_TYPE_VERSION);
		//versionItem->setText(0,"�汾");
		//versionItem->setIcon(0, QIcon("images/greenStone.png"));  
		//IEDsTreeWidget->addTopLevelItem(versionItem);
	}
	//QRect rect = IEDsTreeWidget->rect();
	//rect.setWidth(300);
	//IEDsTreeWidget->setMaximumWidth(350);

	//connect(IEDsTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
	//	this,SLOT(SelectTreeProc(QTreeWidgetItem*)));

	//treeVLayout->addWidget(IEDQueryBar);
	//treeVLayout->addWidget(IEDsTreeWidget);
	//treeContainerWidget->setLayout(treeVLayout);
	//treeTabWidget->addTab(IEDsTreeWidget,tr("�ۺ����"));
	//treeTabWidget->addTab(treeContainerWidget,tr("�ۺ����"));
	//treeTabWidget->addTab(deviceView,tr("����װ����Ϣ"));
	//treeTabWidget->addTab(compareView,tr("IEDһ��"));
	//treeTabWidget->setTabEnabled(2,false);
}

void GraphicBrowserFrm::addChildItem(QTreeWidgetItem *vtItem)
{
	QList<QTreeWidgetItem*> childList;
	QTreeWidgetItem *PTypeItem, *CTypeItem, *STTypeItem, *MUTypeItem, *MSTypeItem, *PCTypeItem, *NOTypeItem;
	PTypeItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	PTypeItem->setText(0, "����װ��");
	PTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	addAssortmentItem(PTypeItem);
	childList.append(PTypeItem);

	CTypeItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	CTypeItem->setText(0, "���װ��");
	CTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	//addAssortmentItem(CTypeItem);
	childList.append(CTypeItem);

	STTypeItem = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	STTypeItem->setText(0, "�����ն�");
	STTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	//addAssortmentItem(STTypeItem);
	childList.append(STTypeItem);

	MUTypeItem = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	MUTypeItem->setText(0, "�ϲ���Ԫ");
	MUTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	//addAssortmentItem(MUTypeItem);
	childList.append(MUTypeItem);

	MSTypeItem = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	MSTypeItem->setText(0, "�ϲ���Ԫ�����ն˻�װ��");
	MSTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	//addAssortmentItem(MSTypeItem);
	childList.append(MSTypeItem);

	PCTypeItem = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	PCTypeItem->setText(0, "����һ��װ��");
	PCTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	//addAssortmentItem(PCTypeItem);
	childList.append(PCTypeItem);

	NOTypeItem = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	NOTypeItem->setText(0, "����װ��");
	NOTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	childList.append(NOTypeItem);

	vtItem->addChildren(childList);
}
void    GraphicBrowserFrm::addAssortmentItem(QTreeWidgetItem *vtItem)
{
	QList<QTreeWidgetItem*> childList;
	QTreeWidgetItem *MItem, *LItem, *STTypeItem, *RCItem, *NOItem;
	MItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	MItem->setText(0, "ĸ�߱���");
	MItem->setIcon(0, QIcon("images/type_folder.png"));
	childList.append(MItem);

	LItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	LItem->setText(0, "��·����");
	LItem->setIcon(0, QIcon("images/type_folder.png"));
	childList.append(LItem);


	STTypeItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	STTypeItem->setText(0, "��·������");
	STTypeItem->setIcon(0, QIcon("images/type_folder.png"));
	childList.append(STTypeItem);


	RCItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	RCItem->setText(0, "�������翹����");
	RCItem->setIcon(0, QIcon("images/type_folder.png"));
	childList.append(RCItem);

	NOItem  = new QTreeWidgetItem(vtItem, NODE_TYPE_NOTHING);
	NOItem->setText(0, "��������");
	NOItem->setIcon(0, QIcon("images/type_folder.png"));
	childList.append(NOItem);
	vtItem->addChildren(childList);
}

QTreeWidgetItem* GraphicBrowserFrm::getAddItem(QTreeWidgetItem*parItem,QString IedName)
{
	  //if(IedName.at())
	QTreeWidgetItem *IEDItem = NULL;
	if(IedName.at(1)=='M')
	{ 
	   IEDItem = parItem->child(0);
	}else if(IedName.at(1)=='L')
	{
	   IEDItem = parItem->child(1);
	}else if(IedName.at(1)=='E'||IedName.at(1)=='B'||IedName.at(1)=='F')
	{
		 IEDItem = parItem->child(2);
	}else if(IedName.at(1)=='R'||IedName.at(1)=='C')
	{
		 IEDItem = parItem->child(3);
	}else 
	{
		 IEDItem = parItem->child(4);
	}
	return IEDItem;
}

QTreeWidgetItem* GraphicBrowserFrm::addChildItemByType(QTreeWidgetItem *vtItem, IEDType iedType,QString iedName)
{
	QTreeWidgetItem *IEDItem = NULL;
	switch (iedType)
	{
	case P:
		IEDItem = new QTreeWidgetItem(NODE_TYPE_IED);
        getAddItem(vtItem->child(0),iedName)->addChild(IEDItem); 
		break;
	case C:
		IEDItem = new QTreeWidgetItem(vtItem->child(1), NODE_TYPE_IED);
		vtItem->child(1)->addChild(IEDItem);
		break;
	case ST:
		IEDItem = new QTreeWidgetItem(vtItem->child(2), NODE_TYPE_IED);
		vtItem->child(2)->addChild(IEDItem);
		break;
	case MU:
		IEDItem = new QTreeWidgetItem(vtItem->child(3), NODE_TYPE_IED);
		vtItem->child(3)->addChild(IEDItem);
		break;
	case MS:
		IEDItem = new QTreeWidgetItem(vtItem->child(4), NODE_TYPE_IED);
		vtItem->child(4)->addChild(IEDItem);
		break;
	case PC:
		IEDItem = new QTreeWidgetItem(vtItem->child(5), NODE_TYPE_IED);
		vtItem->child(5)->addChild(IEDItem);
		break;
	case NO:
		IEDItem = new QTreeWidgetItem(vtItem->child(6), NODE_TYPE_IED);
		vtItem->child(6)->addChild(IEDItem);
	    //IEDItem = new QTreeWidgetItem(vtItem, NODE_TYPE_IED);
		//vtItem->addChild(IEDItem);
		break;
	}
	return IEDItem;
}
void    GraphicBrowserFrm::ChangeItemText(QTreeWidgetItem *vtItem)
{
	bool isempty = false;
	int totalCount = 0;
	for(int y=0;y<5;y++)
	{
		if(vtItem->child(y)->childCount()==0)
		{
			 vtItem->child(y)->setHidden(true);
		}else 
		{
			int count = vtItem->child(y)->childCount();
			totalCount = totalCount + count;
			switch (y)
			{
			case 0:
				 vtItem->child(y)->setText(0, "ĸ�߱��� ��" + QString::number(count) + "̨��");
			     break;
			case 1:
				 vtItem->child(y)->setText(0, "��·���� ��" + QString::number(count) + "̨��");
			     break;
			case 2:
			     vtItem->child(y)->setText(0, "��·������ ��" + QString::number(count) + "̨��");
				 break;
			case 3:
				 vtItem->child(y)->setText(0, "�������翹���� ��" + QString::number(count) + "̨��");
				 break;
			case 4:
				 vtItem->child(y)->setText(0, "�������� ��" + QString::number(count) + "̨��");
				 break;
			}     
			isempty = true;
		}
	}
	if (totalCount > 0)
	{
		vtItem->setText(0, "����װ�ã�"+QString::number(totalCount)+"̨��");
	}
	if(!isempty)
	{
		 vtItem->setHidden(true);
	}
}
void GraphicBrowserFrm::hideEmptyChildItem(QTreeWidgetItem *vtItem)
{
	for (int i = 0; i != 7; i++)
	{
		if(i==0)
		{
		    ChangeItemText(vtItem->child(0));
		}

		if (vtItem->child(i)->childCount() == 0)
		{
			vtItem->child(i)->setHidden(true);
		}else
		{
			if (i != 0)
			{
				QString name = vtItem->child(i)->text(0);
				QString key = name + "��" + QString::number(vtItem->child(i)->childCount()) + "̨��";
				vtItem->child(i)->setText(0, key);
			}
		}
	}
}

void GraphicBrowserFrm::InitSubInfoTreeWidget()
{
	subInfoTreeWidget->clear();
	QStringList lists;
	lists<<"���վ������Ϣ";
	subInfoTreeWidget->setHeaderLabels(lists);
	QList<QTreeWidgetItem*> itemList;
	QVector<QString> *substationList = IEDsLoader->GetSubstationNameList();
	if (substationList->size() == 0)
	{
		return;
	}

	bool bFindStation = false;
	for (int i = 0; i < substationList->size(); i++)
	{
		if(substationList->at(i) == IEDsLoader->GetCurrentSubstationID())
		{
			bFindStation = true;
		}
	}
	if(!bFindStation)
	{
		return;
	}
	
    QString subKey = "220kV." + IEDsLoader->GetCurrentSubstationID();
	QSubStationInfo *subInfo = IEDsLoader->GetSubstationInfo(subKey);
	

	//chenxin modify 2014.10.17 ---> ����Ҫ�޸�ʱ��
	//QList<QString> nameList = subInfo->Name.split("#");
	//QString subName = "";
	//QString loadDateTime = "";
	//if (nameList.size() != 0)
	//{
	//	subName = nameList.at(0);
	//	loadDateTime = nameList.at(1) + "  " + nameList.at(2);
	//}

	QString subName = subInfo->Name;
	//QString loadDateTime = "2014";
	QString loadDateTime = IEDsLoader->GetSubstationImportTime(subKey);
	//chenxin modify 2014.10.17 <---
	QTreeWidgetItem *subNameItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	subNameItem->setText(0, isDeviceOpened()?"վ����":"װ������" + subName); // ֧�ֲ鿴ICD��CID�������޸�
	subNameItem->setIcon(0, QIcon("images/regular.png"));
	subNameItem->setToolTip(0, subName);
	itemList.append(subNameItem);

	QTreeWidgetItem *fileNameItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	fileNameItem->setText(0, "�ļ�����" + subInfo->fileName);
	fileNameItem->setIcon(0, QIcon("images/regular.png"));
	fileNameItem->setToolTip(0, subInfo->fileName);
	itemList.append(fileNameItem);

	QTreeWidgetItem *verItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	verItem->setText(0, "�汾��" + subInfo->version);
	verItem->setIcon(0, QIcon("images/regular.png"));
	verItem->setToolTip(0, subInfo->version);
	itemList.append(verItem);

	QTreeWidgetItem *reVerItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	reVerItem->setText(0, "�޶��汾��" + subInfo->reversion);
	reVerItem->setIcon(0, QIcon("images/regular.png"));
	reVerItem->setToolTip(0, subInfo->reversion);
	itemList.append(reVerItem);

	QString rePersion;
	QVector<QHItemInfo> *pHitm = IEDsLoader->GetHItemList();
	for (int i = 0; i < pHitm->size(); i++)
	{
		if (pHitm->at(i).version == subInfo->version && 
			pHitm->at(i).reversion == subInfo->reversion)
		{
			rePersion = pHitm->at(i).who;
			break;
		}
	}

	QTreeWidgetItem *rePersonItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	rePersonItem->setText(0, "�޶��ˣ�" + rePersion);
	rePersonItem->setIcon(0, QIcon("images/regular.png"));
	rePersonItem->setToolTip(0, rePersion);
	itemList.append(rePersonItem);

	QTreeWidgetItem *chCRCItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	chCRCItem->setText(0, "CRCУ���룺" + subInfo->crc);
	chCRCItem->setIcon(0, QIcon("images/regular.png"));
	chCRCItem->setToolTip(0, subInfo->crc);
	itemList.append(chCRCItem);

	QTreeWidgetItem *toolIDItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	toolIDItem->setText(0, "ToolID��" + subInfo->toolID);
	toolIDItem->setIcon(0, QIcon("images/regular.png"));
	toolIDItem->setToolTip(0, subInfo->toolID);
	itemList.append(toolIDItem);

	QTreeWidgetItem *hItemItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_VERSION);
	hItemItem->setText(0, "��ʷ��Ϣ");
	hItemItem->setIcon(0, QIcon("images/info.png"));
	itemList.append(hItemItem);

	QTreeWidgetItem *xmlItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_XMLFILE);
	xmlItem->setText(0, "XMLԭ��");
	xmlItem->setIcon(0, QIcon("images/regular.png"));
	itemList.append(xmlItem);

	QTreeWidgetItem *nothingItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	nothingItem->setText(0, "");
	nothingItem->setDisabled(true);
	itemList.append(nothingItem);

	QTreeWidgetItem *loadDTItem = new QTreeWidgetItem(subInfoTreeWidget, NODE_TYPE_NOTHING);
	loadDTItem->setText(0, "����ʱ�䣺" + loadDateTime);
	loadDTItem->setIcon(0, QIcon("images/regular.png"));
	loadDTItem->setToolTip(0, loadDateTime);
	itemList.append(loadDTItem);

	subInfoTreeWidget->addTopLevelItems(itemList);
}

void GraphicBrowserFrm::InitSubnetTreeWidget()
{
	subnetTreeWidget->clear();
	QStringList lists;
	lists<<"ͨ�������б�";
	subnetTreeWidget->setHeaderLabels(lists);

	QList<QTreeWidgetItem*> itemList;
	QVector<SubNetItem> *subNets = IEDsLoader->GetSubNetList();

	QTreeWidgetItem *subnetRootItem = new QTreeWidgetItem(subnetTreeWidget, NODE_TYPE_SUBNET_ROOT);
	subnetRootItem->setText(0, "ͨ������");

	for (int i = 0; i != subNets->size(); i++)
	{
		QTreeWidgetItem *subNetItem = new QTreeWidgetItem(subnetRootItem, NODE_TYPE_SUBNET);
		subNetItem->setText(0, subNets->at(i).subNetName);
		subNetItem->setIcon(0, QIcon("images/subneticon.png"));
		itemList.append(subNetItem);
	}

	subnetRootItem->addChildren(itemList);
	subnetTreeWidget->addTopLevelItem(subnetRootItem);

	subnetRootItem->setExpanded(true);
}

void GraphicBrowserFrm::InitSCDValidatorTreeWidget()
{
	if (m_SCLValid->m_CheckResultVec.size() > 0)
	{
		scdValidatorTreeWidget->clear();
		QStringList lists;
		lists<<"У����";
		scdValidatorTreeWidget->setHeaderLabels(lists);
		QList<QTreeWidgetItem*> errorItemList;
        QList<QTreeWidgetItem*> warnItemList;
        QList<QTreeWidgetItem*> infoItemList;

		QTreeWidgetItem *scdValidatorErrorRootItem = new QTreeWidgetItem(scdValidatorTreeWidget, NODE_TYPE_SUBNET_ROOT);
		scdValidatorErrorRootItem->setText(0, "����" + QString::number(m_SCLValid->m_ErrorCount) + "����");
		QTreeWidgetItem *scdValidatorWarnRootItem = new QTreeWidgetItem(scdValidatorTreeWidget, NODE_TYPE_SUBNET_ROOT);
		scdValidatorWarnRootItem->setText(0, "���棨" + QString::number(m_SCLValid->m_WarnCount)+ "����");
        QTreeWidgetItem *scdValidatorInfoRootItem = new QTreeWidgetItem(scdValidatorTreeWidget, NODE_TYPE_SUBNET_ROOT);
        scdValidatorInfoRootItem->setText(0, "��ʾ��" + QString::number(m_SCLValid->m_InfoCount)+ "����");

        QMap<QString, QVector<int>>::iterator itItem = m_SCLValid->m_MsgMap.begin();
        while (itItem != m_SCLValid->m_MsgMap.end())
        {
            QString key = itItem.key();
            if (key.left(2) == "����")
            {
                QString content = key.mid(2) + QString("��%1����").arg(QString::number(itItem.value().size()));
                QTreeWidgetItem *errorItem = new QTreeWidgetItem(scdValidatorErrorRootItem, NODE_TYPE_VALIDATOR_SCHEMA);
                errorItem->setText(0, content);
                errorItem->setIcon(0, QIcon("images/Error.png"));
                errorItemList.append(errorItem);
            }
            else if (key.left(2) == "����")
            {
                QString content = key.mid(2) + QString("��%1����").arg(QString::number(itItem.value().size()));
                QTreeWidgetItem *warnItem = new QTreeWidgetItem(scdValidatorWarnRootItem, NODE_TYPE_VALIDATOR_SCHEMA);
                warnItem->setText(0, content);
                warnItem->setIcon(0, QIcon("images/Warn.png"));
                warnItemList.append(warnItem);
            }
            else if (key.left(2) == "��ʾ")
            {
                QString content = key.mid(2) +QString("��%1����").arg(QString::number(itItem.value().size()));
                QTreeWidgetItem *infoItem = new QTreeWidgetItem(scdValidatorInfoRootItem, NODE_TYPE_VALIDATOR_SCHEMA);
                infoItem->setText(0, content);
                infoItem->setIcon(0, QIcon("images/info.png"));
                infoItemList.append(infoItem);
            }

            itItem++;
        }
		
        scdValidatorErrorRootItem->addChildren(errorItemList);
        scdValidatorWarnRootItem->addChildren(warnItemList);
		scdValidatorInfoRootItem->addChildren(infoItemList);

        scdValidatorTreeWidget->addTopLevelItem(scdValidatorErrorRootItem);
		scdValidatorTreeWidget->addTopLevelItem(scdValidatorWarnRootItem);
        scdValidatorTreeWidget->addTopLevelItem(scdValidatorInfoRootItem);
		

		scdValidatorErrorRootItem->setExpanded(true);
		scdValidatorWarnRootItem->setExpanded(true);
        scdValidatorInfoRootItem->setExpanded(true);
	}	
}

/** 
  * @brief    ExportValidateExcel
  * 
  * ��Ӧ���������Ŀ��ť����������뵽Excel�ļ���
  * 
  * @class    GraphicBrowserFrm
  * @return   void
  * @author   zhang-xch
  * @date     2015/1/20
  * @remarks  
  */ 
void GraphicBrowserFrm::ExportValidateExcel()
{
    QString subKey = "220kV." + IEDsLoader->GetCurrentSubstationID();
    QSubStationInfo *subInfo = IEDsLoader->GetSubstationInfo(subKey);
    QString fileName = subInfo->Name + tr("-��ⱨ��");

    QString filePath = QFileDialog::getSaveFileName(NULL, "������ⱨ��", fileName, "*.xls");  
    if (!filePath.isEmpty())  
    {  
        if (m_SCLValid->ExportExcel(filePath))
        {
            QMessageBox::information(NULL, "������ⱨ��", "�����ɹ�!");
        }
        else
        {
            QMessageBox::warning(NULL, "������ⱨ��", "����ʧ��!");
        }

    }
}

void GraphicBrowserFrm::InitCompareWidget()
{
	ReInitTabDataClear();  //���tab
	SCLNameTranslator translator;
	compareModel->clear();
	QTreeWidgetItem *nothingItem, *EqualItem, *updatedItem, *addedItem, *deleteItem, *iedItem, *versionItem;
	QTableWidgetItem *itemTotalIedName,*itemTotalIedDesc,*itemTotalIedtype,*itemTotalIedmanu;
	QStringList lists;
	lists<<"����ͼ";
	compareModel->setHeaderLabels(lists);
	compareModel->header()->hide();
	//scdComparer->LoadDiffVector();
	tableModel = tableModelDiffImpl;
	QVector<QString> iedList;
	tableModel->GetIEDList("", &iedList);
	//	QVector<IEDDiffStru> diffVec = scdComparer->GetDiffVector();
	if (compareModel->topLevelItemCount() == 0)
	{
		nothingItem = new QTreeWidgetItem(compareModel,NODE_TYPE_NOTHING);
		nothingItem->setHidden(true);
		compareModel->addTopLevelItem(nothingItem);

		EqualItem   = new QTreeWidgetItem(compareModel,NODE_TYPE_WIDGETEQL);
		updatedItem = new QTreeWidgetItem(compareModel,NODE_TYPE_WIDGETUP);
		addedItem   = new QTreeWidgetItem(compareModel,NODE_TYPE_WIDGETADD);
		deleteItem  = new QTreeWidgetItem(compareModel,NODE_TYPE_WIDGETDEL);
		versionItem = new QTreeWidgetItem(compareModel,NODE_TYPE_WIDGETVer);

		int equalSum = 0;
		int updatedSum = 0;
		int addedSum = 0;
		int deleteSum = 0;
		for (int i = 0; i != iedList.size(); i++)
		{
			QString iedKey = iedList.at(i);
			IEDStru iedInfo;
 			tableModel->GetIEDInfo(iedKey, iedInfo);
			if (iedInfo.state == DIFFSTATE_EDITED)
			{
				iedItem = new QTreeWidgetItem(updatedItem, NODE_TYPE_COMPARE_IED);
				iedItem->setData(0, Qt::AccessibleTextRole, iedKey);
				//iedItem->setText(0, ie);
				QString showText="["+iedInfo.IEDName+"]"+iedInfo.IEDDesc.baseAttributeName;
				iedItem->setText(0,showText);
				iedItem->setIcon(0, QIcon("images/modify_sign.png"));
				updatedItem->addChild(iedItem);
				widgetDiffEditTable->insertRow(updatedSum);

				itemTotalIedName=new QTableWidgetItem(QIcon("./images/bianji.png"),iedInfo.IEDName);
				itemTotalIedName->setData(100,iedKey);
				itemTotalIedDesc=new QTableWidgetItem;
				itemTotalIedmanu=new QTableWidgetItem;
				itemTotalIedtype=new QTableWidgetItem;

				if(iedInfo.IEDDesc.state==DIFFSTATE_ADDED)
				{
					 itemTotalIedDesc->setText(iedInfo.IEDDesc.otherAttributeName);
					 itemTotalIedDesc->setIcon(QIcon("./images/zengjia.png"));
				}
				else if(iedInfo.IEDDesc.state==DIFFSTATE_REMOVED)
				{
					itemTotalIedDesc->setText(iedInfo.IEDDesc.baseAttributeName);
					itemTotalIedDesc->setIcon(QIcon("./images/shanchu.png"));

				}
				else if(iedInfo.IEDDesc.state==DIFFSTATE_NORMAL)
				{

					itemTotalIedDesc->setText(iedInfo.IEDDesc.baseAttributeName);
				}
				else if(iedInfo.IEDDesc.state==DIFFSTATE_EDITED)
				{
					itemTotalIedDesc->setText(iedInfo.IEDDesc.baseAttributeName);
					itemTotalIedDesc->setToolTip(iedInfo.IEDDesc.otherAttributeName);
					itemTotalIedDesc->setIcon(QIcon("./images/bianji.png"));
				}


				if(iedInfo.IEDManu.state==DIFFSTATE_ADDED)
				{
					itemTotalIedmanu->setText(iedInfo.IEDManu.otherAttributeName);
					itemTotalIedmanu->setIcon(QIcon("./images/zengjia.png"));

				}   
				else if(iedInfo.IEDManu.state==DIFFSTATE_REMOVED)
				{
					itemTotalIedmanu->setText(iedInfo.IEDManu.baseAttributeName);
					itemTotalIedmanu->setIcon(QIcon("./images/shanchu.png"));

				}
				else if(iedInfo.IEDManu.state==DIFFSTATE_NORMAL)
				{
					itemTotalIedmanu->setText(iedInfo.IEDManu.baseAttributeName);
				}
				else if(iedInfo.IEDManu.state==DIFFSTATE_EDITED)
				{
					itemTotalIedmanu->setToolTip(iedInfo.IEDManu.otherAttributeName);
					itemTotalIedmanu->setText(iedInfo.IEDManu.baseAttributeName);
					itemTotalIedmanu->setIcon(QIcon("./images/bianji.png"));
				}


				if(iedInfo.IEDType.state==DIFFSTATE_ADDED)
				{
					itemTotalIedtype->setText(iedInfo.IEDType.otherAttributeName);
					itemTotalIedtype->setIcon(QIcon("./images/zengjia.png"));

				}   
				else if(iedInfo.IEDType.state==DIFFSTATE_REMOVED)
				{
					itemTotalIedtype->setText(iedInfo.IEDType.baseAttributeName);
					itemTotalIedtype->setIcon(QIcon("./images/shanchu.png"));

				}
				else if(iedInfo.IEDType.state==DIFFSTATE_NORMAL)
				{
					itemTotalIedtype->setText(iedInfo.IEDType.baseAttributeName);
				}
				else if(iedInfo.IEDType.state==DIFFSTATE_EDITED)
				{
					itemTotalIedtype->setToolTip(iedInfo.IEDType.otherAttributeName);
					itemTotalIedtype->setText(iedInfo.IEDType.baseAttributeName);
					itemTotalIedtype->setIcon(QIcon("./images/bianji.png"));
				}

				widgetDiffEditTable->setItem(updatedSum,0,itemTotalIedName);
				widgetDiffEditTable->setItem(updatedSum,1,itemTotalIedDesc);
				widgetDiffEditTable->setItem(updatedSum,2,itemTotalIedmanu);
				widgetDiffEditTable->setItem(updatedSum,3,itemTotalIedtype);
			    updatedSum += 1;

			} 
			else if(iedInfo.state == DIFFSTATE_ADDED)
			{
				iedItem = new QTreeWidgetItem(addedItem, NODE_TYPE_COMPARE_IED);
				iedItem->setData(0, Qt::AccessibleTextRole, iedKey);
				QString showText="["+iedInfo.IEDName+"]"+iedInfo.IEDDesc.otherAttributeName;
				iedItem->setText(0, showText);
				iedItem->setIcon(0, QIcon("images/add_sign.png"));
				addedItem->addChild(iedItem);
				widgetDIffAddTable->insertRow(addedSum);
				itemTotalIedName=new QTableWidgetItem(QIcon("./images/zengjia.png"),iedInfo.IEDName);
				itemTotalIedName->setData(100,iedKey);
				itemTotalIedDesc=new QTableWidgetItem(QIcon("./images/zengjia.png"),iedInfo.IEDDesc.otherAttributeName);
				itemTotalIedmanu=new QTableWidgetItem(QIcon("./images/zengjia.png"),iedInfo.IEDManu.otherAttributeName);
				itemTotalIedtype=new QTableWidgetItem(QIcon("./images/zengjia.png"),iedInfo.IEDType.otherAttributeName);
				widgetDIffAddTable->setItem(addedSum,0,itemTotalIedName);
				widgetDIffAddTable->setItem(addedSum,1,itemTotalIedDesc);
				widgetDIffAddTable->setItem(addedSum,2,itemTotalIedmanu);
				widgetDIffAddTable->setItem(addedSum,3,itemTotalIedtype);

				addedSum += 1;
			}
			else if(iedInfo.state == DIFFSTATE_REMOVED)
			{
				iedItem = new QTreeWidgetItem(deleteItem, NODE_TYPE_COMPARE_IED);
				iedItem->setData(0, Qt::AccessibleTextRole, iedKey);
				QString showText="["+iedInfo.IEDName+"]"+iedInfo.IEDDesc.baseAttributeName;
				iedItem->setText(0, showText);
				iedItem->setIcon(0, QIcon("images/delete_sign.png"));
				deleteItem->addChild(iedItem);

				widgetDiffDelTable->insertRow(deleteSum);

				itemTotalIedName=new QTableWidgetItem(QIcon("./images/shanchu.png"),iedInfo.IEDName);
				itemTotalIedName->setData(100,iedKey);
				itemTotalIedDesc=new QTableWidgetItem(QIcon("./images/shanchu.png"),iedInfo.IEDDesc.baseAttributeName);
				itemTotalIedmanu=new QTableWidgetItem(QIcon("./images/shanchu.png"),iedInfo.IEDManu.baseAttributeName);
				itemTotalIedtype=new QTableWidgetItem(QIcon("./images/shanchu.png"),iedInfo.IEDType.baseAttributeName);
				widgetDiffDelTable->setItem(deleteSum,0,itemTotalIedName);
				widgetDiffDelTable->setItem(deleteSum,1,itemTotalIedDesc);
				widgetDiffDelTable->setItem(deleteSum,2,itemTotalIedmanu);
				widgetDiffDelTable->setItem(deleteSum,3,itemTotalIedtype);
				deleteSum += 1;
			}
			else
			{
				iedItem = new QTreeWidgetItem(EqualItem, NODE_TYPE_COMPARE_IED);
				iedItem->setData(0, Qt::AccessibleTextRole, iedKey);
				QString showText="["+iedInfo.IEDName+"]"+iedInfo.IEDDesc.baseAttributeName;
				iedItem->setText(0, showText);
				iedItem->setIcon(0, QIcon("images/equal_sign.png"));
				EqualItem->addChild(iedItem);

				widgetDiffEqlTable->insertRow(equalSum);

				itemTotalIedName=new QTableWidgetItem(iedInfo.IEDName);
				itemTotalIedName->setData(100,iedKey);
				itemTotalIedDesc=new QTableWidgetItem(iedInfo.IEDDesc.baseAttributeName);
				itemTotalIedmanu=new QTableWidgetItem(iedInfo.IEDManu.baseAttributeName);
				itemTotalIedtype=new QTableWidgetItem(iedInfo.IEDType.baseAttributeName);
				widgetDiffEqlTable->setItem(equalSum,0,itemTotalIedName);
				widgetDiffEqlTable->setItem(equalSum,1,itemTotalIedDesc);
				widgetDiffEqlTable->setItem(equalSum,2,itemTotalIedmanu);
				widgetDiffEqlTable->setItem(equalSum,3,itemTotalIedtype);
				equalSum += 1;
			} 

		}

		//for (int i = 0; i != diffVec.size(); i++)
		//{
		//	IEDDiffStru diffStru = diffVec.at(i);
		//	QStringList iedKeys;
		//	QString tempBaseKey  = diffStru.IEDKey;
		//	QString tempOtherKey = diffStru.otherIEDKey;
		//	if (tempBaseKey == "" && tempOtherKey != "")
		//	{
		//		QString tempName = translator.GetNameFromKey(tempOtherKey.toLocal8Bit().constData()).c_str();
		//		tempBaseKey = scdComparer->GetBaseSubKey() + "." + tempName;
		//	}
		//	if (tempOtherKey == "" && tempBaseKey != "")
		//	{
		//		QString tempName = translator.GetNameFromKey(tempBaseKey.toLocal8Bit().constData()).c_str();
		//		tempOtherKey = scdComparer->GetOtherSubKey() + "." + tempName;
		//	}
		//	iedKeys.append(tempBaseKey);
		//	iedKeys.append(tempOtherKey);
		//	if (diffStru.diffType == IED_update)
		//	{
		//		iedItem = new QTreeWidgetItem(updatedItem, NODE_TYPE_COMPARE_IED);
		//		iedItem->setData(0, Qt::AccessibleTextRole, iedKeys);
		//		iedItem->setText(0, diffStru.IEDDesc);
		//		iedItem->setToolTip(0, diffStru.IEDDesc);
		//		iedItem->setIcon(0, QIcon("images/modify_sign.png"));
		//		updatedItem->addChild(iedItem);
		//		updatedSum += 1;
		//	} 
		//	else if(diffStru.diffType == IED_add)
		//	{
		//		iedItem = new QTreeWidgetItem(addedItem, NODE_TYPE_COMPARE_IED);
		//		iedItem->setData(0, Qt::AccessibleTextRole, iedKeys);
		//		iedItem->setText(0, diffStru.IEDDesc);
		//		iedItem->setToolTip(0, diffStru.IEDDesc);
		//		iedItem->setIcon(0, QIcon("images/add_sign.png"));
		//		addedItem->addChild(iedItem);
		//		addedSum += 1;
		//	}
		//	else if(diffStru.diffType == IED_delete)
		//	{
		//		iedItem = new QTreeWidgetItem(deleteItem, NODE_TYPE_COMPARE_IED);
		//		iedItem->setData(0, Qt::AccessibleTextRole, iedKeys);
		//		iedItem->setText(0, diffStru.IEDDesc);
		//		iedItem->setToolTip(0, diffStru.IEDDesc);
		//		iedItem->setIcon(0, QIcon("images/delete_sign.png"));
		//		deleteItem->addChild(iedItem);
		//		deleteSum += 1;
		//	}
		//	else
		//	{
		//		iedItem = new QTreeWidgetItem(EqualItem, NODE_TYPE_COMPARE_IED);
		//		iedItem->setData(0, Qt::AccessibleTextRole, iedKeys);
		//		iedItem->setText(0, diffStru.IEDDesc);
		//		iedItem->setToolTip(0, diffStru.IEDDesc);
		//		iedItem->setIcon(0, QIcon("images/equal_sign.png"));
		//		EqualItem->addChild(iedItem);
		//		equalSum += 1;
		//	}
		//}
		EqualItem->setText(0,"δ�����IED(" + QString::number(equalSum) + ")");
		EqualItem->setIcon(0, QIcon("images/equal_sign.png"));
		updatedItem->setText(0,"�����IED(" + QString::number(updatedSum) + ")");
		updatedItem->setIcon(0, QIcon("images/modify_sign.png"));
		addedItem->setText(0,"���ӵ�IED(" + QString::number(addedSum) + ")");
		addedItem->setIcon(0, QIcon("images/add_sign.png"));
		deleteItem->setText(0,"ɾ����IED(" + QString::number(deleteSum) + ")");
		deleteItem->setIcon(0, QIcon("images/delete_sign.png"));
		versionItem->setText(0,"�ļ���Ϣ");

	    compareModel->addTopLevelItem(EqualItem);
		compareModel->addTopLevelItem(updatedItem);
		compareModel->addTopLevelItem(addedItem);
	    compareModel->addTopLevelItem(deleteItem);

		if(checkFlag)
		{
			EqualItem->setHidden(false);
			updatedItem->setHidden(false);
			addedItem->setHidden(false);
			deleteItem->setHidden(false);
		}else
		{
			EqualItem->setHidden(true);
			updatedItem->setHidden(true);
			addedItem->setHidden(true);
			deleteItem->setHidden(true);
		}

		if (fileInfoSlectFlag)
		{
			versionItem->setHidden(false);
		}else
		{
			versionItem->setHidden(true);
		}

		compareModel->addTopLevelItem(versionItem);
	}
	initDiffWidgetVer(versionItem);
	connect(compareModel,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		this,SLOT(SelectCompareTreeProc(QTreeWidgetItem*)));
	compareModel->setCurrentItem(NULL);
}
void   GraphicBrowserFrm::initDiffWidgetVer(QTreeWidgetItem *item)
{
	 bool change=false;
	 FileInfoStru infoBase;
	 FileInfoStru infoOther;
	 tableModel->GetBaseFileInfo(infoBase);
	 tableModel->GetOtherFileInfo(infoOther);

	 widgetDiffVerTable->clear();
	 widgetDiffVerTable->setRowCount(0);
	 QStringList  str;
	 str<<"�ļ�����"/*<<"����ʱ��"*/<<"�汾"<<"�޶��汾"<<"CRCУ����";
	 widgetDiffVerTable->setColumnCount(4);
	 widgetDiffVerTable->setHorizontalHeaderLabels(str);

	 widgetDiffVerTable->insertRow(0); //��׼�ļ�
	 widgetDiffVerTable->setItem(0,0,new QTableWidgetItem(infoBase.basefileName));
	// widgetDiffVerTable->setItem(0,1,new QTableWidgetItem(infoBase.basefileTime));
	 widgetDiffVerTable->setItem(0,1,new QTableWidgetItem(infoBase.version.baseAttributeName));
	 widgetDiffVerTable->setItem(0,2,new QTableWidgetItem(infoBase.reVersion.baseAttributeName));
	 widgetDiffVerTable->setItem(0,3,new QTableWidgetItem(infoBase.crc.baseAttributeName));

	 widgetDiffVerTable->insertRow(1); //��׼�ļ�
	 widgetDiffVerTable->setItem(1,0,new QTableWidgetItem(infoOther.otherfileName));
	 // widgetDiffVerTable->setItem(1,1,new QTableWidgetItem(infoOther.otherfileTime));
	 widgetDiffVerTable->setItem(1,1,new QTableWidgetItem(infoOther.version.otherAttributeName));
	 widgetDiffVerTable->setItem(1,2,new QTableWidgetItem(infoOther.reVersion.otherAttributeName));
	 widgetDiffVerTable->setItem(1,3,new QTableWidgetItem(infoOther.crc.otherAttributeName));

	 if(widgetDiffVerTable->item(0,1)->text()!=widgetDiffVerTable->item(1,1)->text())
	 {
		 widgetDiffVerTable->horizontalHeaderItem(1)->setIcon(QIcon("./images/bianji.png"));
		  change=true;
	 }
	 if(widgetDiffVerTable->item(0,2)->text()!=widgetDiffVerTable->item(1,2)->text())
	 {
		 widgetDiffVerTable->horizontalHeaderItem(2)->setIcon(QIcon("./images/bianji.png"));
		  change=true;
	 }
	 if(widgetDiffVerTable->item(0,3)->text()!=widgetDiffVerTable->item(1,3)->text())
	 {
		 widgetDiffVerTable->horizontalHeaderItem(3)->setIcon(QIcon("./images/bianji.png"));
		 change=true;
	 }
	 if(change)
		 item->setIcon(0,QIcon("images/modify_sign.png"));
	 else
	     item->setIcon(0,QIcon("images/equal_sign.png"));

}

void GraphicBrowserFrm::SelectTreeProc(QTreeWidgetItem* item)
{
	if (!item)
		return;
	int type = item->type();
	actionSaveAs->setEnabled(false);
	//QRectF itemsRect;
	//qreal x,y;
	switch (item->type())
	{
	case NODE_TYPE_NOTHING:

		//-----------------��ʱ������-----------------
		//graphicsManger->LoadGraphicData(IEDsLoader->GetSubNetItemFromSubNetName("���̲�A��").subNetKey,"",GRAPHIC_TYPE_NO_SUBNET);
		//UpdateGraphicsView();
		//RecoveryProc();
		//-----------------��ʱ������-----------------

		break;
	case NODE_TYPE_VERSION:
		//graphicOpersEnable(false);
		//versionWin->show();
		PreNextFlagChange();
		setCenterLayer(NODE_TYPE_VERSION);
		//graphicsManger->LoadGraphicData(IEDsLoader->GetIEDItemFromIEDName(item->text(0)).IEDKey,"",GRAPHIC_TYPE_NO_VIRTUAL_LINK);
		//UpdateGraphicsView();
		break;
	case NODE_TYPE_IED_ROOT:
		setCenterLayer(SHOW_ToTal_IED);
		break;
	case NODE_TYPE_SUBNET_ROOT:

		//graphicOpersEnable(false);
		//UpdateGraphicsView();
		//if (graphicScene)
		//	graphicScene->clear();
		//graphicsManger->LoadGraphicData(IEDsLoader->GetIEDItemFromIEDName(item->text(0)).IEDKey,"",GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL);
		//UpdateGraphicsView();
		//return;
		break;
	case NODE_TYPE_SUBNET:
		//graphicOpersEnable(true);
		//InitGraphicsView();
		//graphicsManger->LoadGraphicData(IEDsLoader->GetSubNetItemFromSubNetName(item->text(0)).subNetKey,"",GRAPHIC_TYPE_NO_SUBNET);
		//graphicScene->DrawSubnetProc(IEDsLoader->GetSubNetItemFromSubNetName(item->text(0)).subNetKey);
		//UpdateGraphicsView();

		//CenterAllItems(itemsRect);
		//x = itemsRect.center().x();
		//y = itemsRect.center().y();
		//if (graphicsView == NULL)
		//{
		//	return;
		//}
		//graphicsView->centerOn(x,y);

		//RecoveryProc();
		break;
	case NODE_TYPE_IED:
		//�ر�δ�������·ͼ
		if (vDialog->GetDialogLocked())
		{
			if (!lockedDialogVec.contains(vDialog))
			{
				lockedDialogVec.append(vDialog);
			}
		}
		else
		{
			vDialog->close();
		}
		closeUnlockedVirDlg();

		totalTab->setIEDKey(item->data(0,Qt::AccessibleTextRole).toString());
		setCenterLayer(SHOW_INFO_TABLE);
		//totalTab->setTabEnabled(totalTab->indexOf(gooseRecvWidget),false);
		//totalTab->setTabEnabled(totalTab->indexOf(svRecvWidget),false);
		actionPre->setEnabled(PreFlag);
		actionNext->setEnabled(NextFlag);
		graphicOpersEnable(true);
		graphicsManger->LoadGraphicData(item->data(0,Qt::AccessibleTextRole).toString(),"",GRAPHIC_TYPE_NO_INFOMATION_FLOW);
		graphicsViewDockWidget->setWindowTitle("��ǰװ�� : " + item->text(0));
		UpdateGraphicsView();
		RecoveryProc();

		//fillTabContent(item->data(0,Qt::AccessibleTextRole).toString());

		setTotalTabsEnabled(item->data(0,Qt::AccessibleTextRole).toString());
		totalTabChangeProc(totalTab->currentIndex());
		rightTabChangeProc(rightTab->currentIndex());
		bottmTabChangeProc(bottmTab->currentIndex());
		if(!graphicsManger->IsFirstGraphic())
		{
			actionPre->setEnabled(true);
		}

//        fillXmlData(item->data(0,Qt::AccessibleTextRole).toString());

		break;
	}
	//RecoveryProc();
}

void GraphicBrowserFrm::SelectDeviceProc(const QModelIndex& index)
{
	QStandardItem *tempItem = deviceModel->itemFromIndex(index);
	if (!tempItem)
	{
		return;
	}
	QString IEDKey = tempItem->data(Qt::WhatsThisRole).toString();
	if (!IEDKey.isEmpty())
	{
		if (vDialog->GetDialogLocked())
		{
			if (!lockedDialogVec.contains(vDialog))
			{
				lockedDialogVec.append(vDialog);
			}
		}
		else
		{
			vDialog->close();
		}
		closeUnlockedVirDlg();

		totalTab->setIEDKey(IEDKey);
		setCenterLayer(SHOW_INFO_TABLE);
		//totalTab->setTabEnabled(totalTab->indexOf(gooseRecvWidget),false);
		//totalTab->setTabEnabled(totalTab->indexOf(svRecvWidget),false);
		actionPre->setEnabled(PreFlag);
		actionNext->setEnabled(NextFlag);
		graphicOpersEnable(true);
		graphicsManger->LoadGraphicData(IEDKey,"",GRAPHIC_TYPE_NO_INFOMATION_FLOW);
		graphicsViewDockWidget->setWindowTitle("��ǰװ�� : " + tempItem->text());
		UpdateGraphicsView();
		RecoveryProc();

		//fillTabContent(IEDKey);

		setTotalTabsEnabled(IEDKey);
		totalTabChangeProc(totalTab->currentIndex());
		rightTabChangeProc(rightTab->currentIndex());
		bottmTabChangeProc(bottmTab->currentIndex());
		if(!graphicsManger->IsFirstGraphic())
		{
			actionPre->setEnabled(true);
		}
       //  fillXmlData(IEDKey);
	}
}

void GraphicBrowserFrm::SelectSubInfoTreePorc(QTreeWidgetItem *item)
{
	if (!item)
		return;
	int type = item->type();
	//QRectF itemsRect;
	//qreal x,y;
	actionSaveAs->setEnabled(false);
	QString subStationKey = QString("220kV.") + IEDsLoader->GetCurrentSubstationID();
	switch (item->type())
	{
	case NODE_TYPE_NOTHING:
		graphicsViewDockWidget->setWindowTitle(item->text(0));
		break;
	case NODE_TYPE_VERSION:
		if (vDialog->GetDialogLocked())
		{
			if (!lockedDialogVec.contains(vDialog))
			{
				lockedDialogVec.append(vDialog);
			}
		}
		else
		{
			vDialog->close();
		}
		closeUnlockedVirDlg();
		PreNextFlagChange();
		setCenterLayer(BASIC_INFO);
		graphicsViewDockWidget->setWindowTitle(item->text(0));
		break;
	case NODE_TYPE_XMLFILE:
		if (vDialog->GetDialogLocked())
		{
			if (!lockedDialogVec.contains(vDialog))
			{
				lockedDialogVec.append(vDialog);
			}
		}
		else
		{
			vDialog->close();
		}
		closeUnlockedVirDlg();
		PreNextFlagChange();
		fillPrivateHeaderXmlData(subStationKey);
		setCenterLayer(NODE_TYPE_XMLFILE);
		graphicsViewDockWidget->setWindowTitle(item->text(0));
		break;
	default:
		break;
	}
}

void GraphicBrowserFrm::SelectSubNetTreeProc(QTreeWidgetItem *item)
{
	if (!item)
		return;
	int type = item->type();
	actionSaveAs->setEnabled(false);
	QString tempSubNetKey = "";
	switch (item->type())
	{
	case NODE_TYPE_NOTHING:
		break;
	case NODE_TYPE_SUBNET_ROOT:
		break;
	case NODE_TYPE_SUBNET:
		if (vDialog->GetDialogLocked())
		{
			if (!lockedDialogVec.contains(vDialog))
			{
				lockedDialogVec.append(vDialog);
			}
		}
		else
		{
			vDialog->close();
		}
		closeUnlockedVirDlg();
		actionSaveAs->setEnabled(true);
		tempSubNetKey = IEDsLoader->GetSubNetItemFromSubNetName(item->text(0)).subNetKey;
		totalTab->setIEDKey(tempSubNetKey);
		fillSubNetXmlData(tempSubNetKey);
		setCenterLayer(NODE_TYPE_SUBNET);
		//graphicsManger->LoadGraphicData(IEDsLoader->GetSubNetItemFromSubNetName("���̲�A��").subNetKey,"",GRAPHIC_TYPE_NO_SUBNET);
		graphicsManger->LoadGraphicData(tempSubNetKey,"",GRAPHIC_TYPE_NO_SUBNET);
		UpdateGraphicsView();
		RecoveryProc();
		if(!graphicsManger->IsFirstGraphic())
		{
			actionPre->setEnabled(true);
		}
		break;
	default:
		break;
	}
	graphicsViewDockWidget->setWindowTitle(item->text(0));
}

void GraphicBrowserFrm::SelectSCDValidatorTreeProc(QTreeWidgetItem* item)
{
	if (!item)
		return;
    if (item->parent() == NULL)
    {
        return;
    }

    currentValidatorItemInfo = item->parent()->text(0).mid(0, 2) + item->text(0).left(item->text(0).indexOf("��"));
    InitSCDValidatorView();
    totalTab->clear();
    totalTab->tabbarhide();
    totalTab->addTab(scdValidatorView,"validator");
    graphicsViewDockWidget->setWindowTitle("SCDУ��");

	if (vDialog->GetDialogLocked())
	{
		if (!lockedDialogVec.contains(vDialog))
		{
			lockedDialogVec.append(vDialog);
		}
	}
	else
	{
		vDialog->close();
	}
	closeUnlockedVirDlg();

	PreNextFlagChange();

}

void   GraphicBrowserFrm::PreNextFlagChange()
{
      if(actionPre->isEnabled())
	      PreFlag=true;
	  else 
		  PreFlag=false;

	  if(actionNext->isEnabled())
		  NextFlag=true;
	  else
		  NextFlag=false;

	  actionPre->setEnabled(false);
	  actionNext->setEnabled(false);
}
void    GraphicBrowserFrm::fillTabContentDiff(QString Iedkey)
{
	TableModelDiffImpl  diffSCLImpl;
	QVector<QString> inSignal;
	QVector<ExtRefStru> ExtRefs;
	diffSCLImpl.GetInputsOfIED(Iedkey,ExtRefs);

	gooseRecvWidget->GetExtRefStruInfo(ExtRefs,1);  // 1 ������컯   /
	svRecvWidget->GetExtRefStruInfo(ExtRefs,1);

	QVector<QString> ctrlBlockList;
	diffSCLImpl.GetGooseCtrlBlockListByIED(Iedkey,&ctrlBlockList); //���googse ����
	gooseSendWidget->setIEDKey(Iedkey);
	gooseSendWidget->GetGooseCtrIndex(ctrlBlockList,0);

	if(ctrlBlockList.count()==0)
		totalTab->setTabEnabled(totalTab->indexOf(gooseSendWidget),false);
	else 
		totalTab->setTabEnabled(totalTab->indexOf(gooseSendWidget),true);

	ctrlBlockList.clear();
	diffSCLImpl.GetSVCtrlBlockListByIED(Iedkey,&ctrlBlockList);
	svSendWidget->setIEDKey(Iedkey);
	svSendWidget->GetSvCtrIndex(ctrlBlockList,0);

	if(ctrlBlockList.count()==0)
		totalTab->setTabEnabled(totalTab->indexOf(svSendWidget),false);
	else 
		totalTab->setTabEnabled(totalTab->indexOf(svSendWidget),true);
	subSetting->sync();
	if (subSetting->value("config/IEDXMLCompare").toBool())
	{
		fillXmlData(Iedkey);
	}	
}

void    GraphicBrowserFrm::fillXmlData(QString Iedkey)
{
    SCDDiffCompare *pIns = SCDDiffCompare::GetInstance();

    int Cnt = Iedkey.split(".").size();
    if (Cnt > 2)
    {
        QString xmlData = SCLModelAdapter::GetInstance()->GetIEDXMLInfo(Iedkey);
        xmlViewer->SetXmlData(xmlData);
    }
    else
    {
        QString baseIedKey = pIns->m_BaseSubStationKey + "." + Iedkey;
        QString otherIedKey = pIns->m_OtherSubStationKey + "." + Iedkey;

        QString baseXml = SCLModelAdapter::GetInstance()->GetIEDXMLInfo(baseIedKey);
        QString otherXml = SCLModelAdapter::GetInstance()->GetIEDXMLInfo(otherIedKey);

        if (!baseXml.isEmpty() && !otherXml.isEmpty())
            xmlViewer->SetCmpXmlData(baseXml, otherXml);
        else if (!baseXml.isEmpty())
            xmlViewer->SetXmlData(baseXml);
        else if (!otherXml.isEmpty())
            xmlViewer->SetXmlData(otherXml);
    }
}

void GraphicBrowserFrm::fillSubNetXmlData(const QString& subNetKey)
{
	QString xmlData = SCLModelAdapter::GetInstance()->GetSubNetXMLInfo(subNetKey);
	xmlViewer->SetXmlData(xmlData);
}

void GraphicBrowserFrm::fillPrivateHeaderXmlData(const QString& substationKey)
{
	QString priXmlData = SCLModelAdapter::GetInstance()->GetSubstationPrivateXMLInfo(substationKey);
	QString hdrXmlData = SCLModelAdapter::GetInstance()->GetSubstationHeaderXMLInfo(substationKey);
	QString subXmlData = "";
	if (priXmlData.isEmpty())
	{
		subXmlData = hdrXmlData;
	}
	else
	{
		subXmlData = priXmlData + "\n" + hdrXmlData;
	}
	xmlViewer->SetXmlData(subXmlData);
}

void GraphicBrowserFrm::fillTabContent(MyTabWidget* myTabWidget, QString IedKey, int tabIndex)
{
	if (IedKey.compare("") == 0 || tabIndex == 0)
	{
		return;
	}

	int currentIndex = graphicsManger->GetCurGraphicIndex();
	CTableSCLModelImpl tableSCLImpl;
	QVector<QString> ctrlBlockList;

	if (3 == tabIndex)		//GOOSE����;
	{
		ctrlBlockList.clear();
		tableSCLImpl.GetGooseCtrlBlockListByIED(IedKey,&ctrlBlockList);
		if(ctrlBlockList.count()==0)
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(gooseSendWidget),false);
		}
		else 
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(gooseSendWidget),true);
		}
		gooseSendWidget->GetGooseCtrIndex(ctrlBlockList,1);
		}
	else if (1 == tabIndex)		//SV����;
	{
		ctrlBlockList.clear();
		tableSCLImpl.GetSVCtrlBlockListByIED(IedKey,&ctrlBlockList);
		svSendWidget->GetSvCtrIndex(ctrlBlockList,1);
		if(ctrlBlockList.count()==0)
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(svSendWidget),false);
		}
		else
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(svSendWidget),true);
		}
	}
	else if (5 == tabIndex)		//Report����;
	{
		ctrlBlockList.clear();
		tableSCLImpl.GetReportCtrlBlockListByIED(IedKey,&ctrlBlockList);
		if(ctrlBlockList.count()==0)
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(reportSendWidget),false);
		}
		else
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(reportSendWidget),true);
		}
		reportSendWidget->setIEDKey(IedKey);
		reportSendWidget->GetReportCtrIndex(ctrlBlockList,1);

	}
	else if (6 == tabIndex)		//��ֵ��Ŀ ����;
	{
		QVector<QDataSetInfo> dataSetList;
		tableSCLImpl.GetDsSettingListByIED(IedKey,&dataSetList);
		if(dataSetList.count()==0)
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(dsSettingWidget),false);
		}
		else
		{
			myTabWidget->setTabEnabled(myTabWidget->indexOf(dsSettingWidget),true);
		}
		dsSettingWidget->setIEDKey(IedKey);
		dsSettingWidget->GetDsSettingIndex(dataSetList,1);
		
	}
	else if (4 == tabIndex)		//GOOSE����;
	{
		QVector<QString> inSignal;
		QVector<ExtRefStru> ExtRefs;
		tableSCLImpl.GetInputsOfIED(IedKey,ExtRefs);
		gooseRecvWidget->GetExtRefStruInfo(ExtRefs,0,currentIndex);
	}
	else if (2 == tabIndex)		//SV����;
	{
		QVector<QString> inSignal;
		QVector<ExtRefStru> ExtRefs;
		tableSCLImpl.GetInputsOfIED(IedKey,ExtRefs);
		svRecvWidget->GetExtRefStruInfo(ExtRefs,0);
	}
	else if (7 == tabIndex)
	{
		fillXmlData(IedKey);
	}
    else if (8 == tabIndex)
    {
        QString iedName;
        std::vector<string> keyList = Split(IedKey.toStdString(),".");
        int strNum = keyList.size();
        if(strNum < 4)
        {
            return;
        }
        
        iedName = keyList[3].c_str();
        QList<EquipmentInfo> eqList = m_SSDInfo->m_IEDEquipmentMap.values(iedName);
        if (eqList.size() == 0 && totalTab->tabText(8) == "һ���豸")
        {
            totalTab->removeTab(8);
        }
        widgetIEDSSDTable->clear();
        widgetIEDSSDTable->setRowCount(0);
        widgetIEDSSDTable->setHorizontalHeaderLabels(m_IEDSSDTableHeader);
        for (int i = 0; i < eqList.size(); i++)
        {
            EquipmentInfo temp = eqList.at(i);

            QTableWidgetItem *eqNameItem = new QTableWidgetItem(temp.eqipmentName);
            QTableWidgetItem *eqDescItem = new QTableWidgetItem(temp.eqipmentDesc);
            QTableWidgetItem *volItem = new QTableWidgetItem(temp.voltageLevel);
            QTableWidgetItem *bayItem = new QTableWidgetItem(temp.bayInfo);

            widgetIEDSSDTable->insertRow(i);
            widgetIEDSSDTable->setItem(i, 0, eqNameItem);
            widgetIEDSSDTable->setItem(i, 1, eqDescItem);
            widgetIEDSSDTable->setItem(i, 2, volItem);
            widgetIEDSSDTable->setItem(i, 3, bayItem);
        }
        widgetIEDSSDTable->resizeColumnsToContents();

    }
}

void GraphicBrowserFrm::setTotalTabsEnabled(const QString& IEDKey)
{
	QIEDInfo iedInfo;
	graphicScene->GetSclModel()->GetIEDInfo(IEDKey, iedInfo);
	if (iedInfo.GooseInSigCount == 0)
	{
		totalTab->setTabEnabled(getTabIndexByText("GOOSE����"),false);
		rightTab->setTabEnabled(getTabIndexByText("GOOSE����"),false);
		bottmTab->setTabEnabled(getTabIndexByText("GOOSE����"),false);
	}
	if (iedInfo.GooseOutSigCount == 0)
	{
		totalTab->setTabEnabled(getTabIndexByText("GOOSE����"),false);
		rightTab->setTabEnabled(getTabIndexByText("GOOSE����"),false);
		bottmTab->setTabEnabled(getTabIndexByText("GOOSE����"),false);
	}
	if (iedInfo.SVInSigCount == 0)
	{
		totalTab->setTabEnabled(getTabIndexByText("SV����"),false);
		rightTab->setTabEnabled(getTabIndexByText("SV����"),false);
		bottmTab->setTabEnabled(getTabIndexByText("SV����"),false);
	}
	if (iedInfo.SVOutSigCount == 0)
	{
		totalTab->setTabEnabled(getTabIndexByText("SV����"),false);
		rightTab->setTabEnabled(getTabIndexByText("SV����"),false);
		bottmTab->setTabEnabled(getTabIndexByText("SV����"),false);
	}
	if (iedInfo.ReportCBCount == 0)
	{
		totalTab->setTabEnabled(getTabIndexByText("��Ϣ���"),false);
		rightTab->setTabEnabled(getTabIndexByText("��Ϣ���"),false);
		bottmTab->setTabEnabled(getTabIndexByText("��Ϣ���"),false);
	}
	if (iedInfo.DsSettingCount == 0)
	{
		totalTab->setTabEnabled(getTabIndexByText("��ֵ��Ŀ"),false);
		rightTab->setTabEnabled(getTabIndexByText("��ֵ��Ŀ"),false);
		bottmTab->setTabEnabled(getTabIndexByText("��ֵ��Ŀ"),false);
	}
}

int GraphicBrowserFrm::getTabIndexByText(const QString& tabTxt)
{
	for (uint i = 0; i != 8; i++)
	{
		QString tempTxt = totalTab->tabText(i);
		if (tempTxt.compare(tabTxt) == 0)
		{
			return i;
		}
	}
	return -1;
}

void GraphicBrowserFrm::SelectCompareTreeProc(QTreeWidgetItem *item)
{
	if (!item)
		return;
	actionSaveAs->setEnabled(false);
	if (item->type() == NODE_TYPE_WIDGETADD)
	{
		setCenterLayer(NODE_TYPE_WIDGETADD);
	}
	else if (item->type() == NODE_TYPE_WIDGETDEL)
	{
		setCenterLayer(NODE_TYPE_WIDGETDEL);
	}
	else if (item->type() == NODE_TYPE_WIDGETEQL)
	{
		setCenterLayer(NODE_TYPE_WIDGETEQL);
	}
	else if (item->type() == NODE_TYPE_WIDGETUP)
	{
		setCenterLayer(NODE_TYPE_WIDGETUP);
	}
	else if(item->type() == NODE_TYPE_WIDGETVer)
	{
	     setCenterLayer(NODE_TYPE_WIDGETVer);
	}
	else
	{
		//totalTab->setIEDKey(item->data(0,Qt::AccessibleTextRole).toString());
		//setCenterLayer(SHOW_INFO_TABLE);
		setCenterLayer(NODE_TYPE_COMPARE_IED);
		graphicOpersEnable(true);
		/*graphicsManger->LoadGraphicData(item->data(0,Qt::AccessibleTextRole).toStringList().at(0),
			item->data(0,Qt::AccessibleTextRole).toStringList().at(1),GRAPHIC_TYPE_NO_COMPARE);*/
		graphicsManger->LoadGraphicData(item->data(0,Qt::AccessibleTextRole).toString(),"",GRAPHIC_TYPE_NO_COMPARE);
		graphicsViewDockWidget->setWindowTitle("��ǰװ�� : " + item->text(0));
		UpdateGraphicsView();
		RecoveryProc();
		fillTabContentDiff(item->data(0,Qt::AccessibleTextRole).toString());
       // fillXmlData(item->data(0,Qt::AccessibleTextRole).toString());
	}
	if(!graphicsManger->IsFirstGraphic())
	{
		actionPre->setEnabled(true);
	}
}

void GraphicBrowserFrm::InitVoltageLevelCombBox()
{
	for(int i=0;i<IEDsLoader->GetVoltageLevelNameList()->size();i++)
	{
		comboVoltageLevel->addItem(IEDsLoader->GetVoltageLevelNameList()->at(i));
	}
	connect(comboVoltageLevel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(VoltageLevelChangedProc(int)));
}

void GraphicBrowserFrm::InitSubstationCombBox()
{
	for(int i=0;i<IEDsLoader->GetSubstationNameList()->size();i++)
	{
		comboSubStation->addItem(IEDsLoader->GetSubstationNameList()->at(i));
	}
	connect(comboSubStation, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SubStationChangedProc(int)));
}

void GraphicBrowserFrm::InitGraphicsView()
{
	logsrv_scoped_output(logsrv, "����InitGraphicsView����", "�˳�InitGraphicsView", "GraphicBrowserFrm::InitGraphicsView");
	QPrinter *printer = new QPrinter;
	QSizeF size = printer->paperSize(QPrinter::DevicePixel);
	//graphicScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size.width(), size.height()),this);
	graphicScene = new MyHmiGraphicsScene(NULL,QRectF(0, 0, 2400, 1500),this);
	graphicScene->setBackgroundBrush(QBrush(QColor(255,255,255)));
	graphicScene->setDrag(false);
	connect(graphicScene, SIGNAL(tripClicked(QString,QString,int)),
		this, SLOT(tripClickedProc(QString,QString,int)));
	delete printer;

	// logsrv_output_info(logsrv, L"new MyHmiGraphicsScene���", L"GraphicBrowserFrm::GraphicBrowserFrm");

	graphicsView = new HmiGraphicsView(this);
	graphicsView->setScene(graphicScene);
	graphicsView->setDragMode(QGraphicsView::NoDrag);
	graphicsView->setContextMenuPolicy(Qt::ActionsContextMenu);
	graphicsView->setBackgroundBrush(QBrush(QColor(255,255,255)));
	graphicsView->setToolTip("");

	// logsrv_output_info(logsrv, L"new HmiGraphicsView���", L"GraphicBrowserFrm::GraphicBrowserFrm");
	initTab();
}


void GraphicBrowserFrm::UpdateGraphicsView()
{
	//if(graphicsManger->GetCurGraphicIndex()>=0)
	if (graphicsManger->GetGraphicCount() > 0)
	{
		QByteArray graphicData = graphicsManger->GetCurGraphicData();
		QDataStream stream(&graphicData, QIODevice::ReadWrite);
		graphicScene->clear();
		QRectF rect = graphicScene->load(stream);
		graphicScene->setSceneRect(rect);

		double newScale = graphicsView->width() / rect.width() * graphicsManger->GetCurGraphicScale();
		double newScale1 = graphicsView->height() / rect.width() * graphicsManger->GetCurGraphicScale();
		QMatrix oldMatrix = graphicsView->matrix();
		graphicsView->resetMatrix();
		graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
		graphicsView->scale(qMin(newScale, newScale1),qMin(newScale, newScale1));
		//graphicsView->scroll(0,0);
	}
}

void GraphicBrowserFrm::CenterAllItems(QRectF &rect)
{
	if (graphicScene == NULL)
	{
		return;
	}
	QList<QGraphicsItem *> list = graphicScene->items();
	QList<QGraphicsItem *>::const_iterator it;

	rect.setLeft(9999999);
	rect.setTop(9999999);
	rect.setRight(-999999);
	rect.setBottom(-999999);
	for (it = list.begin();it != list.end(); ++it)
	{
		QGraphicsItem *item = *it;
		QRectF rectCell = item->sceneBoundingRect();
		if (rect.left() > rectCell.left())
			rect.setLeft(rectCell.left());
		if (rect.top() > rectCell.top())
			rect.setTop(rectCell.top());
		if (rect.right() < rectCell.right())
			rect.setRight(rectCell.right());
		if (rect.bottom() < rectCell.bottom())
			rect.setBottom(rectCell.bottom());
	}
}

void GraphicBrowserFrm::InitDeviceView()
{
	QVector<QString> *substationList = IEDsLoader->GetSubstationNameList();
	if (substationList->size() == 0)
	{
		deviceModel->clear();
		return;
	}
	//QString panelKey = "220kV." + substationList->at(IEDsLoader->GetCurrentSubstationIndex()) + ".����.";
	QString panelKey = "220kV." + IEDsLoader->GetCurrentSubstationID() + ".����.";

	IEDsLoader->LoadIEDInfoList();
	QVector<QIEDInfo> *infoList = IEDsLoader->GetIEDInfoList();
	if (infoList->isEmpty())
	{
		deviceModel->clear();
		return;
	}

	QIEDInfo iedInfo;
	QMap<QString,QIEDInfo> iedmap;
	for (int i = 0; i < infoList->size(); i++)
	{
		iedInfo = infoList->at(i);
		QString fullstr = iedInfo.manu + iedInfo.type + iedInfo.name;
		iedmap.insert(fullstr,iedInfo);
	}
	QList<QString> mapKeys = iedmap.keys();

	//����;
	int count  = mapKeys.size();
	int index = 0;
	for (int i = 0; i < count - 1; i++)
	{
		index = i;
		QString sName = mapKeys.at(i);
		for (int j = i + 1; j < count; j++)
		{
			QString dName = mapKeys.at(j);
			//if (strncmp(sName.toLocal8Bit().constData(),dName.toLocal8Bit().constData(),256)>0)
			if (sName.compare(dName) > 0)
			{
				index = j;
				sName = dName;
			}
		}
		if (index != i)
		{
			mapKeys.swap(i,index);
		}
	}
	//�������;

	QString facTemp = "BeginFactory";
	QString typeTemp = "BeginType";

	deviceModel->clear();

	int itemNum = 0;
	int facIndexNum = 0;
	int typeIndexNum = 0;
	int nameIndexNum = 0;
	int typeSum = 0;
	int nameSum = 0;
	int noFacNameSum = 0;
	QStandardItem *facItem = NULL;
	QStandardItem *typeItem = NULL;
	QStandardItem *nameItem = NULL;
	QStandardItem *noFacItem = NULL;
	for (int j = 0; j < mapKeys.size(); j++)
	{
		iedInfo = iedmap.value(mapKeys.at(j));
		QModelIndex facIndex;
		QModelIndex typeIndex;
		QModelIndex nameIndex;
		QModelIndex noFacIndex;
		if (iedInfo.manu.compare(facTemp) != 0)
		{
			if (itemNum != 0)
			{
				facIndex = deviceModel->indexFromItem(facItem);
				typeIndex = deviceModel->indexFromItem(typeItem);
				noFacIndex = deviceModel->indexFromItem(noFacItem);

				QString strFacInfo, strTypeInfo;

				if (typeTemp.compare("") == 0)
				{
					strTypeInfo = tr("δ֪�ͺţ�") + QString::number(nameIndexNum) + tr("̨��");
				}
				else
				{
					strTypeInfo = typeTemp + tr("��") + QString::number(nameIndexNum) + tr("̨��");
				}

				if (facTemp.compare("") == 0)
				{
					strFacInfo = tr("δ֪���̣�") + QString::number(typeIndexNum) + tr("���ͺţ��漰") + QString::number(nameSum) + "̨װ�ã�";
					//noFacNameSum++;
					deviceModel->setData(noFacIndex,strFacInfo);
				}
				else
				{
					strFacInfo = facTemp + tr("��") + QString::number(typeIndexNum) + tr("���ͺţ��漰") + QString::number(nameSum) + "̨װ�ã�";
					deviceModel->setData(facIndex,strFacInfo);
				}
				deviceModel->setData(typeIndex,strTypeInfo);
			}

			if (/*itemNum != 0 && */iedInfo.manu.compare("") == 0)
			{
				if (noFacItem == NULL)
				{
					noFacItem = new QStandardItem(tr("δ֪����"));
				}
				facTemp = "";
				noFacNameSum++;
			}
			else
			{
				facTemp = iedInfo.manu;
				facItem = new QStandardItem(facTemp);
			}
			if (facItem != NULL)
			{
				facItem->setIcon(QIcon("images/manufactory.png"));
			}

			if (iedInfo.type.compare("") == 0)
			{
				typeItem = new QStandardItem(tr("δ֪�ͺ�"));
				typeTemp = "";
			}
			else
			{
				typeTemp = iedInfo.type;
				typeItem = new QStandardItem(typeTemp);
			}
			if (typeItem != NULL)
			{
				typeItem->setIcon(QIcon("images/type_folder.png"));
			}

			typeSum += typeIndexNum;
			typeIndexNum = 1;
			nameIndexNum = 1;
			nameSum = 1;

			QString nameDesc = QString("[") + iedInfo.name + QString("]") + iedInfo.desc;
			nameItem = new QStandardItem(nameDesc);
			nameItem->setData(panelKey + iedInfo.name, Qt::WhatsThisRole);
			nameItem->setIcon(QIcon("images/graphic_two.png"));
			if (facTemp.compare("") == 0)
			{
				//deviceModel->appendRow(noFacItem);
				noFacItem->appendRow(typeItem);
				typeItem->appendRow(nameItem);
			}
			else
			{
				deviceModel->appendRow(facItem);
				facItem->appendRow(typeItem);
				typeItem->appendRow(nameItem);
			}

			facIndexNum++;
			itemNum = itemNum + 3;
		}
		else	//iedInfo.manu.compare(facTemp) == 0
		{
			if (iedInfo.type.compare(typeTemp) != 0)
			{
				typeIndex = deviceModel->indexFromItem(typeItem);
				QString strTypeInfo = typeTemp + tr("��") + QString::number(nameIndexNum) + tr("̨��");
				deviceModel->setData(typeIndex,strTypeInfo);

				if (iedInfo.type.compare("") == 0)
				{
					typeItem = new QStandardItem(tr("δ֪�ͺ�"));
					typeTemp = "";
				}
				else
				{
					typeTemp = iedInfo.type;
					typeItem = new QStandardItem(typeTemp);
				}
				typeItem->setIcon(QIcon("images/type_folder.png"));

				QString nameDesc = QString("[") + iedInfo.name + QString("]") + iedInfo.desc;
				nameItem = new QStandardItem(nameDesc);
				nameItem->setData(panelKey + iedInfo.name, Qt::WhatsThisRole);
				nameItem->setIcon(QIcon("images/graphic_two.png"));
				if (iedInfo.manu.compare("") == 0)
				{
					noFacItem->appendRow(typeItem);
				} 
				else
				{
					facItem->appendRow(typeItem);
				}
				typeItem->appendRow(nameItem);
				nameIndexNum = 0;
				typeIndexNum++;
				nameIndexNum++;
				itemNum = itemNum + 2;
			}
			else
			{
				QString nameDesc = QString("[") + iedInfo.name + QString("]") + iedInfo.desc;
				nameItem = new QStandardItem(nameDesc);
				nameItem->setData(panelKey + iedInfo.name, Qt::WhatsThisRole);
				nameItem->setIcon(QIcon("images/graphic_two.png"));
				typeItem->appendRow(nameItem);
				nameIndexNum++;
				itemNum++;
			}
			nameSum++;

			if (iedInfo.manu.compare("") == 0)
			{
				noFacNameSum++;
			}
		}
		if (j == mapKeys.size() - 1)
		{
			facIndex = deviceModel->indexFromItem(facItem);
			typeIndex = deviceModel->indexFromItem(typeItem);

			QString strFacInfo = facTemp + tr("����") + QString::number(typeIndexNum) + tr("���ͺţ��漰") + QString::number(nameSum) + "̨װ�ã�";
			QString strTypeInfo = typeTemp + tr("��") + QString::number(nameIndexNum) + tr("̨��");

			deviceModel->setData(facIndex,strFacInfo);
			deviceModel->setData(typeIndex,strTypeInfo);

			//typeSum += typeIndexNum;
		}
	}
	if (noFacItem != NULL)
	{
		noFacItem->setIcon(QIcon("images/red_question_mark.png"));
		noFacItem->setText("δ֪����(��" + QString::number(noFacItem->rowCount()) + "���ͺţ��漰" + QString::number(noFacNameSum) + "̨װ�ã�");
		deviceModel->appendRow(noFacItem);
	}

	deviceModel->setHeaderData(0,Qt::Horizontal,tr("װ���ͺţ�") + QString::number(typeSum) + "��");

	deviceView->setModel(deviceModel);
	//deviceView->setSelectionModel(NULL);
	QItemSelectionModel *slcModel = new QItemSelectionModel(deviceModel);
	deviceView->setSelectionModel(slcModel);
	connect(deviceView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)), this, SLOT(SelectDeviceProc(const QModelIndex&)));
	deviceView->setColumnWidth(0,600);
	//deviceView->expandAll();
	deviceView->setEditTriggers(NULL);
	deviceView->setExpandsOnDoubleClick(true);
	//deviceView->setMaximumWidth(300);
	//setCentralWidget(deviceView);
	//deviceView->show();
}

void GraphicBrowserFrm::InitVersionView()
{
	QVector<QHItemInfo> *hItemList = IEDsLoader->GetHItemList();

	QMap<QString, QHItemInfo> hItemMap;
	for (int i = 0; i < hItemList->size(); i++)
	{
		QHItemInfo hItem = hItemList->at(i);
		QString key = hItem.version + ":" + hItem.reversion;
		hItemMap.insert(key, hItem);
	}
	versionModel->clear();
	QList<QHItemInfo> list =  hItemMap.values();
	for (int i = list.size()-1; i >=0; i--)
	{
		QHItemInfo hItem = list.at(i);
		QList<QStandardItem*> itemList;
		QList<QString> hItemNameList = hItem.substationName.split("#");
		//itemList.append(new QStandardItem(hItem.substationName));
		if (hItemNameList.size() == 0)
		{
			hItemNameList.append(" ");
		}
		QStandardItem *hItemNameItem = new QStandardItem(hItemNameList.at(0));
		hItemNameItem->setToolTip(hItemNameList.at(0));
		itemList.append(hItemNameItem);

		QStandardItem *hItemVersionItem = new QStandardItem(hItem.version);
		hItemVersionItem->setToolTip(hItem.version);
		itemList.append(hItemVersionItem);

		QStandardItem *hItemReversionItem = new QStandardItem(hItem.reversion);
		hItemReversionItem->setToolTip(hItem.reversion);
		itemList.append(hItemReversionItem);

		QStandardItem *hItemWhoItem = new QStandardItem(hItem.who);
		hItemWhoItem->setToolTip(hItem.who);
		itemList.append(hItemWhoItem);
		QString timeText = hItem.when.replace("T","  ");

		QStandardItem *hItemWhenItem = new QStandardItem(timeText);
		hItemWhenItem->setToolTip(timeText);
		itemList.append(hItemWhenItem);

		QStandardItem *hItemWhatItem = new QStandardItem(hItem.what);
		hItemWhatItem->setToolTip(hItem.what);
		itemList.append(hItemWhatItem);

		QStandardItem *hItemWhyItem = new QStandardItem(hItem.why);
		hItemWhyItem->setToolTip(hItem.why);
		itemList.append(hItemWhyItem);
		versionModel->appendRow(itemList);
	}

	//versionModel->clear();
	//for (int i = 0; i < hItemList->size(); i++)
	//{
	//	QHItemInfo hItem = hItemList->at(i);
	//	QList<QStandardItem*> itemList;
	//	QList<QString> hItemNameList = hItem.substationName.split("#");
	//	//itemList.append(new QStandardItem(hItem.substationName));
	//	if (hItemNameList.size() == 0)
	//	{
	//		hItemNameList.append(" ");
	//	}
	//	itemList.append(new QStandardItem(hItemNameList.at(0)));
	//	itemList.append(new QStandardItem(hItem.version));
	//	itemList.append(new QStandardItem(hItem.reversion));
	//	itemList.append(new QStandardItem(hItem.who));
	//	itemList.append(new QStandardItem(hItem.when));
	//	itemList.append(new QStandardItem(hItem.what));
	//	itemList.append(new QStandardItem(hItem.why));
	//	versionModel->appendRow(itemList);
	//}

	bool isSuccess = versionModel->setHeaderData(0,Qt::Horizontal,tr("վ��"));
	isSuccess = versionModel->setHeaderData(1,Qt::Horizontal,tr("�汾"));
	isSuccess = versionModel->setHeaderData(2,Qt::Horizontal,tr("�޶��汾"));
	isSuccess = versionModel->setHeaderData(3,Qt::Horizontal,tr("�޶���"));
	isSuccess = versionModel->setHeaderData(4,Qt::Horizontal,tr("�޸�ʱ��"));
	isSuccess = versionModel->setHeaderData(5,Qt::Horizontal,tr("�޸�����"));
	isSuccess = versionModel->setHeaderData(6,Qt::Horizontal,tr("�޸�ԭ��"));

	versionView->setModel(versionModel);
	versionView->setColumnWidth(0,200);
	versionView->setEditTriggers(NULL);
	//setCentralWidget(versionView);
	//versionView->hide();
}

void GraphicBrowserFrm::InitSCDValidatorView()
{
    scdValidatorModel->clear();
    if (m_SCLValid->m_MsgMap[currentValidatorItemInfo].size() > 0)
    {
        for (int i = 0; i < m_SCLValid->m_MsgMap[currentValidatorItemInfo].size(); i++)
        {
            CheckInfo temp = m_SCLValid->m_CheckResultVec.at(m_SCLValid->m_MsgMap[currentValidatorItemInfo].at(i));
            QList<QStandardItem*> itemList;
            QStandardItem *item;
            item = new QStandardItem(QString::number(i+1));
            item->setToolTip(item->text());
            itemList.append(item);

            item = new QStandardItem();
            item->setText(temp.sFlag);
            item->setToolTip(item->text());
            itemList.append(item);

            item = new QStandardItem(temp.sClass);
            item->setToolTip(item->text());
            itemList.append(item);

            item = new QStandardItem();
            item->setText(temp.position);
            item->setToolTip(item->text());
            itemList.append(item);

            item = new QStandardItem();
            item->setText(temp.sDesc);
            item->setToolTip(item->text());
            itemList.append(item);

            item = new QStandardItem();
            item->setText(temp.sStandard);
            item->setToolTip(item->text());
            itemList.append(item);

            item = new QStandardItem();
            item->setText(temp.sXml);
            item->setToolTip(item->text());
            itemList.append(item);
            scdValidatorModel->appendRow(itemList);
        }

    }

    bool isSuccess = scdValidatorModel->setHeaderData(0,Qt::Horizontal,tr("���"));
    isSuccess = scdValidatorModel->setHeaderData(1,Qt::Horizontal,tr("У������"));
    isSuccess = scdValidatorModel->setHeaderData(2,Qt::Horizontal,tr("�����Ŀ"));
    isSuccess = scdValidatorModel->setHeaderData(3,Qt::Horizontal,tr("λ��"));
    isSuccess = scdValidatorModel->setHeaderData(4,Qt::Horizontal,tr("У����"));
    isSuccess = scdValidatorModel->setHeaderData(5,Qt::Horizontal,tr("�ο��淶"));
    isSuccess = scdValidatorModel->setHeaderData(6, Qt::Horizontal,tr("ԭ������"));

    scdValidatorView->setModel(scdValidatorModel);
    scdValidatorView->setColumnWidth(0,60);
    scdValidatorView->setColumnWidth(1,70);
    scdValidatorView->setColumnWidth(2,180);
    scdValidatorView->setColumnWidth(3,100);
    scdValidatorView->setColumnWidth(4,600);
    scdValidatorView->setColumnWidth(5,150);
    scdValidatorView->setColumnHidden(6, true);
    scdValidatorView->setEditTriggers(NULL);

}

/** 
  * @brief    SelectValidatorProc
  * 
  * ˫��������ⴰ�ڵ���ϸ����	
  * 
  * @class    GraphicBrowserFrm
  * @param    const QModelIndex& index : ��ǰ�������Ŀ
  * @return   void
  * @author   zhang-xch
  * @date     2014/11/13
  * @remarks  
  */ 
void GraphicBrowserFrm::SelectValidatorProc(const QModelIndex& index)
{
	qValidateDialog.SetModelIndexValue(index);
	qValidateDialog.exec();
}

void GraphicBrowserFrm::InsertItem(FILE *fp, QString type)
{
	//char wName[MAXSIZE];
	//char wValue[EXLSIZE];
	//char wLevel[MAXSIZE];
	//char wRef[MAXSIZE];

	//int number = 0;
	//while(0 == feof(fp))
	//{
	//	number++;
	//	if (fread(wName,sizeof(char), sizeof(wName)/sizeof(char), fp) == 0)
	//	{
	//		break;
	//	}
	//	fread(wValue,sizeof(char), sizeof(wValue)/sizeof(char), fp);
	//	fread(wLevel,sizeof(char), sizeof(wLevel)/sizeof(char), fp);
	//	fread(wRef,sizeof(char), sizeof(wRef)/sizeof(char), fp);

	//	QList<QStandardItem*> itemList;
	//	QStandardItem *item;
	//	item = new QStandardItem(QString::number(number));
	//	item->setToolTip(item->text());
	//	itemList.append(item);
	//	item = new QStandardItem(type);
	//	item->setToolTip(item->text());
	//	itemList.append(item);
	//	item = new QStandardItem();
	//	item->setText(wName);
	//	item->setToolTip(item->text());
	//	itemList.append(item);
	//	item = new QStandardItem();
	//	item->setText(wValue);
	//	item->setToolTip(item->text());
	//	itemList.append(item);
	//	item = new QStandardItem();
	//	item->setText(wLevel);
	//	item->setToolTip(item->text());
	//	itemList.append(item);
	//	item = new QStandardItem();
	//	item->setText(wRef);
	//	item->setToolTip(item->text());
	//	itemList.append(item);
	//	scdValidatorModel->appendRow(itemList);
	//}
}

void GraphicBrowserFrm::modelChange( QStandardItem *item)
{
	bool mf=false;
	bool ma=false;
	for(int i=0;i<substationModelCompare->rowCount();i++)
	{
		QString text=substationView->model()->index(i,0,substationView->rootIndex()).data().toString();
		if(text=="��׼�ļ�")
		{
		      mf=true;
		}
		else if(text=="�Ƚ��ļ�")
		{
			  ma=true;
		}
	}
	emit SendItemDelegateFlag(mf,ma);
}
void    GraphicBrowserFrm::InitSubStationTable() // �Ƚ�table ��
{
	tablesubstation->clear();
	QStringList str;
    str<<"��׼�ļ�/�Ƚ��ļ�"<<"�ļ���"<<"�ļ�·��";
	QStringList Comboxstr;
	Comboxstr<<"��ѡ��"<<"��׼�ļ�"<<"�Ƚ��ļ�";
	tablesubstation->setColumnCount(3);
	tablesubstation->setHorizontalHeaderLabels(str);
	tablesubstation->setRowCount(0);
	IEDsLoader->LoadSubstationNameList();

	QVector<QString> *substationNameList = IEDsLoader->GetSubstationNameList();
	if (substationNameList->size() <= 1)
		substationCompareBtn->setEnabled(false);
	else
		substationCompareBtn->setEnabled(true);

	for (int i = 0; i != substationNameList->size(); i++)
	{
		tablesubstation->insertRow(i);
		QString substationName = substationNameList->at(i);
		QString substationKey = QString("220kV.") + substationName;
		QSubStationInfo *substationInfo = IEDsLoader->GetSubstationInfo(substationKey);

	    QComboBox *combox=new QComboBox(tablesubstation);
		combox->resize(20,20);
		combox->addItems(Comboxstr);
		tablesubstation->setCellWidget(i,0,combox);
		tablesubstation->setItem(i,1,new QTableWidgetItem(substationInfo->fileName));
		tablesubstation->setItem(i,2,new QTableWidgetItem(substationInfo->filePath));

		tablesubstation->setRowHeight(i,20);
	} 

	tablesubstation->resizeColumnToContents(0);
	tablesubstation->resizeColumnToContents(1);
	tablesubstation->resizeColumnToContents(2);
	tablesubstation->resizeColumnToContents(3);
	//tablesubstation->resizeColumnToContents(4);

}
void GraphicBrowserFrm::InitSubstationView()
{
	IEDsLoader->LoadSubstationNameList();
	QVector<QString> *substationNameList = IEDsLoader->GetSubstationNameList();
	if (substationNameList->size() <= 1)
	{
		substationCompareBtn->setEnabled(false);
	}
	else
	{
		substationCompareBtn->setEnabled(true);
	}
	substationModel->clear();
	substationModelCompare->clear();
	checkBoxItemList.clear();
	for (int i = 0; i != substationNameList->size(); i++)
	{
		QString substationName = substationNameList->at(i);
		QStringList nameList = substationName.split("#");
		QString substationKey = QString("220kV.") + substationName;
		QSubStationInfo *substationInfo = IEDsLoader->GetSubstationInfo(substationKey);
		QList<QStandardItem*> nameRow;

		//QStandardItem *checkBoxItem=new QStandardItem(nameList.at(0));
		QStandardItem *checkBoxItem=new QStandardItem(substationInfo->fileName);
		//checkBoxItem->setCheckable(true);
	    nameRow.append(new QStandardItem("��ѡ��"));
		checkBoxItemList.append(checkBoxItem);
		nameRow.append(checkBoxItem);
		nameRow.append(new QStandardItem(substationInfo->version));
		nameRow.append(new QStandardItem(substationInfo->reversion));

		//chenxin modify 2014.10.21 --->
		//nameRow.append(new QStandardItem(nameList.at(1) + " " + nameList.at(2)));
		nameRow.append(new QStandardItem(substationInfo->fileName));
		//chenxin modify 2014.10.21 <---
		nameRow.append(new QStandardItem(substationInfo->filePath));

		substationModelCompare->appendRow(nameRow);
	} 
	substationModelCompare->setHeaderData(0,Qt::Horizontal,tr("��׼�ļ�/�Ƚ��ļ�"));
	substationModelCompare->setHeaderData(1,Qt::Horizontal,tr("�ļ���"));
	substationModelCompare->setHeaderData(2,Qt::Horizontal,tr("�汾"));
	substationModelCompare->setHeaderData(3,Qt::Horizontal,tr("�޶��汾"));
	substationModelCompare->setHeaderData(4,Qt::Horizontal,tr("����ʱ��"));
	substationModelCompare->setHeaderData(5,Qt::Horizontal,tr("�ļ�·��"));

	substationView->setModel(substationModelCompare);
	itemdele->initFlag();
	//	substationView->setItemDelegate(itemdele);
	//substationView->setColumnWidth(0,200);
	substationView->resizeColumnToContents(0);
	substationView->resizeColumnToContents(1);
	substationView->resizeColumnToContents(2);
	substationView->resizeColumnToContents(3);
	substationView->resizeColumnToContents(4);
	substationView->resizeColumnToContents(5);

	//substationView->setEditTriggers(NULL);
	//connect(substationView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(SubStationCompareProc()));
	connect(substationView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(CheckboxSelect(const QModelIndex &)));
}

void GraphicBrowserFrm::CheckboxSelect(const QModelIndex & index)
{
	// qDebug()<<::GetCurrentProcessId()<<index.row()<<index.column();
	// qDebug()<<::GetCurrentProcessId()<<substationView->currentIndex();

	QItemSelectionModel *sel=substationView->selectionModel();
	QModelIndex index_2=substationView->model()->index(index.row(),0,substationView->rootIndex());
	QModelIndex index_1=substationView->model()->index(index.row(),3,substationView->rootIndex());
	QItemSelection seltion(index_2,index_1);
	sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 

	int i=0;
	for(int y=0;y<checkBoxItemList.count();y++)
	{
	     if(checkBoxItemList.at(y)->checkState()==2)
			 i++;
	}
	if(i>=3)
	{
		  checkBoxItemList.at(index.row())->setCheckState(Qt::Unchecked);
	}

}

void GraphicBrowserFrm::InitSubstationManageView()
{
	IEDsLoader->LoadSubstationNameList();
	QVector<QString> *substationNameList = IEDsLoader->GetSubstationNameList();
	
	substationManageView->setContextMenuPolicy(Qt::CustomContextMenu);
	substationManageModel->clear();
	for (int i = 0; i != substationNameList->size(); i++)
	{
		QString substationName = substationNameList->at(i);

		QString substationKey = QString("220kV.") + substationName;
		QSubStationInfo *substationInfo = IEDsLoader->GetSubstationInfo(substationKey);
		QList<QStandardItem*> nameRow;
		nameRow.append(new QStandardItem(substationInfo->fileName));
		
		// nameRow.append(new QStandardItem(IEDsLoader->GetSubstationImportTime(substationKey)));
		nameRow.append(new QStandardItem(substationInfo->filePath));
		//chenxin modify 2014.10.20 <---

		substationManageModel->appendRow(nameRow);
	}
	substationManageModel->setHeaderData(0,Qt::Horizontal,tr("SCD�ļ���"));
	// substationManageModel->setHeaderData(1,Qt::Horizontal,tr("����ʱ��"));
	substationManageModel->setHeaderData(1,Qt::Horizontal,tr("�ļ�·��"));

	substationManageView->setModel(substationManageModel);
	substationManageView->setColumnWidth(0,200);
	//substationView->setColumnWidth(1,200);
	substationManageView->setEditTriggers(NULL);
	QModelIndex mIndex;
	substationManageView->setCurrentIndex(mIndex);
	connect(substationManageView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(SubStationChangeProc()));
}

void GraphicBrowserFrm::closeEvent(QCloseEvent* event)
{
	//WriteSettings();
	event->accept();
	SclDacLib::Shutdown();
	emit Close();
}

void GraphicBrowserFrm::AboutProc()
{
	//QMessageBox message(QMessageBox::NoIcon, "����...", "�����ݸ��Ƽ����޹�˾"); 
	//message.setIconPixmap(QPixmap("images/roof-logo.png")); 
	//message.exec();

	QDialog aboutWin(this);
	QVBoxLayout aboutVLayout;
	QLabel aboutInfoLabel, aboutLogoLabel;
	QPixmap aboutLogo("images/logo.png");

	QString SoftVersion;
	QFile versionSaver("../cfg/version");
	if (versionSaver.open(QIODevice::ReadOnly))
	{
		QTextStream in(&versionSaver);
		SoftVersion = in.readAll();
		versionSaver.close();
	}

	QString aboutInfoText = "<h2><font color=blue>��Ʒ��Ϣ</font></h2>\n<p>��Ʒ����:SCD���ӻ�����</p>\n<p>�汾��Ϣ:"+ SoftVersion + "</p>\n";
	aboutInfoText = aboutInfoText + "<h2><font color=blue>��˾��Ϣ</font></h2>\n<p>����:ɽ���ݸ�����Ϣ�Ƽ����޹�˾</p>\n<p>�绰:0531-88872188</p>\n<p>��ַ:http://www.tech-roof.com</p>" + 
					"\n<p>��ַ:ɽ��ʡ�����и������������2008�����ɴ���D��4-402</p>\n<p>�ʱ�:250100</p><br/>";
	aboutInfoLabel.setText(aboutInfoText);
	aboutLogoLabel.setPixmap(aboutLogo);
	aboutVLayout.addWidget(&aboutLogoLabel);
	aboutVLayout.addWidget(&aboutInfoLabel);
	aboutVLayout.setAlignment(&aboutLogoLabel, Qt::AlignHCenter);
	aboutVLayout.setAlignment(&aboutInfoLabel, Qt::AlignHCenter);
	aboutWin.setLayout(&aboutVLayout);
	aboutWin.setWindowTitle("����...");
	aboutWin.setStyleSheet("background-color: white");
	aboutWin.setFixedSize(350, 380);
	aboutWin.setModal(true);
	aboutWin.exec();
}

void GraphicBrowserFrm::ShowIEDsTreeProc()
{
	IEDsTreeDockWidget->setVisible(actionShowIEDsTree->isChecked());
}


void GraphicBrowserFrm::ShowgraphicsViewProc()
{
	graphicsViewDockWidget->setVisible(!actionShowgraphicsView->isChecked());
}

void GraphicBrowserFrm::ShowSubstationChangeProc()
{
	logsrv_scoped_output(logsrv, "����ShowSubstationChangeProc", "�˳�ShowSubstationChangeProc", "GraphicBrowserFrm::ShowSubstationChangeProc");
	if(!proCompareFlg)
	{
		InitSubstationView();
        InitSubStationTable();
		substationWin->show();
		// logsrv_output_info(logsrv, L"��������", L"GraphicBrowserFrm::ShowSubstationChangeProc");
		actionCompare->setCheckable(false);
	}else
		progresCompareStyle->show();
}

void GraphicBrowserFrm::ShowSubstationManageProc()
{
	InitSubstationManageView();
	substationManageWin->show();
}
void  GraphicBrowserFrm::InitImportFile(QString fileName)
{
	qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	Start	"<<fileName;

	// qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	LoadVoltageLevelNameList	AddCurVoltageLevel	Start";
	IEDsLoader->LoadVoltageLevelNameList();
	if (IEDsLoader->GetVoltageLevelNameList()->size() == 0)
	{
		scdManger->AddCurVoltageLevel();
	}
	// qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	LoadVoltageLevelNameList	AddCurVoltageLevel	End";

	// qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	�ļ�����	Start";

	QFile nameSaver("../cfg/FileNameSaver.txt");
	if (nameSaver.open(QIODevice::WriteOnly))
	{
		QTextStream out(&nameSaver);
		out << fileName;
		nameSaver.close();
	}
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) 
	{
		QMessageBox::warning(this,
			tr("�ļ�����"),
			fileName);
		QMessageBox::warning(this,
			tr("�ļ�����"),
			tr("�ļ��򿪴���\n%1").arg(fileName));
		return;
	}
	// qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	�ļ�����	End";

	filePath = fileName;
	QString fileID = QString::fromStdString(SclDacLib::CalcFileMark(fileName.toStdString()));
    qDebug() << "fileID   is " << fileID;
    
	QVector<QString> *subStationList = IEDsLoader->GetSubstationNameList();
	bool isExit = false;
	// QString importedFileID;
	for (uint i = 0; i < subStationList->size(); i++)
	{
		if (subStationList->at(i) == fileID)
		{
			isExit = true;
			break;
		}
	}

	if (isExit)
	{
#ifndef WORK_STATION
		QMessageBox::information(NULL, "�����ļ�", "�ļ��Ѿ����룬�����л���", QMessageBox::Ok);
#endif		
		if (fileNameList.last() == filePath)
		{			
			SetSubStation(fileID);
		}
		else
		{
			m_AutoCompareFiles.push_back(fileNameList.at(currentImportFile));
			if(fileNameList.count()-1>currentImportFile)
			{
				currentImportFile++;
				InitImportFile(fileNameList.at(currentImportFile));
				return;
			}
		}
	}
	else
	{
		progresStyle->setPorgressValue(0);    // ��������ʼ��
		progresStyle->setLabeCurrentText("���ڵ���SCD�ļ�.......");
		porgressFlg=true;
		proGressCount=0;
		QFileInfo info(fileName);
		int ms=info.size()/1024/50000;  //Ԥ����average m ����ɵ���
		//��ʱ����1s���ڣ���ÿ100msǰ��15�񣻷���ǰ��10/ms
		if (ms < 1)
		{
			averageSpeed = 15; 
		}
		else
		{
			averageSpeed = 10/ms;
		}
		QString text="  "+info.fileName();
		progresStyle->setFileText(text,currentImportFile,fileNameList.count());
		progresStyle->show();

		// qDebug()<<::GetCurrentProcessId()<<"loadingThread->start()	 isRunning"<<loadingThread->isRunning();
		// qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	loadingThread->setFilePath	Start";

		ShowEvent();
		loadingThread->setFilePath(fileName);
		loadingThread->start();
		timerProgress->start(100);
		// qDebug()<<::GetCurrentProcessId()<<"loadingThread->start()	 isRunning"<<loadingThread->isRunning();
		// qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	loadingThread->setFilePath	End";
		qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::InitImportFile	End	"<<fileName;
	}
}
void GraphicBrowserFrm::LoadSCDProc()
{
	if(!porgressFlg)
	{
		substationManageWin->hide();
		QString fileName = QFileDialog::getOpenFileName(this,"��",filePath, "ȫ��֧�ֵ��ļ� (*.scd *.icd *.cid *.ssd);;Scd �ļ�(*.scd);;ICD �ļ�(*.icd);;CID �ļ�(*.cid);;SSD �ļ�(*.ssd)");
		if (fileName.isEmpty())
			return;
		
		if (vDialog->GetDialogLocked())
		{
			if (!lockedDialogVec.contains(vDialog))
			{
				lockedDialogVec.append(vDialog);
			}
		}
		else
		{
			vDialog->close();
		}
		closeUnlockedVirDlg();
		if (lockedDialogVec.size() != 0)
		{
			if (QMessageBox::information(this, "�Ƿ�ر�", "��⵽����������,�Ƿ�ر�??", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
			{
				closeAllVirDlgs();
			}
		}

		QString houzhui = fileName.right(4);
		if (houzhui.toLower().compare(".scd") != 0 && 
            houzhui.toLower().compare(".icd") != 0 && 
            houzhui.toLower().compare(".cid") != 0 &&
            houzhui.toLower().compare(".ssd") != 0)
		{
			QMessageBox::warning(this,
				tr("�ļ�����"),
				tr("�����ļ���ʽ[") + houzhui + tr("]��֧��!"));
			return;
		}
		fileNameList.clear();
		currentImportFile=0;
		fileNameList.append(fileName);
		InitImportFile(fileName);
	}
	else
		progresStyle->show();
}

void GraphicBrowserFrm::timerCompareOut()
{

	 int compareSpeed=scdDiffCompare->m_CompareSpeed;
	 if(compareSpeed>0&&compareSpeed<=5)
		  progresCompareStyle->setLabeCurrentText("���ڱȽ��ļ���Ϣ.......");
	 else if(compareSpeed>5&&compareSpeed<=55)
		  progresCompareStyle->setLabeCurrentText("���ڱȽϻ�׼�ļ��е�IED.......");
	 else if(compareSpeed>55&&compareSpeed<=95)
		  progresCompareStyle->setLabeCurrentText("���ڱȽϱȽ��ļ��е�IED.......");
	 else if(compareSpeed>95&&compareSpeed<=100)
		  progresCompareStyle->setLabeCurrentText("���ڴ���Map.......");
	 if(compareSpeed>=100)
	 {
		  compareSpeed=99;
	 }
     progresCompareStyle->setPorgressValue(compareSpeed);
}

void GraphicBrowserFrm::timerProgressOut()
{
	    /*���ڵ���SCD�ļ�.....
		���ڽ���SCD�ļ�.......
		���ڵ���ģ�Ϳ�.....
		���ڽ���ģ�͹���.....
		����ɹ�,��ȴ�....*/
	 if(proGressCount<97)
	 {
		 if(proGressCount>0&&proGressCount<=20)
			 progresStyle->setLabeCurrentText("���ڵ���SCD�ļ�.......");
		 else if(proGressCount>20&&proGressCount<=40)
			 progresStyle->setLabeCurrentText("���ڽ���SCD�ļ�.......");
		 else if(proGressCount>40&&proGressCount<=60)
			 progresStyle->setLabeCurrentText("���ڵ���ģ�Ϳ�.......");
		 else if(proGressCount>60&&proGressCount<=88)
			 progresStyle->setLabeCurrentText("���ڽ���ģ�͹���.......");
		 else if(proGressCount>88)
			 progresStyle->setLabeCurrentText("����ɹ�,��ȴ�.......");
		 proGressCount=proGressCount+averageSpeed;
		 if(proGressCount>=100)
		 {
			 proGressCount=99;
		     progresStyle->setLabeCurrentText("����ɹ�,��ȴ�.......");
			 timerProgress->stop();
		 }
		 progresStyle->setPorgressValue(proGressCount);
	 }
	 else 
		 timerProgress->stop();

}

void GraphicBrowserFrm::timerProgressCheckOut()
{
	    /*���ڵ���SCD�ļ�.....
		���ڽ���SCD�ļ�.......
		���ڵ���ģ�Ϳ�.....
		���ڽ���ģ�͹���.....
		����ɹ�,��ȴ�....*/
	 if(proGressCount<97)
	 {
		 if(proGressCount>0&&proGressCount<=20)
			 progresCheckStyle->setLabeCurrentText("���ڵ���SCD�ļ�.......");
		 else if(proGressCount>20&&proGressCount<=40)
			 progresCheckStyle->setLabeCurrentText("���ڽ���SCD�ļ�.......");
		 else if(proGressCount>40&&proGressCount<=88)
			 progresCheckStyle->setLabeCurrentText("���ڼ���ļ�.......");
		 else if(proGressCount>88)
			 progresCheckStyle->setLabeCurrentText("���ɹ�,��ȴ�.......");
		 proGressCount=proGressCount+averageSpeed;
		 if(proGressCount>=100)
		 {
			 proGressCount=99;
		     progresCheckStyle->setLabeCurrentText("���ɹ�,��ȴ�.......");
			 timerCheckProgress->stop();
		 }
		 progresCheckStyle->setPorgressValue(proGressCount);
	 }
	 else 
		 timerCheckProgress->stop();

}


void GraphicBrowserFrm::LoadSCDProc1(int loadResult)
{
	qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::LoadSCDProc1	timerProgress->stop	Start";
	timerProgress->stop(); //ֹͣα��ʱ��
	qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::LoadSCDProc1	timerProgress->stop	Stop";

	qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::LoadSCDProc1	loadingThread->terminate loadingThread->wait	Start	"<<loadingThread->isRunning();
	loadingThread->wait();//��ֹ���̲߳���������ֹͣ���ȴ�����ȫ��ֹ
	// loadingThread->terminate();//�߳���ֹ
	qDebug()<<::GetCurrentProcessId()<<"GraphicBrowserFrm::LoadSCDProc1	loadingThread->terminate loadingThread->wait	Stop	"<<loadingThread->isRunning();
	QString subStation;
	switch(loadResult)
	{
	case 0:
		QMessageBox::warning(this,"�������","�ļ����ڴ���������ٵ��룡");
		progresStyle->hide();     // ���ؽ�����
		porgressFlg=false;
		subStation = subSetting->value("config/CurrentSubstationID").toString();
		IEDsLoader->SetCurrentSubstationID(subStation);
        m_SSDInfo->SetCurrentSubstation(subStation);
		return;
		// break;
	default:
		m_AutoCompareFiles.push_back(fileNameList.at(currentImportFile));
		if(fileNameList.count()-1>currentImportFile)
		{
			currentImportFile++;
			InitImportFile(fileNameList.at(currentImportFile));
			return;
		}
		else 
		{ 
			qDebug()<<::GetCurrentProcessId()<<"setPorgressValue();		Start";
			progresStyle->setPorgressValue(100);
			progresStyle->setLabeCurrentText("�������");
			progresStyle->hide();    //���ؽ�����// ����ɹ�  ���״̬
			proGressCount=0;
			porgressFlg=false; 
			// QMessageBox::warning(this,"����ɹ�","SCD�ļ��Ѿ��ɹ�����!");
			qDebug()<<::GetCurrentProcessId()<<"setPorgressValue();		Stop";
	    }
		break;
	}

	qDebug()<<::GetCurrentProcessId()<<"HideEvent();		Start";
	HideEvent();
	IEDsLoader->LoadVoltageLevelNameList();
	IEDsLoader->SetCurrentVoltageLevelIndex(0);
	IEDsLoader->LoadSubstationNameList();
	QVector<QString> *substationList = IEDsLoader->GetSubstationNameList();

	//chenxin modify 2014.10.26 --->
	QString loadedFileMark = SclDacLib::CalcFileMark(fileNameList.last().toLocal8Bit().constData()).c_str();
	//int index = 0;
	//if (substationList->size() - 1 > 0)
	//{
	//	index = substationList->size() - 1;
	//}
	//QString loadedFileMark = substationList->at(index);
	IEDsLoader->SetCurrentSubstationID(loadedFileMark);
    m_SSDInfo->SetCurrentSubstation(loadedFileMark);
	//chenxin modify 2014.10.26 <---	
	QString curSubStationID = IEDsLoader->GetCurrentSubstationID();
	subSetting->setValue("config/CurrentSubstationID", curSubStationID);

    QString subKey = "220kV." + curSubStationID + ".����";
    IntervalCreator creator(subKey);
    creator.SetIEDInterval();
	//subSetting->setValue("config/CurrentSubstationID", loadedFileMark);
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadIEDInfoList();
	IEDsLoader->LoadHItemList();
	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
	InitVersionView();
	InitDeviceView();
    InitSSDTreeWidget();
	qDebug()<<::GetCurrentProcessId()<<"HideEvent();		Stop";

	qDebug()<<::GetCurrentProcessId()<<"clearGraphics();		Start";
	graphicsManger->clearGraphics();
	QString subName;
	setCenterLayer(SHOW_ToTal_IED);
	if (substationList->size() == 0)
	{
		subName = "";
		setWindowTitle(TrivalInfo + "SCD���ӻ� - []");
		IEDsTreeDockWidget->setWindowTitle(getDockTitleName());
	} 
	else
	{
		//chenxin modify 2014.10.26 --->
		QString subKey = "220kV." + curSubStationID;
		//chenxin modify 2014.10.26 <---

		QSubStationInfo *subInfo = IEDsLoader->GetSubstationInfo(subKey);

		//chenxin modify 2014.10.17 --->
		setWindowTitle(TrivalInfo + "SCD���ӻ�" + " - " + subInfo->fileName);
		IEDsTreeDockWidget->setWindowTitle(getDockTitleName() + subInfo->Name);
		//chenxin modify 2014.10.17 <---

	}
	lDialog->hide();

	treeTabWidget->clear();
	treeTabWidget->addTab(infoTreeContainerWidget,tr("������Ϣ"));
	treeTabWidget->addTab(treeContainerWidget,tr("IED�б�"));
	treeTabWidget->addTab(deviceView,tr("��վ����װ���ͺ�"));
	treeTabWidget->addTab(subnetTreeContainerWidget,tr("ͨ������"));
    treeTabWidget->addTab(ssdTreeContainerWidget, tr("һ���豸"));
	scdValidatorView->hide();
	treeTabWidget->setCurrentIndex(1);

    QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo(subKey);
    if (subInfo->fileName.right(3).compare("SSD", Qt::CaseInsensitive) == 0)
    {
        treeTabWidget->setCurrentIndex(4);
    }

	if (m_IsAutoCompare && !porgressFlg)
	{
		StratCompareFromCMDAfterLoad();
	}
	qDebug()<<::GetCurrentProcessId()<<"clearGraphics();		Stop";
}

void GraphicBrowserFrm::on_substationManageView_customContextMenuRequested(const QPoint &pos)
{
	int cRow = substationManageView->currentIndex().row();
	if (cRow == -1)
	{
		return;
	}
	QMenu *smMenu = new QMenu();
	QAction *deleteSubAction = smMenu->addAction(tr("ɾ��SCD"));
	connect(deleteSubAction, SIGNAL(triggered(bool)),this, SLOT(SubStationDeleteProc()));
	smMenu->exec(QCursor::pos());
}

void GraphicBrowserFrm::SubStationDeleteProc()
{
	int cRow = -1;
	cRow = substationManageView->currentIndex().row();
	if (cRow == -1)
	{
		QMessageBox::warning(this,"ɾ��SCD","��ѡ��Ҫɾ���ı��վ!");
		return;
	}
	// Ҫɾ���ı��վ�͵�ǰ���վ
	QString deleteFileMark = IEDsLoader->GetSubstationNameList()->at(cRow);
	QString curSubStation = IEDsLoader->GetCurrentSubstationID();
	bool isCurrentSubstation = false;
	if (curSubStation == deleteFileMark)
	{
		isCurrentSubstation = true;
		if(QMessageBox::warning(this,"ɾ��SCD","��SCDΪ��ǰ���ڲ鿴��SCD,�Ƿ�ȷ��ɾ����",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
		{
			cRow = -1;
			return;
		}
	}
	else 
	{
		if(QMessageBox::warning(this,"ɾ��SCD","ȷ��ɾ����SCD��",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
		{
			cRow = -1;
			return;
		}
	}

	// ��Ҫɾ�����ȹر����·ͼ
	if (vDialog->GetDialogLocked())
	{
		if (!lockedDialogVec.contains(vDialog))
		{
			lockedDialogVec.append(vDialog);
		}
	}
	else
	{
		vDialog->close();
	}
	closeUnlockedVirDlg();
	if (lockedDialogVec.size() != 0)
	{
		if (QMessageBox::information(this, "�Ƿ�ر�", "��⵽����������,�Ƿ�ر�??", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
		{
			closeAllVirDlgs();
		}
	}


	QString substationKey = "220kV." + deleteFileMark;
	int deleteStatu = scdManger->DeleteSubstation(substationKey);
	IEDsLoader->UnloadScdFile(deleteFileMark);
	// ���ɾ�����վ���ǵ�ǰվ����ֱ�ӷ���
	if(!isCurrentSubstation)
	{
		InitSubstationManageView();
		return;
	}
	QVector<QString> *NameList = IEDsLoader->GetSubstationNameList();
	if (NameList->size() <= 0)
	{
		IEDsLoader->SetCurrentSubstationID("");
		setWindowTitle(TrivalInfo + "SCD���ӻ� - []");
		IEDsTreeDockWidget->setWindowTitle(getDockTitleName());
		graphicsViewDockWidget->setWindowTitle("��ǰװ�� : ��");
		subSetting->setValue("config/CurrentSubstationID", "");
		setCenterLayer(-1);
	}else
	{
		IEDsLoader->SetCurrentSubstationID(NameList->at(0));
        m_SSDInfo->SetCurrentSubstation(NameList->at(0));
		subSetting->setValue("config/CurrentSubstationID", NameList->at(0));

		QString subKey = "220kV." + NameList->at(0);
		QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo(subKey);
		//chenxin modify 2014.10.20 --->
		setWindowTitle(TrivalInfo + "SCD���ӻ� - " + subInfo->fileName);
		graphicsViewDockWidget->setWindowTitle("��ǰװ�� : ��");
		IEDsTreeDockWidget->setWindowTitle(getDockTitleName() + subInfo->Name);
		setCenterLayer(SHOW_ToTal_IED);
		//chenxin modify 2014.10.20 <---
	}

	IEDsLoader->LoadSubstationNameList();
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadIEDInfoList();
	IEDsLoader->LoadHItemList();
	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
	InitVersionView();
	InitDeviceView();
    InitSSDTreeWidget();
	graphicsManger->clearGraphics();
	graphicScene->clear();
	actionPre->setEnabled(false);
	actionNext->setEnabled(false);
	

	QMessageBox::warning(this,"ɾ��SCD","SCDɾ���ɹ�!");
	InitSubstationManageView();
	// substationManageWin->hide();
}

void GraphicBrowserFrm::SubStationChangeProc()
{
	logsrv_scoped_output(logsrv, "����SubStationChangeProc����", "�˳�SubStationChangeProc����", "GraphicBrowserFrm::SubStationChangeProc");
	if(substationManageWin->windowTitle()=="ɾ��SCD")
		return;
	int crow = substationManageView->currentIndex().row();
	if (crow == -1)
	{
		substationManageWin->hide();
		return;
	}
	QString subStation = IEDsLoader->GetSubstationNameList()->at(crow);
	QString curSubStation = IEDsLoader->GetCurrentSubstationID();

	if (subStation == curSubStation)
	{
		substationManageWin->hide();
		return;
	}

	if (vDialog->GetDialogLocked())
	{
		if (!lockedDialogVec.contains(vDialog))
		{
			lockedDialogVec.append(vDialog);
		}
	}
	else
	{
		vDialog->close();
	}
	closeUnlockedVirDlg();
	if (lockedDialogVec.size() != 0)
	{
		if (QMessageBox::information(this, "�Ƿ�ر�", "��⵽����������,�Ƿ�ر�??", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
		{
			closeAllVirDlgs();
		}
	}

	// logsrv_output_info(logsrv, L"SetCurrentSubstationID Start", L"GraphicBrowserFrm::SubStationChangeProc");
	substationManageWin->hide();
	m_SwitchProcessBar->show();
	QCoreApplication::processEvents(QEventLoop::AllEvents,1);
	IEDsLoader->SetCurrentSubstationID(subStation);
    m_SSDInfo->SetCurrentSubstation(subStation);
	subSetting->setValue("config/CurrentSubstationID", subStation);
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadIEDInfoList();
	IEDsLoader->LoadHItemList();
	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
	InitVersionView();
	InitDeviceView();
    InitSSDTreeWidget();
	graphicsManger->clearGraphics();
	graphicScene->clear();
	actionPre->setEnabled(false);
	actionNext->setEnabled(false);
	
	
	// logsrv_output_info(logsrv, L"SetCurrentSubstationID End", L"GraphicBrowserFrm::SubStationChangeProc");

	QString subKey = "220kV." + subStation;
	QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo(subKey);
	setCenterLayer(SHOW_ToTal_IED);
	QString subName = subInfo->fileName;
	setWindowTitle(TrivalInfo + "SCD���ӻ�" +  " - " + subInfo->fileName);
	IEDsTreeDockWidget->setWindowTitle(getDockTitleName() + subInfo->Name);

	graphicsViewDockWidget->setWindowTitle("��ǰװ�� : ��");

	treeTabWidget->clear();
	treeTabWidget->addTab(infoTreeContainerWidget,tr("������Ϣ"));
	treeTabWidget->addTab(treeContainerWidget,tr("IED�б�"));
	treeTabWidget->addTab(deviceView,tr("��վ����װ���ͺ�"));
	treeTabWidget->addTab(subnetTreeContainerWidget,tr("ͨ������"));
    treeTabWidget->addTab(ssdTreeContainerWidget, tr("һ���豸"));
	scdValidatorView->hide();
	treeTabWidget->setCurrentIndex(1);
    if (subName.right(3).compare("SSD", Qt::CaseInsensitive) == 0)
    {
        treeTabWidget->setCurrentIndex(4);
    }
	
	QCoreApplication::processEvents(QEventLoop::AllEvents,1);
}

void GraphicBrowserFrm::ImportValidateFinished()
{
    m_SwitchProcessBar->hide();
    InitSSDTreeWidget();
}

void GraphicBrowserFrm::SetSubStation(QString subStationID)
{
	IEDsLoader->SetCurrentSubstationID(subStationID);
    m_SSDInfo->SetCurrentSubstation(subStationID);
	QString curSubStationID = IEDsLoader->GetCurrentSubstationID();
	subSetting->setValue("config/CurrentSubstationID", curSubStationID);
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadIEDInfoList();
	IEDsLoader->LoadHItemList();
	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
	InitVersionView();
	InitDeviceView();
    InitSSDTreeWidget();
	graphicsManger->clearGraphics();
	graphicScene->clear();
	actionPre->setEnabled(false);
	actionNext->setEnabled(false);
	substationManageWin->hide();
	QApplication::processEvents();
	// logsrv_output_info(logsrv, L"SetCurrentSubstationID End", L"GraphicBrowserFrm::SubStationChangeProc");
	setCenterLayer(SHOW_ToTal_IED);
	QString subKey = "220kV." + curSubStationID;
	QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo(subKey);

	QString subName = subInfo->fileName;
	setWindowTitle(TrivalInfo + "SCD���ӻ�" +  " - " + subInfo->fileName);
	IEDsTreeDockWidget->setWindowTitle(getDockTitleName() + subInfo->Name);

	graphicsViewDockWidget->setWindowTitle("��ǰװ�� : ��");

	treeTabWidget->clear();
	treeTabWidget->addTab(infoTreeContainerWidget,tr("������Ϣ"));
	treeTabWidget->addTab(treeContainerWidget,tr("IED�б�"));
	treeTabWidget->addTab(deviceView,tr("��վ����װ���ͺ�"));
	treeTabWidget->addTab(subnetTreeContainerWidget,tr("ͨ������"));
    treeTabWidget->addTab(ssdTreeContainerWidget, tr("һ���豸"));
	scdValidatorView->hide();
	treeTabWidget->setCurrentIndex(1);
    if (subName.right(3).compare("SSD", Qt::CaseInsensitive) == 0)
    {
        treeTabWidget->setCurrentIndex(4);
    }
}

void GraphicBrowserFrm::SubStationCompareProc()
{
	logsrv_scoped_output(logsrv, "����SubStationCompareProc����", "�˳�SubStationCompareProc����", "GraphicBrowserFrm::SubStationCompareProc");
	int count=0;
	int curRow=5555;
	int crow=5555;
	for(int i=0;i<tablesubstation->rowCount();i++)
	{
		QComboBox *combox=(QComboBox *)tablesubstation->cellWidget(i,0);
        if(combox->currentText()=="��׼�ļ�")
		{
			curRow=i;
			count++;
		}
		else if(combox->currentText()=="�Ƚ��ļ�")
		{
			crow=i;
			count++;
		}
	}

	if(count<2)
	{
		QMessageBox::about(this, tr("����"),tr(" ����ѡ��2��SCD�ļ�."));
		return;
	}
	if(count>2)
	{
		QMessageBox::about(this, tr("����"),tr(" ���ѡ��2��SCD�ļ�."));
		return;
	}

	if(curRow==5555)
	{
		QMessageBox::about(this, tr("����"),tr("����ͬʱѡ��Ƚ��ļ�."));
		return;
	}

	if(crow==5555)
	{
		QMessageBox::about(this, tr("����"),tr("����ͬʱѡ���׼�ļ�."));
		return;
	}
	// logsrv_output_info(logsrv, L"��ʼ��ȡ�����ļ�", L"GraphicBrowserFrm::SubStationCompareProc");
	bool isSelected = false;
	checkFlag=false;
	fileInfoSlectFlag = false;
	QSettings *config = new QSettings("../cfg/CompareConf", QSettings::IniFormat);
	//��ȡ�û�������Ϣ
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		ConfigureInfo[i] = config->value("config/" + m_ConfPair[i].confEnDesc).toBool();

		if (ConfigureInfo[i])
		{
			if(i>2)
			{
				checkFlag=true;
			}else
			{
				fileInfoSlectFlag = true;
			}
			
			isSelected = true;
		}
	}
	// logsrv_output_info(logsrv, L"������ȡ�����ļ�", L"GraphicBrowserFrm::SubStationCompareProc");
	if (!isSelected)
	{
		QMessageBox::warning(this, tr("SCD�Ƚ�"), tr("�������������"), QMessageBox::Close);
		ShowSubStationCompareConfigProc();
		return;
	}
	if (!checkFlag)
	{
		if (QMessageBox::No == QMessageBox::warning(NULL, "���컯�Ƚ�", "������Ϣ��IED��Ϣδѡ���Ƿ������", QMessageBox::Yes, QMessageBox::No))
		{
			return;
		}
	}
	// logsrv_output_info(logsrv, L"�ر���������", L"GraphicBrowserFrm::SubStationCompareProc");
	if (vDialog->GetDialogLocked())
	{
		if (!lockedDialogVec.contains(vDialog))
		{
			lockedDialogVec.append(vDialog);
		}
	}
	else
	{
		vDialog->close();
	}
	closeUnlockedVirDlg();
	
 	if (lockedDialogVec.size() != 0)
	{
		if (QMessageBox::information(this, "�Ƿ�ر�", "��⵽����������,�Ƿ�ر�??", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
		{
			closeAllVirDlgs();
		}
	}

	graphicScene->changeModel(true);
	// logsrv_output_info(logsrv, L"�����ر���������", L"GraphicBrowserFrm::SubStationCompareProc");
	isComparing = true;
	ClearTableWidgetData(); // qing kong 
	ListBaseName.clear();
	proCompareFlg=true;
	QVector<QString> *substationNameList = IEDsLoader->GetSubstationNameList();
	QString baseName = substationNameList->at(curRow);
	QString otherName = substationNameList->at(crow);
	QString baseKey = "220kV." + baseName;
	QString otherKey = "220kV." + otherName;
	QSubStationInfo baseInfo, otherInfo;
	QSubStationInfo *tempSubInfo = IEDsLoader->GetSubstationInfo(baseKey);
	baseInfo.Name = tempSubInfo->Name;
	baseInfo.desc = tempSubInfo->desc;
	baseInfo.fileName = tempSubInfo->fileName;
	baseInfo.version = tempSubInfo->version;
	baseInfo.reversion = tempSubInfo->reversion;
	tempSubInfo = IEDsLoader->GetSubstationInfo(otherKey);
	otherInfo.Name = tempSubInfo->Name;
	otherInfo.desc = tempSubInfo->desc;
	otherInfo.fileName = tempSubInfo->fileName;
	otherInfo.version = tempSubInfo->version;
	otherInfo.reversion = tempSubInfo->reversion;
	QStringList baseNameList = baseName.split("#");
	QStringList otherNameList = otherName.split("#");

	ListBaseName.append(baseInfo.fileName);
	ListBaseName.append(otherInfo.fileName);

	//���ǲ��컯����ΪNULL
	IEDsTreeWidget->setCurrentItem(NULL);

	substationWin->hide();
	// logsrv_output_info(logsrv, L"substationWin->hide()����", L"GraphicBrowserFrm::SubStationCompareProc");
	progresCompareStyle->setPorgressValue(0);
	//QString textFile=baseInfo->fileName+"��"+otherInfo->fileName+"�Ƚ�:";
	QString textFile="  ���ڱȽ�SCD�ļ�:";
	progresCompareStyle->setFileText(textFile,0,0);
	progresCompareStyle->show();
	timerCompare->start(200);

    comparethread->setKey(baseKey,otherKey);
	comparethread->start();

	graphicsViewDockWidget->setWindowTitle("��ǰװ�� : ��");
}
void GraphicBrowserFrm::FinishCompareThread(bool result)
{
	timerCompare->stop();   //ֹͣ����
	progresCompareStyle->setPorgressValue(100);
	if (!result)
	{
		progresCompareStyle->setLabeCurrentText("�Ƚ�ʧ��");
		progresCompareStyle->hide();
		proCompareFlg=false;
		Endcompare=true;
		SubStationCompareEndProc();
		QMessageBox::warning(this, "���컯�Ƚ�", "�ڴ治�㣬�������Ƚϣ�");
		return;
	}
	progresCompareStyle->setLabeCurrentText("�Ƚ����");
	QString filename="[%1]��[%2]�Ƚϳɹ�";
	QString realfilename=filename.arg(ListBaseName.at(0)).arg(ListBaseName.at(1));
	//QMessageBox::about(this,tr("�ɹ�"),tr(realfilename));
	// QMessageBox::about(this,tr("�Ƚϳɹ�"),realfilename);
	progresCompareStyle->hide();
	proCompareFlg=false;
	Endcompare=false;

	InitCompareWidget();
	treeTabWidget->removeTab(4);
	treeTabWidget->removeTab(3);
	treeTabWidget->removeTab(2);
	treeTabWidget->removeTab(1);
	treeTabWidget->removeTab(0);
	treeTabWidget->addTab(DiffTreeContainerWidget,tr("IEDһ��"));
	setCenterLayer(NODE_TYPE_COMPARE_IED);
	actionCompareEnd->setVisible(true);
	actionCompareEnd->setEnabled(true);
	actionExportICDInfo->setDisabled(true);

	actionExportFile->setVisible(true);
	actionExportWordFile->setVisible(true);
	actionCompare->setCheckable(false);
	
	actionCheck->setDisabled(true);
	actionImportSCD->setEnabled(false);
	actionSwitch->setEnabled(false);
	actionDelete->setEnabled(false);

	graphicsManger->clearGraphics();
	actionPre->setEnabled(false);
	actionNext->setEnabled(false);
	
	graphicScene->clear();
	QString winTitle = TrivalInfo + "SCD�Ƚ� : [" + ListBaseName.at(0) +"]" +"��[" + ListBaseName.at(1)+"]";
	setWindowTitle(winTitle);
	IEDsTreeDockWidget->setWindowTitle("SCD�Ƚ� : [" + ListBaseName.at(0) +"]" +"��[" + ListBaseName.at(1)+"]");
}

void GraphicBrowserFrm::FinishCheckThread()
{
	qDebug()<<::GetCurrentProcessId()<<"FinishCheckThread			Start";
	timerProgress->stop();   //ֹͣ����
	qDebug()<<::GetCurrentProcessId()<<"timerProgress->stop()		End";
	progresCheckStyle->setPorgressValue(100);
	progresCheckStyle->setLabeCurrentText("������");
	progresCheckStyle->hide();
	proCheckFlg = false;
	

	InitSCDValidatorTreeWidget();
	treeTabWidget->clear();
	treeTabWidget->addTab(infoTreeContainerWidget,tr("������Ϣ"));
	treeTabWidget->addTab(scdValidatorWidget, tr("У����"));
	treeTabWidget->addTab(treeContainerWidget,tr("IED�б�"));
	treeTabWidget->addTab(deviceView,tr("��վ����װ���ͺ�"));
	treeTabWidget->addTab(subnetTreeContainerWidget,tr("ͨ������"));
    treeTabWidget->addTab(ssdTreeContainerWidget, tr("һ���豸"));
	treeTabWidget->setCurrentIndex(1);
	ExportValidateExcelButton->show();
	scdValidatorTreeWidget->show();
    totalTab->clear();
    totalTab->tabbarhide();
    totalTab->addTab(scdValidatorView,"validator");
	qDebug()<<::GetCurrentProcessId()<<"FinishCheckThread			End";
}

void GraphicBrowserFrm::SubStationCompareEndProc()
{
    qDebug()<<"SubStationCompareEndProc----------------1";
	scdDiffCompare->ClearMap();
	graphicScene->changeModel(false);

    std::vector<std::string> loadList = SclDacLib::GetLoadedFileMarks();
    for(uint i=0; i<loadList.size(); i++)
    {
        SclDacLib::UnloadScdFile(loadList.at(i));
    }
    qDebug()<<"SubStationCompareEndProc----------------2";
	ClearTableWidgetData();
    qDebug()<<"SubStationCompareEndProc----------------222";
	actionCompare->setCheckable(true);
	actionCompareEnd->setEnabled(false);
	actionCompareEnd->setVisible(false);
	actionExportFile->setVisible(false);
	actionExportWordFile->setVisible(false);
	actionExportICDInfo->setDisabled(false);
	actionCheck->setEnabled(true);
	
	actionImportSCD->setEnabled(true);
	actionSwitch->setEnabled(true);
	actionDelete->setEnabled(true);
    qDebug()<<"SubStationCompareEndProc----------------3";
	Endcompare=true;
	treeTabWidget->removeTab(0);
	graphicScene->clear();
	graphicsManger->clearGraphics();
	treeTabWidget->addTab(infoTreeContainerWidget,tr("������Ϣ"));
	treeTabWidget->addTab(treeContainerWidget,tr("IED�б�"));
	treeTabWidget->addTab(deviceView,tr("��վ����װ���ͺ�"));
	treeTabWidget->addTab(subnetTreeContainerWidget,tr("ͨ������"));
    treeTabWidget->addTab(ssdTreeContainerWidget, tr("һ���豸"));
	treeTabWidget->setCurrentIndex(1);

	tableModel = tableModelImpl;
	isComparing = false;
    qDebug()<<"SubStationCompareEndProc----------------4";
	QString subStation = subSetting->value("config/CurrentSubstationID").toString();

	IEDsLoader->SetCurrentSubstationID(subStation);
    m_SSDInfo->SetCurrentSubstation(subStation);
	QString subKey = "220kV." + IEDsLoader->GetCurrentSubstationID();
	QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo(subKey);
    qDebug()<<"SubStationCompareEndProc----------------5";
	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
    InitSSDTreeWidget();
    qDebug()<<"SubStationCompareEndProc----------------6";
	setCenterLayer(SHOW_INFO_TABLE);
	graphicsViewDockWidget->setWindowTitle("��ǰװ�� : ��");
	graphicsManger->clearGraphics();
	actionPre->setEnabled(false);
	actionNext->setEnabled(false);
	setCenterLayer(SHOW_ToTal_IED);
	//chenxin modify 2014.10.21 --->
	setWindowTitle(TrivalInfo + "SCD���ӻ� " + " - " + subInfo->fileName);
	IEDsTreeDockWidget->setWindowTitle(getDockTitleName() + subInfo->Name);
    qDebug()<<"SubStationCompareEndProc----------------7";
    if (subInfo->fileName.right(3).compare("SSD", Qt::CaseInsensitive) == 0)
    {
        treeTabWidget->setCurrentIndex(4);
    }
	//chenxin modify 2014.10.21 <---
}

void GraphicBrowserFrm::SubStationCompareExportProc()
{
	CXLCreat xlCreat;
	xlCreat.Run();
}
void GraphicBrowserFrm::SubStationCompareExportWordProc()  // word
{
	CWordCreat docCreat;
	docCreat.CreatHtml();
}

void GraphicBrowserFrm::queryIEDProc(const QString& iedStr)
{
	if (iedStr.isEmpty())
	{
		InitIEDsTreeWidget();
	} 
	else
	{
		QString tempStr = iedStr.toLower();
		QVector<QIEDInfo> selectedIEDs;
		//QVector<SubNetItem> selectedSubNets;
		selectedIEDs.clear();
		//selectedSubNets.clear();
		//QVector<SubNetItem> *subNets = IEDsLoader->GetSubNetList();
		QVector<QIEDInfo> *iedInfoList = IEDsLoader->GetIEDInfoList();
		//for (int sIndex = 0; sIndex != subNets->size(); sIndex++)
		//{
		//	SubNetItem tempItem = subNets->at(sIndex);
		//	QString subName = tempItem.subNetName;
		//	if (subName.toLower().contains(tempStr))
		//	{
		//		selectedSubNets.append(tempItem);
		//	}
		//}
		for (int i = 0; i != iedInfoList->size(); i++)
		{
			QIEDInfo iedInfo = iedInfoList->at(i);
			QString iedName = iedInfo.name.toLower();
			QString iedDesc = iedInfo.desc.toLower();
			if (iedName.contains(tempStr) || iedDesc.contains(tempStr))
			{
				selectedIEDs.append(iedInfo);
			}
		}
		if (selectedIEDs.size() == 0)
		{
			IEDsTreeWidget->clear();
			QTreeWidgetItem *nothingItem = new QTreeWidgetItem(IEDsTreeWidget,NODE_TYPE_NOTHING);
			nothingItem->setText(0, "δ�ҵ���Ŀ");
			IEDsTreeWidget->addTopLevelItem(nothingItem);
			return;
		}
		IEDsTreeWidget->clear();
		QStringList lists;
		lists<<"����ͼ";
		IEDsTreeWidget->setHeaderLabels(lists);
		IEDsTreeWidget->header()->hide();
		QTreeWidgetItem /**subNetRootItem, *subNetItem, */*IEDItemRoot, *IEDItem;
		//subNetRootItem = new  QTreeWidgetItem(IEDsTreeWidget,NODE_TYPE_SUBNET_ROOT);
		//subNetRootItem->setText(0,"����");
		//subNetRootItem->setIcon(0, QIcon("images/greenStone.png"));   
		//IEDsTreeWidget->addTopLevelItem(subNetRootItem);  
		//for(int i=0;i<IEDsLoader->GetSubNetList()->size();i++)
		//{
		//	subNetItem = new  QTreeWidgetItem(subNetRootItem,NODE_TYPE_SUBNET);
		//	subNetItem->setText(0,IEDsLoader->GetSubNetList()->at(i).subNetName);
		//	subNetItem->setIcon(0, QIcon("images/redStone.png"));   
		//	IEDsTreeWidget->setItemExpanded(subNetItem,false);
		//	subNetRootItem->addChild(subNetItem); 
		//}
		IEDItemRoot = new QTreeWidgetItem(IEDsTreeWidget, NODE_TYPE_IED_ROOT);
		IEDItemRoot->setText(0, "����װ��");
		IEDItemRoot->setIcon(0, QIcon("images/greenStone.png"));   
		IEDsTreeWidget->addTopLevelItem(IEDItemRoot);

		QString substationName = IEDsLoader->GetCurrentSubstationID();
		QString subKey = "220kV." + substationName;
		for(int j = 0; j != selectedIEDs.size(); j++)
		{
			QString IEDKey = subKey + ".����." + selectedIEDs.at(j).name;
			QString IEDName = "[" + selectedIEDs.at(j).name + "]" + selectedIEDs.at(j).desc;
			IEDItem = new QTreeWidgetItem(IEDItemRoot,NODE_TYPE_IED);
			IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
			IEDItem->setText(0,IEDName);
			IEDItem->setToolTip(0,IEDName);
			IEDItem->setIcon(0,QIcon("images/graphic_two.png"));
			IEDItemRoot->addChild(IEDItem);
		}
		IEDItemRoot->setExpanded(true);
	}
}

void GraphicBrowserFrm::queryDiffIEDProc(const QString& iedStr)
{
	int equalNumber=0,editNumber=0,addNumber=0,delNumber=0;

	if (iedStr.isEmpty())
	{
		InitCompareWidget();
	} 
	else
	{
		QString tempStr = iedStr.toLower();
		QVector<IEDStru> selectedIEDs;
		selectedIEDs.clear();
		QVector<QString> iedList;
		tableModel->GetIEDList("", &iedList);
		for (int i = 0; i != iedList.size(); i++)
		{
			IEDStru DiffIedInfo;
			tableModel->GetIEDInfo(iedList.at(i), DiffIedInfo);
			QString iedName = DiffIedInfo.IEDName.toLower();
			QString iedDesc = DiffIedInfo.IEDDesc.baseAttributeName.toLower();
			if (iedName.contains(tempStr) || iedDesc.contains(tempStr))
			{
				selectedIEDs.append(DiffIedInfo);
			}
		}
		if (selectedIEDs.size() == 0)
		{
			compareModel->clear();
			QTreeWidgetItem *nothingItem = new QTreeWidgetItem(compareModel,NODE_TYPE_NOTHING);
			nothingItem->setText(0, "δ�ҵ���Ŀ");
			compareModel->addTopLevelItem(nothingItem);
			return;
		}
		compareModel->clear();
		QStringList lists;
		lists<<"����ͼ";
		compareModel->setHeaderLabels(lists);
		compareModel->header()->hide();
		QTreeWidgetItem /**subNetRootItem, *subNetItem, */*IEDItemRoot, *IEDItem;
		QTreeWidgetItem  *editItemRoot,*addItemRoot,*delItemRoot, *equalItemRoot;

		//IEDItemRoot = new QTreeWidgetItem(compareModel, NODE_TYPE_IED_ROOT);
		//IEDItemRoot->setText(0, "����װ��");
	//	IEDItemRoot->setIcon(0, QIcon("images/greenStone.png"));   
		//IEDsTreeWidget->addTopLevelItem(IEDItemRoot);

		// δ�����
		equalItemRoot = new QTreeWidgetItem(compareModel, NODE_TYPE_IED_ROOT);
		equalItemRoot->setText(0,"δ�����IED");
		equalItemRoot->setIcon(0,QIcon("images/equal_sign.png"));
		IEDsTreeWidget->addTopLevelItem(equalItemRoot);
		//�����
		editItemRoot = new QTreeWidgetItem(compareModel, NODE_TYPE_IED_ROOT);
		editItemRoot->setText(0,"�����IED");
		editItemRoot->setIcon(0,QIcon("images/modify_sign.png"));
		IEDsTreeWidget->addTopLevelItem(editItemRoot);
		//���ӵ� 
		addItemRoot = new QTreeWidgetItem(compareModel, NODE_TYPE_IED_ROOT);
		addItemRoot->setText(0,"���ӵ�IED");
		addItemRoot->setIcon(0,QIcon("images/add_sign.png"));
		IEDsTreeWidget->addTopLevelItem(addItemRoot);
		//ɾ����
		delItemRoot = new QTreeWidgetItem(compareModel, NODE_TYPE_IED_ROOT);
		delItemRoot->setText(0,"ɾ����IED");
		delItemRoot->setIcon(0,QIcon("images/delete_sign.png"));
		IEDsTreeWidget->addTopLevelItem(delItemRoot);

		for(int j = 0; j != selectedIEDs.size(); j++)
		{
			QString IEDKey = "����." + selectedIEDs.at(j).IEDName;
			QString IEDName;
			if (selectedIEDs.at(j).IEDDesc.baseAttributeName != "")
			{
				IEDName = "[" + selectedIEDs.at(j).IEDName + "]" + selectedIEDs.at(j).IEDDesc.baseAttributeName;
			} 
			else
			{
				IEDName = "[" + selectedIEDs.at(j).IEDName + "]" + selectedIEDs.at(j).IEDDesc.otherAttributeName;
			}
			/*IEDItem = new QTreeWidgetItem(editItemRoot,NODE_TYPE_IED);
			IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
			IEDItem->setText(0,IEDName);
			IEDItem->setToolTip(0,IEDName);*/
			if (selectedIEDs.at(j).state == DIFFSTATE_EDITED)
			{  
				editNumber++;
				IEDItem = new QTreeWidgetItem(editItemRoot,NODE_TYPE_IED);
				IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
				IEDItem->setText(0,IEDName);
				IEDItem->setToolTip(0,IEDName);
				IEDItem->setIcon(0, QIcon("images/modify_sign.png"));
			}
			else if(selectedIEDs.at(j).state == DIFFSTATE_ADDED)
			{
				addNumber++;
				IEDItem = new QTreeWidgetItem(addItemRoot,NODE_TYPE_IED);
				IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
				IEDItem->setText(0,IEDName);
				IEDItem->setToolTip(0,IEDName);
				IEDItem->setIcon(0, QIcon("images/add_sign.png"));
			}
			else if (selectedIEDs.at(j).state == DIFFSTATE_REMOVED)
			{
				delNumber++;
				IEDItem = new QTreeWidgetItem(delItemRoot,NODE_TYPE_IED);
				IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
				IEDItem->setText(0,IEDName);
				IEDItem->setToolTip(0,IEDName);
				IEDItem->setIcon(0, QIcon("images/delete_sign.png"));
			}
			else
			{
				equalNumber++;
				IEDItem = new QTreeWidgetItem(equalItemRoot,NODE_TYPE_IED);
				IEDItem->setData(0,Qt::AccessibleTextRole,IEDKey);
				IEDItem->setText(0,IEDName);
				IEDItem->setToolTip(0,IEDName);
				IEDItem->setIcon(0, QIcon("images/equal_sign.png"));
			}

		//	IEDItemRoot->addChild(IEDItem);
		//	editItemRoot->addChild(IEDItem);
		}

		equalItemRoot->setText(0,"δ�����IED ("+QString::number(equalNumber,10)+")");
		editItemRoot->setText(0,"�����IED ("+QString::number(editNumber,10)+")");
		addItemRoot->setText(0,"���ӵ�IED ("+QString::number(addNumber,10)+")");
		delItemRoot->setText(0,"ɾ����IED ("+QString::number(delNumber,10)+")");

		equalItemRoot->setExpanded(true);
		editItemRoot->setExpanded(true);
		addItemRoot->setExpanded(true);
		delItemRoot->setExpanded(true);

		//IEDItemRoot->setExpanded(true);
	}
}

void GraphicBrowserFrm::ClearDBProc()
{
	removeDataFiles();

	QList<QString> strList = filePath.split("/");
	QString fName = strList.at(strList.size() - 1);
	QString fDir = filePath.left(filePath.size() - fName.size());

	QFile nameSaver("../cfg/FileNameSaver.txt");
	if (nameSaver.open(QIODevice::WriteOnly))
	{
		QTextStream out(&nameSaver);
		out << fDir;
		nameSaver.close();
	}
}

void GraphicBrowserFrm::CheckSCDProc()
{
	QString fileName = QFileDialog::getOpenFileName(this,"��",filePath);
	if (fileName.isEmpty())
		return;

	QString houzhui = fileName.right(4);
	if (houzhui.toLower().compare(".scd") != 0)
	{
		QMessageBox::warning(this,
			tr("�ļ�����"),
			tr("�����ļ���SCD�ļ�!"));
		return;
	}

	SCDValidator scdValidator;
	scdValidator.SCDValidate(fileName);

	QString scdName = scdValidator.getName();
	QString scdVersion = scdValidator.getVersion();
	QString scdReversion = scdValidator.getReversion();

	int errorSize = scdValidator.getErrorSize();
	int warningSize = scdValidator.getWarningSize();

	QVector<QString> errorNameVec = scdValidator.getErrorNameVec();
	QVector<QString> errorValueVec = scdValidator.getErrorValueVec();
	QVector<QString> warningNameVec = scdValidator.getWarningNameVec();
	QVector<QString> warningValueVec = scdValidator.getWarningValueVec();

	QMainWindow *checkWin = new QMainWindow();
	QSplitter *vSplitter = new QSplitter(Qt::Vertical);

	QTableView *errorTable = new QTableView(checkWin);
	QStandardItemModel *errorModel = new QStandardItemModel(checkWin);

	QBrush b;
	b.setColor(Qt::red);

	QStandardItem *errItem = new QStandardItem(tr("����(Error)"));
	errItem->setBackground(b);
	QStandardItem *errSizeItem = new QStandardItem(tr("��(") + QString::number(errorSize) + tr(")��"));
	errSizeItem->setBackground(b);

	for (int i = 0; i < errorSize; i++)
	{
		QList<QStandardItem*> eItemList;
		eItemList.append(new QStandardItem(errorNameVec.at(i)));
		eItemList.append(new QStandardItem(errorValueVec.at(i)));
		errorModel->appendRow(eItemList);
	}

	errorModel->setHorizontalHeaderItem(0,errItem);
	errorModel->setHorizontalHeaderItem(1,errSizeItem);
	errorTable->setModel(errorModel);
	errorTable->setColumnWidth(0,100);
	errorTable->setColumnWidth(1,1000);

	QTableView *warningTable = new QTableView(checkWin);
	QStandardItemModel *warningModel = new QStandardItemModel(checkWin);

	b.setColor(Qt::yellow);
	QStandardItem *warnItem = new QStandardItem(tr("����(Warning)"));
	warnItem->setBackground(b);
	QStandardItem *warnSizeItem = new QStandardItem(tr("��(") + QString::number(warningSize) + tr(")��"));
	warnSizeItem->setBackground(b);

	for (int j = 0; j < warningSize; j++)
	{
		QList<QStandardItem*> wItemList;
		wItemList.append(new QStandardItem(warningNameVec.at(j)));
		wItemList.append(new QStandardItem(warningValueVec.at(j)));
		warningModel->appendRow(wItemList);
	}

	warningModel->setHorizontalHeaderItem(0,warnItem);
	warningModel->setHorizontalHeaderItem(1,warnSizeItem);
	warningTable->setModel(warningModel);
	warningTable->setColumnWidth(0,100);
	warningTable->setColumnWidth(1,1000);

	vSplitter->addWidget(errorTable);
	vSplitter->addWidget(warningTable);

	checkWin->setCentralWidget(vSplitter);
	checkWin->setWindowModality(Qt::WindowModal);
	checkWin->setMinimumSize(1200,700);
	checkWin->show();
}

void GraphicBrowserFrm::UpdateTableDate(bool ForBack)
{


}

void GraphicBrowserFrm::PreProc()
{
	actionNext->setEnabled(true);
	if(graphicsManger->MovePreGraphic())
	{
		UpdateGraphicsView();
	//	UpdateTableDate(false);
		RecoveryProc();

		int curGraphicType = graphicsManger->GetCurGraphicType();

		if (GRAPHIC_TYPE_NO_INFOMATION_FLOW == curGraphicType || GRAPHIC_TYPE_NO_COMPARE == curGraphicType)
		{
			totalTab->setIEDKey(graphicsManger->GetCurGraphicIEdName());
			if (GRAPHIC_TYPE_NO_INFOMATION_FLOW == curGraphicType)
			{
				setCenterLayer(SHOW_INFO_TABLE);
			} 
			else
			{
				setCenterLayer(NODE_TYPE_COMPARE_IED);
			}
			IEDStru iedInfo;
			tableModel->GetIEDInfo(graphicsManger->GetCurGraphicIEdName(), iedInfo);
		//	fillXmlData(graphicsManger->GetCurGraphicIEdName());
			QString nameDesc;
			if (iedInfo.IEDDesc.baseAttributeName.compare("") != 0)
			{
				nameDesc = "��ǰװ��  �� [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.baseAttributeName;
			}
			else
			{
				nameDesc = "��ǰװ��  �� [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.otherAttributeName;
			}
			graphicsViewDockWidget->setWindowTitle(nameDesc);
			if(Endcompare)
			{
				//fillTabContent(graphicsManger->GetCurGraphicIEdName());
				setTotalTabsEnabled(graphicsManger->GetCurGraphicIEdName());
				totalTabChangeProc(totalTab->currentIndex());
				rightTabChangeProc(rightTab->currentIndex());
				bottmTabChangeProc(bottmTab->currentIndex());
			}
			else
			{
				fillTabContentDiff(graphicsManger->GetCurGraphicIEdName());
			}
		}
		else if(GRAPHIC_TYPE_NO_SUBNET == curGraphicType)
		{
			totalTab->setIEDKey(graphicsManger->GetCurGraphicIEdName());
			setCenterLayer(NODE_TYPE_SUBNET);
		}

		if(graphicsManger->IsFirstGraphic())
		{
			actionPre->setEnabled(false);
		}
	}
}

void GraphicBrowserFrm::NextProc()
{
	actionPre->setEnabled(true);
	if(graphicsManger->MoveNextGraphic())
	{
		UpdateGraphicsView();
		//UpdateTableDate(true);
		RecoveryProc();

		int curGraphicType = graphicsManger->GetCurGraphicType();

		if (GRAPHIC_TYPE_NO_INFOMATION_FLOW == curGraphicType || GRAPHIC_TYPE_NO_COMPARE == curGraphicType)
		{
			totalTab->setIEDKey(graphicsManger->GetCurGraphicIEdName());
			if (GRAPHIC_TYPE_NO_INFOMATION_FLOW == curGraphicType)
			{
				setCenterLayer(SHOW_INFO_TABLE);
			} 
			else
			{
				setCenterLayer(NODE_TYPE_COMPARE_IED);
			}
			IEDStru iedInfo;
			tableModel->GetIEDInfo(graphicsManger->GetCurGraphicIEdName(), iedInfo);
			//fillXmlData(graphicsManger->GetCurGraphicIEdName());
			QString nameDesc;
			if (iedInfo.IEDDesc.baseAttributeName.compare("") != 0)
			{
				nameDesc = "��ǰװ��  �� [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.baseAttributeName;
			}
			else
			{
				nameDesc = "��ǰװ��  �� [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.otherAttributeName;
			}
			graphicsViewDockWidget->setWindowTitle(nameDesc);
			if(Endcompare)
			{
				//fillTabContent(graphicsManger->GetCurGraphicIEdName());
				setTotalTabsEnabled(graphicsManger->GetCurGraphicIEdName());
				totalTabChangeProc(totalTab->currentIndex());
				rightTabChangeProc(rightTab->currentIndex());
				bottmTabChangeProc(bottmTab->currentIndex());
			}
			else
			{
				fillTabContentDiff(graphicsManger->GetCurGraphicIEdName());
			}
		}
		else if (GRAPHIC_TYPE_NO_SUBNET == curGraphicType)
		{
			totalTab->setIEDKey(graphicsManger->GetCurGraphicIEdName());
			setCenterLayer(NODE_TYPE_SUBNET);
		}

		if(graphicsManger->IsLastGraphic())
		{
			actionNext->setEnabled(false);
		}
	}
}

void GraphicBrowserFrm::ZoomInProc()
{
	if (graphicsView == NULL)
	{
		return;
	}
	graphicsView->scale(1.2, 1.2);
	graphicsManger->SetCurGraphicScale(graphicsManger->GetCurGraphicScale()*1.2);
}

void GraphicBrowserFrm::ZoomOutProc()
{
	if (graphicsView == NULL)
	{
		return;
	}
	graphicsView->scale(0.8, 0.8);
	graphicsManger->SetCurGraphicScale(graphicsManger->GetCurGraphicScale()*0.8);
}

void GraphicBrowserFrm::RoamProc()
{
	if (graphicsView == NULL)
	{
		return;
	}

	if (actionRoam->isChecked())
	{
		((HmiGraphicsView *)graphicsView)->setDragMode(QGraphicsView::ScrollHandDrag);
		((HmiGraphicsScene *)graphicsView->scene())->setDrag(true);
	}
	else
	{
		((HmiGraphicsView *)graphicsView)->setDragMode(QGraphicsView::RubberBandDrag);
		((HmiGraphicsScene *)graphicsView->scene())->setDrag(false);
	}
}

void GraphicBrowserFrm::RecoveryProc()
{
	if (graphicsView == NULL)
	{
		return;
	}

	//----
	//QRectF itemsRect;
	//CenterAllItems(itemsRect);
	//qreal x = itemsRect.center().x();
	//qreal y = itemsRect.center().y();
	//if (graphicsView == NULL)
	//{
	//	return;
	//}
	//graphicsView->centerOn(x,y);

	////graphicsView->scale(.9,.9);
	//QRectF rect = itemsRect;//((HmiGraphicsView *)graphicsView)->sceneRect();
	//double newScale = 0.9;//graphicsView->width() / rect.width() * 3.5;//0.85;
	//double newScale1 =0.9;// graphicsView->height() / rect.width() * 3.5;//0.85;

	//QMatrix oldMatrix = graphicsView->matrix();
	//graphicsView->resetMatrix();
	//graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
	////graphicsView->scale(qMin(newScale, newScale1),qMin(newScale, newScale1));
	//graphicsView->scale(qMin(newScale, newScale1),qMin(newScale, newScale1));

	//---
	double graphicScale = 0.95;
	int graphicType = graphicsManger->GetCurGraphicType();
	if (graphicType == GRAPHIC_TYPE_NO_INFOMATION_FLOW || graphicType == GRAPHIC_TYPE_NO_COMPARE)
	{
		graphicScale = 1.05;
	}
	else if (graphicType == GRAPHIC_TYPE_NO_VIRTUAL_LINK_DIFF_GOOSE || graphicType == GRAPHIC_TYPE_NO_VIRTUAL_LINK_DIFF_SV)
	{
		graphicScale = 0.9;
	}


	QRectF rect = ((HmiGraphicsView *)graphicsView)->sceneRect();
	double newScale = graphicsView->width() / rect.width() * graphicScale;
	//double newScale1 = graphicsView->height() / rect.width() * 0.85;
	//double newScale1 = graphicsView->height() / rect.height() * 0.9;

	QMatrix oldMatrix = graphicsView->matrix();
	graphicsView->resetMatrix();
	graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
	graphicsView->scale(newScale,newScale);
	graphicsView->verticalScrollBar()->setSliderPosition(0);
	graphicsView->horizontalScrollBar()->setSliderPosition(0);
}

void GraphicBrowserFrm::ShowToolbarProc()
{
	bool visiblemainToolbar = mainToolbar->isVisible();
	mainToolbar->setVisible(!visiblemainToolbar);	
}

void GraphicBrowserFrm::ShowStatusbarProc()
{
	bool visibleStatusBar = statusBar()->isVisible();
	statusBar()->setVisible(!visibleStatusBar);	
}

void GraphicBrowserFrm::ShowOutSignalInTerminalGraphic()
{
    bool iShowOutSignal = actionShowOutSignal->isChecked();
    QSettings *setting = new QSettings("../cfg/SubStationInfo.ini", QSettings::IniFormat);
    
    if (iShowOutSignal)
    {
        setting->setValue("config/ShowOutSignalInTerminalGraphic", true);
    }
    else
    {
        setting->setValue("config/ShowOutSignalInTerminalGraphic", false);
    }
    
}

void GraphicBrowserFrm::SaveAsProc()
{
	// �õ���ǰչʾIED�����Ƽ�����
	QString PNGName;
	if (GRAPHIC_TYPE_NO_SUBNET == graphicsManger->GetCurGraphicType())
	{
		PNGName = "����ͼ";
	}else if ((0 < graphicsManger->GetCurGraphicType()) &&
		(6 > graphicsManger->GetCurGraphicType()) )
	{
		QString IEDKey = graphicsManger->GetCurGraphicIEdName();
		QIEDInfo iedInfo;
		graphicScene->GetSclModel()->GetIEDInfo(IEDKey, iedInfo);
		PNGName = "[" + iedInfo.name + "]" + iedInfo.desc;
	}else
	{
		PNGName = "ͼƬ" ;
	}
	


	QString path = QFileDialog::getSaveFileName(this, tr("���Ϊ PNG"), PNGName, tr("png �ļ� (*.png)"));

	if (path.isEmpty())
		return;

	if (graphicScene == NULL)
	{
		return;
	}

	QImage image(graphicScene->width(),graphicScene->height(),QImage::Format_RGB32);
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	graphicScene->render(&painter);
	// bool rtn = image.save(path);
	if (image.save(path))
	{
		QMessageBox::information(NULL, "ͼƬ���Ϊ", "ͼƬ����ɹ���");
	}
	else
	{
		QMessageBox::information(NULL, "ͼƬ���Ϊ", "ͼƬ����ʧ�ܣ�");
	}
}

/** 
* @brief    ExportICDInfoProc
* 
* ����ICD�汾��Ϣ
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/9/9
* @remarks  
*/ 
void GraphicBrowserFrm::ExportICDInfoProc()
{
	QString SubstationID = IEDsLoader->GetCurrentSubstationID();
	if (SubstationID == "")
	{
		QMessageBox::warning(NULL, tr("����ICD�汾��Ϣ"), tr("��ǰ���վΪ��!"));
		return;
	}
	QString subKey = "220kV." + SubstationID;
	CExportICDInfo cExportInfo;
	cExportInfo.SetSubStationKey(subKey);
	cExportInfo.ExportICDVersionInfo();
}

void GraphicBrowserFrm::PrintProc()
{
}

void GraphicBrowserFrm::VoltageLevelChangedProc(int index)
{
	IEDsLoader->SetCurrentVoltageLevelIndex(index);
	IEDsLoader->LoadVoltageLevelNameList();
	IEDsLoader->SetCurrentSubstationID(IEDsLoader->GetSubstationNameList()->at(0));
	m_SSDInfo->SetCurrentSubstation(IEDsLoader->GetSubstationNameList()->at(0));
	InitSubstationCombBox();
	IEDsLoader->LoadSubstationNameList();
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadHItemList();

	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
	// InitSCDValidatorTreeWidget();
	setCenterLayer(SHOW_ToTal_IED);
}

void GraphicBrowserFrm::SubStationChangedProc(int index)
{
	QString subStationName = IEDsLoader->GetSubstationNameList()->at(index);
	IEDsLoader->SetCurrentSubstationID(subStationName);
    m_SSDInfo->SetCurrentSubstation(subStationName);
	IEDsLoader->LoadSubstationNameList();
	IEDsLoader->LoadSubNetList();
	IEDsLoader->LoadIEDList();
	IEDsLoader->LoadHItemList();
	InitIEDsTreeWidget();
	InitSubInfoTreeWidget();
	InitSubnetTreeWidget();
    InitSSDTreeWidget();
	//InitSCDValidatorTreeWidget();

	setCenterLayer(SHOW_ToTal_IED);
	timer->stop();
}

void GraphicBrowserFrm::timerEvent(QTimerEvent* event)
{
	if(event->timerId() ==  progressTimerID)
	{
		//analysisProgressBar->setValue(scdManger->GetCurSubStationProcProgress());
		//progresStyle->setPorgressValue(scdManger->GetCurSubStationProcProgress());
	}
	else
	{
		QWidget::timerEvent(event);
	}
}

void GraphicBrowserFrm::ShowEvent()
{
	//progressTimerID = startTimer(3000);
}

void GraphicBrowserFrm::HideEvent()
{
	killTimer(progressTimerID);
	progressTimerID = 0;
}

void GraphicBrowserFrm::timePro()
{
	//int curProgress = graphicsManger->GetAnalysisProgress();
	//analysisProgressBar->setValue(0);
}

void GraphicBrowserFrm::InitProgressDlg()
{
	progressDlg = NULL;
}

void GraphicBrowserFrm::tripClickedProc(QString masterKey, QString slaveKey, int type)
{
	if (vDialog->GetDialogLocked())
	{
		if (!lockedDialogVec.contains(vDialog))
		{
			lockedDialogVec.append(vDialog);
		}
	}
	else
	{
		if (type != 2 && type != 3)
		{
			vDialog->close();
		}
	}
	closeUnlockedVirDlg();
	//graphicsManger->LoadGraphicData(IEDsLoader->GetIEDItemFromIEDName("վ�����װ��").IEDKey,"",GRAPHIC_TYPE_NO_INFOMATION_FLOW);
	IEDStru iedInfo;
	QSubNetInfo subnetInfo;
	QString nameDesc;
	switch (type)
	{
	case 0:
		actionPre->setEnabled(PreFlag);
		actionNext->setEnabled(NextFlag);
		graphicOpersEnable(true);
		graphicsManger->LoadGraphicData(masterKey,"",GRAPHIC_TYPE_NO_SUBNET);
		IEDsTreeWidget->setCurrentItem(NULL);
		subnetTreeWidget->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		totalTab->setIEDKey(masterKey);
		setCenterLayer(NODE_TYPE_SUBNET);
		graphicScene->GetSclModel()->GetSubNetInfo(masterKey,subnetInfo);
		graphicsViewDockWidget->setWindowTitle(subnetInfo.name);
		break;
	case 1:
		actionPre->setEnabled(PreFlag);
		actionNext->setEnabled(NextFlag);
		graphicOpersEnable(true);
		tableModelImpl->GetIEDInfo(masterKey,iedInfo);
		graphicsManger->LoadGraphicData(masterKey,"",GRAPHIC_TYPE_NO_INFOMATION_FLOW);
		totalTab->setIEDKey(masterKey);
		setCenterLayer(SHOW_INFO_TABLE);
		//fillTabContent(masterKey);
		setTotalTabsEnabled(masterKey);
		totalTabChangeProc(totalTab->currentIndex());
		rightTabChangeProc(rightTab->currentIndex());
		bottmTabChangeProc(bottmTab->currentIndex());
		IEDsTreeWidget->setCurrentItem(NULL);
		subnetTreeWidget->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		graphicsViewDockWidget->setWindowTitle("��ǰװ�� : [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.baseAttributeName);
		break;
	case 2:
		if (vDialog->GetDialogLocked())
		{
			CVirLoopDialog *tempVDlg = new CVirLoopDialog(this);
			vDialog = tempVDlg;
		}
		vDialog->DrawVirLoopGraphic(masterKey, slaveKey, type);
		vDialog->show();
		//graphicsManger->LoadGraphicData(masterKey,slaveKey,GRAPHIC_TYPE_NO_VIRTUAL_LINK_GOOSE);
		IEDsTreeWidget->setCurrentItem(NULL);
		break;
	case 3:
		if (vDialog->GetDialogLocked())
		{
			CVirLoopDialog *tempVDlg = new CVirLoopDialog(this);
			vDialog = tempVDlg;
		}
		vDialog->DrawVirLoopGraphic(masterKey, slaveKey, type);
		vDialog->show();
		//graphicsManger->LoadGraphicData(masterKey,slaveKey,GRAPHIC_TYPE_NO_VIRTUAL_LINK_SV);
		IEDsTreeWidget->setCurrentItem(NULL);
		break;
	case 4:
		actionPre->setEnabled(PreFlag);
		actionNext->setEnabled(NextFlag);
		graphicOpersEnable(true);
		graphicsManger->LoadGraphicData(masterKey,"",GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL);
		IEDsTreeWidget->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		break;
	case 5:
		graphicsManger->LoadGraphicData(masterKey,slaveKey,GRAPHIC_TYPE_NO_VIRTUAL_LINK_DIFF_GOOSE);
		compareModel->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		break;
	case 6:
		graphicsManger->LoadGraphicData(masterKey,slaveKey,GRAPHIC_TYPE_NO_VIRTUAL_LINK_DIFF_SV);
		compareModel->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		break;
	case 7:
		graphicsManger->LoadGraphicData(masterKey,slaveKey,GRAPHIC_TYPE_NO_COMPARE);
		compareModel->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		fillTabContentDiff(masterKey);
		tableModelDiffImpl->GetIEDInfo(masterKey,iedInfo);
		if (iedInfo.IEDDesc.baseAttributeName.compare("") != 0)
		{
			nameDesc = "��ǰװ�� : [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.baseAttributeName;
		} 
		else
		{
			nameDesc = "��ǰװ�� : [" + iedInfo.IEDName + "]" + iedInfo.IEDDesc.otherAttributeName;
		}
		graphicsViewDockWidget->setWindowTitle(nameDesc);
		break;
	case 8:
		graphicsManger->LoadGraphicData(masterKey,slaveKey,GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL_DIFF);
		compareModel->setCurrentItem(NULL);
		UpdateGraphicsView();
		RecoveryProc();
		break;
	}
	if (type != 2 && type != 3)
	{
		actionPre->setEnabled(true);
	}
}

void GraphicBrowserFrm::graphicOpersEnable(bool enable)
{
	actionZoomIn->setEnabled(enable);
	actionZoomOut->setEnabled(enable);
	actionRoam->setEnabled(enable);
	actionRecovery->setEnabled(enable);
	actionSaveAs->setEnabled(enable);
}

void GraphicBrowserFrm::removeDataFiles()
{
	bool isRemoved = QFile::remove("../data/PrototypeRdb/__db.001");
	isRemoved = QFile::remove("../data/PrototypeRdb/__db.002");
	isRemoved = QFile::remove("../data/PrototypeRdb/__db.003");
	QFile::remove("../data/PrototypeRdb/AP.db");
	QFile::remove("../data/PrototypeRdb/HItem.db");
	QFile::remove("../data/PrototypeRdb/Ied.db");
	QFile::remove("../data/PrototypeRdb/InSignal.db");
	QFile::remove("../data/PrototypeRdb/OutSignal.db");
	QFile::remove("../data/PrototypeRdb/Panel.db");
	QFile::remove("../data/PrototypeRdb/SoftTrip.db");
	QFile::remove("../data/PrototypeRdb/SubNet.db");
	QFile::remove("../data/PrototypeRdb/Substation.db");
	QFile::remove("../data/PrototypeRdb/VirtualLink.db");
	QFile::remove("../data/PrototypeRdb/VoltageLevel.db");
}

QString GraphicBrowserFrm::getTitleStr()
{
	QFile nameSaver("../cfg/FileNameSaver.txt");
	if (nameSaver.open(QIODevice::ReadOnly))
	{
		QByteArray bArray = nameSaver.readLine();
		filePath = bArray;
		QList<QString> strList = filePath.split("/");
		QString titleStr = strList.at(strList.size() - 1);
		if (titleStr.compare("") == 0)
		{
			titleStr = "���ļ�";
		}
		nameSaver.close();
		return titleStr;
	}
	else
	{
		QMessageBox::warning(this, tr("�ļ�����"), tr("�ļ��򿪴���"));
		return "";
	}
}
void GraphicBrowserFrm::dropEvent(QDropEvent *event)
{
	if(event->mimeData()->hasUrls())
	{
		QList<QUrl> urls = event->mimeData()->urls();
		if (urls.isEmpty())
			return;
		fileNameList.clear();
		if(!porgressFlg)
		{
			for(int i=0;i<urls.count();i++)
			{
				QString fileName = urls.at(i).toLocalFile();
				if (fileName.isEmpty())
					break;
				QString houzhui = fileName.right(4);
				if ((houzhui.toLower().compare(".scd") == 0) ||
                    (houzhui.toLower().compare(".cid") == 0) ||
                    (houzhui.toLower().compare(".icd") == 0) ||
                    (houzhui.toLower().compare(".ssd") == 0))
				{
					fileNameList.append(fileName);
				}
			}

			QString fileName = fileNameList.at(0);
			if (fileName.isEmpty())
				return;
			currentImportFile=0;
			InitImportFile(fileName);

		}
		else
		{
			progresStyle->show();
		}
	}
	else
	{
		QMainWindow::dropEvent(event);
	}
}
void GraphicBrowserFrm::dragEnterEvent(QDragEnterEvent *event)
{
	if (isComparing)
	{
		event->ignore();
		return;
	}
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
	else
	{
		QString fenli="0";
		QString hebing="1";
		QByteArray hb=event->mimeData()->data(hebing);
		QByteArray fl=event->mimeData()->data(fenli);
		if(!fl.isEmpty())
		{
			int index;       //��ô���  ������ľ��
			bool ok;
			QString strIndex =event->mimeData()->text();  //0, 1, 2, 3
			index = strIndex.toInt(&ok,10);
			QWidget *dedao = totalTab->widget(index);
			strIndex = totalTab->tabText(index);

			QSize vbox1_size=centralWidget()->size();  //�õ����layoout���귶Χ
			if(mMousePoint.x()>vbox1_size.width()/2&&mMousePoint.x()<vbox1_size.width()&&mMousePoint.y()>0&&mMousePoint.y()<vbox1_size.height()/2)
			{
				rightTab->addTab(dedao,strIndex);
				//��ӵ�vbox��
				//	Vbox_1->addWidget(rightTab);
				Vbox_1HashRemove.insert(strIndex,rightTab);
				Vbox_1Hash.insert(strIndex,dedao);
				rightTab->show();
			}
			else if(mMousePoint.x()>0&&mMousePoint.x()<vbox1_size.width()&&mMousePoint.y()>vbox1_size.height()/2&&mMousePoint.y()<vbox1_size.height())   //��ӵ�Vbox_2��
			{
				bottmTab->addTab(dedao,strIndex);
				//  Vbox_2->addWidget(bottmTab);
				Vbox_2HashRemove.insert(strIndex,bottmTab);
				Vbox_2Hash.insert(strIndex,dedao);
				bottmTab->show();
			}
			update();
		}
		else if (!hb.isEmpty())
		{
			QSize tabzong_size=totalTab->size();
			QSize youfenli_size=rightTab->size();
			QSize xiafenli_size=bottmTab->size();

			int index;
			QString strIndex =event->mimeData()->text();

			QRect youRect;    //3�����ڵľ��η�Χ
			QRect tabRect;
			QRect xiaRect;
			if (rightTab->size().width()==0)
			{
				tabRect=QRect(0,0,tabzong_size.width()/2,tabzong_size.height());
				youRect=QRect(tabzong_size.width()/2,0,tabzong_size.width()/2,youfenli_size.height()); 
				xiaRect=QRect(0,tabzong_size.height(),xiafenli_size.width(),xiafenli_size.height());
			} 
			else if(bottmTab->size().height()==0)
			{
				tabRect=QRect(0,0,tabzong_size.width(),tabzong_size.height()/2);
				youRect=QRect(tabzong_size.width(),0,youfenli_size.width(),youfenli_size.height()); 
				xiaRect=QRect(0,tabzong_size.height()/2,xiafenli_size.width(),tabzong_size.height()/2);
			}else if (tabzong_size.width()==0)
			{
				tabRect=QRect(0,0,youfenli_size.width()/2,youfenli_size.height());
				youRect=QRect(youfenli_size.width()/2,0,youfenli_size.width()/2,youfenli_size.height()); 
				xiaRect=QRect(0,youfenli_size.height(),xiafenli_size.width(),xiafenli_size.height());
			}
			else
			{
				tabRect=QRect(0,0,tabzong_size.width(),tabzong_size.height());
				youRect=QRect(tabzong_size.width(),0,youfenli_size.width(),youfenli_size.height()); 
				xiaRect=QRect(0,tabzong_size.height(),xiafenli_size.width(),xiafenli_size.height());
			} 

			if(Vbox_1Hash.contains(strIndex))
			{
				// �õ�tabҳ���ڵ�ǰ���괰�ڵľ��η�Χ
				if(youRect.contains(mMousePoint))
					return;
				else if(tabRect.contains(mMousePoint))
				{
					QWidget *wid=Vbox_1Hash.value(strIndex);
					totalTab->addTab(wid,strIndex);
					//QWidget *a=Vbox_1HashRemove.value(strIndex);
					//���ڴ��������Ƴ�
					Vbox_1Hash.remove(strIndex);
				}
				else if (xiaRect.contains(mMousePoint))
				{
					qDebug()<<::GetCurrentProcessId()<<"xiarect"<<xiaRect;
					QWidget *wid=Vbox_1Hash.value(strIndex);
					bottmTab->addTab(wid,strIndex);
					Vbox_2Hash.insert(strIndex,wid);
					Vbox_1Hash.remove(strIndex);
				}
			}
			else if(Vbox_2Hash.contains(strIndex))
			{
				if(xiaRect.contains(mMousePoint))
				{
					return;
				}
				else if(tabRect.contains(mMousePoint))
				{
					QWidget *wid=Vbox_2Hash.value(strIndex);
					totalTab->addTab(wid,strIndex);
					Vbox_2Hash.remove(strIndex);
					Vbox_2Hash.remove(strIndex);
					if(Vbox_2Hash.size()==0)
						bottmTab->hide();
				}
				else if (youRect.contains(mMousePoint))
				{
					QWidget *wid=Vbox_2Hash.value(strIndex);
					rightTab->addTab(wid,strIndex);
					Vbox_1Hash.insert(strIndex,wid);
					Vbox_2Hash.remove(strIndex);
				}

			}
			if(Vbox_1Hash.size()==0)
				rightTab->hide();
			else 
				rightTab->show();
			if(Vbox_2Hash.size()==0)
				bottmTab->hide();
			else 
				bottmTab->show();
		}

		if(totalTab->count()==0)
			totalTab->hide();
		else
			totalTab->show();

		//��tabȫ�����뵽�ӿ��е�ʱ�� �任����
		if(rightTab->count()==stringlist.count())
		{
			for(int i=0;i<stringlist.count();i++)
			{
				QWidget *wid=Vbox_1Hash.value(stringlist.at(i));
				totalTab->addTab(wid,stringlist.at(i));
				Vbox_1Hash.remove(stringlist.at(i));
			}
			totalTab->show();
			rightTab->hide();
			bottmTab->hide();
		}
		else if(bottmTab->count()==stringlist.count() )
		{
			for(int i=0;i<stringlist.count();i++)
			{
				QWidget *wid=Vbox_2Hash.value(stringlist.at(i));
				totalTab->addTab(wid,stringlist.at(i));
				Vbox_2Hash.remove(stringlist.at(i));
			}	
			totalTab->show();
			rightTab->hide();
			bottmTab->hide();
		}
		update();
	}
}

void GraphicBrowserFrm::receiveslot(QPoint mousePos)
{
	mMousePoint=mousePos;

}
void GraphicBrowserFrm::enableTabFouces(bool enbale)
{
	if(sender() == svRecvWidget)
	{
	    totalTab->setTabEnabled(totalTab->indexOf(svRecvWidget),enbale);
	}
	else if(sender() == gooseRecvWidget)
	{
	    totalTab->setTabEnabled(totalTab->indexOf(gooseRecvWidget),enbale);
	}

}
void GraphicBrowserFrm::initTab()
{
	logsrv_scoped_output(logsrv, "����initTab����", "�˳�initTab", "GraphicBrowserFrm::initTab");
	stringlist<<"��Ϣͼ"<<"SV����"<<"SV����"<<"GOOSE����"<<"GOOSE����"<<"��Ϣ���"<<"��ֵ��Ŀ"<<"XML"<<"һ���豸";
	spmain=new QSplitter(Qt::Vertical,this);
	//��ʼ�� 2���Ӳ����ļ�ָ�� 
	tabVbox_1List.clear();
	rightTab =new MyTabWidget(1,centralWidget(),this);
	bottmTab =new MyTabWidget(1,centralWidget(),this);

	rightTab->hide();
	bottmTab->hide();

	// goose chuangti

	gooseSendWidget=new GooseSendWidget(this);
	gooseRecvWidget=new GooseReceiveWidget(this);

	svSendWidget=new SvSendWidget(this);
	svRecvWidget=new SvReceiveWidget(this);
	connect(gooseRecvWidget,SIGNAL(enbleTanFouce(bool)),this,SLOT(enableTabFouces(bool)));
	connect(svRecvWidget,SIGNAL(enbleTanFouce(bool)),this,SLOT(enableTabFouces(bool)));

	ipWidget = new CIPWidget(this);
	macWidget = new CMacWidget(this);
	ipWidget->setHidden(true);
	macWidget->setHidden(true);

	totalTab=new MyTabWidget(0,centralWidget(),this);
	//QTableWidget *info=new QTableWidget;

	reportSendWidget = new ReportSendWidget(this);
	dsSettingWidget = new DsSettingWidget(this);

    totalTab->addTab(graphicsView,stringlist.at(0));
    totalTab->addTab(svSendWidget,stringlist.at(1));
    totalTab->addTab(svRecvWidget,stringlist.at(2));
    totalTab->addTab(gooseSendWidget,stringlist.at(3));
    totalTab->addTab(gooseRecvWidget,stringlist.at(4));
    totalTab->addTab(reportSendWidget, stringlist.at(5));
    totalTab->addTab(dsSettingWidget, stringlist.at(6));
    totalTab->addTab(xmlViewer, stringlist.at(7));
    // totalTab->addTab(widgetIEDSSDTable, stringlist.at(8));
	connect(totalTab, SIGNAL(currentChanged(int)), this, SLOT(totalTabChangeProc(int)));
	connect(rightTab, SIGNAL(currentChanged(int)), this, SLOT(rightTabChangeProc(int)));
	connect(bottmTab, SIGNAL(currentChanged(int)), this, SLOT(bottmTabChangeProc(int)));
	
	QSplitter *sptop=new QSplitter(Qt::Horizontal,spmain);
	sptop->addWidget(totalTab);
	sptop->addWidget(rightTab);
	spmain->addWidget(bottmTab);
	graphicsViewDockWidget->setWidget(spmain);
	//setCentralWidget(spmain);;
	Vbox_1Hash.clear();
	Vbox_2Hash.clear();
}

void GraphicBrowserFrm::totalTabChangeProc(const int tabIndex)
{
	if (isComparing)
	{
		return;
	}
	if (tabIndex == -1)
	{
		return;
	}
	if (isComparing)
	{
		return;
	}

	QString tabTxt = totalTab->tabText(tabIndex);
	int realIndex = 0;
	if (tabTxt.compare("SV����") == 0)
	{
		realIndex = 1;
	} 
	else if(tabTxt.compare("SV����") == 0)
	{
		realIndex = 2;
	}
	else if (tabTxt.compare("GOOSE����") == 0)
	{
		realIndex = 3;
	}
	else if (tabTxt.compare("GOOSE����") == 0)
	{
		realIndex = 4;
	}
	else if (tabTxt.compare("��Ϣ���") == 0)
	{
		realIndex = 5;
	}
	else if (tabTxt.compare("��ֵ��Ŀ") == 0)
	{
		realIndex = 6;
	}
	else if (tabTxt.compare("XML") == 0)
	{
		realIndex = 7;
    }
    else if (tabTxt.compare("һ���豸") == 0)
    {
        realIndex = 8;
    }
	fillTabContent(totalTab, totalTab->getIEDKey(), realIndex);
}

void GraphicBrowserFrm::rightTabChangeProc(const int tabIndex)
{
	if (tabIndex == -1)
	{
		return;
	}
	if (isComparing)
	{
		return;
	}

	QString tabTxt = rightTab->tabText(tabIndex);
	int realIndex = 0;
	if (tabTxt.compare("SV����") == 0)
	{
		realIndex = 1;
	} 
	else if(tabTxt.compare("SV����") == 0)
	{
		realIndex = 2;
	}
	else if (tabTxt.compare("GOOSE����") == 0)
	{
		realIndex = 3;
	}
	else if (tabTxt.compare("GOOSE����") == 0)
	{
		realIndex = 4;
	}
	else if (tabTxt.compare("��Ϣ���") == 0)
	{
		realIndex = 5;
	}
	else if (tabTxt.compare("XML") == 0)
	{
		realIndex = 6;
	}
	fillTabContent(rightTab, totalTab->getIEDKey(), realIndex);
}

void GraphicBrowserFrm::bottmTabChangeProc(const int tabIndex)
{
	if (tabIndex == -1)
	{
		return;
	}
	if (isComparing)
	{
		return;
	}

	QString tabTxt = bottmTab->tabText(tabIndex);
	int realIndex = 0;
	if (tabTxt.compare("SV����") == 0)
	{
		realIndex = 1;
	} 
	else if(tabTxt.compare("SV����") == 0)
	{
		realIndex = 2;
	}
	else if (tabTxt.compare("GOOSE����") == 0)
	{
		realIndex = 3;
	}
	else if (tabTxt.compare("GOOSE����") == 0)
	{
		realIndex = 4;
	}
	else if (tabTxt.compare("��Ϣ���") == 0)
	{
		realIndex = 5;
	}
	else if (tabTxt.compare("XML") == 0)
	{
		realIndex = 6;
	}
	fillTabContent(bottmTab, totalTab->getIEDKey(), realIndex);
}

void    GraphicBrowserFrm::setCenterLayer(int i)  // 0 ����չʾȫ��ied  1 ���ܱ��վ������Ŀ 2 ����չʾ�汾��Ϣ;
{
	cunrrentindex=totalTab->tabText(totalTab->currentIndex());
	totalTab->disconnect(this);
	rightTab->disconnect(this);
	bottmTab->disconnect(this);
    if(i==SHOW_ToTal_IED)
	{
        totalTab->clear();
		totalTab->tabbarhide();
        totalTab->addTab(widgetIEDTable,"IEd");
	}
	else if(i==SHOW_INFO_TABLE)
	{
	    totalTab->clear();
		totalTab->tabBarShow();
		totalTab->addTab(graphicsView,stringlist.at(0));
		totalTab->addTab(svSendWidget,stringlist.at(1));
		totalTab->addTab(svRecvWidget,stringlist.at(2));
		totalTab->addTab(gooseSendWidget,stringlist.at(3));
		totalTab->addTab(gooseRecvWidget,stringlist.at(4));
		totalTab->addTab(reportSendWidget, stringlist.at(5));
		totalTab->addTab(dsSettingWidget, stringlist.at(6));
		totalTab->addTab(xmlViewer, stringlist.at(7));
        
        QString iedKey = totalTab->getIEDKey();
        QString iedName = iedKey.mid(iedKey.lastIndexOf(".") + 1);
        if (m_SSDInfo->IsIEDHaveEquipment(iedName))
        {
            totalTab->addTab(widgetIEDSSDTable, stringlist.at(8));
        }
        
		QList <QString> keyStr;
		keyStr=Vbox_1Hash.keys();

		for(int i=0;i<keyStr.size();i++)
		{
			rightTab->addTab(Vbox_1Hash.value(keyStr.at(i)),keyStr.at(i));
		}

		keyStr=Vbox_2Hash.keys();
		for(int i=0;i<keyStr.size();i++)
		{
			bottmTab->addTab(Vbox_2Hash.value(keyStr.at(i)),keyStr.at(i));
		}
	}
	else if (i == NODE_TYPE_COMPARE_IED)
	{
		totalTab->clear();
		totalTab->tabBarShow();
		totalTab->addTab(graphicsView,stringlist.at(0));
		totalTab->addTab(svSendWidget,stringlist.at(1));
		totalTab->addTab(svRecvWidget,stringlist.at(2));
		totalTab->addTab(gooseSendWidget,stringlist.at(3));
		totalTab->addTab(gooseRecvWidget,stringlist.at(4));
		subSetting->sync();
		if (subSetting->value("config/IEDXMLCompare").toBool())
		{
			totalTab->addTab(xmlViewer, stringlist.at(7));
		}		
	}
	else if (i == NODE_TYPE_SUBNET)
	{
		totalTab->clear();
		totalTab->tabBarShow();
		totalTab->addTab(graphicsView, "����ͼ");
		totalTab->addTab(ipWidget, "IP��Ϣ");
		totalTab->addTab(macWidget, "MAC��Ϣ");
		totalTab->addTab(xmlViewer, "XMLƬ��");
		ipWidget->setHidden(false);
		macWidget->setHidden(false);
		ipWidget->setSubnetKey(totalTab->getIEDKey());
		macWidget->setSubnetKey(totalTab->getIEDKey());
		graphicsViewDockWidget->setWindowTitle("����ͼ");
	}
	else if(i == BASIC_INFO)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(versionView,"verison");
		graphicsViewDockWidget->setWindowTitle("��ʷ��Ϣ");
	}
	else if (i == NODE_TYPE_XMLFILE)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(xmlViewer,"XMLԭ��");
		graphicsViewDockWidget->setWindowTitle("XMLԭ��");
	}
	else if(i==NODE_TYPE_VALIDATOR_SCHEMA ||
		i==NODE_TYPE_VALIDATOR_PROPERTIES ||
		i==NODE_TYPE_VALIDATOR_UNIQUENESS ||
		i==NODE_TYPE_VALIDATOR_MODELINTEGRITY ||
		i==NODE_TYPE_VALIDATOR_REFERENCE ||
		i==NODE_TYPE_VALIDATOR_STRUCT ||
		i==NODE_TYPE_VALIDATOR_SCHEMA_WARN ||
		i==NODE_TYPE_VALIDATOR_PROPERTIES_WARN ||
		i==NODE_TYPE_VALIDATOR_UNIQUENESS_WARN ||
		i==NODE_TYPE_VALIDATOR_MODELINTEGRITY_WARN ||
		i==NODE_TYPE_VALIDATOR_REFERENCE_WARN ||
		i==NODE_TYPE_VALIDATOR_STRUCT_WARN)
	{
		currentValidatorItem = i;
		InitSCDValidatorView();
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(scdValidatorView,"validator");
		graphicsViewDockWidget->setWindowTitle("SCDУ��");
	}
	else if(i==NODE_TYPE_WIDGETADD)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(widgetDIffAddTable,"IEd");
		graphicsViewDockWidget->setWindowTitle("���컯�Ƚ�");
	}else if(i==NODE_TYPE_WIDGETDEL)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(widgetDiffDelTable,"IEd");
		graphicsViewDockWidget->setWindowTitle("���컯�Ƚ�");
	}else if(i==NODE_TYPE_WIDGETEQL)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(widgetDiffEqlTable,"IEd");
		graphicsViewDockWidget->setWindowTitle("���컯�Ƚ�");
	}else if(i==NODE_TYPE_WIDGETUP)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(widgetDiffEditTable,"IEd");
		graphicsViewDockWidget->setWindowTitle("���컯�Ƚ�");
	}else if(i==NODE_TYPE_WIDGETVer)
	{
		totalTab->clear();
		totalTab->tabbarhide();
		totalTab->addTab(widgetDiffVerTable,"IEd");
		graphicsViewDockWidget->setWindowTitle("���컯�Ƚ�");
	}
	else
	{
		totalTab->clear();
		totalTab->tabbarhide();
	}

	int current;
	if(cunrrentindex=="��Ϣͼ")
	{
		current= totalTab->indexOf(graphicsView);
	}
	else if (cunrrentindex=="SV����")
	{
		current= totalTab->indexOf(svSendWidget);
	}
	else if(cunrrentindex=="SV����")
	{
		current= totalTab->indexOf(svRecvWidget);

	}else if(cunrrentindex=="GOOSE����")
	{
		current= totalTab->indexOf(gooseSendWidget);
	}
	else if(cunrrentindex=="GOOSE����")
	{
		current= totalTab->indexOf(gooseRecvWidget);
	}
	else if (cunrrentindex=="��Ϣ���")
	{
		current = totalTab->indexOf(reportSendWidget);
	}
	else if (cunrrentindex=="��ֵ��Ŀ")
	{
		current = totalTab->indexOf(dsSettingWidget);
	}
    else if (cunrrentindex=="XML")
    {
        current = totalTab->indexOf(xmlViewer);
    }
	totalTab->setCurrentIndex(current);
	connect(totalTab, SIGNAL(currentChanged(int)), this, SLOT(totalTabChangeProc(int)));
	connect(rightTab, SIGNAL(currentChanged(int)), this, SLOT(rightTabChangeProc(int)));
	connect(bottmTab, SIGNAL(currentChanged(int)), this, SLOT(bottmTabChangeProc(int)));
}

void   GraphicBrowserFrm::RootItemDisplay()
{
	//QTreeView *f=new QTreeView;
	//setCentralWidget(f);
}

/** 
* @brief    SetConfigureInfo
* 
* ����������Ϣ
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::SetConfigureInfo()
{
	SetConfigureInfoPair(FILEINFO_VERSION, "FILEINFO_VERSION", "�汾��Ϣ");
	SetConfigureInfoPair(FILEINFO_REVERSION, "FILEINFO_REVERSION", "�޶��汾��Ϣ");
	SetConfigureInfoPair(FILEINFO_CRC, "FILEINFO_CRC", "�ļ�CRC��Ϣ");

	SetConfigureInfoPair(IED_DESC, "IED_DESC", "IED����");
	SetConfigureInfoPair(IED_TYPE, "IED_TYPE", "IED����");
	SetConfigureInfoPair(IED_MANU, "IED_MANU", "IED����");
	SetConfigureInfoPair(IED_CRC, "IED_CRC", "IED CRC��Ϣ");

	SetConfigureInfoPair(IED_SV_APPID, "IED_SV_APPID", "AppID");
	SetConfigureInfoPair(IED_SV_VLANID, "IED_SV_VLANID", "VLanID");
	SetConfigureInfoPair(IED_SV_VLANPRIORITY, "IED_SV_VLANPRIORITY", "VLan���ȼ�");
	SetConfigureInfoPair(IED_SV_MACADDR, "IED_SV_MACADDR", "Mac��ַ");
	SetConfigureInfoPair(IED_SV_CONFVERSION, "IED_SV_CONFVERSION", "���ð汾��Ϣ");
	//SetConfigureInfoPair(IED_SV_ASDUCOUNT, "IED_SV_ASDUCOUNT", "ASDU��Ŀ");
	SetConfigureInfoPair(IED_SV_SMPRATE, "IED_SV_SMPRATE", "������");
	SetConfigureInfoPair(IED_SV_CBNAME, "IED_SV_CBNAME", "���ƿ�����");
	// SetConfigureInfoPair(IED_SV_CBDESC, "IED_SV_CBDESC", "���ƿ�����");
	SetConfigureInfoPair(IED_SV_DATASETNAME, "IED_SV_DATASETNAME", "���ݼ�����");
	SetConfigureInfoPair(IED_SV_DATASETDESC, "IED_SV_DATASETDESC", "���ݼ�����");
	SetConfigureInfoPair(IED_SV_DATASETCOUNT, "IED_SV_DATASETCOUNT", "��Ϣ����");
	SetConfigureInfoPair(IED_SV_DATASETADDR, "IED_SV_DATASETADDR", "���ݼ���ַ");
	SetConfigureInfoPair(IED_SV_CBAPAPPID, "IED_SV_CBAPAPPID", "���ƿ�Ap��ַ");
	SetConfigureInfoPair(IED_SV_DATASET_ORD, "IED_SV_DATASET_ORD", "���");
	SetConfigureInfoPair(IED_SV_DATASET_DOIDU, "IED_SV_DATASET_DOIDU", "dUֵ");
	SetConfigureInfoPair(IED_SV_DATASET_DODESC, "IED_SV_DATASET_DODESC", "DO����");
	SetConfigureInfoPair(IED_SV_DATASET_LNDESC, "IED_SV_DATASET_LNDESC", "LN����");
	SetConfigureInfoPair(IED_SV_DATASET_FC, "IED_SV_DATASET_FC", "����Լ��");
	SetConfigureInfoPair(IED_SV_DATASET_BDATYPE, "IED_SV_DATASET_BDATYPE", "BDA����");
	SetConfigureInfoPair(IED_SV_DATASET_TRIPNAME, "IED_SV_DATASET_TRIPNAME", "ѹ������");
	SetConfigureInfoPair(IED_SV_DATASET_SIGDESC, "IED_SV_DATASET_SIGDESC", "�ź�����");
	SetConfigureInfoPair(IED_SV_DATASET_PORTINDEX, "IED_SV_DATASET_PORTINDEX", "����˿�����");
	// SetConfigureInfoPair(IED_SV_VLINK, "IED_SV_VLINK", "������");
	// GOOSE
	SetConfigureInfoPair(IED_GOOSE_APPID, "IED_GOOSE_APPID", "AppID");
	SetConfigureInfoPair(IED_GOOSE_VLANID, "IED_GOOSE_VLANID", "VLanID");
	SetConfigureInfoPair(IED_GOOSE_VLANPRIORITY, "IED_GOOSE_VLANPRIORITY", "VLan���ȼ�");
	SetConfigureInfoPair(IED_GOOSE_MACADDR, "IED_GOOSE_MACADDR", "Mac��ַ");
	SetConfigureInfoPair(IED_GOOSE_CONFVERSION, "IED_GOOSE_CONFVERSION", "���ð汾��Ϣ");
	SetConfigureInfoPair(IED_GOOSE_MAXTIME, "IED_GOOSE_MAXTIME", "MaxTime");
	SetConfigureInfoPair(IED_GOOSE_MINTIME, "IED_GOOSE_MINTIME", "MinTime");
	SetConfigureInfoPair(IED_GOOSE_CBNAME, "IED_GOOSE_CBNAME", "���ƿ�����");
	// SetConfigureInfoPair(IED_GOOSE_CBDESC, "IED_GOOSE_CBDESC", "���ƿ�����");
	SetConfigureInfoPair(IED_GOOSE_DATASETNAME, "IED_GOOSE_DATASETNAME", "���ݼ�����");
	SetConfigureInfoPair(IED_GOOSE_DATASETDESC, "IED_GOOSE_DATASETDESC", "���ݼ�����");
	SetConfigureInfoPair(IED_GOOSE_DATASETCOUNT, "IED_GOOSE_DATASETCOUNT", "��Ϣ����");
	SetConfigureInfoPair(IED_GOOSE_GOCBREF, "IED_GOOSE_GOCBREF", "goCBRef");
	SetConfigureInfoPair(IED_GOOSE_DATASETADDR, "IED_GOOSE_DATASETADDR", "���ݼ���ַ");
	SetConfigureInfoPair(IED_GOOSE_CBAPAPPID, "IED_GOOSE_CBAPAPPID", "���ƿ�Ap��ַ");
	SetConfigureInfoPair(IED_GOOSE_DATASET_ORD, "IED_GOOSE_DATASET_ORD", "���");
	SetConfigureInfoPair(IED_GOOSE_DATASET_DOIDU, "IED_GOOSE_DATASET_DOIDU", "dUֵ");
	SetConfigureInfoPair(IED_GOOSE_DATASET_DODESC, "IED_GOOSE_DATASET_DODESC", "DO����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_LNDESC, "IED_GOOSE_DATASET_LNDESC", "LN����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_FC, "IED_GOOSE_DATASET_FC", "����Լ��");
	SetConfigureInfoPair(IED_GOOSE_DATASET_BDATYPE, "IED_GOOSE_DATASET_BDATYPE", "BDA����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_TRIPNAME, "IED_GOOSE_DATASET_TRIPNAME", "ѹ������");
	SetConfigureInfoPair(IED_GOOSE_DATASET_SIGDESC, "IED_GOOSE_DATASET_SIGDESC", "�ź�����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_PORTINDEX, "IED_GOOSE_DATASET_PORTINDEX", "����˿�����");
	// SetConfigureInfoPair(IED_GOOSE_VLINK, "IED_GOOSE_VLINK", "������");

	SetConfigureInfoPair(IED_INPUTS_EXTREFCOUNT, "IED_INPUTS_EXTREFCOUNT", "��������Ŀ");
	//SetConfigureInfoPair(IED_INPUTS_EXTREF_ORD, "IED_INPUTS_EXTREF_ORD", "���");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_DESC, "IED_INPUTS_EXTREF_DESC", "����");
	//	SetConfigureInfoPair(IED_INPUTS_EXTREF_TYPE, "IED_INPUTS_EXTREF_TYPE", "����");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_INTADDR, "IED_INPUTS_EXTREF_INTADDR", "�ڲ���ַ");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_OUTADDR, "IED_INPUTS_EXTREF_OUTADDR", "�ⲿ��ַ");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_PORTNAME, "IED_INPUTS_EXTREF_PORTNAME", "����˿�����");
	//	SetConfigureInfoPair(IED_INPUTS_EXTREF_SRCSIGNALID, "IED_INPUTS_EXTREF_SRCSIGNALID", "Դ�ź�");
	//	SetConfigureInfoPair(IED_INPUTS_EXTREF_DESTSIGNALID, "IED_INPUTS_EXTREF_DESTSIGNALID", "Ŀ���ź�");
	//	SetConfigureInfoPair(IED_INPUTS_EXTREF_VLINK, "IED_INPUTS_EXTREF_VLINK", "������");
	//SetConfigureInfoPair(IED_INPUTS_DESTSIG_ORD, "IED_INPUTS_DESTSIG_ORD", "���");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_DOIDU, "IED_INPUTS_DESTSIG_DOIDU", "dUֵ");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_DODESC, "IED_INPUTS_DESTSIG_DODESC", "DO����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_LNDESC, "IED_INPUTS_DESTSIG_LNDESC", "LN����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_FC, "IED_INPUTS_DESTSIG_FC", "����Լ��");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_BDATYPE, "IED_INPUTS_DESTSIG_BDATYPE", "BDA����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_TRIPNAME, "IED_INPUTS_DESTSIG_TRIPNAME", "ѹ������");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_SIGDESC, "IED_INPUTS_DESTSIG_SIGDESC", "�ź�����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_PORTINDEX, "IED_INPUTS_DESTSIG_PORTINDEX", "����˿�����");

	//	SetConfigureInfoPair(IED_PHY_NAME, "IED_PHY_NAME", "����˿�����");
	//SetConfigureInfoPair(IED_PHY_TRANSMEDIA, "IED_PHY_TRANSMEDIA", "����˿�����");
	SetConfigureInfoPair(IED_PHY_PORTINDEX, "IED_PHY_PINDEX", "����˿����");
	SetConfigureInfoPair(IED_PHY_PLUG, "IED_PHY_PLUG", "����˿ڲ�ͷ����");
	SetConfigureInfoPair(IED_PHY_TYPE, "IED_PHY_TYPE", "����˿ڽӿ�����");
	//SetConfigureInfoPair(IED_PHY_CABLE, "IED_PHY_CABLE", "�����������");
}

/** 
* @brief    SetConfigureInfoPair
* 
* ����������Ϣ������
* 
* @class    GraphicBrowserFrm
* @param    ConfInfo ord    : ���
* @param    QString EnDesc  : Ӣ������
* @param    QString ChDesc  : ��������
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::SetConfigureInfoPair(ConfInfo ord, QString EnDesc, QString ChDesc)
{
	m_ConfPair[ord].confEnDesc = EnDesc;
	m_ConfPair[ord].confChDesc = ChDesc;
}


/** 
* @brief    CreatCompareConfTrue
* 
* ����������
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::CreatCompareConfTree()
{
	substationCompareConfigModel->clear();
	substationCompareConfigModel->setHorizontalHeaderLabels(QStringList()<<"������");

	QStandardItem *FILEINFO = new QStandardItem("�ļ���Ϣ");;  
	FILEINFO->setCheckable(true);
	FILEINFO->setTristate(true);
	substationCompareConfigModel->appendRow(FILEINFO);

	CreatSubstationCompareConfigView(FILEINFO, FILEINFO_VERSION);
	CreatSubstationCompareConfigView(FILEINFO, FILEINFO_REVERSION);
	CreatSubstationCompareConfigView(FILEINFO, FILEINFO_CRC);

	QStandardItem *IED = new QStandardItem("IED��Ϣ");
	IED->setCheckable(true);
	IED->setTristate(true);
	substationCompareConfigModel->appendRow(IED); 

	CreatSubstationCompareConfigView(IED, IED_DESC);
	CreatSubstationCompareConfigView(IED, IED_TYPE);
	CreatSubstationCompareConfigView(IED, IED_MANU);
	CreatSubstationCompareConfigView(IED, IED_CRC);
	QStandardItem *IED_SV = new QStandardItem("SV���ƿ�");;  
	IED_SV->setCheckable(true);
	IED_SV->setTristate(true);
	IED->appendRow(IED_SV); 
	QStandardItem *IED_GOOSE = new QStandardItem("Goose���ƿ�");;  
	IED_GOOSE->setCheckable(true);
	IED_GOOSE->setTristate(true);
	IED->appendRow(IED_GOOSE); 
	QStandardItem *IED_INPUTS = new QStandardItem("���·");;  
	IED_INPUTS->setCheckable(true);
	IED_INPUTS->setTristate(true);
	IED->appendRow(IED_INPUTS); 
	QStandardItem *IED_PHY = new QStandardItem("����˿�");;  
	IED_PHY->setCheckable(true);
	IED_PHY->setTristate(true);
	IED->appendRow(IED_PHY); 

	// SV���ƿ���������
	CreatSubstationCompareConfigView(IED_SV, IED_SV_APPID);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_VLANID);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_VLANPRIORITY);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_MACADDR);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_CONFVERSION);
	//CreatSubstationCompareConfigView(IED_SV, IED_SV_ASDUCOUNT);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_SMPRATE);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_CBNAME);
	//CreatSubstationCompareConfigView(IED_SV, IED_SV_CBDESC);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_DATASETNAME);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_DATASETDESC);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_DATASETCOUNT);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_DATASETADDR);
	CreatSubstationCompareConfigView(IED_SV, IED_SV_CBAPAPPID);
	QStandardItem *IED_SV_DATASET = new QStandardItem("���ݼ�");;  
	IED_SV_DATASET->setCheckable(true);
	IED_SV_DATASET->setTristate(true);
	IED_SV->appendRow(IED_SV_DATASET); 
	//	CreatSubstationCompareConfigView(IED_SV, IED_SV_VLINK);
	//SV�����ݼ�������
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_ORD);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_DOIDU);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_DODESC);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_LNDESC);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_FC);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_BDATYPE);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_TRIPNAME);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_SIGDESC);
	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_PORTINDEX);
	//	CreatSubstationCompareConfigView(IED_SV_DATASET, IED_SV_DATASET_TYPE);

	// Goose���ƿ���������
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_APPID);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_VLANID);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_VLANPRIORITY);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_MACADDR);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_CONFVERSION);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_MAXTIME);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_MINTIME);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_CBNAME);
	//	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_CBDESC);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_DATASETNAME);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_DATASETDESC);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_DATASETCOUNT);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_GOCBREF);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_DATASETADDR);
	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_CBAPAPPID);
	QStandardItem *IED_GOOSE_DATASET = new QStandardItem("���ݼ�");;  
	IED_GOOSE_DATASET->setCheckable(true);
	IED_GOOSE_DATASET->setTristate(true);
	IED_GOOSE->appendRow(IED_GOOSE_DATASET); 
	//	CreatSubstationCompareConfigView(IED_GOOSE, IED_GOOSE_VLINK);
	//SV�����ݼ�������
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_ORD);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_DOIDU);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_DODESC);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_LNDESC);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_FC);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_BDATYPE);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_TRIPNAME);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_SIGDESC);
	CreatSubstationCompareConfigView(IED_GOOSE_DATASET, IED_GOOSE_DATASET_PORTINDEX);

	// Inputs
	CreatSubstationCompareConfigView(IED_INPUTS, IED_INPUTS_EXTREFCOUNT);
	QStandardItem *IED_INPUTS_EXTREF = new QStandardItem("Extref");;  
	IED_INPUTS_EXTREF->setCheckable(true);
	IED_INPUTS_EXTREF->setTristate(true);
	IED_INPUTS->appendRow(IED_INPUTS_EXTREF); 
	//	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_ORD);
	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_DESC);
	//	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_TYPE);
	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_INTADDR);
	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_OUTADDR);
	//	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_SRCSIGNALID);
	//	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_DESTSIGNALID);
	//	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_VLINK);
	CreatSubstationCompareConfigView(IED_INPUTS_EXTREF, IED_INPUTS_EXTREF_PORTNAME);
	QStandardItem *IED_INPUTS_DESTSIG = new QStandardItem("�����ź�");;  
	IED_INPUTS_DESTSIG->setCheckable(true);
	IED_INPUTS_DESTSIG->setTristate(true);
	IED_INPUTS_EXTREF->appendRow(IED_INPUTS_DESTSIG);
	//	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_ORD);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_DOIDU);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_DODESC);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_LNDESC);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_FC);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_BDATYPE);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_TRIPNAME);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_SIGDESC);
	CreatSubstationCompareConfigView(IED_INPUTS_DESTSIG, IED_INPUTS_DESTSIG_PORTINDEX);

	// ����˿�
	//	CreatSubstationCompareConfigView(IED_PHY, IED_PHY_NAME);
	//CreatSubstationCompareConfigView(IED_PHY, IED_PHY_TRANSMEDIA);
	CreatSubstationCompareConfigView(IED_PHY, IED_PHY_PORTINDEX);
	CreatSubstationCompareConfigView(IED_PHY, IED_PHY_PLUG);
	CreatSubstationCompareConfigView(IED_PHY, IED_PHY_TYPE);
	//CreatSubstationCompareConfigView(IED_PHY, IED_PHY_CABLE);

	substationCompareConfigView->setEditTriggers(NULL);
	substationCompareConfigView->setModel(substationCompareConfigModel);
}

/** 
* @brief    CreatSubstationCompareConfigView
* 
* ����������
* 
* @class    GraphicBrowserFrm
* @param    ConfInfo itemParent : ���ڵ�
* @param    ConfInfo itemChild  : ��ǰ�ڵ�
* @param    QString desc        : ��ǰ�ڵ�����
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::CreatSubstationCompareConfigView(QStandardItem *itemParent, ConfInfo itemChild)
{
	ConfigItem[itemChild].setText(m_ConfPair[itemChild].confChDesc);
	ConfigItem[itemChild].setCheckable(true);
	itemParent->appendRow(&ConfigItem[itemChild]);
}

/** 
* @brief    GetSubstationCompareConfigInfoFromFile
* 
* �������ļ��ж�ȡ������Ϣ
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::GetSubstationCompareConfigInfoFromFile()
{
	QSettings *config = new QSettings("../cfg/CompareConf", QSettings::IniFormat);
	//��ȡ�û�������Ϣ
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		ConfigureInfo[i] = config->value("config/" + m_ConfPair[i].confEnDesc).toBool();
	}
}

/** 
* @brief    InitSubstationCompareConfigView
* 
* ��ʼ��������
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::InitSubstationCompareConfigView()
{
	GetSubstationCompareConfigInfoFromFile();
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		if (ConfigureInfo[i])
		{
			ConfigItem[i].setCheckState(Qt::Checked);
		}else
		{
			ConfigItem[i].setCheckState(Qt::Unchecked);
		}
	}


}

/** 
* @brief    SubStationConfigProc
* 
* �ȽϽ��������������
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::ShowSubStationCompareConfigProc()
{
	logsrv_scoped_output(logsrv, "����ShowSubStationCompareConfigProc����", "�˳�ShowSubStationCompareConfigProc����", "GraphicBrowserFrm::ShowSubStationCompareConfigProc");
	InitSubstationCompareConfigView();
	SubstationCompareConfigWin->show();
}

/** 
* @brief    SubStationCompareConfigSetProc
* 
* �����ý�������ô��������ļ���
* 
* @class    GraphicBrowserFrm
* @return   void
* @author   zhang-xch
* @date     2014/5/18
* @remarks  
*/ 
void GraphicBrowserFrm::SubStationCompareConfigSetProc()
{
	QSettings *config = new QSettings("../cfg/CompareConf", QSettings::IniFormat);
	config->beginGroup("config");	
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		Qt::CheckState state = ConfigItem[i].checkState ();
		if (state == Qt::Checked)
		{ 

			config->setValue(m_ConfPair[i].confEnDesc, 1);
		}
		else
		{
			config->setValue(m_ConfPair[i].confEnDesc, 0);
		}
	}
	config->endGroup();
	SubstationCompareConfigWin->hide();
}


void GraphicBrowserFrm::treeItemChanged(QStandardItem * item)
{
	if ( item == nullptr )  
		return ;  
	if ( item->isCheckable ())  
	{  
		//�����Ŀ�Ǵ��ڸ�ѡ��ģ���ô�ͽ�������Ĳ���  
		Qt::CheckState state = item->checkState (); //��ȡ��ǰ��ѡ��״̬  
		if ( item->isTristate ())  
		{  
			//�����Ŀ����̬�ģ�˵�����Զ���Ŀ¼����ȫѡ��ȫ��ѡ������  
			if ( state != Qt::PartiallyChecked )  
			{  
				//��ǰ��ѡ��״̬����Ҫ��������Ŀ����ȫѡ  
				treeItem_checkAllChild (item, state == Qt::Checked ? true : false );  
			}  
		}  
		else  
		{  
			//˵������̬�ģ���̬��Ը�������̬��Ӱ��  
			//�ж��ֵܽڵ�����  
			treeItem_CheckChildChanged ( item );  
		}  
	}  
}
void GraphicBrowserFrm::treeItem_checkAllChild(QStandardItem * item, bool check)  
{
	if(item == nullptr)  
		return;  
	int rowCount = item->rowCount();  
	for(int i=0;i<rowCount;++i)  
	{  
		QStandardItem* childItems = item->child(i);  
		treeItem_checkAllChild_recursion(childItems,check);  
	}  
	if(item->isCheckable())  
		item->setCheckState(check ? Qt::Checked : Qt::Unchecked);  
}
void GraphicBrowserFrm::treeItem_checkAllChild_recursion(QStandardItem * item,bool check)  
{  
	if(item == nullptr)  
		return;  
	int rowCount = item->rowCount();  
	for(int i=0;i<rowCount;++i)  
	{
		QStandardItem* childItems = item->child(i);  
		treeItem_checkAllChild_recursion(childItems,check);  
	}  
	if(item->isCheckable())  
		item->setCheckState(check ? Qt::Checked : Qt::Unchecked);  
}  
void GraphicBrowserFrm::treeItem_CheckChildChanged(QStandardItem * item)  
{  
	if(nullptr == item)  
		return;  
	Qt::CheckState siblingState = checkSibling(item);  
	QStandardItem * parentItem = item->parent();  
	if(nullptr == parentItem)  
		return;  
	if(Qt::PartiallyChecked == siblingState)  
	{  
		if(parentItem->isCheckable() && parentItem->isTristate())  
			parentItem->setCheckState(Qt::PartiallyChecked);  
	}
	else if(Qt::Checked == siblingState)  
	{  
		if(parentItem->isCheckable())  
			parentItem->setCheckState(Qt::Checked);  
	}  
	else  
	{  
		if(parentItem->isCheckable())  
			parentItem->setCheckState(Qt::Unchecked);  
	}  
	treeItem_CheckChildChanged(parentItem);  
}  
Qt::CheckState GraphicBrowserFrm::checkSibling(QStandardItem * item)  
{  
	//��ͨ�����ڵ��ȡ�ֵܽڵ�  
	QStandardItem * parent = item->parent();  
	if(nullptr == parent)  
		return item->checkState();  
	int brotherCount = parent->rowCount();  
	int checkedCount(0),unCheckedCount(0);  
	Qt::CheckState state;  
	for(int i=0;i<brotherCount;++i)  
	{  
		QStandardItem* siblingItem = parent->child(i);  
		state = siblingItem->checkState();  
		if(Qt::PartiallyChecked == state)  
			return Qt::PartiallyChecked;  
		else if(Qt::Unchecked == state)  
			++unCheckedCount;  
		else  
			++checkedCount;  
		if(checkedCount>0 && unCheckedCount>0)  
			return Qt::PartiallyChecked;  
	}  
	if(unCheckedCount>0)  
		return Qt::Unchecked;  
	return Qt::Checked;  
} 


void GraphicBrowserFrm::StartFromCMD()
{
	if (argc1.isEmpty())
		return;

	QString houzhui = argc1.right(4);
	if (houzhui.toLower().compare(".scd") != 0)
	{
		QMessageBox::warning(this,
			tr("�ļ�����"),
			tr("�����ļ���SCD�ļ�!"));

		QMessageBox::warning(this,
			tr("�ļ�����"),
			argc1);

		return;
	}

	if(!porgressFlg)
	{
		substationManageWin->hide();

		if (argc1.isEmpty())
			return;

		QString houzhui = argc1.right(4);
		if (houzhui.toLower().compare(".scd") != 0)
		{
			QMessageBox::warning(this,
				tr("�ļ�����"),
				tr("�����ļ���SCD�ļ�!"));

			QMessageBox::warning(this,
				tr("�ļ�����"),
				argc1);

			return;
		}
		fileNameList.clear();
		currentImportFile=0;
		fileNameList.append(argc1);
		InitImportFile(argc1);
	}
	else
		progresStyle->show();

}

void GraphicBrowserFrm::StartMoreFromCMD()
{
	// ֻ�����ļ��б����2ʱ�����������ļ�ʱ�Ž��бȽ�
	if (fileNameList.count() == 2)
	{
		m_IsAutoCompare = true;
	}

	// ��������SCD�ļ�
	if(!porgressFlg)
	{
		if (fileNameList.count()>0)
		{
			QString fileName = fileNameList.at(0);
			if (fileName.isEmpty())
				return;
			currentImportFile=0;
			InitImportFile(fileName);
		}
	}else 
		progresStyle->show();

}

void GraphicBrowserFrm::StratCompareFromCMDAfterLoad()
{
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB0\n");
	// �Ƚϵ����SCD�ļ�
	bool isSelected = false;
	checkFlag=false;
	fileInfoSlectFlag = false;
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB1\n");
	QSettings *config = new QSettings("../cfg/CompareConf", QSettings::IniFormat);
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB2\n");
	//��ȡ�û�������Ϣ
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		ConfigureInfo[i] = config->value("config/" + m_ConfPair[i].confEnDesc).toBool();

		if (ConfigureInfo[i])
		{
			if(i>=2)
			{
				checkFlag=true;
			}else 
			{
				fileInfoSlectFlag = true;
			}
			isSelected = true;
		}
	}
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB3\n");

	if (!isSelected)
	{
		QMessageBox::warning(this, tr("SCD�Ƚ�"), tr("�������������"), QMessageBox::Close);
		return;
	}
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB4\n");
	ClearTableWidgetData(); // qing kong 
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB5\n");
	ListBaseName.clear();
	proCompareFlg=true;
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB6\n");
	QVector<QString> *substationNameList = IEDsLoader->GetSubstationNameList();
	OutputDebugStringA("BBBBBBBBBBBBBBBBBBBBBBB7\n");
	int count = substationNameList->size();
	QString baseName, otherName;
	if (m_AutoCompareFiles.size() == 2 && count >= 2)
	{
		QString lastFile = (m_AutoCompareFiles.last().split("/")).last();
		QList<QString> mList = lastFile.split("_");
		if (mList.size() > 0 && mList[0] == "1")
		{
			baseName = QString::fromStdString(SclDacLib::CalcFileMark(m_AutoCompareFiles.last().toStdString()));
			otherName = QString::fromStdString(SclDacLib::CalcFileMark(m_AutoCompareFiles.at(m_AutoCompareFiles.size() - 2).toStdString()));
		}else
		{
			otherName = QString::fromStdString(SclDacLib::CalcFileMark(m_AutoCompareFiles.last().toStdString()));
			baseName = QString::fromStdString(SclDacLib::CalcFileMark(m_AutoCompareFiles.at(m_AutoCompareFiles.size() - 2).toStdString()));
		}
	}
	else
	{
		return;
	}
	graphicScene->changeModel(true);
	//QString baseName = substationNameList->at(count - 2);
	//QString otherName = substationNameList->at(count - 1);
	QString baseKey = "220kV." + baseName;
	QString otherKey = "220kV." + otherName;
	QSubStationInfo baseInfo, otherInfo;
	QSubStationInfo *tempSubInfo = IEDsLoader->GetSubstationInfo(baseKey);
	baseInfo.Name = tempSubInfo->Name;
	baseInfo.desc = tempSubInfo->desc;
	baseInfo.fileName = tempSubInfo->fileName;
	baseInfo.version = tempSubInfo->version;
	baseInfo.reversion = tempSubInfo->reversion;
	tempSubInfo = IEDsLoader->GetSubstationInfo(otherKey);
	otherInfo.Name = tempSubInfo->Name;
	otherInfo.desc = tempSubInfo->desc;
	otherInfo.fileName = tempSubInfo->fileName;
	otherInfo.version = tempSubInfo->version;
	otherInfo.reversion = tempSubInfo->reversion;
	QStringList baseNameList = baseName.split("#");
	QStringList otherNameList = otherName.split("#");

	ListBaseName.append(baseInfo.fileName);
	ListBaseName.append(otherInfo.fileName);

	//���ǲ��컯����ΪNULL
	IEDsTreeWidget->setCurrentItem(NULL);

	substationWin->hide();
	progresCompareStyle->setPorgressValue(0);
	//QString textFile=baseInfo->fileName+"��"+otherInfo->fileName+"�Ƚ�:";
	QString textFile="  ���ڱȽ�SCD�ļ�:";
	progresCompareStyle->setFileText(textFile,0,0);
	progresCompareStyle->show();
	timerCompare->start(200);

	comparethread->setKey(baseKey,otherKey);
	comparethread->start();

	m_IsAutoCompare = false;
}

void  GraphicBrowserFrm::ClearTableWidgetData()
{
    qDebug()<<"ClearTableWidgetData--------1--------";
	gooseSendWidget->clearData();
	svSendWidget->clearData();
	gooseRecvWidget->clearData();
	svRecvWidget->clearData();
    qDebug()<<"ClearTableWidgetData--------2--------";
//    totalTab->clear();
    qDebug()<<"ClearTableWidgetData--------3--------";
}

void GraphicBrowserFrm::closeUnlockedVirDlg()
{
	QVector<int> toDeleteVirDlgs;
	for (int i = 0; i != lockedDialogVec.size(); i++)
	{
		if (!lockedDialogVec.at(i)->GetDialogLocked())
		{
			toDeleteVirDlgs.append(i);
			lockedDialogVec.at(i)->close();
		}
	}

	if (toDeleteVirDlgs.size() != 0)
	{
		for (int j = toDeleteVirDlgs.size() - 1; j != -1; j--)
		{
			delete lockedDialogVec.at(toDeleteVirDlgs.at(j));
			lockedDialogVec.remove(toDeleteVirDlgs.at(j));
		}
	}
}

void GraphicBrowserFrm::closeAllVirDlgs()
{
	logsrv_scoped_output(logsrv, "����closeAllVirDlgs����", "�˳�closeAllVirDlgs����", "GraphicBrowserFrm::closeAllVirDlgs");
	if (vDialog != NULL)
	{
		vDialog->SetDialogLocked(false);
		vDialog->close();
	}
	for (int i = 0; i != lockedDialogVec.size(); i++)
	{
		if (lockedDialogVec.at(i) == vDialog)
		{
			continue;
		}
		lockedDialogVec.at(i)->close();
		delete lockedDialogVec.at(i);
	}
	lockedDialogVec.clear();
}

bool GraphicBrowserFrm::isDeviceOpened()
{
    QString curSubId = IEDsLoader->GetCurrentSubstationID();
    if (curSubId.isEmpty())
        return true;

    QSubStationInfo *subInfo =  IEDsLoader->GetSubstationInfo("220kV." + curSubId);

    QString ext = subInfo->fileName.right(4);
    if (ext.compare(".scd", Qt::CaseInsensitive) == 0)
        return true;
    else if (ext.compare(".icd", Qt::CaseInsensitive) == 0 ||
             ext.compare(".cid", Qt::CaseInsensitive) == 0 ||
             ext.compare(".ssd", Qt::CaseInsensitive) == 0)
        return false;
    return false;
}

QString GraphicBrowserFrm::getDockTitleName()
{
    if (isDeviceOpened())
        return "  ���վ����";
    return "  װ������";
}

//======================================================SSD��Ϣ====================================================================================
void GraphicBrowserFrm::InitSSDUiWidget()
{
    m_IEDSSDTableHeader << "һ���豸����" << "һ���豸����" << "������ѹ�ȼ�" << "�������";
    // ��ʼ����ͷ
    m_TerminalTableHeader << "����" << "����" << "���վ" << "��ѹ�ȼ�" << "���" << "���ӵ�����" << "���ӵ�·��";
    m_DeviceTabletHeader << "����" << "����" << "�Ƿ�Ϊ�����豸" << "����";
    m_LnodeTableHeader << "iedName" << "ldInst" << "prefix" << "lnClass" << "lnInst" << "lnType";
    m_ConnectivityNodeTableHeader << "����" << "����" << "pathName";
    m_SSDValidateTableHeader << "У������" << "У����Ŀ" << "λ��" << "У����" << "�ο��淶" << "ԭ������";


    // -----------------------------------------------------------------------------����ͼ
    SSDTreeWidget     = new QTreeWidget(this);
    // ������ʱ չʾ���
    connect(SSDTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
        this, SLOT(SelectSSDTreeProc(QTreeWidgetItem*)));

    ExportSSDValidateExcelButton = new QPushButton(tr("���������"), this);
    connect(ExportSSDValidateExcelButton, SIGNAL(clicked()), this, SLOT(ExportSSDValidateExcel()));

    ssdTreeVLayout    = new QVBoxLayout(this);
    ssdTreeContainerWidget  = new QWidget(treeTabWidget);
    ssdTreeVLayout->addWidget(ExportSSDValidateExcelButton);
    ssdTreeVLayout->addWidget(SSDTreeWidget);
    ssdTreeContainerWidget->setLayout(ssdTreeVLayout); // չʾSSD��ѹ�ȼ������������

    // -----------------------------------------------------------------------------������ͼ
    // ������Ϣ
    basicInfoGroupBox = new QGroupBox(tr("������Ϣ"));
    basicNameLabel = new QLabel(tr("����:"));
    basicNameLabel->setFixedSize(basicNameLabel->sizeHint());
    basicDescLabel = new QLabel(tr("����:"));
    basicDescLabel->setFixedSize(basicDescLabel->sizeHint());
    basicDTypeLabel = new QLabel(tr("����:"));
    basicDTypeLabel->setFixedSize(basicDTypeLabel->sizeHint());
    basicAreaNameLabel = new QLabel(tr("��������:"));
    basicAreaNameLabel->setFixedSize(basicAreaNameLabel->sizeHint());
    basicAreaDescLabel = new QLabel(tr("��������:"));
    basicAreaDescLabel->setFixedSize(basicAreaDescLabel->sizeHint());
    basicName = new QLabel();
    basicDesc = new QLabel();
    basicDType = new QLabel();
    basicAreaName = new QLabel();
    basicAreaDesc = new QLabel();
    basicInfoLayout = new QHBoxLayout();
    basicInfoLayout->addWidget(basicNameLabel);
    basicInfoLayout->addWidget(basicName);
    basicInfoLayout->addWidget(basicDescLabel);
    basicInfoLayout->addWidget(basicDesc);
    basicInfoLayout->addWidget(basicDTypeLabel);
    basicInfoLayout->addWidget(basicDType);
    basicInfoLayout->addWidget(basicAreaNameLabel);
    basicInfoLayout->addWidget(basicAreaName);
    basicInfoLayout->addWidget(basicAreaDescLabel);
    basicInfoLayout->addWidget(basicAreaDesc);
    basicInfoGroupBox->setLayout(basicInfoLayout);

    basicValidateTableWidget = new QTableWidget();
    basicValidateTableWidget->setColumnCount(6);
    basicValidateTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    basicValidateTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    basicValidateTableWidget->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    basicValidateTableWidget->setHorizontalHeaderLabels(m_SSDValidateTableHeader);

    basicMainLayout = new QVBoxLayout();
    basicMainLayout->addWidget(basicInfoGroupBox);
    basicMainLayout->addWidget(basicValidateTableWidget);
    basicContainer = new QWidget();
    basicContainer->setLayout(basicMainLayout);

    // ��ѹ�ȼ���Ϣ
    vlInfoGroupBox = new QGroupBox(tr("������Ϣ"));
    vlNameLabel = new QLabel(tr("����:"));
    vlNameLabel->setFixedSize(vlNameLabel->sizeHint());
    vlDescLabel = new QLabel(tr("����:"));
    vlDescLabel->setFixedSize(vlDescLabel->sizeHint());
    vlMultiplierLabel = new QLabel(tr("Multiplier:"));
    vlMultiplierLabel->setFixedSize(vlMultiplierLabel->sizeHint());
    vlUnitLabel = new QLabel(tr("Unit:"));
    vlUnitLabel->setFixedSize(vlUnitLabel->sizeHint());
    vlVoltageLabel = new QLabel(tr("Voltageֵ:"));
    vlVoltageLabel->setFixedSize(vlVoltageLabel->sizeHint());

    vlName = new QLabel();
    vlDesc = new QLabel();
    vlMultiplier = new QLabel();
    vlUnit = new QLabel();
    vlVoltage = new QLabel();
    vlInfoLayout = new QHBoxLayout();
    vlInfoLayout->addWidget(vlNameLabel);
    vlInfoLayout->addWidget(vlName);
    vlInfoLayout->addWidget(vlDescLabel);
    vlInfoLayout->addWidget(vlDesc);
    vlInfoLayout->addWidget(vlMultiplierLabel);
    vlInfoLayout->addWidget(vlMultiplier);
    vlInfoLayout->addWidget(vlUnitLabel);
    vlInfoLayout->addWidget(vlUnit);
    vlInfoLayout->addWidget(vlVoltageLabel);
    vlInfoLayout->addWidget(vlVoltage);
    vlInfoGroupBox->setLayout(vlInfoLayout);

    vlValidateTableWidget = new QTableWidget();
    vlValidateTableWidget->setColumnCount(6);
    vlValidateTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    vlValidateTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    vlValidateTableWidget->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    vlValidateTableWidget->setHorizontalHeaderLabels(m_SSDValidateTableHeader);
    vlMainLayout = new QVBoxLayout();
    vlMainLayout->addWidget(vlInfoGroupBox);
    vlMainLayout->addWidget(vlValidateTableWidget);
    vlContainer = new QWidget();
    vlContainer->setLayout(vlMainLayout);

    // ���������Ϣ
    bayInfoGroupBox = new QGroupBox(tr("������Ϣ"));
    bayNameLabel = new QLabel(tr("�������:"), this);
    bayNameLabel->setFixedSize(bayNameLabel->sizeHint());
    bayDescLabel = new QLabel(tr("�������:"), this);
    bayDescLabel->setFixedSize(bayDescLabel->sizeHint());
    bayTypeLabel = new QLabel(tr("�������:"), this);
    bayTypeLabel->setFixedSize(bayTypeLabel->sizeHint());   
    bayName = new QLabel(this);
    bayDesc = new QLabel(this);
    bayType = new QLabel(this);
    bayInfoLayout = new QHBoxLayout(this);
    bayInfoLayout->addWidget(bayNameLabel);
    bayInfoLayout->addWidget(bayName);
    bayInfoLayout->addWidget(bayDescLabel);
    bayInfoLayout->addWidget(bayDesc);
    bayInfoLayout->addWidget(bayTypeLabel);
    bayInfoLayout->addWidget(bayType);
    bayInfoGroupBox->setLayout(bayInfoLayout);

    
    // ���������Ϣ
    bayInfoGroupBox = new QGroupBox(tr("������Ϣ"));
    bayNameLabel = new QLabel(tr("�������:"), this);
    bayNameLabel->setFixedSize(bayNameLabel->sizeHint());
    bayDescLabel = new QLabel(tr("�������:"), this);
    bayDescLabel->setFixedSize(bayDescLabel->sizeHint());
    bayTypeLabel = new QLabel(tr("�������:"), this);
    bayTypeLabel->setFixedSize(bayTypeLabel->sizeHint());   
    bayName = new QLabel(this);
    bayDesc = new QLabel(this);
    bayType = new QLabel(this);
    bayInfoLayout = new QHBoxLayout(this);
    bayInfoLayout->addWidget(bayNameLabel);
    bayInfoLayout->addWidget(bayName);
    bayInfoLayout->addWidget(bayDescLabel);
    bayInfoLayout->addWidget(bayDesc);
    bayInfoLayout->addWidget(bayTypeLabel);
    bayInfoLayout->addWidget(bayType);
    bayInfoGroupBox->setLayout(bayInfoLayout);
    
    // ��ѹ���豸
    powerTransformWidget = new QWidget(this);
    powerTransformLayout = new QVBoxLayout(this);

    powerTransformInfoGroupBox = new QGroupBox();
    ptNameLabel = new QLabel("�豸����:", this);
    ptNameLabel->setFixedSize(ptNameLabel->sizeHint());
    ptDescLabel = new QLabel("�豸����:", this);
    ptDescLabel->setFixedSize(ptDescLabel->sizeHint());
    ptVirtualLabel = new QLabel("�Ƿ�Ϊ�����豸:", this);
    ptVirtualLabel->setFixedSize(ptVirtualLabel->sizeHint());
    ptTypeLabel = new QLabel("�豸����:", this);
    ptTypeLabel->setFixedSize(ptTypeLabel->sizeHint());
    ptType = new QLabel();
    ptNameIcon = new QLabel(); // ��ʾ�����ͼ��
    ptNameIcon->hide();
    ptName = new QLabel();
    ptDesc = new QLabel();
    ptVirtual  = new QLabel();
    ptInfoLayout = new QHBoxLayout();
    ptInfoLayout->addWidget(ptNameLabel);
    ptInfoLayout->addWidget(ptNameIcon);
    ptInfoLayout->addWidget(ptName);
    ptInfoLayout->addWidget(ptDescLabel);
    ptInfoLayout->addWidget(ptDesc);
    ptInfoLayout->addWidget(ptVirtualLabel);
    ptInfoLayout->addWidget(ptVirtual);
    ptInfoLayout->addWidget(ptTypeLabel);
    ptInfoLayout->addWidget(ptType);
    powerTransformInfoGroupBox->setLayout(ptInfoLayout);
    powerTransformLayout->addWidget(powerTransformInfoGroupBox); // ��ӵ�����
    
    // �����豸
    ptTWTableWidget = new QTableWidget();
    ptTWTableWidget->setColumnCount(4);
    ptTWTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptTWTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    ptTWTableWidget->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    ptTWTableWidget->setHorizontalHeaderLabels(m_DeviceTabletHeader);
    connect(ptTWTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(fillPowerTransformTWChild()));

    // LNode�豸
    ptlnodeTableWidget = new QTableWidget();
    ptlnodeTableWidget->setColumnCount(6);
    ptlnodeTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptlnodeTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    ptlnodeTableWidget->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    ptlnodeTableWidget->setHorizontalHeaderLabels(m_LnodeTableHeader);
    connect(ptlnodeTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

    ptChildTabWidget = new QTabWidget();
    ptChildTabWidget->setTabPosition(QTabWidget::West);
    ptChildTabWidget->addTab(ptTWTableWidget, "����");
    ptChildTabWidget->addTab(ptlnodeTableWidget, "LNode");
    powerTransformLayout->addWidget(ptChildTabWidget);

    // Terminal
    ptTWTerminalTable = new QTableWidget();
    ptTWTerminalTable->setColumnCount(7);
    ptTWTerminalTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptTWTerminalTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    ptTWTerminalTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    ptTWTerminalTable->setHorizontalHeaderLabels(m_TerminalTableHeader);
    // LNode
    ptTWLNodeTable = new QTableWidget();
    ptTWLNodeTable->setColumnCount(6);
    ptTWLNodeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ptTWLNodeTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    ptTWLNodeTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    ptTWLNodeTable->setHorizontalHeaderLabels(m_LnodeTableHeader);
    connect(ptTWLNodeTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

    ptTWChildTabWidget = new QTabWidget();
    //ptTWChildTabWidget->addTab(ptTWTerminalTable, "Terminal");
    //ptTWChildTabWidget->addTab(ptTWLNodeTable, "LNode");
    
    powerTransformLayout->addWidget(ptTWChildTabWidget);
    powerTransformWidget->setLayout(powerTransformLayout);


    // �����豸
    conductingEquipmentTable = new QTableWidget();
    conductingEquipmentTable->setColumnCount(4);
    conductingEquipmentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    conductingEquipmentTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    conductingEquipmentTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    conductingEquipmentTable->setHorizontalHeaderLabels(m_DeviceTabletHeader);
    connect(conductingEquipmentTable, SIGNAL(itemSelectionChanged()), this, SLOT(fillConductingEquipmentChild()));
    
    // Terminal
    ceTerminalTable = new QTableWidget();
    ceTerminalTable->setColumnCount(7);
    ceTerminalTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ceTerminalTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    ceTerminalTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    ceTerminalTable->setHorizontalHeaderLabels(m_TerminalTableHeader);
    // LNode
    ceLNodeTable = new QTableWidget();
    ceLNodeTable->setColumnCount(6);
    ceLNodeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ceLNodeTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    ceLNodeTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    ceLNodeTable->setHorizontalHeaderLabels(m_LnodeTableHeader);
    connect(ceLNodeTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

    ceChildTab = new QTabWidget();
    //ceChildTab->addTab(ceTerminalTable, "Terminal");
    //ceChildTab->addTab(ceLNodeTable, "LNode");

    conductingEquipmentLayout = new QVBoxLayout();
    conductingEquipmentLayout->addWidget(conductingEquipmentTable);
    conductingEquipmentLayout->addWidget(ceChildTab);
    conductingEquipmentWidget = new QWidget();
    conductingEquipmentWidget->setLayout(conductingEquipmentLayout);

    //���ӵ�
    connectivityNodeTable = new QTableWidget();
    connectivityNodeTable->setColumnCount(3);
    connectivityNodeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connectivityNodeTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    connectivityNodeTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    connectivityNodeTable->setHorizontalHeaderLabels(m_ConnectivityNodeTableHeader);

    connectivityNodeLayout = new QVBoxLayout();
    connectivityNodeLayout->addWidget(connectivityNodeTable);
    connectivityNodeWidget = new QWidget();
    connectivityNodeWidget->setLayout(connectivityNodeLayout);

    // ����豸��Ϣ
    bayDeviceTabWidget = new QTabWidget();
    //bayDeviceTabWidget->addTab(powerTransformWidget, "��ѹ��");
    //bayDeviceTabWidget->addTab(conductingEquipmentWidget, "�����豸");
    //bayDeviceTabWidget->addTab(connectivityNodeWidget, "���ӵ�");

    widgetIEDSSDTable = new QTableWidget();
    widgetIEDSSDTable->setColumnCount(4);
    widgetIEDSSDTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    widgetIEDSSDTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭;
    widgetIEDSSDTable->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ;
    widgetIEDSSDTable->setHorizontalHeaderLabels(m_IEDSSDTableHeader);
    connect(widgetIEDSSDTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickdobleCell(int,int)));

    bayContainer = new QWidget(this);
    bayMainLayout = new QVBoxLayout(this);
    bayMainLayout->addWidget(bayInfoGroupBox);
    bayMainLayout->addWidget(bayDeviceTabWidget);
    bayContainer->setLayout(bayMainLayout);
}

// һ���豸������
void GraphicBrowserFrm::InitSSDTreeWidget()
{
    SSDTreeWidget->clear();
    QStringList lists;
    lists<<"����б�";
    SSDTreeWidget->setHeaderLabels(lists);

    // ��ȡ��ѹ�ȼ�;
    QVector<QString> voltageLevelList;
    m_SSDInfo->GetVoltagelevelListFromSSDInfo(voltageLevelList);

    QList<QTreeWidgetItem*> itemList;
    // ��ӻ�����Ϣ
    QTreeWidgetItem *ssdBasicRootItem = new QTreeWidgetItem();
    ssdBasicRootItem->setText(0, "������Ϣ");
    QString subStationName = QString::fromStdString(m_SSDInfo->m_SSDSubStationCache.name);
    SetSSDValidateInfoForTableWidgetByKey(ssdBasicRootItem, subStationName);
    itemList.append(ssdBasicRootItem);
    

    for (int i = 0; i < voltageLevelList.size(); i++)
    {
        QTreeWidgetItem *voltageLevelItem = new QTreeWidgetItem();
        voltageLevelItem->setText(0, voltageLevelList.at(i));
        QString vlKey = subStationName + "/" + voltageLevelList.at(i);
        SetSSDValidateInfoForTableWidgetByKey(voltageLevelItem, vlKey);

        QMap<QString, QString> bayListMap;
        m_SSDInfo->GetBayListByVoltageLevel(voltageLevelList.at(i), bayListMap);
        for (int j = 0; j < bayListMap.size(); j++)
        {
            QTreeWidgetItem *bayItem = new QTreeWidgetItem(voltageLevelItem);
            if ((bayListMap.begin() + j).value().size() > 0)
            {
                bayItem->setText(0, (bayListMap.begin()+j).value());
            }
            else
            {
                bayItem->setText(0, (bayListMap.begin()+j).key());
            }
            
            QString bayKey = vlKey + "/" + (bayListMap.begin()+j).key();
            SetSSDValidateInfoForTableWidgetByKey(bayItem, bayKey);
        }
        itemList.append(voltageLevelItem);
    }
    SSDTreeWidget->insertTopLevelItems(0, itemList);
}

GraphicBrowserFrm::Flags GraphicBrowserFrm::GetSSDValidateInfoByKey(QString key, QString &info)
{
    GraphicBrowserFrm::Flags flag = OK;

    for (int i = 0; i < IEDsLoader->m_SCLValid->m_SSDResultMap[key].size(); i++)
    {
        CheckInfo temp = IEDsLoader->m_SCLValid->m_CheckResultVec[IEDsLoader->m_SCLValid->m_SSDResultMap[key].at(i)];
        info += temp.sDesc + "\n";
        if (temp.sFlag == "����" && flag > E)
        {
            flag = E;
        }
        else if (temp.sFlag == "����" && flag > W)
        {
            flag = W;
        }
        else if (temp.sFlag == "��ʾ" && flag > R)
        {
            flag = R;
        }
    }  
    return flag;
}

void GraphicBrowserFrm::SetSSDValidateInfoForTableWidgetByKey(QTreeWidgetItem* item, QString key)
{
    GraphicBrowserFrm::Flags flag = OK;
    QString toolTipInfo;
    for (int i = 0; i < IEDsLoader->m_SCLValid->m_SSDResultMap[key].size(); i++)
    {
        CheckInfo temp = IEDsLoader->m_SCLValid->m_CheckResultVec[IEDsLoader->m_SCLValid->m_SSDResultMap[key].at(i)];
        toolTipInfo += temp.sDesc + "\n";
        if (temp.sFlag == "����" && flag > E)
        {
            flag = E;
        }
        else if (temp.sFlag == "����" && flag > W)
        {
            flag = W;
        }
        else if (temp.sFlag == "��ʾ" && flag > R)
        {
            flag = R;
        }
    }  

    item->setToolTip(0, toolTipInfo);
    if (flag == E)
    {
        item->setIcon(0, QIcon("images/Error.png"));
    }
    else if (flag == W)
    {
        item->setIcon(0, QIcon("images/Warn.png"));
    }
    else if (flag == R)
    {
        item->setIcon(0, QIcon("images/info.png"));
    }
}

void GraphicBrowserFrm::SetSSDValidateInfoForTableWidgetByKey(QTableWidgetItem* item, QString key)
{
    GraphicBrowserFrm::Flags flag = OK;
    QString toolTipInfo;
    for (int i = 0; i < IEDsLoader->m_SCLValid->m_SSDResultMap[key].size(); i++)
    {
        CheckInfo temp = IEDsLoader->m_SCLValid->m_CheckResultVec[IEDsLoader->m_SCLValid->m_SSDResultMap[key].at(i)];
        toolTipInfo += temp.sDesc + "\n";
        if (temp.sFlag == "����" && flag > E)
        {
            flag = E;
        }
        else if (temp.sFlag == "����" && flag > W)
        {
            flag = W;
        }
        else if (temp.sFlag == "��ʾ" && flag > R)
        {
            flag = R;
        }
    }  

    item->setToolTip(toolTipInfo);
    if (flag == E)
    {
        item->setIcon(QIcon("images/Error.png"));
    }
    else if (flag == W)
    {
        item->setIcon(QIcon("images/Warn.png"));
    }
    else if (flag == R)
    {
        item->setIcon(QIcon("images/info.png"));
    }
}

// ѡ��������
void GraphicBrowserFrm::SelectSSDTreeProc(QTreeWidgetItem* item)
{
    m_CurrentSSDTreeKey = "";
    if (item == NULL)
    {
        return;
    }

    QSSDSubstationCache ssdBasicInfo;
    m_SSDInfo->GetSubStationInfo(ssdBasicInfo);
    
    QTreeWidgetItem * parent = item->parent();
    if (NULL == parent)
    {      
        QString text = item->text(0);
        if (text == "������Ϣ")
        {
            m_CurrentSSDTreeKey = ssdBasicInfo.name;

            basicName->setText(ssdBasicInfo.name);
            basicDesc->setText(ssdBasicInfo.desc);
            basicDType->setText(ssdBasicInfo.dtype);
            basicAreaName->setText(ssdBasicInfo.areaName);
            basicAreaDesc->setText(ssdBasicInfo.areaDesc);

            QVector<int> ssdBasicValidateList = IEDsLoader->m_SCLValid->m_SSDResultMap[ssdBasicInfo.name];

            basicValidateTableWidget->clear();
            basicValidateTableWidget->setRowCount(0);
            basicValidateTableWidget->setHorizontalHeaderLabels(m_SSDValidateTableHeader);
            for (int i = 0; i < ssdBasicValidateList.size(); i++)
            {
                CheckInfo temp = m_SCLValid->m_CheckResultVec.at(ssdBasicValidateList.at(i));

                QTableWidgetItem *orderItem = new QTableWidgetItem(QString::number(i+1));
                QTableWidgetItem *sFlagItem = new QTableWidgetItem(temp.sFlag);
                QTableWidgetItem *sClassItem = new QTableWidgetItem(temp.sClass);
                QTableWidgetItem *positionItem = new QTableWidgetItem(temp.position);
                QTableWidgetItem *sDescItem = new QTableWidgetItem(temp.sDesc);
                QTableWidgetItem *sStandardItem = new QTableWidgetItem(temp.sStandard);
                QTableWidgetItem *sXmlItem = new QTableWidgetItem(temp.sXml);

                basicValidateTableWidget->insertRow(i);
                // basicValidateTableWidget->setItem(i, 0, orderItem);
                basicValidateTableWidget->setItem(i, 0, sFlagItem);
                basicValidateTableWidget->setItem(i, 1, sClassItem);
                basicValidateTableWidget->setItem(i, 2, positionItem);
                basicValidateTableWidget->setItem(i, 3, sDescItem);
                basicValidateTableWidget->setItem(i, 4, sStandardItem);
                basicValidateTableWidget->setItem(i, 5, sXmlItem);                
            }
            basicValidateTableWidget->resizeColumnsToContents();
            totalTab->clear();
            totalTab->tabbarhide();
            totalTab->addTab(basicContainer,"SSDBasicInfo");
        }
        else
        {
            QString key = ssdBasicInfo.name + "/" + text;
            m_CurrentSSDTreeKey = key;

            QVoltageLevelCache vlCache;
            m_SSDInfo->GetSSDVoltageLevelInfoByKey(text, vlCache);
            vlName->setText(vlCache.name);
            vlDesc->setText(vlCache.desc);
            vlMultiplier->setText(vlCache.voltage.multiplier);
            vlUnit->setText(vlCache.voltage.unit);
            vlVoltage->setText(vlCache.voltage.text);


            QVector<int> ssdVlValidateList = IEDsLoader->m_SCLValid->m_SSDResultMap[key];
            vlValidateTableWidget->clear();
            vlValidateTableWidget->setHorizontalHeaderLabels(m_SSDValidateTableHeader);
            vlValidateTableWidget->setRowCount(0);

            for (int i = 0; i < ssdVlValidateList.size(); i++)
            {
                CheckInfo temp = m_SCLValid->m_CheckResultVec.at(ssdVlValidateList.at(i));
                
                // QTableWidgetItem *orderItem = new QTableWidgetItem(QString::number(i+1));
                QTableWidgetItem *sFlagItem = new QTableWidgetItem(temp.sFlag);
                QTableWidgetItem *sClassItem = new QTableWidgetItem(temp.sClass);
                QTableWidgetItem *positionItem = new QTableWidgetItem(temp.position);
                QTableWidgetItem *sDescItem = new QTableWidgetItem(temp.sDesc);
                QTableWidgetItem *sStandardItem = new QTableWidgetItem(temp.sStandard);
                QTableWidgetItem *sXmlItem = new QTableWidgetItem(temp.sXml);

                vlValidateTableWidget->insertRow(i);
                // vlValidateTableWidget->setItem(i, 0, orderItem);
                vlValidateTableWidget->setItem(i, 0, sFlagItem);
                vlValidateTableWidget->setItem(i, 1, sClassItem);
                vlValidateTableWidget->setItem(i, 2, positionItem);
                vlValidateTableWidget->setItem(i, 3, sDescItem);
                vlValidateTableWidget->setItem(i, 4, sStandardItem);
                vlValidateTableWidget->setItem(i, 5, sXmlItem);                
            }
            vlValidateTableWidget->resizeColumnsToContents();
            totalTab->clear();
            totalTab->tabbarhide();
            totalTab->addTab(vlContainer,"VoltageInfo");
        }
        return;
    }

    
    // ��ȡ�����Ϣ
    QString voltageLevel = parent->text(0);
    QString bay = item->text(0);


        // �����Ϣ
    QMap<QString, QString> bayListMap;
    m_SSDInfo->GetBayListByVoltageLevel(voltageLevel, bayListMap);
    
    QMap<QString, QString>::iterator it = bayListMap.find(bay);
    if (it == bayListMap.end())
    {
        QMap<QString, QString>::iterator itBay = bayListMap.begin();
        while (itBay != bayListMap.end())
        {
            if (itBay.value() == bay)
            {
                bay = itBay.key();
                break;
            }
            itBay++;
        }
    }

    fillSSDBayContainer(voltageLevel, bay);

    //QBayCache bayInfo;
    //ssdInfo.GetBayInfoByBayKey(voltageLevel, bay, bayInfo);
    //bayName->setText(bayInfo.name);
    //bayDesc->setText(bayInfo.desc);
    //bayType->setText(bayInfo.dtype);

    //m_CurrentSSDTreeKey = ssdBasicInfo.name + "/" + voltageLevel + "/" + bayInfo.name;

    //bayDeviceTabWidget->clear();
    //// ���±�ѹ����Ϣ
    //m_ptVec.clear();
    //ssdInfo.GetPowerTransformByBay(voltageLevel, bay, m_ptVec);
    //if (m_ptVec.size() > 0)
    //{
    //    // ������Ϣ
    //    ptName->setText(m_ptVec.at(0).name);
    //    QString deviceKey = m_CurrentSSDTreeKey + "/" + m_ptVec.at(0).name;
    //    QString toolTipInfo;
    //    GraphicBrowserFrm::Flags flag = GetSSDValidateInfoByKey(deviceKey, toolTipInfo);
    //    if (flag == E)
    //    {
    //        ptNameIcon->show();
    //        ptNameIcon->setPixmap(QPixmap("images/Error.png"));
    //        ptNameIcon->setFixedSize(QPixmap("images/Error.png").size());
    //        ptNameIcon->setToolTip(toolTipInfo);
    //    }
    //    else if (flag == W)
    //    {
    //        ptNameIcon->show();
    //        ptNameIcon->setPixmap(QPixmap("images/Warn.png"));
    //        ptNameIcon->setFixedSize(QPixmap("images/Warn.png").size());
    //        ptNameIcon->setToolTip(toolTipInfo);
    //    }
    //    else if (flag == R)
    //    {
    //        ptNameIcon->show();
    //        ptNameIcon->setPixmap(QPixmap("images/info.png"));
    //        ptNameIcon->setFixedSize(QPixmap("images/info.png").size());
    //        ptNameIcon->setToolTip(toolTipInfo);
    //    }
    //    
    //    ptDesc->setText(m_ptVec.at(0).desc);
    //    ptVirtual->setText(m_ptVec.at(0).vir);
    //    ptType->setText(m_ptVec.at(0).type);

    //    ptChildTabWidget->clear();

    //    // ������Ϣ
    //    if (m_ptVec.at(0).tfWindingVec.size() > 0)
    //    {
    //        ptTWTableWidget->clear();
    //        ptTWTableWidget->setHorizontalHeaderLabels(m_DeviceTabletHeader);
    //        ptTWTableWidget->setRowCount(0);
    //        for (int i = 0; i < m_ptVec.at(0).tfWindingVec.size(); i++)
    //        {

    //            QTransformerWindingCache twTemp = m_ptVec.at(0).tfWindingVec.at(i);
    //            QTableWidgetItem *twName = new QTableWidgetItem(twTemp.name);
    //            SetSSDValidateInfoForTableWidgetByKey(twName, deviceKey + "/" + twTemp.name);
    //            QTableWidgetItem *twDesc = new QTableWidgetItem(twTemp.desc);
    //            QTableWidgetItem *twVir = new QTableWidgetItem(twTemp.vir);
    //            QTableWidgetItem *twType = new QTableWidgetItem(twTemp.type);

    //            if (i == 0)
    //            {
    //                ptTWTableWidget->selectRow(0);
    //            }

    //            ptTWTableWidget->insertRow(i);
    //            ptTWTableWidget->setItem(i, 0, twName);
    //            ptTWTableWidget->setItem(i, 1, twDesc);
    //            ptTWTableWidget->setItem(i, 2, twVir);
    //            ptTWTableWidget->setItem(i, 3, twType);
    //        }
    //        fillPowerTransformTWChild();
    //        ptTWTableWidget->resizeColumnsToContents();
    //        ptChildTabWidget->addTab(ptTWTableWidget, "����");
    //    }      

    //    // LNode ��Ϣ
    //    if (m_ptVec.at(0).lnodeVec.size() > 0)
    //    {
    //        ptlnodeTableWidget->clear();
    //        ptlnodeTableWidget->setHorizontalHeaderLabels(m_LnodeTableHeader);
    //        ptlnodeTableWidget->setRowCount(0);
    //        for (int i = 0; i < m_ptVec.at(0).lnodeVec.size(); i++)
    //        {

    //            QLNodeCache lnTemp = m_ptVec.at(0).lnodeVec.at(i);
    //            QTableWidgetItem *lnIedName = new QTableWidgetItem(lnTemp.iedName);
    //            QTableWidgetItem *lnLdInst = new QTableWidgetItem(lnTemp.ldInst);
    //            QTableWidgetItem *lnPrefix = new QTableWidgetItem(lnTemp.prefix);
    //            QTableWidgetItem *lnLnClass = new QTableWidgetItem(lnTemp.lnClass);
    //            QTableWidgetItem *lnLnInst = new QTableWidgetItem(lnTemp.lnInst);
    //            QTableWidgetItem *lnLnType = new QTableWidgetItem(lnTemp.lnType);

    //            ptlnodeTableWidget->insertRow(i);
    //            ptlnodeTableWidget->setItem(i, 0, lnIedName);
    //            ptlnodeTableWidget->setItem(i, 1, lnLdInst);
    //            ptlnodeTableWidget->setItem(i, 2, lnPrefix);
    //            ptlnodeTableWidget->setItem(i, 3, lnLnClass);
    //            ptlnodeTableWidget->setItem(i, 4, lnLnInst);
    //            ptlnodeTableWidget->setItem(i, 5, lnLnType);
    //        }
    //        ptlnodeTableWidget->resizeColumnsToContents();
    //        ptChildTabWidget->addTab(ptlnodeTableWidget, "LNode");
    //    }
    //    

    //    bayDeviceTabWidget->addTab(powerTransformWidget, "��ѹ��");
    //}

    //// ���µ����豸
    //m_ceVec.clear();
    //ssdInfo.GetConductingEquipmentByBay(voltageLevel, bay, m_ceVec);
    //if (m_ceVec.size() > 0)
    //{
    //    conductingEquipmentTable->clear();
    //    conductingEquipmentTable->setHorizontalHeaderLabels(m_DeviceTabletHeader);
    //    conductingEquipmentTable->setRowCount(0);
    //    for (int i = 0; i < m_ceVec.size(); i++)
    //    {
    //        QConductingEquipmentCache ceTemp = m_ceVec.at(i);
    //        QTableWidgetItem *ceName = new QTableWidgetItem(ceTemp.name);
    //        SetSSDValidateInfoForTableWidgetByKey(ceName, m_CurrentSSDTreeKey+"/"+ceTemp.name);
    //        QTableWidgetItem *ceDesc = new QTableWidgetItem(ceTemp.desc);
    //        QTableWidgetItem *ceVir = new QTableWidgetItem(ceTemp.vir);
    //        QTableWidgetItem *ceType = new QTableWidgetItem(ceTemp.type);
    //        
    //        conductingEquipmentTable->insertRow(i);
    //        conductingEquipmentTable->setItem(i, 0, ceName);
    //        conductingEquipmentTable->setItem(i, 1, ceDesc);
    //        conductingEquipmentTable->setItem(i, 2, ceVir);
    //        conductingEquipmentTable->setItem(i, 3, ceType);
    //    }
    //    fillConductingEquipmentChild();
    //    conductingEquipmentTable->resizeColumnsToContents();
    //    bayDeviceTabWidget->addTab(conductingEquipmentWidget, "�����豸");
    //}

    //// ���½ڵ�
    //m_cnVec.clear();
    //ssdInfo.GetConnectivityNodeByBay(voltageLevel, bay, m_cnVec);
    //if (m_cnVec.size() > 0)
    //{
    //    connectivityNodeTable->clear();
    //    connectivityNodeTable->setHorizontalHeaderLabels(m_ConnectivityNodeTableHeader);
    //    connectivityNodeTable->setRowCount(0);
    //    for (int i = 0; i < m_cnVec.size(); i++)
    //    {
    //        QConnectivityNodeCache cnTemp = m_cnVec.at(i);
    //        QTableWidgetItem *cnName = new QTableWidgetItem(cnTemp.name);
    //        SetSSDValidateInfoForTableWidgetByKey(cnName, m_CurrentSSDTreeKey+"/"+cnTemp.name);
    //        QTableWidgetItem *cnDesc = new QTableWidgetItem(cnTemp.desc);
    //        QTableWidgetItem *cnPath = new QTableWidgetItem(cnTemp.pathname);

    //        connectivityNodeTable->insertRow(i);
    //        connectivityNodeTable->setItem(i, 0, cnName);
    //        connectivityNodeTable->setItem(i, 1, cnDesc);
    //        connectivityNodeTable->setItem(i, 2, cnPath);
    //    }
    //    connectivityNodeTable->resizeColumnsToContents();
    //    bayDeviceTabWidget->addTab(connectivityNodeWidget, "���ӵ�");
    //}

    totalTab->clear();
    totalTab->tabbarhide();
    totalTab->addTab(bayContainer,"SSD");
}

void GraphicBrowserFrm::fillSSDBayContainer(QString voltageLevel, QString bay)
{
    QBayCache bayInfo;
    m_SSDInfo->GetBayInfoByBayKey(voltageLevel, bay, bayInfo);
    bayName->setText(bayInfo.name);
    bayDesc->setText(bayInfo.desc);
    bayType->setText(bayInfo.dtype);

    m_CurrentSSDTreeKey = QString::fromStdString(m_SSDInfo->m_SSDSubStationCache.name) + "/" + voltageLevel + "/" + bayInfo.name;

    bayDeviceTabWidget->clear();
    // ���±�ѹ����Ϣ
    m_ptVec.clear();
    m_SSDInfo->GetPowerTransformByBay(voltageLevel, bay, m_ptVec);
    if (m_ptVec.size() > 0)
    {
        // ������Ϣ
        ptName->setText(m_ptVec.at(0).name);
        QString deviceKey = m_CurrentSSDTreeKey + "/" + m_ptVec.at(0).name;
        QString toolTipInfo;
        GraphicBrowserFrm::Flags flag = GetSSDValidateInfoByKey(deviceKey, toolTipInfo);
        if (flag == E)
        {
            ptNameIcon->show();
            ptNameIcon->setPixmap(QPixmap("images/Error.png"));
            ptNameIcon->setFixedSize(QPixmap("images/Error.png").size());
            ptNameIcon->setToolTip(toolTipInfo);
        }
        else if (flag == W)
        {
            ptNameIcon->show();
            ptNameIcon->setPixmap(QPixmap("images/Warn.png"));
            ptNameIcon->setFixedSize(QPixmap("images/Warn.png").size());
            ptNameIcon->setToolTip(toolTipInfo);
        }
        else if (flag == R)
        {
            ptNameIcon->show();
            ptNameIcon->setPixmap(QPixmap("images/info.png"));
            ptNameIcon->setFixedSize(QPixmap("images/info.png").size());
            ptNameIcon->setToolTip(toolTipInfo);
        }

        ptDesc->setText(m_ptVec.at(0).desc);
        ptVirtual->setText(m_ptVec.at(0).vir);
        ptType->setText(m_ptVec.at(0).type);

        ptChildTabWidget->clear();

        // ������Ϣ
        if (m_ptVec.at(0).tfWindingVec.size() > 0)
        {
            ptTWTableWidget->clear();
            ptTWTableWidget->setHorizontalHeaderLabels(m_DeviceTabletHeader);
            ptTWTableWidget->setRowCount(0);
            for (int i = 0; i < m_ptVec.at(0).tfWindingVec.size(); i++)
            {

                QTransformerWindingCache twTemp = m_ptVec.at(0).tfWindingVec.at(i);
                QTableWidgetItem *twName = new QTableWidgetItem(twTemp.name);
                SetSSDValidateInfoForTableWidgetByKey(twName, deviceKey + "/" + twTemp.name);
                QTableWidgetItem *twDesc = new QTableWidgetItem(twTemp.desc);
                QTableWidgetItem *twVir = new QTableWidgetItem(twTemp.vir);
                QTableWidgetItem *twType = new QTableWidgetItem(twTemp.type);

                if (i == 0)
                {
                    ptTWTableWidget->selectRow(0);
                }

                ptTWTableWidget->insertRow(i);
                ptTWTableWidget->setItem(i, 0, twName);
                ptTWTableWidget->setItem(i, 1, twDesc);
                ptTWTableWidget->setItem(i, 2, twVir);
                ptTWTableWidget->setItem(i, 3, twType);
            }
            fillPowerTransformTWChild();
            ptTWTableWidget->resizeColumnsToContents();
            ptChildTabWidget->addTab(ptTWTableWidget, "����");
        }      

        // LNode ��Ϣ
        if (m_ptVec.at(0).lnodeVec.size() > 0)
        {
            ptlnodeTableWidget->clear();
            ptlnodeTableWidget->setHorizontalHeaderLabels(m_LnodeTableHeader);
            ptlnodeTableWidget->setRowCount(0);
            for (int i = 0; i < m_ptVec.at(0).lnodeVec.size(); i++)
            {

                QLNodeCache lnTemp = m_ptVec.at(0).lnodeVec.at(i);
                QTableWidgetItem *lnIedName = new QTableWidgetItem(lnTemp.iedName);
                QTableWidgetItem *lnLdInst = new QTableWidgetItem(lnTemp.ldInst);
                QTableWidgetItem *lnPrefix = new QTableWidgetItem(lnTemp.prefix);
                QTableWidgetItem *lnLnClass = new QTableWidgetItem(lnTemp.lnClass);
                QTableWidgetItem *lnLnInst = new QTableWidgetItem(lnTemp.lnInst);
                QTableWidgetItem *lnLnType = new QTableWidgetItem(lnTemp.lnType);

                ptlnodeTableWidget->insertRow(i);
                ptlnodeTableWidget->setItem(i, 0, lnIedName);
                ptlnodeTableWidget->setItem(i, 1, lnLdInst);
                ptlnodeTableWidget->setItem(i, 2, lnPrefix);
                ptlnodeTableWidget->setItem(i, 3, lnLnClass);
                ptlnodeTableWidget->setItem(i, 4, lnLnInst);
                ptlnodeTableWidget->setItem(i, 5, lnLnType);
            }
            ptlnodeTableWidget->resizeColumnsToContents();
            ptChildTabWidget->addTab(ptlnodeTableWidget, "LNode");
        }


        bayDeviceTabWidget->addTab(powerTransformWidget, "��ѹ��");
    }

    // ���µ����豸
    m_ceVec.clear();
    m_SSDInfo->GetConductingEquipmentByBay(voltageLevel, bay, m_ceVec);
    if (m_ceVec.size() > 0)
    {
        conductingEquipmentTable->clear();
        conductingEquipmentTable->setHorizontalHeaderLabels(m_DeviceTabletHeader);
        conductingEquipmentTable->setRowCount(0);
        for (int i = 0; i < m_ceVec.size(); i++)
        {
            QConductingEquipmentCache ceTemp = m_ceVec.at(i);
            QTableWidgetItem *ceName = new QTableWidgetItem(ceTemp.name);
            SetSSDValidateInfoForTableWidgetByKey(ceName, m_CurrentSSDTreeKey+"/"+ceTemp.name);
            QTableWidgetItem *ceDesc = new QTableWidgetItem(ceTemp.desc);
            QTableWidgetItem *ceVir = new QTableWidgetItem(ceTemp.vir);
            QTableWidgetItem *ceType = new QTableWidgetItem(ceTemp.type);

            conductingEquipmentTable->insertRow(i);
            conductingEquipmentTable->setItem(i, 0, ceName);
            conductingEquipmentTable->setItem(i, 1, ceDesc);
            conductingEquipmentTable->setItem(i, 2, ceVir);
            conductingEquipmentTable->setItem(i, 3, ceType);
        }
        fillConductingEquipmentChild();
        conductingEquipmentTable->resizeColumnsToContents();
        bayDeviceTabWidget->addTab(conductingEquipmentWidget, "�����豸");
    }

    // ���½ڵ�
    m_cnVec.clear();
    m_SSDInfo->GetConnectivityNodeByBay(voltageLevel, bay, m_cnVec);
    if (m_cnVec.size() > 0)
    {
        connectivityNodeTable->clear();
        connectivityNodeTable->setHorizontalHeaderLabels(m_ConnectivityNodeTableHeader);
        connectivityNodeTable->setRowCount(0);
        for (int i = 0; i < m_cnVec.size(); i++)
        {
            QConnectivityNodeCache cnTemp = m_cnVec.at(i);
            QTableWidgetItem *cnName = new QTableWidgetItem(cnTemp.name);
            SetSSDValidateInfoForTableWidgetByKey(cnName, m_CurrentSSDTreeKey+"/"+cnTemp.name);
            QTableWidgetItem *cnDesc = new QTableWidgetItem(cnTemp.desc);
            QTableWidgetItem *cnPath = new QTableWidgetItem(cnTemp.pathname);

            connectivityNodeTable->insertRow(i);
            connectivityNodeTable->setItem(i, 0, cnName);
            connectivityNodeTable->setItem(i, 1, cnDesc);
            connectivityNodeTable->setItem(i, 2, cnPath);
        }
        connectivityNodeTable->resizeColumnsToContents();
        bayDeviceTabWidget->addTab(connectivityNodeWidget, "���ӵ�");
    }

}

void GraphicBrowserFrm::ExportSSDValidateExcel()
{
    QString subKey = IEDsLoader->GetCurrentSubstationID();
    QSubStationInfo *subInfo = IEDsLoader->GetSubstationInfo("220kV." + subKey);
    QString fileName = subInfo->Name + tr("-��ⱨ��");

    QString filePath = QFileDialog::getSaveFileName(NULL, "������ⱨ��", fileName, "*.xls");  
    if (!filePath.isEmpty())  
    {  
        if (m_SCLValid->ExportSSDExcel(filePath, subKey))
        {
            QMessageBox::information(NULL, "������ⱨ��", "�����ɹ�!");
        }
        else
        {
            QMessageBox::warning(NULL, "������ⱨ��", "����ʧ��!");
        }
    }
}    

void GraphicBrowserFrm::fillPowerTransformTWChild()
{
    if (m_ptVec.at(0).tfWindingVec.size() == 0)
    {
        return;
    }

    QTransformerWindingCache tfWindingTemp;
    if (ptTWTableWidget->selectedItems().size() == 0)
    {
        tfWindingTemp = m_ptVec.at(0).tfWindingVec.at(0);
    }
    else
    {
        int selectRow = ptTWTableWidget->row(ptTWTableWidget->selectedItems().at(0));
        if (selectRow < m_ptVec.at(0).tfWindingVec.size())
        {
           tfWindingTemp = m_ptVec.at(0).tfWindingVec.at(selectRow);
        }
    }

     QString currentDeviceKey = m_CurrentSSDTreeKey + "/" + m_ptVec.at(0).name + "/" + tfWindingTemp.name; 
    ptTWChildTabWidget->clear();
    if (tfWindingTemp.terminalVec.size() > 0)
    {
        ptTWTerminalTable->clear();
        ptTWTerminalTable->setHorizontalHeaderLabels(m_TerminalTableHeader);
        ptTWTerminalTable->setRowCount(0);
        for (int i = 0; i < tfWindingTemp.terminalVec.size(); i++)
        {
            QTerminalCache terTemp = tfWindingTemp.terminalVec.at(i);
            QTableWidgetItem *terName = new QTableWidgetItem(terTemp.name);
            SetSSDValidateInfoForTableWidgetByKey(terName, currentDeviceKey + "/" + terTemp.name);
            QTableWidgetItem *terDesc = new QTableWidgetItem(terTemp.desc);
            QTableWidgetItem *terSubstationName = new QTableWidgetItem(terTemp.substationName);
            QTableWidgetItem *terVoltageLevelName = new QTableWidgetItem(terTemp.voltageLevelName);
            QTableWidgetItem *terBayName = new QTableWidgetItem(terTemp.bayName);
            QTableWidgetItem *terCnodeName = new QTableWidgetItem(terTemp.cnodeName);
            QTableWidgetItem *terConnectivityNode = new QTableWidgetItem(terTemp.connectivityNode);

            ptTWTerminalTable->insertRow(i);
            ptTWTerminalTable->setItem(i, 0, terName);
            ptTWTerminalTable->setItem(i, 1, terDesc);
            ptTWTerminalTable->setItem(i, 2, terSubstationName);
            ptTWTerminalTable->setItem(i, 3, terVoltageLevelName);
            ptTWTerminalTable->setItem(i, 4, terBayName);
            ptTWTerminalTable->setItem(i, 5, terCnodeName);
            ptTWTerminalTable->setItem(i, 6, terConnectivityNode);
        }
        ptTWTerminalTable->resizeColumnsToContents();
        ptTWChildTabWidget->addTab(ptTWTerminalTable, "Terminal");
    }
    

    if (tfWindingTemp.lnVec.size() > 0)
    {
        ptTWLNodeTable->clear();
        ptTWLNodeTable->setHorizontalHeaderLabels(m_LnodeTableHeader);
        ptTWLNodeTable->setRowCount(0);
        for (int i = 0; i < tfWindingTemp.lnVec.size(); i++)
        {
            QLNodeCache lnTemp = tfWindingTemp.lnVec.at(i);
            QTableWidgetItem *lnIedName = new QTableWidgetItem(lnTemp.iedName);
            SetSSDValidateInfoForTableWidgetByKey(lnIedName, lnTemp.key);
            QTableWidgetItem *lnLdInst = new QTableWidgetItem(lnTemp.ldInst);
            QTableWidgetItem *lnPrefix = new QTableWidgetItem(lnTemp.prefix);
            QTableWidgetItem *lnLnClass = new QTableWidgetItem(lnTemp.lnClass);
            QTableWidgetItem *lnLnInst = new QTableWidgetItem(lnTemp.lnInst);
            QTableWidgetItem *lnLnType = new QTableWidgetItem(lnTemp.lnType);

            ptTWLNodeTable->insertRow(i);
            ptTWLNodeTable->setItem(i, 0, lnIedName);
            ptTWLNodeTable->setItem(i, 1, lnLdInst);
            ptTWLNodeTable->setItem(i, 2, lnPrefix);
            ptTWLNodeTable->setItem(i, 3, lnLnClass);
            ptTWLNodeTable->setItem(i, 4, lnLnInst);
            ptTWLNodeTable->setItem(i, 5, lnLnType);
        }
        ptTWLNodeTable->resizeColumnsToContents();
        ptTWChildTabWidget->addTab(ptTWLNodeTable, "LNode");
    }    
}

void GraphicBrowserFrm::fillConductingEquipmentChild()
{
    if (m_ceVec.size() == 0)
    {
        return;
    }

    QConductingEquipmentCache ceTemp;
    if (conductingEquipmentTable->selectedItems().size() == 0)
    {
        ceTemp = m_ceVec.at(0);
    }
    else
    {
        int selectRow = conductingEquipmentTable->row(conductingEquipmentTable->selectedItems().at(0));
        if (selectRow < m_ceVec.size())
        {
            ceTemp = m_ceVec.at(selectRow);
        }
    }

    QString curentDeviceKey = m_CurrentSSDTreeKey + "/" + ceTemp.name;
    ceChildTab->clear();
    if (ceTemp.terminalVec.size() > 0)
    {
        ceTerminalTable->clear();
        ceTerminalTable->setHorizontalHeaderLabels(m_TerminalTableHeader);
        ceTerminalTable->setRowCount(0);    
        for (int i = 0; i < ceTemp.terminalVec.size(); i++)
        {
            QTerminalCache terTemp = ceTemp.terminalVec.at(i);
            QTableWidgetItem *terName = new QTableWidgetItem(terTemp.name);
            SetSSDValidateInfoForTableWidgetByKey(terName, curentDeviceKey + "/" + terTemp.name);
            QTableWidgetItem *terDesc = new QTableWidgetItem(terTemp.desc);
            QTableWidgetItem *terSubstationName = new QTableWidgetItem(terTemp.substationName);
            QTableWidgetItem *terVoltageLevelName = new QTableWidgetItem(terTemp.voltageLevelName);
            QTableWidgetItem *terBayName = new QTableWidgetItem(terTemp.bayName);
            QTableWidgetItem *terCnodeName = new QTableWidgetItem(terTemp.cnodeName);
            QTableWidgetItem *terConnectivityNode = new QTableWidgetItem(terTemp.connectivityNode);

            ceTerminalTable->insertRow(i);
            ceTerminalTable->setItem(i, 0, terName);
            ceTerminalTable->setItem(i, 1, terDesc);
            ceTerminalTable->setItem(i, 2, terSubstationName);
            ceTerminalTable->setItem(i, 3, terVoltageLevelName);
            ceTerminalTable->setItem(i, 4, terBayName);
            ceTerminalTable->setItem(i, 5, terCnodeName);
            ceTerminalTable->setItem(i, 6, terConnectivityNode);
        }
        ceTerminalTable->resizeColumnsToContents();
        ceChildTab->addTab(ceTerminalTable, "Terminal");
    }
    
    if (ceTemp.lnVec.size() > 0)
    {
        ceLNodeTable->clear();
        ceLNodeTable->setHorizontalHeaderLabels(m_LnodeTableHeader);
        ceLNodeTable->setRowCount(0);
        for (int i = 0; i < ceTemp.lnVec.size(); i++)
        {
            QLNodeCache lnTemp = ceTemp.lnVec.at(i);
            QTableWidgetItem *lnIedName = new QTableWidgetItem(lnTemp.iedName);
            SetSSDValidateInfoForTableWidgetByKey(lnIedName, lnTemp.key);
            QTableWidgetItem *lnLdInst = new QTableWidgetItem(lnTemp.ldInst);
            QTableWidgetItem *lnPrefix = new QTableWidgetItem(lnTemp.prefix);
            QTableWidgetItem *lnLnClass = new QTableWidgetItem(lnTemp.lnClass);
            QTableWidgetItem *lnLnInst = new QTableWidgetItem(lnTemp.lnInst);
            QTableWidgetItem *lnLnType = new QTableWidgetItem(lnTemp.lnType);

            ceLNodeTable->insertRow(i);
            ceLNodeTable->setItem(i, 0, lnIedName);
            ceLNodeTable->setItem(i, 1, lnLdInst);
            ceLNodeTable->setItem(i, 2, lnPrefix);
            ceLNodeTable->setItem(i, 3, lnLnClass);
            ceLNodeTable->setItem(i, 4, lnLnInst);
            ceLNodeTable->setItem(i, 5, lnLnType);
        }
        ceLNodeTable->resizeColumnsToContents();
        ceChildTab->addTab(ceLNodeTable, "LNode");
    }
    
}

//  ���������
void GraphicBrowserFrm::OnEditConfigTriggered()
{
    CheckConfigDlg->show();
}

void GraphicBrowserFrm::UpdateConfigFileMenu(QString configName)
{
    QString appPath = QApplication::applicationDirPath();
    QString configDir = appPath + "/../cfg/private";

    QDir dirPrivate(configDir);
    if (dirPrivate.exists())
    {
        QList<QAction*> groupActionList = group->actions();
        foreach(QAction* tempAct, groupActionList)
        {
            group->removeAction(tempAct);
            checkMenu->removeAction(tempAct);
        }

        dirPrivate.setFilter(QDir::Files);
        dirPrivate.setSorting(QDir::Time);
        QFileInfoList fileList = dirPrivate.entryInfoList();
        foreach(QFileInfo fileInfo, fileList)
        {
            QString absPath = fileInfo.absoluteFilePath();
            QString fileName = fileInfo.baseName();
            CheckConfigDlg->m_mapCfgFileList.insert(fileName, absPath);

            QAction *act = group->addAction(fileName);
            act->setCheckable(true);
            if (!configName.isEmpty() && fileName == configName)
            {
                act->setChecked(true);
            }
            checkMenu->addAction(act);
        }

    }
    
}


