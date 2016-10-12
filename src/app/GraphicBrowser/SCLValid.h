#ifndef SCLVALID_H
#define SCLVALID_H

#include <QString>
#include <QVector>
#include <QMap>

#include "libxl.h"
using namespace libxl;

#include <boost/shared_ptr.hpp>

typedef std::vector<void*>                  PtrVec;
typedef std::pair<std::string, PtrVec>      DataValue;
typedef std::vector<DataValue>              DatValVec;
typedef std::map<std::string, DataValue>    DataItem;
typedef boost::shared_ptr<DataItem>         DataPtr;
typedef std::vector<DataPtr>                DataList;


#define Title_Format                "Title"
#define Header_Format               "Header"
#define Content_Format              "Content"
#define Link_Format                 "Link"

#define ITEM_SHEET                  "sheet"
#define ITEM_ROW                    "row"
#define ITEM_NAME                   "name"
#define ITEM_DESC                   "desc"
#define ITEM_TYPE                   "type"
#define ITEM_RESULT                 "result"
#define ITEM_SUBSTATION             "substation"
#define ITEM_BAY                    "bay"
#define ITEM_VOLTAGELEVEL           "voltagelevel"
#define ITEM_CONDUCTINGEQUIPMENT    "conductingequipment"
#define ITEM_CONNECTIVITYNODE       "connectivitynode"
#define ITEM_TERMINAL               "terminal"
#define ITEM_LNODE                  "lnode"
#define ITEM_IED                    "ied"
#define ITEM_LN                     "ln"
#define ITEM_LDINST	                "ldInst"
#define ITEM_PREFIX	                "prefix"
#define ITEM_LNCLASS                "lnClass"
#define ITEM_LNINST                 "lnInst"
#define ITEM_ERR_LIST               "err_list"

#define SHEET_SUBSTATION            "���վ��������"
#define SHEET_VOLTAGELEVEL          "��ѹ�ȼ���������"
#define SHEET_BAY                   "�����������"
#define SHEET_CE                    "�����豸��������"
#define SHEET_LNODE                 "LN��������"
#define SHEET_RESULT                "�����"


struct CheckInfo
{
    QString position;
    QString sKey;
    QString sName;
    QString sDesc;
    QString sStandard;
    QString sFlag;
    QString sClass;
    QString sXml;
};
typedef QVector<CheckInfo> CheckResultsVec;


typedef std::map<std::string, Format*>  SsdExcelFormat;

class CSCLValid
{
private:
	CSCLValid();
	virtual ~CSCLValid();

    static CSCLValid* m_SCLValidInstance;

public:    
    static CSCLValid* GetInstance()
    {
        if (m_SCLValidInstance == NULL)
        {
            m_SCLValidInstance = new CSCLValid();
        }
        return m_SCLValidInstance;
    }

	void StartCheck();
	void SetFilePath(QString filePath, QString exePath, QString configPath); // ���ü������
    bool ExportExcel(QString exportPath); // ���������

    bool ExportSSDExcel(QString exportPath, QString subKey); // ����SSD�����

protected:
    SsdExcelFormat CreateFormats(Book* pBook);

    PtrVec AddIntoPtrVec(int c, ...);

    PtrVec AddErrCodesIntoResult(std::string name, DataList &data_result);

    std::wstring toW(std::string str);

    std::string toAddr(std::string row);

    void SetColWidth(Sheet *pSheet, const int ColumnWidth[], int cnt);

    void SetCelValue(Sheet *pSheet, DataValue &value, SsdExcelFormat &fmt, int cnt, int row, int &col, bool isFormula = false, std::string DispName = ITEM_NAME);

    void SetCelVec(Sheet *pSheet, PtrVec &vec, SsdExcelFormat &fmt, int cnt, int row, int &col, std::string DispName = ITEM_NAME, bool isFormula = true);

    void SetCelResult(Sheet *pSheet, DataItem &item, SsdExcelFormat &fmt, int cnt, int row, int &col);

    void CreateStationSheet(DataList &data_substation, SsdExcelFormat &fmt, Book* pBook);

    void CreateVoltageLevelSheet(DataList &data_vl, SsdExcelFormat &fmt, Book* pBook);

    void CreateBaySheet(DataList &data_bay, SsdExcelFormat &fmt, Book* pBook);

    void CreateCESheet(DataList &data_CE, SsdExcelFormat &fmt, Book* pBook);

    void CreateLNSheet(DataList &data_LN, SsdExcelFormat &fmt, Book* pBook);

    void CreateResultTable(DataList &data, SsdExcelFormat &fmt, Book* pBook, std::string Belong = ITEM_NAME);

    void AnalyseData(QString subKey, DataList &data_station, DataList &data_volLevel, 
        DataList &data_bay, DataList &data_ConEqu, DataList &data_ConnNode, 
        DataList &data_terminal, DataList &data_LN, DataList &data_IED, DataList &data_result);

    void LayoutData(DataList &data_station, DataList &data_volLevel, DataList &data_bay, 
        DataList &data_ConEqu, DataList &data_ConnNode, DataList &data_terminal,
        DataList &data_LN, DataList &data_IED);

public:
    CheckResultsVec              m_CheckResultVec; //�洢���еļ����
    QMap<QString, QVector<int>>  m_MsgMap;         // ����Ϣ���з���

    QMap<QString, QVector<int>>  m_SSDResultMap; // �洢SSD�����Ϣ

    int                          m_ErrorCount;     // ͳ�ƴ�������
    int                          m_WarnCount;      // ͳ�ƾ������� 
    int                          m_InfoCount;      // ͳ����ʾ����

private:
	QString sFilePath;    // SCD�ļ�
	QString sExePath;     // ���Ӧ��
    QString sConfigPath;  // �����ļ�

    QMap<QString, QString> mapMsgE2C;
	QString                      m_ScdValidInfo; // δ���������
    void ParseSCDValidInfo(); // �Լ�������н���
};
#endif 

