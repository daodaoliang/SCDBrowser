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

    // ���������
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
	void timerProgressOut();   // ������ ����
	void timerCompareOut();    // �ȽϽ�����
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
	// �Ƚ�����������Ӧ
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
	void    UpdateTableDate(bool ForBack); // true ����ǰ�� false ����;
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
	void	CenterAllItems(QRectF &rect);//���ݵ�ǰ���е�ͼԪ���Զ���ʾ��������λ��

	void	InitDeviceView();
	void	InitVersionView();
	void	InitSCDValidatorView();
	void    InsertItem(FILE *fp, QString type);
	void	InitSubstationView();
	void    InitSubStationTable();
	void    InitSubStationTableData();
	// ����������Ϣ��
	typedef struct ConfPair 
	{
		QString		confEnDesc;
		QString		confChDesc;
	}CONFPAIR;
	CONFPAIR m_ConfPair[ITEMCOUNT];
	void	SetConfigureInfo();
	void	SetConfigureInfoPair(ConfInfo ord, QString EnDesc, QString ChDesc);
	// ����������Ϣ��
	QStandardItem *ConfigItem;
	void	CreatCompareConfTree();
	void	CreatSubstationCompareConfigView(QStandardItem *itemParent, ConfInfo itemChild);
	// ��ȡ������Ϣ
	bool    ConfigureInfo[ITEMCOUNT];
	void	GetSubstationCompareConfigInfoFromFile();
	// ����������̬�л�
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
		// ��ӵ���ICD�汾��Ϣ�˵���	2014.9.9
		*actionExportICDInfo,

		*actionPrintPreview,
		*actionExit,
		*actionShowStatusBar,
        *actionShowOutSignal, // ������ͼ�Ƿ�չʾ����ź�
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
		*substationConfigBtn,			// �Ƚ�ʱ�����ð�ť
		*substationConfigSureBtn,
		*substationConfigCancelBtn;

	QDockWidget     *IEDsTreeDockWidget;
	QDockWidget     *graphicsViewDockWidget;

	Myprogress      *progresStyle;
	Myprogress      *progresCompareStyle;
	Myprogress		*progresCheckStyle;
	bool            Endcompare;   //�����Ƚ� ǰ�� ����
	bool			isComparing;	//�Ƿ��ڱȽ�״̬;
	bool            porgressFlg;    //��¼�Ƿ���ڵ����߳�
	bool            proCompareFlg;  //�Ƚϱ�־λ
	bool			proCheckFlg;	// ����־λ
	QTimer          *timerProgress; // ����������
	QTimer          *timerCompare;  //�Ƚ����������
	QTimer			*timerCheckProgress; //�������
	int             proGressCount;  // ������������
	int             averageSpeed;   
	qComItemDelegate *itemdele;  //�Ƚϴ���
	QList<QString> ListBaseName;

public:
	QList<QString> fileNameList; // ���ļ�����洢

	QString			   exePath;	// ���ڼ�����Demo·��
private:
	int currentImportFile;

	int currentValidatorItem;

	QWidget			*treeContainerWidget;
	QWidget			*DiffTreeContainerWidget;
	QWidget			*infoTreeContainerWidget;
	QWidget			*subnetTreeContainerWidget;
	QWidget			*scdValidatorWidget;	//��������
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
	QTableWidget       *tablesubstation; //�Ƚ��б�
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

	bool PreFlag; //ǰ�� ���� ���Ϊ;
	bool NextFlag;
public:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void initTab();
public:
	QValidateDialog qValidateDialog;

    // SSD չʾ----------------------------------------------------------------------
public:
    // ��IED Tab�����һ���豸չʾ
    QTableWidget     *widgetIEDSSDTable;
    QStringList      m_IEDSSDTableHeader;

    // ����ͼ
    QPushButton     *ExportSSDValidateExcelButton; // ���������
    QTreeWidget		*SSDTreeWidget;
    QVBoxLayout     *ssdTreeVLayout;
    QWidget         *ssdTreeContainerWidget;

    // ������Ϣ
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

    QTableWidget    *basicValidateTableWidget; // ������Ϣ

    // ��ѹ�ȼ���Ϣ
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
    QTableWidget    *vlValidateTableWidget; // ������Ϣ

    // �����Ϣ
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
    QTabWidget      *bayDeviceTabWidget; // ����豸


    QString m_CurrentSSDTreeKey; // ��¼SSD����Key
    QVector<QPowerTransformerCache> m_ptVec; // ����ڱ�ѹ���豸
    QConductingEquipmentCacheVec m_ceVec; // ����ڵ����豸
    QConnectivityNodeCacheVec m_cnVec;  // ����ڽڵ�

    // ��ѹ���ؼ�
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
    // PT��Ԫ��
    QTabWidget      *ptChildTabWidget; 
    QTableWidget    *ptTWTableWidget; // TW ����
    QTableWidget    *ptlnodeTableWidget; // LNode����
    QTabWidget      *ptTWChildTabWidget; 
    QTableWidget    *ptTWTerminalTable; // TW��Ԫ��
    QTableWidget    *ptTWLNodeTable; // TW��Ԫ��

    // �����豸�ؼ�
    QWidget         *conductingEquipmentWidget;
    QVBoxLayout     *conductingEquipmentLayout;
    QTableWidget    *conductingEquipmentTable; // �����豸 ����
    QTabWidget      *ceChildTab;
    QTableWidget    *ceTerminalTable;// Terminal ����
    QTableWidget    *ceLNodeTable; // LNode ����

    // ���ӵ�
    QWidget         *connectivityNodeWidget;
    QVBoxLayout     *connectivityNodeLayout;
    QTableWidget    *connectivityNodeTable; // ConnectivityNode����

    // ��ͷ
    QStringList m_DeviceTabletHeader; // ��ѹ���������豸������
    QStringList m_TerminalTableHeader;
    QStringList m_LnodeTableHeader;
    QStringList m_ConnectivityNodeTableHeader;
    QStringList m_SSDValidateTableHeader;

    CSSDLoader *m_SSDInfo; // �洢SSD��Ϣ

    void fillSSDBayContainer(QString voltageLevel, QString bay); // ������Ϣ �л�SSD��ͼ
    QString GetSCDIEDKeyBySSDLnIED(QString iedName); // ����SSD LNode�е�iedName�����Ҷ����豸���Ƿ��ж�Ӧ��IED

    void InitSSDUiWidget(); // ��ʼ������ؼ�
    void InitSSDTreeWidget(); // ��ʼ�������
    enum Flags
    {
        E, 
        W, 
        R,
        OK
    };

    FixedProcessBar *m_SwitchProcessBar; // �л�������

    Flags GetSSDValidateInfoByKey(QString key, QString &info);
    void SetSSDValidateInfoForTableWidgetByKey(QTreeWidgetItem* item, QString key);
    void SetSSDValidateInfoForTableWidgetByKey(QTableWidgetItem* item, QString key);
public slots:
    void SelectSSDTreeProc(QTreeWidgetItem* item); // ����������Ķ��� 
    void fillPowerTransformTWChild(); // �л������豸�� ����������ӽڵ�
    void fillConductingEquipmentChild(); // �л������豸ʱ  �����ӽڵ�
    void ExportSSDValidateExcel();
    void ImportValidateFinished();

    //---------------------SCD���------------------------------------------------------------------
public:

    CCheckConfigDlg *CheckConfigDlg;
    QString currentValidatorItemInfo; // ���������ͼѡ�������
    
public slots:
    void OnEditConfigTriggered();
    void UpdateConfigFileMenu(QString configName);

private:
    QActionGroup *group;  //�����ѡ��
	QMenu     *checkMenu;
    QAction   *actionEditConfig; // ���������
};

#endif