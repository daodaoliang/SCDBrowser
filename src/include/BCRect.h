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
#define NOT_SHOW				1  // ������ʹ��

#define MRECT_TEXT				2
#define MRECT_EXPRESS			4
#define MRECT_STAEXPRESS		8
#define MRECT_LINE				16
#define	MRECT_PAINTBMP			32
#define MRECT_BITMAP			64
#define MRECT_TIME				128
#define MRECT_NUMBER			256
#define MRECT_STAEXPRESS_ENUM	512		//for value to string convert

#define MRECT_NULLSTAEXPRESS  ( NOT_SHOW|MRECT_STAEXPRESS  ) // from  version 2.0  it is ����ʾ��׼���ʽ��2011.07.13 modi

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
	//��Ԫ�����Χ4�����������ֵ
	int		dLeft,dRight,dTop,dBottom;

	//��Ԫ����Χ4������Ļ���
	QPen		*leftPen,*rightPen,*topPen,*bottomPen;

	//��Ԫ��4���������ɫ
	QColor		leftColor,rightColor,topColor,bottomColor;

	//�ĸ������ߵ�����,�߿�
	int			iLeftWidth,iTopWidth,iRightWidth,iBottomWidth;

	//�ĸ������ߵ�����
	Qt::PenStyle	iLeftStyle,iTopStyle,iRightStyle,iBottomStyle;

	//��������
	QFont		*textFont;
	QFont		printPreviewFont;//��ӡԤ��������
	QString		textFontName;
	int			nTextFontSize,textBoldFlag;
	bool		textItalicFlag,textUnderlineFlag;

	//����ɫ���ı�ɫ
	QColor		backColor,textColor;

	//�õ�Ԫ�����ֵĶ��뷽ʽ
	qint8		Lay;

	//ǰ��ָ��
	BCRect	*Next,*Prev;

	//��ӡʱ��ʾ����
	QString		strPrintText;

	//�Ƿ�ѡ��õ�Ԫ��
	bool		bHaveSelected;

	//ͼƬ����ר��
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
	//��д��ͼ����
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


	//��ʼ��
	void		InitBCRect();

	//��д
	QRectF		boundingRect() const;


	//���Ƹõ�Ԫ��
	void	DrawClient(QPainter *painter,QRectF &rect,bool ADraw = false);

	//�Ƿ���Ҫ��ʾ
	bool	CanShow(QRectF &rect);

	//����δѡ���״̬ 
	void	DrawNormalLine(QPainter *painter);

	//����ѡ�иõ�Ԫ���״̬
	void	DrawSelectedLine(QPainter *painter,bool ADraw);

	//�����ı�
	void	DrawText(QPainter *painter,bool ADraw);

	//�ж��Ƿ���õ�Ԫ������
	bool	IsLinkWith( BCRect * p , int type );  

	void	FillTextFontParam();
};

//������¼������ʱ�����ĵ�Ԫ��
struct  SERCH_MRECT_INFOR
{
	QPoint  searchPoint ;
	BCRect   *pSearchedMR ;
	int      ptInMrectType ;
};

typedef		QList<BCRect *>			BCList;