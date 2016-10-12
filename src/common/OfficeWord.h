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

	//����

//**********************�������ĵ�*******************************************
	BOOL CreateApp(); //����һ���µ�WORDӦ�ó���
	BOOL CreateDocumtent(); //����һ���µ�Word�ĵ�
	BOOL Create(); //�����µ�WORDӦ�ó��򲢴���һ���µ��ĵ�
	void ShowApp(); //��ʾWORD�ĵ�
//**********************�ı���д����*******************************************
	void WriteText(LPCTSTR szText); //д�ı�
	void NewLine(int nLineCount); //�س���N��
	void WriteNewLineText(LPCTSTR szText, int nLineCount = 1); //��N��д��

//**********************������*******************************************
	void CreateTable(int nRow, int nColumn);
	void WriteCellText(int nRow, int nColumne, LPCTSTR szText); //�������д��
	void WriteCellNewLineText(int nRow, int nColumne, LPCTSTR szText, int nLineCount = 1); //���N��д��
	void New2StringArray(LPCTSTR** pszArr, int nRow, int nColumn); //������ά�ַ�������
	void WriteTableText(LPCTSTR** pszText, int nRow, int nColumn); //�ö�ά�ַ������������
	void SelectCell(int nRow, int nColumn); //ѡ�б��
	void MergeCells(int nRowOrColumn, int nCellStart, int nCellEnd, BOOL bIsRow = TRUE);//�ϲ���Ԫ��
	void MoveDown(int nRow);//�����ƶ�nRow�У�ָ���ڱ���У��Ƴ�
//*********************��������*******************************************
	void SetFont(float fSize, LPCTSTR szFontName = L"����", long lFontColor = 0);
	void SetFont(long lColor, long lBackColor = 0);	
	void SetFont(BOOL bBold, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE);	
//*********************��ʽ����*******************************************
	void SetParaphFormat(int nAlignment);
//����
	void SetActiveTable(int nItem); //���õ�ǰ���񣬿��Բ����ı��
	long GetTableCount(); //�õ�������

};

