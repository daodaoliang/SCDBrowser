#pragma once
#include <QtGui>
#include "reportdll_global.h"

#define POINT_OUT   0
#define POINT_LEFT	1
#define POINT_RIGHT	2
#define POINT_TOP	3
#define POINT_BOTTOM 4
#define POINT_RECT	5

#define VERT_DIRECTOR   0x001 
#define HORZ_DIRECTOR   0x002

#define MAXBYTE		0xFF
//Lay
#define LAY_LEFT	1
#define LAY_RIGHT	2
#define LAY_CENTER	4
#define LAY_UP		8
#define LAY_DOWN	16
#define LAY_VERT_CENTER	32
#define HAVE_RESULT	64

//RectID
#define NOT_SHOW				1  // 不单独使用

#define MRECT_TEXT				2
#define MRECT_EXPRESS			4
#define MRECT_STAEXPRESS		8
#define MRECT_LINE				16
#define	MRECT_PAINTBMP			32
#define MRECT_BITMAP			64
#define MRECT_TIME				128
#define MRECT_NUMBER			256
#define MRECT_STAEXPRESS_ENUM	512		//for value to string convert

#define MRECT_NULLSTAEXPRESS  ( NOT_SHOW|MRECT_STAEXPRESS  ) // from  version 2.0  it is 不显示标准表达式，2011.07.13 modi

#define			MAX_PAGE_NUM		100

extern  long     LINE_ADJ 	       ;//1 * iPixel_Logical
extern  long     MIN_MRECT_SIZE   ;//5  * iPixel_Logical

struct MRect_Base_Info
{
	QString			strUri;
	QString			strText;
	uint			RectID;
	uint			ExpressID;
	uint			PageNo;
	bool			bShowAlways;
};


class REPORTDLL_EXPORT BCRect :
	public MRect_Base_Info , public QGraphicsItem
{
	friend class ReportGraphicsScene;
public:
	BCRect(void);
	BCRect(const BCRect * pSrcMRect, int & iMaxID ); 
	~BCRect(void);
	BCRect *Clone();

	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);
	int		GetFontSize() {return nTextFontSize;}
	QString	GetFontName() {return textFontName;}
	int		GetBoldFlag() {return textBoldFlag;}
	bool	GetItalicFlag() {return textItalicFlag;}
	bool	GetUnderLineFlag() {return textUnderlineFlag;}
	qint8	GetAlignment() {return Lay;}
protected:
	//单元格的外围4个方向的坐标值
	int		dLeft,dRight,dTop,dBottom;

	//单元格外围4个方向的画笔
	QPen		*leftPen,*rightPen,*topPen,*bottomPen;

	//单元格4个方向的颜色
	QColor		leftColor,rightColor,topColor,bottomColor;

	//四个方向线的属性,线宽
	int			iLeftWidth,iTopWidth,iRightWidth,iBottomWidth;

	//四个方向线的线型
	Qt::PenStyle	iLeftStyle,iTopStyle,iRightStyle,iBottomStyle;

	//文字字体
	QFont		*textFont;
	QFont		printPreviewFont;//打印预览的字体
	QString		textFontName;
	int			nTextFontSize,textBoldFlag;
	bool		textItalicFlag,textUnderlineFlag;

	//背景色和文本色
	QColor		backColor,textColor;

	//该单元格文字的对齐方式
	qint8		Lay;

	//前后指针
	BCRect	*Next,*Prev;

	//打印时显示内容
	QString		strPrintText;

	//是否选择该单元格
	bool		bHaveSelected;

	//图片对象专用
	QImage image;
	QString			strImagePath;
	QSize			ImageSize;
	//QPixmap pixmap;

	bool		bPrintPreviewFlag;
	QPixmap		pixMap;
public:
	void		SetPrintPreview(bool flag);
	void		SetImagePath(QString str);
	int			PointInBCRect(QPoint &point);

	void		SetPrintPreviewFont(QFont &font);
	//重写绘图函数
	void		paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget);

	QFont *		GetTextFont();
	void		SetTextFont(QFont *font);

	void		SetPixmap(QPixmap &map);
	void		SetImageSize(QSize& size);
private:
	//QVariant itemChange(GraphicsItemChange change,
	//	const QVariant &value);


	//初始化
	void		InitBCRect();

	//重写
	QRectF		boundingRect() const;


	//绘制该单元格
	void	DrawClient(QPainter *painter,QRectF &rect,bool ADraw = false);

	//是否需要显示
	bool	CanShow(QRectF &rect);

	//绘制未选择的状态 
	void	DrawNormalLine(QPainter *painter);

	//绘制选中该单元格的状态
	void	DrawSelectedLine(QPainter *painter,bool ADraw);

	//绘制文本
	void	DrawText(QPainter *painter,bool ADraw);

	//判读是否与该单元格相连
	bool	IsLinkWith( BCRect * p , int type );  

	void	FillTextFontParam();
};

//用来记录鼠标操作时遍历的单元格
struct  SERCH_MRECT_INFOR
{
	QPoint  searchPoint ;
	BCRect   *pSearchedMR ;
	int      ptInMrectType ;
};

typedef		QList<BCRect *>			BCList;