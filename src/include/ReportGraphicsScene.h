#ifndef REPORTGRAPHICSSCENE_H
#define REPORTGRAPHICSSCENE_H
#include "reportdll_global.h"

#include <QGraphicsScene>
#include "BCRect.h"
#include <QGraphicsRectItem>

#define MOVE_VERTLINE	1
#define MOVE_HORZLINE	2
#define SELECT_MRECT	3
#define DROG_MRECT		4
#define DROG_COPY_MRECT	5
#define MIN_DELAY		250

class Report;

class REPORTDLL_EXPORT ReportGraphicsScene : public QGraphicsScene
{
	friend class Report;
	friend class ReportGraphicsView;

	Q_OBJECT

public:
	ReportGraphicsScene(const QRectF &Rect,QObject *parent);
	~ReportGraphicsScene();

	void		SetModifyFlag(bool bmodified = true);
	bool		GetModifyFlag();
	void ShowCurrentPageItems();
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);
	//从安措信息结构中填充票具体内容
	int		FillCellDetails(QStringList& m_sFileContent);

protected:
	//添加单元格
	void	AddBCRect(BCRect *p);

	//分割单元，新建表格通用
	void	SplitterClient(int nRowNum,int nColNum) ;

	//删除表格
	void	DeleteTable(bool bPrompt = true);

	//释放选中态
	void	ReleaseSelect();

	//开始选择单元格
	void	BeginSelectBCRect(QPointF &point);

	//查找当前单元格状态
	bool	SearchLMDR(SERCH_MRECT_INFOR * pSearchInfor) ;


	//继续选择单元格，与BeginSelectBCRect配合使用
	void	ContinueSelectBCRect(QPointF &point);

	//持续选择单元格
	void	ContinueSelectBCRect(BCRect *startp,BCRect *endp);

	//选择整列单元格
	void	SelectColBCRect(QPointF &point);

	//选择整行单元格
	void	SelectLineBCRect(QPointF &point);

	//选择整列单元格
	void	SelectColLineBCRect(QPointF &point);

	void	GetSelectedBCRects(  QList<BCRect*> & list ) ;
	
	QRect	GetBCRectsRect( const QList<BCRect*> & list ) ;

	//插入列
	void	InsertColumn();

	//插入行
	void	InsertRow();

	//删除列
	void	DeleteColumn();

	//删除行
	void	DeleteRow();

	//插入子列
	void	InsertSubColumn();

	//插入子行
	void	InsertSubRow();

	//删除子列
	void	DeleteSubColumn();

	//删除子行
	void	DeleteSubRow();

	//增加新页
	void	AddPage(bool bPrompt = true);

	//插入新的附图页
	void	AddPageOnlyAdditionalGraph();

	//删除最后一页
	void	RemovePage();

	//合并单元格
	void	DoUnionCells();

	//拆分单元格
	void	DoSplitCells();

	void	FontSizeChange(int size);

	void	FontNameChange(QString &name);
	//文字左对齐
	void	FontAlignLeft();

	//文字水平居中
	void	FontAlignCenter();

	//文字右对齐
	void	FontAlignRight();

	//文字居顶
	void	FontAlignUp();

	//文字垂直居中
	void	FontAlignVCenter();

	//文字居底
	void	FontAlignDown();

	void	FontBold();
	void	FontItalic();
	void	FontUnderline();
	//设置边框显示格式，根据工具栏下拉菜单/列表框进行
	void	BorderShowChange(const int &index);
	void	BorderSytleChange(const int &nID);
	void	BorderWidthChange(const int &nID);

	void	CellBorderColorChanged(const QColor &clr);

	void	CellBackColorChanged(const QColor &clr);

	void	TextColorChanged(const QColor &clr);

	//设置边框式样，根据对话框窗口进行
	void	CellFrameSetProc();

	//选中单元格变窄
	void	CellSizingLeft();

	//选中单元格变矮
	void	CellSizingTop();

	//选中单元格变宽
	void	CellSizingRight();

	//选中单元格变高
	void	CellSizingBottom();

	//置前
	void	bringToFront();

	//置后
	void	sendToBack();

	//初始化纸张的显示
	void	InitPaper();

	//获得最新的单元格识别号
	uint	GetExpressID();

	//void	SetMouseMoveCursor(QPointF &point);//SCENE中不能用setCursor函数 
	//开始移动垂直线
	bool BeginMoveVertLine(QPainter *painter, const SERCH_MRECT_INFOR * pSearchedInfor );

	//开始移动水平线
	bool BeginMoveHorzLine(QPainter *painter, const SERCH_MRECT_INFOR * pSearchedInfor ) ;

	//结束移动水平线
	void EndMoveHorzLine(QPainter *painter);

	//结束移动垂直线
	void EndMoveVertLine(QPainter *painter);

	//鼠标弹起时
	void EndAction(QPainter *painter);

	//绘制移动中的虚直线
	void DrawMoveLine(QPainter *painter);

	//准备移动垂直线
	bool PrepareMoveVertLine(const SERCH_MRECT_INFOR * pSearchedInfor);

	//结束移动垂直线
	void EndMoveVertLine();

	//右方的所有格子保留宽度移动
	void EndMoveVertLine2();

	//准备移动水平线
	bool PrepareMoveHorzLine(const SERCH_MRECT_INFOR * pSearchedInfor);

	//结束移动水平线
	void EndMoveHorzLine();

	//下方的所有格子对象保留高度移动
	void EndMoveHorzLine2();

	//取得可移动的单元格
	void GetCanMoveRect( QRectF & t_RC ) ;

	//获取拖动线时鼠标的有效移动范围
	void GetValidMoveLinePoint( const  QPointF & p2 ) ; 

	//删除移动直线的缓存信息
	void DeleteMoveLineInfor() ;

	//判断点是否在整个区域
	int  PtOnTotalBound(const QPointF & pot ) ; // return POINT_LEFT  .... if not in bound it is POINT_OUT

	//
	void InsertListByOrder( BCList & list , BCRect *  p , int dir) ;

	//取得连着的单元格列表
	void GetLinkedMRect( BCList & list ) ;

	//
	bool HaveSameBeginAndEnd( const BCList &  list1, const BCList &  list2 ) ;

	//
	void GetMoveArrange1();

	//
	void GetMoveArrangeAndPointBE();

	//移动全部单元格
	void MoveAllMRect(  double move_x, double move_y ) ;

	//移动子单元格
	void MoveSubMRect( QPoint & point, double move_val, int director ) ;

	//对子区域分割单元格
	void SplitterSubMRects( const BCList & list , bool bSplitterRows ) ;

	//删除子格
	void DeleteSubMRects(  const BCList & list , bool bDeleteRows) ;

	//搜索连接的格子
	void FindTouchedMRects( BCList & list , const QRectF & rc , int dir ) ;

	void UpdateStatusBarInfo();

	void MoveTextToPrinttext();
	int			GetLineOfFileFromExpress(QString ExpressText);
	int			GetTheIDOfAutoIndex(QString ExpText,uint pageno);//2005.01.27 取得自动序号

	bool	ShowAlways();

	BCRect* GetLeftBCRect( BCRect *pMRect );
	BCRect* GetRightBCRect( BCRect *pMRect );
	BCRect* GetUpBCRect( BCRect *pMRect );
	BCRect* GetDownBCRect( BCRect *pMRect );

	void SetBeginSel( BCRect *pMRect );

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	bool	FillItemInNewPage(BCRect *pCurr,int pageno,QString &info);
public:
	//void ShowCurrentPageItems();
	//bool	save(QDataStream &stream);
	//bool	load(QDataStream &stream);
	void    gotoFirstPage();
	void	gotoLastPage();
	void	gotoPrevPage();
	void	gotoNextPage();

	void	GetCurrentPageBCRects(int &PageIndex,BCList &list);
	BCRect *GetSelectedBCRect(){return SelectBeginBCRect;}
protected://override functions
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	//void mouseDoubleClickEvent(QMouseEvent *event);

	//bool eventFilter(QObject *obj,QEvent *e);
protected:
	//QCursor			Current_Cursor;
	//QCursor			Backup_Cursor;
	//QCursor			Cursor_VertLine;
	//QCursor			Cursor_HorzLine;
	//QCursor			Cursor_Text;
	//QCursor			Cursor_Arrow;
	struct  MOVE_LINE_INFOR{
		QPoint  BPoint;
		int    iMovingTotalBound ; // POINT_LEFT  POINT_RIGHT POINT_TOP POINT_BOTTOM or POINT_RECT 
								   // POINT_RECT means not moving On totalRectSides

		int    moveLineType ;	   //  线:  VERT_LINE OR HORZ_LINE 

		BCList m_List1 ;  // WHEN move vert_line  m_list1 is being moved MRects on left and  m_list2 is right ;
		BCList m_List2 ;  // when move horz_line  m_list1 point to MRect above move_line 
								   //        and  m_list2  is under move_line
		QSize   MoveArrange;	   // 可移动范围 such as CSize( -20, 30 )  when move vert_line  是X轴的可移动的正负范围
		QPoint  MoveLineB , MoveLineE ; // 画虚线 ..
	}   moveLineInfor ;

	BCRect *RectStr[MAX_PAGE_NUM];
	BCRect *SelectBeginBCRect;
	BCRect *topleftP,*bottomrightP;

	uint			m_uiPageNo;
	uint			m_uiPageNum;
	QRect			TotalRect[MAX_PAGE_NUM]; 

	int				Action;
	int				m_iMoveLineStep;

	QGraphicsLineItem *lineMoved;
	QMainWindow			*mainWnd;
	QString			fullPath;
	//char				fullPath[256];

	QGraphicsRectItem *backgroundItem;

	bool			bModifiedFlag;
	int			iLeftPage,iTopPage,iRightPage,iBottomPage;//页边距
};

#endif // REPORTGRAPHICSSCENE_H
