#ifndef GraphicBrowserFrm_H
#define GraphicBrowserFrm_H

//#include <WinBase.h>
#include <QMainWindow>
#include <QTreeView>
#include <QLayout>
#include "QSettings"
#include "MyHmiGraphicsScene.h"
#include "DrawIEDSelf.h"
#include "HmiGraphicsView.h"
#include "GraphicManger.h"
#include "GraphicLoader.h"
#include "SCDManger.h"
#include "SCDComparer.h"
#include "SCDDifferCompare.h"
#include "MyLoadingThread.h"
#include "LoadDialog.h"
#include "TableModelInterface.h"

#include "myprogress.h"
#include "TableSCLModelImpl.h"
#include "TableModelDiffImpl.h"
#include "mytabwidget.h"
#include "Receivewidget.h"
#include "Sendwidget.h"
#include "IPWidget.h"
#include "MacWidget.h"
#include "comparethread.h"
#include "qcomitemdelegate.h"
#include "VirLoopDialog.h"
#include "checkthread.h"
#include "qvalidatedialog.h"
#include "qxmlviewer.h"
#include <QLabel>
#include "listenthread.h"
#include "checkconfigdlg.h"
#include "SSDLoader.h"

#include "logsrv.h"
typedef int handle;
extern handle logsrv;

class QSplitter;
class QTreeWidget;
class QTableWidget;
class QTreeWidgetItem;
class QGraphicsView;
class QLabel;
class QTableView;
class QDockWidget;
class QTimer;
class QTimerEvent;

class GraphicBrowserFrm : public QMainWindow
{
	Q_OBJECT
public:
	GraphicBrowserFrm(QWidget *parent);
	~GraphicBrowserFrm();

    // 多进程启动
    ListenThread *m_ListenThread;
    HWND hwnd;

	void StartFromCMD();
	void StartMoreFromCMD();
	void StratCompareFromCMDAfterLoad();
	bool m_IsAutoCompare;
	QStringList m_AutoCompareFiles;

public:
	QString TrivalInfo;
	QString argc1;

protected:
	void closeEvent(QCloseEvent* event);
	void ShowEvent();
	void HideEvent();
	void timerEvent(QTimerEvent* event);
private:
	QStackedWidget *stackedWidget;

private slots:
    void ThreadRead(QString arg);
	void ShowDeleDialog();
	void ShowSwithDialog();
	void StartCheckSCD();
	void receiveslot(QPoint mousePos);
	void clickdobleCell(int,int);
	void enableTabFouces(bool enbale);
	void timerProgressOut();   // 进度条 导入
	void timerCompareOut();    // 比较进度条
	void timerProgressCheckOut();
	void CheckboxSelect(const QModelIndex &);
	void modelChange( QStandardItem *);
	void FinishCompareThread(bool result);
	void FinishCheckThread();
	void ExportValidateExcel();
	
	void ShowIEDsTreeProc();
	void ShowgraphicsViewProc();
	void ShowSubstationChangeProc();
	void SetSubStation(QString subStationID);
	void ShowSubstationManageProc();
	void LoadSCDProc();
	void LoadSCDProc1(int loadResult);
	void ClearDBProc();
	void CheckSCDProc();
	void ZoomInProc();
	void ZoomOutProc();
	void PreProc();
	void NextProc();
	void RoamProc();
	void RecoveryProc();
	void ShowToolbarProc();
	void ShowStatusbarProc();
    void ShowOutSignalInTerminalGraphic();
	void SubStationChangeProc();
	void SubStationCompareProc();
	void SubStationCompareEndProc();
	void SubStationCompareExportProc();
	void SubStationCompareExportWordProc();
	void SubStationDeleteProc();
	void queryIEDProc(const QString& iedStr);
	void queryDiffIEDProc(const QString& iedStr);
	void SaveAsProc();

	void ExportICDInfoProc();
	void PrintProc();
	void AboutProc();

	void SelectTreeProc(QTreeWidgetItem *item);
	void SelectCompareTreeProc(QTreeWidgetItem *item);
	void SelectSubInfoTreePorc(QTreeWidgetItem *item);
	void SelectSubNetTreeProc(QTreeWidgetItem *item);
	void SelectSCDValidatorTreeProc(QTreeWidgetItem* item);
	void SelectDeviceProc(const QModelIndex& index);
	void SelectValidatorProc(const QModelIndex& index);
	void VoltageLevelChangedProc(int index);
	void SubStationChangedProc(int index);
	void timePro();
	void tripClickedProc(QString masterKey, QString slaveKey, int type);

	void on_substationManageView_customContextMenuRequested(const QPoint &pos);
	void on_IEDsTreeDockWidget_closed();

	void ShowSubStationCompareConfigProc();
	void SubStationCompareConfigSetProc();
	// 比较配置项点击响应
	void treeItemChanged(QStandardItem * item);
	//__________________________________________
	void totalTabChangeProc(const int tabIndex);
	void rightTabChangeProc(const int tabIndex);
	void bottmTabChangeProc(const int tabIndex);
	
signals:
	void	Close();
	void	SendItemDelegateFlag(bool,bool);

private:
	void    setCenterLayer(int i);
	void    InitIEDTotalTab();
	void	setTotalTabsEnabled(const QString& IEDKey);
	int		getTabIndexByText(const QString& tabTxt);
	void    RootItemDisplay();
	void    UpdateTableDate(bool ForBack); // true 代表前进 false 后退;
	void    fillTabContent(MyTabWidget* myTabWidget, QString IedKey, int tabIndex);
	void    fillTabContentDiff(QString Iedkey);
	void    fillXmlData(QString Iedkey);
	void	fillPrivateHeaderXmlData(const QString& substationKey);
	void	fillSubNetXmlData(const QString& subNetKey);
	void    InitImportFile(QString fileName);
	void    initDiffWidgetVer(QTreeWidgetItem *item);
	void    ReInitTabDataClear();
	void    PreNextFlagChange();
	void    ClearTableWidgetData();

	void    CreateAction(void);
	void    CreateMenus(void);
	void	CreateToolbars();
	void    CreateStatusBar();

	void	InitIEDsTreeWidget();
	void	InitSubInfoTreeWidget();
	void	InitSubnetTreeWidget();
	void    InitSCDValidatorTreeWidget();
	void	InitGraphicsView();
	void    UpdateGraphicsView();
	void	CenterAllItems(QRectF &rect);//根据当前已有的图元，自动显示到其中心位置

	void	InitDeviceView();
	void	InitVersionView();
	void	InitSCDValidatorView();
	void    InsertItem(FILE *fp, QString type);
	void	InitSubstationView();
	void    InitSubStationTable();
	void    InitSubStationTableData();
	// 设置配置信息对
	typedef struct ConfPair 
	{
		QString		confEnDesc;
		QString		confChDesc;
	}CONFPAIR;
	CONFPAIR m_ConfPair[ITEMCOUNT];
	void	SetConfigureInfo();
	void	SetConfigureInfoPair(ConfInfo ord, QString EnDesc, QString ChDesc);
	// 创建配置信息树
	QStandardItem *ConfigItem;
	void	CreatCompareConfTree();
	void	CreatSubstationCompareConfigView(QStandardItem *itemParent, ConfInfo itemChild);
	// 读取配置信息
	bool    ConfigureInfo[ITEMCOUNT];
	void	GetSubstationCompareConfigInfoFromFile();
	// 配置树的三态切换
	void	InitSubstationCompareConfigView();
	void	treeItem_checkAllChild(QStandardItem * item, bool check);
	void	treeItem_checkAllChild_recursion(QStandardItem * item,bool check);
	void	treeItem_CheckChildChanged(QStandardItem * item);
	Qt::CheckState checkSibling(QStandardItem * item);

	void	InitSubstationManageView();
	void	InitCompareWidget();

	void	ReadSettings();
	void	WriteSettings();

	void    InitVoltageLevelCombBox();
	void    InitSubstationCombBox();

	void    InitProgressDlg();
	void	removeDataFiles();
	void	graphicOpersEnable(bool enable);
	QString getTitleStr();

	void	addChildItem(QTreeWidgetItem *vtItem);
	void    addAssortmentItem(QTreeWidgetItem *vtItem);
	QTreeWidgetItem* addChildItemByType(QTreeWidgetItem *vtItem, IEDType iedType, QString iedName);
	QTreeWidgetItem* getAddItem(QTreeWidgetItem*parItem,QString IedName);
	void	hideEmptyChildItem(QTreeWidgetItem *vtItem);
	void    ChangeItemText(QTreeWidgetItem *vtItem);

	void	closeUnlockedVirDlg();
	void	closeAllVirDlgs();

    bool isDeviceOpened();
    QString getDockTitleName();

private:
	bool            checkFlag;
	bool			fileInfoSlectFlag;
	QToolBar		*mainToolbar;
	QMenu			*graphicPopupMenu;
	QMenu			*fileMenu,*operatorMenu,*visMenu,*helpMenu;
	CompareThread   *comparethread;
	CheckThread  	*checkThread;
	CSCLValid		*m_SCLValid;
	QAction			*actionShowIEDsTree,
		*actionCheckSCD,
		*actionLoadSCD,
		*actionClearDB,
		*actionManageSubstation,
		*actionShowgraphicsView,
		*actionSubStationChange,
		*actionCompareEnd,
		*actionExportFile,  //excel
		*actionExportWordFile, //word

		*actionPre,
		*actionNext,
		*actionZoomIn,
		*actionZoomOut,
		*actionRoam,
		*actionRecovery,
		*actionVoltageChange,
		*actionSubStation,

		*actionAbout,
		*actionSaveAs,
		// 添加导出ICD版本信息菜单项	2014.9.9
		*actionExportICDInfo,

		*actionPrintPreview,
		*actionExit,
		*actionShowStatusBar,
        *actionShowOutSignal, // 端子排图是否展示输出信号
		*actionShowMenu,
		*actionShowToolBar,
		*actionImportSCD,  // sl 
		*actionDelete,
		*actionCheck,
		*actionSwitch,
		*actionCompare;

	QPushButton     *surebutton,
		*cancelButton,
		*substationLoadBtn,
		*substationDeleteBtn,
		*substationChangeBtn,
		*substationCancelBtn,
		*substationCompareBtn,
		*substationManageCancelBtn,
		*substationConfigBtn,			// 比较时的配置按钮
		*substationConfigSureBtn,
		*substationConfigCancelBtn;

	QDockWidget     *IEDsTreeDockWidget;
	QDockWidget     *graphicsViewDockWidget;

	Myprogress      *progresStyle;
	Myprogress      *progresCompareStyle;
	Myprogress		*progresCheckStyle;
	bool            Endcompare;   //结束比较 前进 后退
	bool			isComparing;	//是否处于比较状态;
	bool            porgressFlg;    //记录是否存在导入线程
	bool            proCompareFlg;  //比较标志位
	bool			proCheckFlg;	// 检测标志位
	QTimer          *timerProgress; // 进度条闹钟
	QTimer          *timerCompare;  //比较闹钟溢出器
	QTimer			*timerCheckProgress; //检测闹钟
	int             proGressCount;  // 进度条计数器
	int             averageSpeed;   
	qComItemDelegate *itemdele;  //比较代理
	QList<QString> ListBaseName;

public:
	QList<QString> fileNameList; // 多文件导入存储

	QString			   exePath;	// 用于检测查中Demo路径
private:
	int currentImportFile;

	int currentValidatorItem;

	QWidget			*treeContainerWidget;
	QWidget			*DiffTreeContainerWidget;
	QWidget			*infoTreeContainerWidget;
	QWidget			*subnetTreeContainerWidget;
	QWidget			*scdValidatorWidget;	//检测项界面
	QVBoxLayout		*treeVLayout;
	QVBoxLayout		*DiffTreeVLayout;
	QVBoxLayout		*infoTreeVLayout;
	QVBoxLayout		*subnetTreeVLayout;
	QVBoxLayout		*scdValidatorTreeVLayout;
	QLabel          *Query;
	QLabel          *DiffQuery;
	QHBoxLayout     *QueryHLayout;
	QLineEdit		*IEDQueryBar;
	QHBoxLayout     *DiffQueryHLayout;
	QLineEdit		*DiffIEDQueryBar;
	QLabel          *labelVotageLevel;
	QLabel          *labelSubstation;
	QComboBox       *comboVoltageLevel;
	QComboBox       *comboSubStation;
	QTableWidget     *widgetIEDTable;
	QTableWidget     *widgetDiffEditTable;
	QTableWidget     *widgetDiffDelTable;
	QTableWidget     *widgetDIffAddTable;
	QTableWidget     *widgetDiffEqlTable;
	QTableWidget     *widgetDiffVerTable;
	QTabWidget		*treeTabWidget;
	QTreeWidget		*IEDsTreeWidget;
	QTreeWidget		*subInfoTreeWidget;
	QTreeWidget		*subnetTreeWidget;
	QPushButton     *ExportValidateExcelButton;
	QTreeWidget		*scdValidatorTreeWidget;
	QTreeWidget		*compareModel;
	MyHmiGraphicsScene *graphicScene;
	HmiGraphicsView	   *graphicsView;
	QStandardItemModel *versionModel;
	QStandardItemModel *deviceModel;
	QStandardItemModel *substationModel;
	QStandardItemModel *substationModelCompare;
	QStandardItemModel *scdValidatorModel;;

	QStandardItemModel *substationCompareConfigModel;
	QStandardItemModel *substationManageModel;
	QTableWidget       *tablesubstation; //比较列表
	QTreeView          *deviceView;
	QTreeView          *versionView;
	QTreeView          *scdValidatorView;
	QTreeView		   *substationView;
	QTreeView		   *substationCompareConfigView;
	QTreeView		   *substationManageView;
	GraphicsManger     *graphicsManger;
	IEDLoader          *IEDsLoader;
	SCDManger          *scdManger;
	SCDComparer		   *scdComparer;
	SCDDiffCompare	   *scdDiffCompare;
	QSettings		   *subSetting;
	TableModelInterface *tableModel;
	CTableSCLModelImpl *tableModelImpl;
	TableModelDiffImpl *tableModelDiffImpl;

	QDialog			   *versionWin;
	QVBoxLayout		   *hLayout;
	QDialog			   *substationWin;
	QVBoxLayout		   *substationVLayout;
	QHBoxLayout		   *substationHLayout;

	QDialog			   *scdValidatorWin;
	QVBoxLayout		   *hScdValidatorLayout;
	QPushButton        *button1;

	QDialog			   *SubstationCompareConfigWin;
	QVBoxLayout		   *SubstationCompareConfigVLayout;
	QHBoxLayout		   *SubstationCompareConfigHLayout;

	QDialog			   *substationManageWin;
	QVBoxLayout		   *substationManageVLayout;
	QHBoxLayout		   *substationManageHLayout;

	int                progressTimerID;
	QProgressBar       *analysisProgressBar;
	QProgressDialog    *progressDlg; 
	QTimer             *timer;
	CMyLoadingThread   *loadingThread;
	CLoadDialog		   *lDialog;
	QString			   filePath;
	QList<QStandardItem*> checkBoxItemList;

	CVirLoopDialog *vDialog;
	QVector<CVirLoopDialog*> lockedDialogVec;

	//_____________________________
	QSplitter *spmain;
	MyTabWidget *totalTab;
	MyTabWidget *rightTab;
	MyTabWidget *bottmTab;
	QString cunrrentindex;

	GooseSendWidget *gooseSendWidget;
	GooseReceiveWidget *gooseRecvWidget;

	QXmlViewer *xmlViewer;

	SvSendWidget    *svSendWidget;
	SvReceiveWidget *svRecvWidget;

	ReportSendWidget *reportSendWidget;
	DsSettingWidget *dsSettingWidget;

	CIPWidget *ipWidget;
	CMacWidget *macWidget;

	QMap <QString,QWidget*> Vbox_1Hash;
	QMap <QString,QWidget*> Vbox_1HashRemove;

	QMap <QString,QWidget*> Vbox_2Hash;
	QMap <QString,QWidget*> Vbox_2HashRemove;

	QPoint mMousePoint;
	QStringList stringlist;
	QList <MyTabWidget*> tabVbox_1List;

	bool PreFlag; //前进 后退 标记为;
	bool NextFlag;
public:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void initTab();
public:
	QValidateDialog qValidateDialog;

    // SSD 展示----------------------------------------------------------------------
public:
    // 在IED Tab中添加一次设备展示
    QTableWidget     *widgetIEDSSDTable;
    QStringList      m_IEDSSDTableHeader;

    // 树视图
    QPushButton     *ExportSSDValidateExcelButton; // 导出检测结果
    QTreeWidget		*SSDTreeWidget;
    QVBoxLayout     *ssdTreeVLayout;
    QWidget         *ssdTreeContainerWidget;

    // 基本信息
    QWidget         *basicContainer;
    QVBoxLayout     *basicMainLayout;

    QGroupBox       *basicInfoGroupBox;
    QHBoxLayout     *basicInfoLayout;
    QLabel *basicNameLabel;
    QLabel *basicDescLabel;
    QLabel *basicDTypeLabel;
    QLabel *basicAreaNameLabel;
    QLabel *basicAreaDescLabel;
    QLabel *basicName;
    QLabel *basicDesc;
    QLabel *basicDType;
    QLabel *basicAreaName;
    QLabel *basicAreaDesc;

    QTableWidget    *basicValidateTableWidget; // 错误信息

    // 电压等级信息
    QWidget         *vlContainer;
    QVBoxLayout     *vlMainLayout;
    QGroupBox       *vlInfoGroupBox;
    QHBoxLayout     *vlInfoLayout;
    QLabel *vlNameLabel;
    QLabel *vlDescLabel;
    QLabel *vlVoltageLabel;
    QLabel *vlMultiplierLabel;
    QLabel *vlUnitLabel;

    QLabel *vlName;
    QLabel *vlDesc;
    QLabel *vlVoltage;
    QLabel *vlMultiplier;
    QLabel *vlUnit;
    QTableWidget    *vlValidateTableWidget; // 错误信息

    // 间隔信息
    QWidget         *bayContainer;
    QVBoxLayout     *bayMainLayout;
    QGroupBox       *bayInfoGroupBox;
    QHBoxLayout     *bayInfoLayout;
    QLabel *bayTypeLabel;
    QLabel *bayDescLabel;
    QLabel *bayNameLabel;
    QLabel          *bayName;
    QLabel          *bayDesc;
    QLabel          *bayType;
    QTabWidget      *bayDeviceTabWidget; // 间隔设备


    QString m_CurrentSSDTreeKey; // 记录SSD树的Key
    QVector<QPowerTransformerCache> m_ptVec; // 间隔内变压器设备
    QConductingEquipmentCacheVec m_ceVec; // 间隔内导电设备
    QConnectivityNodeCacheVec m_cnVec;  // 间隔内节点

    // 变压器控件
    QWidget         *powerTransformWidget;
    QVBoxLayout     *powerTransformLayout;
    QGroupBox       *powerTransformInfoGroupBox;
    QHBoxLayout     *ptInfoLayout;
    QLabel *ptNameLabel;
    QLabel *ptDescLabel;
    QLabel *ptVirtualLabel;
    QLabel *ptTypeLabel;
    QLabel *ptName;
    QLabel *ptNameIcon;
    QLabel *ptDesc;
    QLabel *ptVirtual;
    QLabel *ptType;
    // PT子元素
    QTabWidget      *ptChildTabWidget; 
    QTableWidget    *ptTWTableWidget; // TW 数据
    QTableWidget    *ptlnodeTableWidget; // LNode数据
    QTabWidget      *ptTWChildTabWidget; 
    QTableWidget    *ptTWTerminalTable; // TW子元素
    QTableWidget    *ptTWLNodeTable; // TW子元素

    // 导电设备控件
    QWidget         *conductingEquipmentWidget;
    QVBoxLayout     *conductingEquipmentLayout;
    QTableWidget    *conductingEquipmentTable; // 导电设备 数据
    QTabWidget      *ceChildTab;
    QTableWidget    *ceTerminalTable;// Terminal 数据
    QTableWidget    *ceLNodeTable; // LNode 数据

    // 连接点
    QWidget         *connectivityNodeWidget;
    QVBoxLayout     *connectivityNodeLayout;
    QTableWidget    *connectivityNodeTable; // ConnectivityNode数据

    // 表头
    QStringList m_DeviceTabletHeader; // 变压器、导电设备、绕组
    QStringList m_TerminalTableHeader;
    QStringList m_LnodeTableHeader;
    QStringList m_ConnectivityNodeTableHeader;
    QStringList m_SSDValidateTableHeader;

    CSSDLoader *m_SSDInfo; // 存储SSD信息

    void fillSSDBayContainer(QString voltageLevel, QString bay); // 根据信息 切换SSD视图
    QString GetSCDIEDKeyBySSDLnIED(QString iedName); // 根据SSD LNode中的iedName，查找二次设备中是否有对应的IED

    void InitSSDUiWidget(); // 初始化界面控件
    void InitSSDTreeWidget(); // 初始化间隔树
    enum Flags
    {
        E, 
        W, 
        R,
        OK
    };

    FixedProcessBar *m_SwitchProcessBar; // 切换进度条

    Flags GetSSDValidateInfoByKey(QString key, QString &info);
    void SetSSDValidateInfoForTableWidgetByKey(QTreeWidgetItem* item, QString key);
    void SetSSDValidateInfoForTableWidgetByKey(QTableWidgetItem* item, QString key);
public slots:
    void SelectSSDTreeProc(QTreeWidgetItem* item); // 点击间隔树后的动作 
    void fillPowerTransformTWChild(); // 切换绕组设备后 更新绕组的子节点
    void fillConductingEquipmentChild(); // 切换电子设备时  更新子节点
    void ExportSSDValidateExcel();
    void ImportValidateFinished();

    //---------------------SCD检测------------------------------------------------------------------
public:

    CCheckConfigDlg *CheckConfigDlg;
    QString currentValidatorItemInfo; // 检测项树视图选择的内容
    
public slots:
    void OnEditConfigTriggered();
    void UpdateConfigFileMenu(QString configName);

private:
    QActionGroup *group;  //检测项选择
	QMenu     *checkMenu;
    QAction   *actionEditConfig; // 检测项配置
};

#endif