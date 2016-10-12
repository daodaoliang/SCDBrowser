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

#define SHEET_SUBSTATION            "变电站对象审查表"
#define SHEET_VOLTAGELEVEL          "电压等级对象审查表"
#define SHEET_BAY                   "间隔对象审查表"
#define SHEET_CE                    "导电设备对象审查表"
#define SHEET_LNODE                 "LN对象审查表"
#define SHEET_RESULT                "检测结果"


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
	void SetFilePath(QString filePath, QString exePath, QString configPath); // 设置检测条件
    bool ExportExcel(QString exportPath); // 导出检测结果

    bool ExportSSDExcel(QString exportPath, QString subKey); // 导出SSD检测结果

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
    CheckResultsVec              m_CheckResultVec; //存储所有的检测结果
    QMap<QString, QVector<int>>  m_MsgMap;         // 对信息进行分类

    QMap<QString, QVector<int>>  m_SSDResultMap; // 存储SSD结果信息

    int                          m_ErrorCount;     // 统计错误数量
    int                          m_WarnCount;      // 统计警告数量 
    int                          m_InfoCount;      // 统计提示数量

private:
	QString sFilePath;    // SCD文件
	QString sExePath;     // 检测应用
    QString sConfigPath;  // 配置文件

    QMap<QString, QString> mapMsgE2C;
	QString                      m_ScdValidInfo; // 未解析检测结果
    void ParseSCDValidInfo(); // 对检测结果进行解析
};
#endif 

