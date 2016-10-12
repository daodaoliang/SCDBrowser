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
	//�Ӱ�����Ϣ�ṹ�����Ʊ��������
	int		FillCellDetails(QStringList& m_sFileContent);

protected:
	//��ӵ�Ԫ��
	void	AddBCRect(BCRect *p);

	//�ָԪ���½����ͨ��
	void	SplitterClient(int nRowNum,int nColNum) ;

	//ɾ�����
	void	DeleteTable(bool bPrompt = true);

	//�ͷ�ѡ��̬
	void	ReleaseSelect();

	//��ʼѡ��Ԫ��
	void	BeginSelectBCRect(QPointF &point);

	//���ҵ�ǰ��Ԫ��״̬
	bool	SearchLMDR(SERCH_MRECT_INFOR * pSearchInfor) ;


	//����ѡ��Ԫ����BeginSelectBCRect���ʹ��
	void	ContinueSelectBCRect(QPointF &point);

	//����ѡ��Ԫ��
	void	ContinueSelectBCRect(BCRect *startp,BCRect *endp);

	//ѡ�����е�Ԫ��
	void	SelectColBCRect(QPointF &point);

	//ѡ�����е�Ԫ��
	void	SelectLineBCRect(QPointF &point);

	//ѡ�����е�Ԫ��
	void	SelectColLineBCRect(QPointF &point);

	void	GetSelectedBCRects(  QList<BCRect*> & list ) ;
	
	QRect	GetBCRectsRect( const QList<BCRect*> & list ) ;

	//������
	void	InsertColumn();

	//������
	void	InsertRow();

	//ɾ����
	void	DeleteColumn();

	//ɾ����
	void	DeleteRow();

	//��������
	void	InsertSubColumn();

	//��������
	void	InsertSubRow();

	//ɾ������
	void	DeleteSubColumn();

	//ɾ������
	void	DeleteSubRow();

	//������ҳ
	void	AddPage(bool bPrompt = true);

	//�����µĸ�ͼҳ
	void	AddPageOnlyAdditionalGraph();

	//ɾ�����һҳ
	void	RemovePage();

	//�ϲ���Ԫ��
	void	DoUnionCells();

	//��ֵ�Ԫ��
	void	DoSplitCells();

	void	FontSizeChange(int size);

	void	FontNameChange(QString &name);
	//���������
	void	FontAlignLeft();

	//����ˮƽ����
	void	FontAlignCenter();

	//�����Ҷ���
	void	FontAlignRight();

	//���־Ӷ�
	void	FontAlignUp();

	//���ִ�ֱ����
	void	FontAlignVCenter();

	//���־ӵ�
	void	FontAlignDown();

	void	FontBold();
	void	FontItalic();
	void	FontUnderline();
	//���ñ߿���ʾ��ʽ�����ݹ����������˵�/�б�����
	void	BorderShowChange(const int &index);
	void	BorderSytleChange(const int &nID);
	void	BorderWidthChange(const int &nID);

	void	CellBorderColorChanged(const QColor &clr);

	void	CellBackColorChanged(const QColor &clr);

	void	TextColorChanged(const QColor &clr);

	//���ñ߿�ʽ�������ݶԻ��򴰿ڽ���
	void	CellFrameSetProc();

	//ѡ�е�Ԫ���խ
	void	CellSizingLeft();

	//ѡ�е�Ԫ��䰫
	void	CellSizingTop();

	//ѡ�е�Ԫ����
	void	CellSizingRight();

	//ѡ�е�Ԫ����
	void	CellSizingBottom();

	//��ǰ
	void	bringToFront();

	//�ú�
	void	sendToBack();

	//��ʼ��ֽ�ŵ���ʾ
	void	InitPaper();

	//������µĵ�Ԫ��ʶ���
	uint	GetExpressID();

	//void	SetMouseMoveCursor(QPointF &point);//SCENE�в�����setCursor���� 
	//��ʼ�ƶ���ֱ��
	bool BeginMoveVertLine(QPainter *painter, const SERCH_MRECT_INFOR * pSearchedInfor );

	//��ʼ�ƶ�ˮƽ��
	bool BeginMoveHorzLine(QPainter *painter, const SERCH_MRECT_INFOR * pSearchedInfor ) ;

	//�����ƶ�ˮƽ��
	void EndMoveHorzLine(QPainter *painter);

	//�����ƶ���ֱ��
	void EndMoveVertLine(QPainter *painter);

	//��굯��ʱ
	void EndAction(QPainter *painter);

	//�����ƶ��е���ֱ��
	void DrawMoveLine(QPainter *painter);

	//׼���ƶ���ֱ��
	bool PrepareMoveVertLine(const SERCH_MRECT_INFOR * pSearchedInfor);

	//�����ƶ���ֱ��
	void EndMoveVertLine();

	//�ҷ������и��ӱ�������ƶ�
	void EndMoveVertLine2();

	//׼���ƶ�ˮƽ��
	bool PrepareMoveHorzLine(const SERCH_MRECT_INFOR * pSearchedInfor);

	//�����ƶ�ˮƽ��
	void EndMoveHorzLine();

	//�·������и��Ӷ������߶��ƶ�
	void EndMoveHorzLine2();

	//ȡ�ÿ��ƶ��ĵ�Ԫ��
	void GetCanMoveRect( QRectF & t_RC ) ;

	//��ȡ�϶���ʱ������Ч�ƶ���Χ
	void GetValidMoveLinePoint( const  QPointF & p2 ) ; 

	//ɾ���ƶ�ֱ�ߵĻ�����Ϣ
	void DeleteMoveLineInfor() ;

	//�жϵ��Ƿ�����������
	int  PtOnTotalBound(const QPointF & pot ) ; // return POINT_LEFT  .... if not in bound it is POINT_OUT

	//
	void InsertListByOrder( BCList & list , BCRect *  p , int dir) ;

	//ȡ�����ŵĵ�Ԫ���б�
	void GetLinkedMRect( BCList & list ) ;

	//
	bool HaveSameBeginAndEnd( const BCList &  list1, const BCList &  list2 ) ;

	//
	void GetMoveArrange1();

	//
	void GetMoveArrangeAndPointBE();

	//�ƶ�ȫ����Ԫ��
	void MoveAllMRect(  double move_x, double move_y ) ;

	//�ƶ��ӵ�Ԫ��
	void MoveSubMRect( QPoint & point, double move_val, int director ) ;

	//��������ָԪ��
	void SplitterSubMRects( const BCList & list , bool bSplitterRows ) ;

	//ɾ���Ӹ�
	void DeleteSubMRects(  const BCList & list , bool bDeleteRows) ;

	//�������ӵĸ���
	void FindTouchedMRects( BCList & list , const QRectF & rc , int dir ) ;

	void UpdateStatusBarInfo();

	void MoveTextToPrinttext();
	int			GetLineOfFileFromExpress(QString ExpressText);
	int			GetTheIDOfAutoIndex(QString ExpText,uint pageno);//2005.01.27 ȡ���Զ����

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

		int    moveLineType ;	   //  ��:  VERT_LINE OR HORZ_LINE 

		BCList m_List1 ;  // WHEN move vert_line  m_list1 is being moved MRects on left and  m_list2 is right ;
		BCList m_List2 ;  // when move horz_line  m_list1 point to MRect above move_line 
								   //        and  m_list2  is under move_line
		QSize   MoveArrange;	   // ���ƶ���Χ such as CSize( -20, 30 )  when move vert_line  ��X��Ŀ��ƶ���������Χ
		QPoint  MoveLineB , MoveLineE ; // ������ ..
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
	int			iLeftPage,iTopPage,iRightPage,iBottomPage;//ҳ�߾�
};

#endif // REPORTGRAPHICSSCENE_H
