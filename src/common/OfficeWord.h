#pragma once
#include <windows.h>


#ifdef _LIBWORD_API_
#define LIBWORD_API __declspec(dllexport)
#else
#define LIBWORD_API __declspec(dllimport)
#endif


class CWordOffice;

class LIBWORD_API COfficeWord
{
private:
	CWordOffice* m_pWordImpl;

public:
	COfficeWord(void);
	~COfficeWord(void);

	//操作

//**********************创建新文档*******************************************
	BOOL CreateApp(); //创建一个新的WORD应用程序
	BOOL CreateDocumtent(); //创建一个新的Word文档
	BOOL Create(); //创建新的WORD应用程序并创建一个新的文档
	void ShowApp(); //显示WORD文档
//**********************文本书写操作*******************************************
	void WriteText(LPCTSTR szText); //写文本
	void NewLine(int nLineCount); //回车换N行
	void WriteNewLineText(LPCTSTR szText, int nLineCount = 1); //换N行写字

//**********************表格操作*******************************************
	void CreateTable(int nRow, int nColumn);
	void WriteCellText(int nRow, int nColumne, LPCTSTR szText); //往表格中写字
	void WriteCellNewLineText(int nRow, int nColumne, LPCTSTR szText, int nLineCount = 1); //表格换N行写字
	void New2StringArray(LPCTSTR** pszArr, int nRow, int nColumn); //创建二维字符串数组
	void WriteTableText(LPCTSTR** pszText, int nRow, int nColumn); //用二维字符串数组填充表格
	void SelectCell(int nRow, int nColumn); //选中表格
	void MergeCells(int nRowOrColumn, int nCellStart, int nCellEnd, BOOL bIsRow = TRUE);//合并单元格
	void MoveDown(int nRow);//向下移动nRow行，指针在表格中，移出
//*********************字体设置*******************************************
	void SetFont(float fSize, LPCTSTR szFontName = L"宋体", long lFontColor = 0);
	void SetFont(long lColor, long lBackColor = 0);	
	void SetFont(BOOL bBold, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE);	
//*********************格式设置*******************************************
	void SetParaphFormat(int nAlignment);
//方法
	void SetActiveTable(int nItem); //设置当前活动表格，可以操作的表格
	long GetTableCount(); //得到表格个数

};

