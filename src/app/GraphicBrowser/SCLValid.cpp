#include "SCLValid.h"
#include <QString>
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include "CMarkUp/Markup.h"
#include <tchar.h>
#include <QDebug>

#include <boost/make_shared.hpp>

#include "SSDLoader.h"

CSCLValid * CSCLValid::m_SCLValidInstance = NULL;

CSCLValid::CSCLValid()
{
    mapMsgE2C["E"] = "错误";
    mapMsgE2C["R"] = "提示";
    mapMsgE2C["W"] = "警告";

    m_ErrorCount = 0;
    m_WarnCount = 0;
    m_InfoCount = 0;
}

CSCLValid::~CSCLValid()
{
}

void CSCLValid::SetFilePath(QString filePath, QString exePath, QString configPath)
{
	sFilePath = filePath;
	sExePath = exePath;
    sConfigPath = configPath;
}


void CSCLValid::StartCheck()
{
    m_ScdValidInfo.clear();
    m_CheckResultVec.clear();
    m_MsgMap.clear();
    m_SSDResultMap.clear();
    m_ErrorCount = 0;
    m_WarnCount = 0;
    m_InfoCount = 0;

    QProcess checkProcess;
    QStringList args;
    args << sConfigPath << sFilePath << "utf8";
    qDebug() <<  "Start Checking:" << sExePath << sConfigPath << sFilePath;
    checkProcess.start(sExePath, args);

    
    if (checkProcess.waitForFinished(15*60*1000))
    {
        int status = checkProcess.exitStatus();
        if(QProcess::NormalExit == checkProcess.exitStatus())
        {
			qDebug("检测成功");
            QByteArray checkResult = checkProcess.readAllStandardOutput();
            m_ScdValidInfo = QString::fromUtf8(checkResult);
            QFileInfo scdInfo(sFilePath);
            QFile resultFile(scdInfo.absolutePath() + "/result.xml");
            if (resultFile.open(QIODevice::WriteOnly))
            {
                resultFile.write(checkResult);
                resultFile.close();
            }
            ParseSCDValidInfo();
        }
    }
}

void CSCLValid::ParseSCDValidInfo()
{
    CMarkup xml;
    xml.SetDoc(m_ScdValidInfo.toStdWString());

    if (!xml.FindElem(L"CheckResults"))
    {
        return;
    }

    while (xml.FindChildElem(L"CheckInfo"))
    {
        xml.IntoElem();
        CheckInfo ci;
        ci.sKey = QString::fromStdWString(xml.GetAttrib(L"checkItemKey"));
        ci.sName = QString::fromStdWString(xml.GetAttrib(L"checkItemName"));
        ci.sDesc = QString::fromStdWString(xml.GetAttrib(L"desc"));
        ci.position = QString::fromStdWString(xml.GetAttrib(L"position"));
        ci.sStandard = QString::fromStdWString(xml.GetAttrib(L"standard"));
        ci.sFlag = mapMsgE2C[QString::fromStdWString(xml.GetAttrib(L"flag"))];
        ci.sClass = QString::fromStdWString(xml.GetAttrib(L"checkClass"));
        ci.sXml = QString::fromStdWString(xml.GetAttrib(L"xml"));


        int iNum = m_CheckResultVec.size();
        m_CheckResultVec.push_back(ci);
        m_MsgMap[ci.sFlag + ci.sClass].push_back(iNum);

        if (xml.HasAttrib(L"checkItemPath"))
        {
            QString checkItemPath = QString::fromStdWString(xml.GetAttrib(L"checkItemPath"));
            m_SSDResultMap[checkItemPath].push_back(iNum);
        }

        if (ci.sFlag == "错误")
        {
            m_ErrorCount++;
        }
        else if (ci.sFlag == "警告")
        {
            m_WarnCount++;
        }
        else if (ci.sFlag == "提示")
        {
            m_InfoCount++;
        }

        xml.OutOfElem();
    }

}

bool CSCLValid::ExportExcel(QString exportPath)
{
    Book* pBook = xlCreateBook();
    if(pBook==NULL)
    {
        return false;
    }

    pBook->setKey(L"济南容弗科技有限公司", L"windows-26292c0705cde60766bc6667a2hce3l9");
    Sheet* pSheet = pBook->addSheet(L"检测报告");
    if(pSheet == NULL)
    {
        QMessageBox::warning(NULL, "保存检测结果", "创建工作薄失败,请重试!");
        return false;
    }

    pBook->setRgbMode();
    //表头格式
    Format* pHeaderFormat = pBook->addFormat();
    pHeaderFormat->setAlignH(ALIGNH_CENTER);
    pHeaderFormat->setBorder(BORDERSTYLE_THIN);
    libxl::Font* pHeaderFont = pBook->addFont();
    pHeaderFont->setBold(true);
    pHeaderFont->setSize(10);
    pHeaderFont->setName(L"宋体");
    pHeaderFormat->setFont(pHeaderFont);
    pHeaderFormat->setWrap(true);
    pHeaderFormat->setAlignV(ALIGNV_CENTER);
    /*内容行格式*/
    //内容字体
    libxl::Font* pContentFont = pBook->addFont();
    pContentFont->setBold(false);
    pContentFont->setSize(9);
    pContentFont->setName(L"宋体");
    //横向左对齐

    Format* pContentFormatOddLeft = pBook->addFormat();
    pContentFormatOddLeft->setFont(pContentFont);
    pContentFormatOddLeft->setBorder(BORDERSTYLE_THIN);
    pContentFormatOddLeft->setWrap(true);
    pContentFormatOddLeft->setAlignV(ALIGNV_CENTER);
    pContentFormatOddLeft->setAlignH(ALIGNH_LEFT);
    //偶数
    //横向左对齐
    Format* pContentFormatEvenLeft = pBook->addFormat();
    pContentFormatEvenLeft = pBook->addFormat();
    libxl::Color clrEven = pBook->colorPack(240,240,240);
    pContentFormatEvenLeft->setFont(pContentFont);
    pContentFormatEvenLeft->setFillPattern(FILLPATTERN_SOLID);
    pContentFormatEvenLeft->setPatternForegroundColor(clrEven);
    pContentFormatEvenLeft->setBorder(BORDERSTYLE_THIN);
    pContentFormatEvenLeft->setWrap(true);
    pContentFormatEvenLeft->setAlignV(ALIGNV_CENTER);
    pContentFormatEvenLeft->setAlignH(ALIGNH_LEFT);

    //填充sheet内容
    QString sValue;
    const int ColumnWidth[] = {8, 20, 10, 100, 10, 10, 20, 200};
    //设置列宽信息
    for(int iCol = 0; iCol < sizeof(ColumnWidth)/sizeof(int); iCol++)
    {
        pSheet->setCol(iCol, iCol, ColumnWidth[iCol]);
    }
    //填写 表头
    {
        pSheet->writeStr(0, 0, L"序号", pHeaderFormat);
        pSheet->writeStr(0, 1, L"校验项目", pHeaderFormat);
        pSheet->writeStr(0, 2, L"所在行", pHeaderFormat);
        pSheet->writeStr(0, 3, L"检测结果", pHeaderFormat);
        pSheet->writeStr(0, 4, L"参考规范", pHeaderFormat);
        pSheet->writeStr(0, 5, L"校验类型", pHeaderFormat);
        pSheet->writeStr(0, 6, L"原文内容", pHeaderFormat);
        pSheet->setRow(0, 25);
    }
    //填写 表内容
    for(int iRecord = 1; iRecord < m_CheckResultVec.size() + 1; iRecord++)
    {
        CheckInfo& ci = m_CheckResultVec[iRecord - 1];
        Format* pFormat = (iRecord % 2)? pContentFormatEvenLeft : pContentFormatOddLeft;

        pSheet->writeStr(iRecord, 0, QString::number(iRecord).toStdWString().c_str(), pFormat);
        pSheet->writeStr(iRecord, 1, ci.sClass.toStdWString().c_str(), pFormat);
        pSheet->writeStr(iRecord, 2, ci.position.toStdWString().c_str(), pFormat);
        pSheet->writeStr(iRecord, 3, ci.sDesc.toStdWString().c_str(), pFormat);
        pSheet->writeStr(iRecord, 4, ci.sStandard.toStdWString().c_str(), pFormat);
        pSheet->writeStr(iRecord, 5, ci.sFlag.toStdWString().c_str(), pFormat);
        pSheet->writeStr(iRecord, 6, ci.sXml.toStdWString().c_str(), pFormat);
        pSheet->setRow(iRecord, 25);
    }

    if (pBook->save(exportPath.toStdWString().c_str()))
    {
        pBook->release();
        return true;
    }   
   return false;
}

std::wstring CSCLValid::toW(std::string str)
{
    return QString(str.c_str()).toStdWString();
}

std::string CSCLValid::toAddr(std::string row)
{
    return "A"+row;
}

void CSCLValid::SetColWidth(Sheet *pSheet, const int ColumnWidth[], int cnt)
{
    for(int iCol = 0; iCol < cnt; iCol++)
    {
        pSheet->setCol(iCol, iCol, ColumnWidth[iCol]);
    }
}

void CSCLValid::SetCelValue(Sheet *pSheet, DataValue &value, SsdExcelFormat &fmt, int cnt, int row, int &col, bool isFormula, std::string DispName)
{
    std::wstring strVal;

    if (cnt <= 0) cnt = 1;

    if (isFormula)
    {
        PtrVec &vec = value.second;
        if (vec.size())
        {
            DataItem &item = *((DataItem*)vec[0]);
            strVal = toW("HYPERLINK(\"#" + item[ITEM_SHEET].first + "!" + toAddr(item[ITEM_ROW].first) + "\",\"" + item[DispName].first +"\")");
        }
    }
    else
        strVal = toW(value.first);

    pSheet->setMerge(row, row+cnt-1, col, col);
    for (int i = row; i <= (row+cnt-1); i++)
    {
        if (isFormula && !strVal.empty())
        {
            pSheet->writeFormula(i, col, strVal.c_str(), fmt[Link_Format]);
        }
        else
            pSheet->writeStr(i, col, strVal.c_str(), fmt[Content_Format]);
    }
    col++;
}

void CSCLValid::SetCelVec(Sheet *pSheet, PtrVec &vec, SsdExcelFormat &fmt, int cnt, int row, int &col, std::string DispName, bool isFormula)
{
    int i = 0;
    for (PtrVec::iterator it = vec.begin(); it != vec.end(); it++, i++)
    {
        DataItem &item = *(DataItem*)(*it);
        if (isFormula)
        {
            std::wstring LinkStr = toW("HYPERLINK(\"#" + item[ITEM_SHEET].first + "!" + toAddr(item[ITEM_ROW].first) + "\",\"" + item[DispName].first +"\")");
            pSheet->writeFormula(row+i, col, LinkStr.c_str(), fmt[Link_Format]);
        }
        else
            pSheet->writeStr(row+i, col, toW(item[DispName].first).c_str(), fmt[Content_Format]);
    }

    for (; (i < cnt) || (cnt == 0 && i == 0); i++)
    {
        pSheet->writeStr(row+i, col, L"", fmt[Content_Format]);
    }

    col++;
}

void CSCLValid::SetCelResult(Sheet *pSheet, DataItem &item, SsdExcelFormat &fmt, int cnt, int row, int &col)
{
    if (cnt <= 0) cnt = 1;
    pSheet->setMerge(row, row+cnt-1, col, col);
    PtrVec resultVec = item[ITEM_RESULT].second;

    bool bHasResult = false;
    std::wstring ErrLinkStr;
    if (resultVec.size())
    {
        DataItem &item = *(DataItem*)resultVec[0];
        QString result = QString("%1").arg(item[ITEM_ERR_LIST].second.size());
        ErrLinkStr = toW("HYPERLINK(\"#" + item[ITEM_SHEET].first + "!" + toAddr(item[ITEM_ROW].first) + "\",\"" + result.toStdString() +"\")");
        if (item[ITEM_ERR_LIST].second.size())
            bHasResult = true;
    }

    for (int i = row; i <= (row+cnt-1); i++)
    {
        if (bHasResult)
            pSheet->writeFormula(i, col, ErrLinkStr.c_str(), fmt[Link_Format]);
        else
            pSheet->writeNum(i, col, 0, fmt[Content_Format]);
    }
    col++;
}

void CSCLValid::CreateStationSheet(DataList &data_substation, SsdExcelFormat &fmt, Book* pBook)
{
    Sheet *pStationSheet = nullptr;
    int row = 0;
    for (DataList::iterator it = data_substation.begin(); it != data_substation.end(); it++)
    {
        DataItem &item = (*(*it));

        if (!pStationSheet)
        {
            pStationSheet = pBook->addSheet(QString(item[ITEM_SHEET].first.c_str()).toStdWString().c_str());
            int col = 0;

            pStationSheet->writeStr(row, col++, L"变电站名称", fmt[Header_Format]);
            pStationSheet->writeStr(row, col++, L"变电站描述", fmt[Header_Format]);
            pStationSheet->writeStr(row, col++, L"变电站类型", fmt[Header_Format]);
            pStationSheet->writeStr(row, col++, L"所含电压等级", fmt[Header_Format]);
            pStationSheet->writeStr(row, col++, L"检测结果", fmt[Header_Format]);

            const int ColumnWidth[] = {20, 20, 20, 30, 20, 20, 30};
            SetColWidth(pStationSheet, ColumnWidth, sizeof(ColumnWidth)/sizeof(int));

            row ++;
        }

        PtrVec Voltage = item[ITEM_VOLTAGELEVEL].second;
        int cnt = Voltage.size();

        int col = 0;
        SetCelValue(pStationSheet, item[ITEM_NAME], fmt, cnt, row, col);
        SetCelValue(pStationSheet, item[ITEM_DESC], fmt, cnt, row, col);
        SetCelValue(pStationSheet, item[ITEM_TYPE], fmt, cnt, row, col);
        SetCelVec(pStationSheet, Voltage, fmt, cnt, row, col);
        SetCelResult(pStationSheet, item, fmt, cnt, row, col);

        row += cnt?cnt:1;
    }
}

void CSCLValid::CreateVoltageLevelSheet(DataList &data_vl, SsdExcelFormat &fmt, Book* pBook)
{
    Sheet *pVLSheet = nullptr;
    int row = 0;
    for (DataList::iterator it = data_vl.begin(); it != data_vl.end(); it++)
    {
        DataItem &item = (*(*it));

        if (!pVLSheet)
        {
            pVLSheet = pBook->addSheet(QString(item[ITEM_SHEET].first.c_str()).toStdWString().c_str());
            int col = 0;

            pVLSheet->writeStr(row, col++, L"变电名称", fmt[Header_Format]);
            pVLSheet->writeStr(row, col++, L"变电描述", fmt[Header_Format]);
            pVLSheet->writeStr(row, col++, L"所属变电站", fmt[Header_Format]);
            pVLSheet->writeStr(row, col++, L"所含间隔", fmt[Header_Format]);
            pVLSheet->writeStr(row, col++, L"检测结果", fmt[Header_Format]);

            const int ColumnWidth[] = {20, 20, 20, 40, 20, 25, 30};
            SetColWidth(pVLSheet, ColumnWidth, sizeof(ColumnWidth)/sizeof(int));

            row ++;
        }

        PtrVec Bay = item[ITEM_BAY].second;
        int cnt = Bay.size();

        int col = 0;
        SetCelValue(pVLSheet, item[ITEM_NAME], fmt, cnt, row, col);
        SetCelValue(pVLSheet, item[ITEM_DESC], fmt, cnt, row, col);
        SetCelValue(pVLSheet, item[ITEM_SUBSTATION], fmt, cnt, row, col, true);
        SetCelVec(pVLSheet, Bay, fmt, cnt, row, col, ITEM_DESC);
        SetCelResult(pVLSheet, item, fmt, cnt, row, col);

        row += cnt?cnt:1;
    }
}

void CSCLValid::CreateBaySheet(DataList &data_bay, SsdExcelFormat &fmt, Book* pBook)
{
    Sheet *pBaySheet = nullptr;
    int row = 0;
    for (DataList::iterator it = data_bay.begin(); it != data_bay.end(); it++)
    {
        DataItem &item = (*(*it));

        if (!pBaySheet)
        {
            pBaySheet = pBook->addSheet(QString(item[ITEM_SHEET].first.c_str()).toStdWString().c_str());
            int col = 0;

            pBaySheet->writeStr(row, col++, L"间隔名称", fmt[Header_Format]);
            pBaySheet->writeStr(row, col++, L"间隔描述", fmt[Header_Format]);
            pBaySheet->writeStr(row, col++, L"间隔类型", fmt[Header_Format]);
            pBaySheet->writeStr(row, col++, L"所属电压等级", fmt[Header_Format]);
            pBaySheet->writeStr(row, col++, L"所含导电设备", fmt[Header_Format]);
            pBaySheet->writeStr(row, col++, L"所含连接点", fmt[Header_Format]);
            pBaySheet->writeStr(row, col++, L"检测结果", fmt[Header_Format]);

            const int ColumnWidth[] = {20, 20, 20, 30, 30, 30, 18};
            SetColWidth(pBaySheet, ColumnWidth, sizeof(ColumnWidth)/sizeof(int));

            row ++;
        }

        PtrVec CE = item[ITEM_CONDUCTINGEQUIPMENT].second;
        PtrVec CN = item[ITEM_CONNECTIVITYNODE].second;
        int cnt = std::max(CE.size(), CN.size());

        int col = 0;
        SetCelValue(pBaySheet, item[ITEM_NAME], fmt, cnt, row, col);
        SetCelValue(pBaySheet, item[ITEM_DESC], fmt, cnt, row, col);
        SetCelValue(pBaySheet, item[ITEM_TYPE], fmt, cnt, row, col);
        SetCelValue(pBaySheet, item[ITEM_VOLTAGELEVEL], fmt, cnt, row, col, true);
        SetCelVec(pBaySheet, item[ITEM_CONDUCTINGEQUIPMENT].second, fmt, cnt, row, col);
        SetCelVec(pBaySheet, item[ITEM_CONNECTIVITYNODE].second, fmt, cnt, row, col, ITEM_DESC, false);
        SetCelResult(pBaySheet, item, fmt, cnt, row, col);

        row += cnt?cnt:1;
    }
}


void CSCLValid::CreateCESheet(DataList &data_CE, SsdExcelFormat &fmt, Book* pBook)
{
    Sheet *pCESheet = nullptr;
    int row = 0;
    for (DataList::iterator it = data_CE.begin(); it != data_CE.end(); it++)
    {
        DataItem &item = (*(*it));

        if (!pCESheet)
        {
            pCESheet = pBook->addSheet(QString(item[ITEM_SHEET].first.c_str()).toStdWString().c_str());
            int col = 0;

            pCESheet->writeStr(row, col++, L"设备名称", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"设备描述", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"设备类型", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"所属间隔", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"所含Terminal", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"所含LN", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"关联IED", fmt[Header_Format]);
            pCESheet->writeStr(row, col++, L"检测结果", fmt[Header_Format]);

            const int ColumnWidth[] = {20, 40, 20, 25, 25, 40, 20, 18};
            SetColWidth(pCESheet, ColumnWidth, sizeof(ColumnWidth)/sizeof(int));

            row ++;
        }

        PtrVec Terminal = item[ITEM_TERMINAL].second;
        PtrVec LNode = item[ITEM_LNODE].second;
        PtrVec Ied = item[ITEM_IED].second;
        int cnt = std::max(Terminal.size(), LNode.size());
        cnt = std::max(cnt, (int)Ied.size());

        int col = 0;
        SetCelValue(pCESheet, item[ITEM_NAME], fmt, cnt, row, col);
        SetCelValue(pCESheet, item[ITEM_DESC], fmt, cnt, row, col);
        SetCelValue(pCESheet, item[ITEM_TYPE], fmt, cnt, row, col);
        SetCelValue(pCESheet, item[ITEM_BAY], fmt, cnt, row, col, true, ITEM_DESC);
        SetCelVec(pCESheet, item[ITEM_TERMINAL].second, fmt, cnt, row, col, ITEM_NAME, false);
        SetCelVec(pCESheet, item[ITEM_LNODE].second, fmt, cnt, row, col, ITEM_LN);
        SetCelValue(pCESheet, item[ITEM_IED], fmt, cnt, row, col);
        SetCelResult(pCESheet, item, fmt, cnt, row, col);

        row += cnt?cnt:1;
    }
}


void CSCLValid::CreateLNSheet(DataList &data_LN, SsdExcelFormat &fmt, Book* pBook)
{
    Sheet *pLNSheet = nullptr;
    int row = 0;
    for (DataList::iterator it = data_LN.begin(); it != data_LN.end(); it++)
    {
        DataItem &item = (*(*it));

        if (!pLNSheet)
        {
            pLNSheet = pBook->addSheet(QString(item[ITEM_SHEET].first.c_str()).toStdWString().c_str());
            int col = 0;

            pLNSheet->writeStr(row, col++, L"LN", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"ldInst", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"prefix", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"lnClass", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"lnInst", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"所属装置", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"关联IED", fmt[Header_Format]);
            pLNSheet->writeStr(row, col++, L"检测结果", fmt[Header_Format]);

            const int ColumnWidth[] = {40, 20, 20, 30, 30, 60, 20, 18};
            SetColWidth(pLNSheet, ColumnWidth, sizeof(ColumnWidth)/sizeof(int));

            row ++;
        }

        PtrVec Terminal = item[ITEM_TERMINAL].second;
        PtrVec LNode = item[ITEM_LNODE].second;
        PtrVec Ied = item[ITEM_IED].second;
        int cnt = std::max(Terminal.size(), LNode.size());
        cnt = std::max(cnt, (int)Ied.size());

        int col = 0;
        SetCelValue(pLNSheet, item[ITEM_LN], fmt, cnt, row, col);
        SetCelValue(pLNSheet, item[ITEM_LDINST], fmt, cnt, row, col);
        SetCelValue(pLNSheet, item[ITEM_PREFIX], fmt, cnt, row, col);
        SetCelValue(pLNSheet, item[ITEM_LNCLASS], fmt, cnt, row, col);
        SetCelValue(pLNSheet, item[ITEM_LNINST], fmt, cnt, row, col);
        SetCelValue(pLNSheet, item[ITEM_CONDUCTINGEQUIPMENT], fmt, cnt, row, col, true, ITEM_DESC);
        SetCelValue(pLNSheet, item[ITEM_IED], fmt, cnt, row, col);
        SetCelResult(pLNSheet, item, fmt, cnt, row, col);

        row += cnt?cnt:1;
    }
}

Sheet* FindSheet(Book* pBook, std::wstring sheetName)
{
    for (int i = 0; i < pBook->sheetCount(); i++)
    {
        Sheet *pSheet = pBook->getSheet(i);
        if (sheetName == pSheet->name())
        {
            return pSheet;
        }
    }
    return nullptr;
}

void CSCLValid::CreateResultTable(DataList &data, SsdExcelFormat &fmt, Book* pBook, std::string Belong)
{
    if (data.size() <= 0)
        return;

    bool bHasTitle = false;
    int row = 0;
    for (DataList::iterator it = data.begin(); it != data.end(); it++)
    {
        DataItem &item = *(*it);
        PtrVec &errVec = item[ITEM_RESULT].second;

        if (errVec.size() <= 0)
            continue;

        DataItem &errItem = *(DataItem*)errVec[0];

        std::wstring sheet_name = toW(errItem[ITEM_SHEET].first);
        Sheet *pSheet = FindSheet(pBook, sheet_name);

        if (!bHasTitle)
        {
            QString strRow(errItem[ITEM_ROW].first.c_str());
            row = strRow.toInt() - 3;

            int col = 0;
            pSheet->setMerge(row, row, col, col+6);
            for (int c = col; c <= col+6; c++)
                pSheet->writeStr(row, c, L"检测结果清单", fmt[Title_Format]);
            row++;

            pSheet->writeStr(row, col++, L"归属", fmt[Header_Format]);
            pSheet->writeStr(row, col++, L"校验项目", fmt[Header_Format]);
            pSheet->writeStr(row, col++, L"所在行", fmt[Header_Format]);
            pSheet->writeStr(row, col++, L"检测结果", fmt[Header_Format]);
            pSheet->writeStr(row, col++, L"参考规范", fmt[Header_Format]);
            pSheet->writeStr(row, col++, L"校验类型", fmt[Header_Format]);
            pSheet->writeStr(row, col++, L"原文内容", fmt[Header_Format]);
            bHasTitle = true;
            row++;
        }

        PtrVec &err_list = errItem[ITEM_ERR_LIST].second;
        pSheet->setMerge(row, row+err_list.size()-1, 0, 0);
        for (PtrVec::iterator it_err = err_list.begin(); it_err != err_list.end(); it_err++)
        {
            int idx = (int)*it_err;
            CheckInfo &info = m_CheckResultVec[idx];
            int col = 0;
            pSheet->writeStr(row, col++, toW(item[Belong].first.c_str()).c_str(), fmt[Content_Format]);
            pSheet->writeStr(row, col++, toW(info.sClass.toStdString().c_str()).c_str(), fmt[Content_Format]);
            pSheet->writeStr(row, col++, toW(info.position.toStdString().c_str()).c_str(), fmt[Content_Format]);
            pSheet->writeStr(row, col++, toW(info.sDesc.toStdString().c_str()).c_str(), fmt[Content_Format]);
            pSheet->writeStr(row, col++, toW(info.sStandard.toStdString().c_str()).c_str(), fmt[Content_Format]);
            pSheet->writeStr(row, col++, toW(info.sFlag.toStdString().c_str()).c_str(), fmt[Content_Format]);
            pSheet->writeStr(row, col++, toW(info.sXml.toStdString().c_str()).c_str(), fmt[Content_Format]);
            row++;
        }
    }
}

SsdExcelFormat CSCLValid::CreateFormats(Book* pBook)
{
    SsdExcelFormat _stFormat;
    //表头格式
    Format* pHeaderFormat = pBook->addFormat();
    if (pHeaderFormat)
    {
        _stFormat[Header_Format] = pHeaderFormat;
        pHeaderFormat->setAlignH(ALIGNH_CENTER);
        pHeaderFormat->setBorder(BORDERSTYLE_THIN);
        libxl::Font* pHeaderFont = pBook->addFont();
        pHeaderFont->setBold(true);
        pHeaderFont->setSize(14);
        pHeaderFont->setName(L"宋体");
        pHeaderFormat->setFont(pHeaderFont);
        pHeaderFormat->setWrap(false);
        pHeaderFormat->setAlignV(ALIGNV_CENTER);
        pHeaderFormat->setPatternForegroundColor(COLOR_PALEBLUE);
        pHeaderFormat->setFillPattern(FILLPATTERN_SOLID);
    }

    Format* pTitleFormat = pBook->addFormat();
    if (pTitleFormat)
    {
        _stFormat[Title_Format] = pTitleFormat;
        pTitleFormat->setAlignH(ALIGNH_CENTER);
        pTitleFormat->setBorder(BORDERSTYLE_THIN);
        libxl::Font* pTitleFont = pBook->addFont();
        pTitleFont->setBold(true);
        pTitleFont->setSize(16);
        pTitleFont->setName(L"宋体");
        pTitleFormat->setFont(pTitleFont);
        pTitleFormat->setWrap(false);
        pTitleFormat->setAlignV(ALIGNV_CENTER);
        pTitleFormat->setPatternForegroundColor(COLOR_YELLOW_CL);
        pTitleFormat->setFillPattern(FILLPATTERN_SOLID);
    }

    //横向左对齐
    Format* pContentFormatOddLeft = pBook->addFormat();
    if (pContentFormatOddLeft)
    {
        _stFormat[Content_Format] = pContentFormatOddLeft;

        libxl::Font* pContentFont = pBook->addFont();
        pContentFont->setBold(false);
        pContentFont->setSize(12);
        pContentFont->setName(L"宋体");

        pContentFormatOddLeft->setFont(pContentFont);
        pContentFormatOddLeft->setBorder(BORDERSTYLE_THIN);
        pContentFormatOddLeft->setWrap(true);
        pContentFormatOddLeft->setAlignV(ALIGNV_CENTER);
        pContentFormatOddLeft->setAlignH(ALIGNH_CENTER);
    }

    //偶数
    //横向左对齐
    Format* pLinkFormat = pBook->addFormat();
    if (pLinkFormat)
    {
        _stFormat[Link_Format] = pLinkFormat;
        libxl::Font* pContentFont = pBook->addFont();
        pContentFont->setBold(false);
        pContentFont->setSize(12);
        pContentFont->setName(L"宋体");
        pContentFont->setColor(COLOR_BLUE);
        pContentFont->setUnderline(UNDERLINE_SINGLE);

        pLinkFormat->setFont(pContentFont);
        pLinkFormat->setBorder(BORDERSTYLE_THIN);
        pLinkFormat->setWrap(true);
        pLinkFormat->setAlignV(ALIGNV_CENTER);
        pLinkFormat->setAlignH(ALIGNH_CENTER);
        
    }

    return _stFormat;
}

void CSCLValid::LayoutData(DataList &data_station, DataList &data_volLevel, DataList &data_bay, 
						   DataList &data_ConEqu, DataList &data_ConnNode, DataList &data_terminal,
						   DataList &data_LN, DataList &data_IED)
{
    typedef std::function<int(DataItem &)> CallProc;

    auto LayoutSheet = [](DataList &data, CallProc proc) {
        DatValVec tmp_result;
        int i = 2;
        for (DataList::iterator it = data.begin(); it != data.end(); it++)
        {
            DataItem &item = *(*it);
            QString row = QString("%1").arg(i);

            item[ITEM_ROW] = DataValue(row.toStdString(), PtrVec());

            i += proc(item);

            PtrVec result = item[ITEM_RESULT].second;
            if (result.size())
            {
                DataItem &resultItem =*(DataItem*)result[0];
                resultItem[ITEM_SHEET] = item[ITEM_SHEET];
            }

            tmp_result.push_back(item[ITEM_RESULT]);
        }
        i += 5;

        for (DatValVec::iterator it_res = tmp_result.begin(); it_res != tmp_result.end(); it_res++)
        {
            DataValue &res_val = *it_res;
            PtrVec &vec = res_val.second;
            if (vec.size())
            {
                DataItem &itemErr = *(DataItem*)vec[0];

                QString strRow = QString("%1").arg(i);
                itemErr[ITEM_ROW] = DataValue(strRow.toStdString(), PtrVec());
                i += itemErr[ITEM_ERR_LIST].second.size();
            }
        }
    };

    LayoutSheet(data_station, [](DataItem & item) -> int{
        int cnt = item[ITEM_VOLTAGELEVEL].second.size();
        return cnt > 0 ? cnt : 1;
    });

    LayoutSheet(data_volLevel, [](DataItem & item) -> int{
        int cnt = item[ITEM_BAY].second.size();
        return cnt > 0 ? cnt : 1;
    });

    LayoutSheet(data_bay, [](DataItem & item) -> int{
        int cnt = std::max(item[ITEM_CONDUCTINGEQUIPMENT].second.size(), item[ITEM_CONNECTIVITYNODE].second.size());
        return cnt > 0 ? cnt : 1;
    });

    LayoutSheet(data_ConEqu, [](DataItem & item) -> int{
        int cnt = std::max(item[ITEM_TERMINAL].second.size(), item[ITEM_LNODE].second.size());
        cnt = std::max(cnt, (int)item[ITEM_IED].second.size());
        return cnt > 0 ? cnt : 1;
    });
    
    LayoutSheet(data_LN, [](DataItem & item) -> int{
        return 1;
    });

}

bool CSCLValid::ExportSSDExcel(QString exportPath, QString subKey)
{
    Book* pBook = xlCreateBook();
    if(pBook==NULL)
    {
        return false;
    }
    pBook->setKey(L"济南容弗科技有限公司", L"windows-26292c0705cde60766bc6667a2hce3l9");
    pBook->setRgbMode();

    SsdExcelFormat _stFormat = CreateFormats(pBook);

    DataList data_substation, data_volLevel, data_bay, data_ConEqu, data_ConnNode, data_terminal, data_LN, data_IED, data_Result;

    AnalyseData(subKey, data_substation, data_volLevel, data_bay, data_ConEqu, data_ConnNode, data_terminal, data_LN, data_IED, data_Result);

    LayoutData(data_substation, data_volLevel, data_bay, data_ConEqu, data_ConnNode, data_terminal, data_LN, data_IED);

    CreateStationSheet(data_substation, _stFormat, pBook);

    CreateResultTable(data_substation, _stFormat, pBook);

    CreateVoltageLevelSheet(data_volLevel, _stFormat, pBook);

    CreateResultTable(data_volLevel, _stFormat, pBook);

    CreateBaySheet(data_bay, _stFormat, pBook);

    CreateResultTable(data_bay, _stFormat, pBook);

    CreateCESheet(data_ConEqu, _stFormat, pBook);

    CreateResultTable(data_ConEqu, _stFormat, pBook);

    CreateLNSheet(data_LN, _stFormat, pBook);

    CreateResultTable(data_LN, _stFormat, pBook, ITEM_LN);

    if (pBook->save(exportPath.toStdWString().c_str()))
    {
        pBook->release();
        return true;
    }   
    return false;

}

PtrVec CSCLValid::AddIntoPtrVec(int c, ...)
{
    PtrVec vec;
    va_list argp;
    va_start(argp, c);
    for (int i = 0; i < c; i++)
    {
        void* p = va_arg(argp, void*);
        vec.push_back(p);
    }
    va_end(argp);
    return vec;
}

PtrVec CSCLValid::AddErrCodesIntoResult(std::string name, DataList &data_result)
{
    PtrVec ErrCodeVec;
    QVector<int> &stErrList = m_SSDResultMap[name.c_str()];
    for (QVector<int>::iterator itStErr = stErrList.begin(); itStErr != stErrList.end(); itStErr++)
    {
        ErrCodeVec.push_back((void*)*itStErr);
    }

    DataPtr pResult = boost::make_shared<DataItem>();
    (*pResult)[ITEM_ERR_LIST] = DataValue(ITEM_ERR_LIST, ErrCodeVec);
    data_result.push_back(pResult);

    PtrVec ResultVec;
    ResultVec.push_back(pResult.get());

    return ResultVec;
}

void CSCLValid::AnalyseData(QString subKey, DataList &data_station, DataList &data_volLevel, 
                            DataList &data_bay, DataList &data_ConEqu, DataList &data_ConnNode,
                            DataList &data_terminal, DataList &data_LN, DataList &data_IED, 
                            DataList &data_result)
{
    CSSDLoader ssd;
    ssd.SetCurrentSubstation(subKey);

    SubstationCache &substation = ssd.m_SSDSubStationCache;

    DataPtr pSubstation = boost::make_shared<DataItem>();

    (*pSubstation)[ITEM_SHEET] = DataValue(SHEET_SUBSTATION, PtrVec());
    (*pSubstation)[ITEM_NAME] = DataValue(substation.name,PtrVec());
    (*pSubstation)[ITEM_DESC] = DataValue(substation.desc,PtrVec());
    (*pSubstation)[ITEM_TYPE] = DataValue(substation.dtype,PtrVec());
    (*pSubstation)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(substation.name, data_result));

    PtrVec vl_vec;
    VoltageLevelVector &vVoltageLevel = substation.vVoltageLevel;
    for (VoltageLevelVector::iterator itVL = vVoltageLevel.begin(); itVL != vVoltageLevel.end(); itVL++)
    {
        // 别名
        VoltageLevelCache &VoltageLevel = *itVL;

        DataPtr pVoltageLevel = boost::make_shared<DataItem>();
        vl_vec.push_back(pVoltageLevel.get());

        (*pVoltageLevel)[ITEM_SHEET] = DataValue(SHEET_VOLTAGELEVEL, PtrVec());
        (*pVoltageLevel)[ITEM_NAME] = DataValue(VoltageLevel.name, PtrVec());
        (*pVoltageLevel)[ITEM_DESC] = DataValue(VoltageLevel.desc, PtrVec());
        (*pVoltageLevel)[ITEM_SUBSTATION] = DataValue(ITEM_SUBSTATION, AddIntoPtrVec(1, pSubstation.get()));
        (*pVoltageLevel)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(substation.name + "/" + VoltageLevel.name, data_result));

        PtrVec bay_vec;
        BayCacheMap &baies = itVL->bayMap;
        for (BayCacheMap::iterator itBay = baies.begin(); itBay != baies.end(); itBay++)
        {
            BayCache &bay = itBay->second;

            DataPtr pBay = boost::make_shared<DataItem>();
            bay_vec.push_back(pBay.get());

            (*pBay)[ITEM_SHEET] = DataValue(SHEET_BAY, PtrVec());
            (*pBay)[ITEM_NAME] = DataValue(bay.name, PtrVec());
            (*pBay)[ITEM_DESC] = DataValue(bay.desc, PtrVec());
            (*pBay)[ITEM_TYPE] = DataValue(bay.dtype, PtrVec());
            (*pBay)[ITEM_VOLTAGELEVEL] = DataValue(ITEM_VOLTAGELEVEL, AddIntoPtrVec(1, pVoltageLevel.get()));
            (*pBay)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(substation.name + "/" + VoltageLevel.name + "/" + bay.name, data_result));

            PtrVec ceVec;
            ConductingEquipmentCacheMap &ConductingEquipmentMap = bay.ceMap;
            for (ConductingEquipmentCacheMap::iterator itCE = ConductingEquipmentMap.begin(); itCE != ConductingEquipmentMap.end(); itCE++)
            {
                ConductingEquipmentCache &ConductingEquipment = itCE->second;

                DataPtr pCE = boost::make_shared<DataItem>();
				if (pCE && ConductingEquipment.type != "PTR")
				{
					data_ConEqu.push_back(pCE);
					ceVec.push_back(pCE.get());

					(*pCE)[ITEM_SHEET] = DataValue(SHEET_CE, PtrVec());
					(*pCE)[ITEM_NAME] = DataValue(ConductingEquipment.name, PtrVec());
					(*pCE)[ITEM_DESC] = DataValue(ConductingEquipment.desc, PtrVec());
					(*pCE)[ITEM_TYPE] = DataValue(ConductingEquipment.type, PtrVec());
					(*pCE)[ITEM_BAY] = DataValue(ITEM_BAY, AddIntoPtrVec(1, pBay.get()));
					(*pCE)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(substation.name + "/" + VoltageLevel.name + "/" + bay.name + "/" + ConductingEquipment.name, data_result));

// 					PtrVec iedVec;

					PtrVec terminalVec;
					TerminalCacheMap &TerminalMap = ConductingEquipment.terminalMap;
					for (TerminalCacheMap::iterator itTer = TerminalMap.begin(); itTer != TerminalMap.end(); itTer++)
					{
						TerminalCache &Terminal = itTer->second;

						DataPtr pTerminal = boost::make_shared<DataItem>();
						terminalVec.push_back(pTerminal.get());

						(*pTerminal)[ITEM_NAME] = DataValue(Terminal.name, PtrVec());
						data_terminal.push_back(pTerminal);
					}
					(*pCE)[ITEM_TERMINAL] = DataValue(ITEM_TERMINAL, terminalVec);

					PtrVec lnVec;
					LNodeCacheMap &LNodeMap = ConductingEquipment.lnMap;
					for (LNodeCacheMap::iterator itLN = LNodeMap.begin(); itLN != LNodeMap.end(); itLN++)
					{
						LNodeCache &LNode = itLN->second;

						DataPtr pLN = boost::make_shared<DataItem>();
						lnVec.push_back(pLN.get());

						std::string path = LNode.iedName + "." + LNode.ldInst + "/" + LNode.prefix + LNode.lnClass + LNode.lnInst;
						(*pLN)[ITEM_SHEET] = DataValue(SHEET_LNODE, PtrVec());
						(*pLN)[ITEM_LN] = DataValue(path, PtrVec());
						(*pLN)[ITEM_LDINST] = DataValue(LNode.ldInst, PtrVec());
						(*pLN)[ITEM_PREFIX] = DataValue(LNode.prefix, PtrVec());
						(*pLN)[ITEM_LNCLASS] = DataValue(LNode.lnClass, PtrVec());
						(*pLN)[ITEM_LNINST] = DataValue(LNode.lnInst, PtrVec());
						(*pLN)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(path, data_result));

						PtrVec t_ceVec;
						t_ceVec.push_back(pCE.get());
						(*pLN)[ITEM_CONDUCTINGEQUIPMENT] = DataValue(ITEM_CONDUCTINGEQUIPMENT, t_ceVec);

						(*pLN)[ITEM_IED] = DataValue(LNode.iedName, PtrVec());

						(*pCE)[ITEM_IED] = DataValue(LNode.iedName, PtrVec());

						data_LN.push_back(pLN);
					}
					(*pCE)[ITEM_LNODE] = DataValue(ITEM_LNODE, lnVec);
				}

				PowerTransformerCacheMap &PowerTransformer = bay.ptMap;
				for (PowerTransformerCacheMap::iterator it_PT = PowerTransformer.begin(); it_PT != PowerTransformer.end(); it_PT++)
				{
					PowerTransformerCache &pt = it_PT->second;

					DataPtr pPT = boost::make_shared<DataItem>();
					data_ConEqu.push_back(pPT);
					ceVec.push_back(pPT.get());

					(*pPT)[ITEM_SHEET] = DataValue(SHEET_CE, PtrVec());
					(*pPT)[ITEM_NAME] = DataValue(pt.name, PtrVec());
					(*pPT)[ITEM_DESC] = DataValue(pt.desc, PtrVec());
					(*pPT)[ITEM_TYPE] = DataValue(pt.type, PtrVec());
					(*pPT)[ITEM_BAY] = DataValue(ITEM_BAY, AddIntoPtrVec(1, pBay.get()));
					(*pPT)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(substation.name + "/" + VoltageLevel.name + "/" + bay.name + "/" + pt.name, data_result));

					PtrVec lnVec;
					LNodeCacheMap &LNodeMap = pt.lnodeMap;
					for (LNodeCacheMap::iterator itLN = LNodeMap.begin(); itLN != LNodeMap.end(); itLN++)
					{
						LNodeCache &LNode = itLN->second;

						DataPtr pLN = boost::make_shared<DataItem>();
						lnVec.push_back(pLN.get());

						std::string path = LNode.iedName + "." + LNode.ldInst + "/" + LNode.prefix + LNode.lnClass + LNode.lnInst;
						(*pLN)[ITEM_SHEET] = DataValue(SHEET_LNODE, PtrVec());
						(*pLN)[ITEM_LN] = DataValue(path, PtrVec());
						(*pLN)[ITEM_LDINST] = DataValue(LNode.ldInst, PtrVec());
						(*pLN)[ITEM_PREFIX] = DataValue(LNode.prefix, PtrVec());
						(*pLN)[ITEM_LNCLASS] = DataValue(LNode.lnClass, PtrVec());
						(*pLN)[ITEM_LNINST] = DataValue(LNode.lnInst, PtrVec());
						(*pLN)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(path, data_result));

						PtrVec t_ceVec;
						t_ceVec.push_back(pPT.get());
						(*pLN)[ITEM_CONDUCTINGEQUIPMENT] = DataValue(ITEM_CONDUCTINGEQUIPMENT, t_ceVec);
						(*pLN)[ITEM_IED] = DataValue(LNode.iedName, PtrVec());
						(*pPT)[ITEM_IED] = DataValue(LNode.iedName, PtrVec());

						data_LN.push_back(pLN);
					}
					(*pPT)[ITEM_LNODE] = DataValue(ITEM_LNODE, lnVec);

					//////////////////////////////////////////////////////////////////////////
					TransformerWindingCacheMap &WindingMap = pt.tfWindingMap;
					for (TransformerWindingCacheMap::iterator it_winding = WindingMap.begin(); it_winding != WindingMap.end(); it_winding++)
					{
						TransformerWindingCache &Winding = it_winding->second;

						DataPtr pWinding = boost::make_shared<DataItem>();
						data_ConEqu.push_back(pWinding);
						ceVec.push_back(pWinding.get());

						(*pWinding)[ITEM_SHEET] = DataValue(SHEET_CE, PtrVec());
						(*pWinding)[ITEM_NAME] = DataValue(Winding.name, PtrVec());
						(*pWinding)[ITEM_DESC] = DataValue(Winding.desc, PtrVec());
						(*pWinding)[ITEM_TYPE] = DataValue(Winding.type, PtrVec());
						(*pWinding)[ITEM_BAY] = DataValue(ITEM_BAY, AddIntoPtrVec(1, pBay.get()));
						(*pWinding)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(substation.name + "/" + VoltageLevel.name + "/" + bay.name + "/" + pt.name + "/" + Winding.name, data_result));

						PtrVec terminalVec;
						TerminalCacheMap &TerminalMap = Winding.terminalMap;
						for (TerminalCacheMap::iterator itTer = TerminalMap.begin(); itTer != TerminalMap.end(); itTer++)
						{
							TerminalCache &Terminal = itTer->second;

							DataPtr pTerminal = boost::make_shared<DataItem>();
							terminalVec.push_back(pTerminal.get());

							(*pTerminal)[ITEM_NAME] = DataValue(Terminal.name, PtrVec());
							data_terminal.push_back(pTerminal);
						}
						(*pWinding)[ITEM_TERMINAL] = DataValue(ITEM_TERMINAL, terminalVec);


						PtrVec lnVec;
						LNodeCacheMap &LNodeMap = Winding.lnMap;
						for (LNodeCacheMap::iterator itLN = LNodeMap.begin(); itLN != LNodeMap.end(); itLN++)
						{
							LNodeCache &LNode = itLN->second;

							DataPtr pLN = boost::make_shared<DataItem>();
							lnVec.push_back(pLN.get());

							std::string path = LNode.iedName + "." + LNode.ldInst + "/" + LNode.prefix + LNode.lnClass + LNode.lnInst;
							(*pLN)[ITEM_SHEET] = DataValue(SHEET_LNODE, PtrVec());
							(*pLN)[ITEM_LN] = DataValue(path, PtrVec());
							(*pLN)[ITEM_LDINST] = DataValue(LNode.ldInst, PtrVec());
							(*pLN)[ITEM_PREFIX] = DataValue(LNode.prefix, PtrVec());
							(*pLN)[ITEM_LNCLASS] = DataValue(LNode.lnClass, PtrVec());
							(*pLN)[ITEM_LNINST] = DataValue(LNode.lnInst, PtrVec());
							(*pLN)[ITEM_RESULT] = DataValue(ITEM_RESULT, AddErrCodesIntoResult(path, data_result));

							PtrVec t_ceVec;
							t_ceVec.push_back(pPT.get());
							(*pLN)[ITEM_CONDUCTINGEQUIPMENT] = DataValue(ITEM_CONDUCTINGEQUIPMENT, t_ceVec);
							(*pLN)[ITEM_IED] = DataValue(LNode.iedName, PtrVec());
							(*pWinding)[ITEM_IED] = DataValue(LNode.iedName, PtrVec());

							data_LN.push_back(pLN);
						}
						(*pPT)[ITEM_LNODE] = DataValue(ITEM_LNODE, lnVec);
					}
				}
			}

            (*pBay)[ITEM_CONDUCTINGEQUIPMENT] = DataValue(ITEM_CONDUCTINGEQUIPMENT, ceVec);

            PtrVec cnVec;
            ConnectivityNodeCacheMap &ConnNodeMap = bay.cnMap;
            for (ConnectivityNodeCacheMap::iterator itCN = ConnNodeMap.begin(); itCN != ConnNodeMap.end(); itCN++)
            {
                ConnectivityNodeCache &ConnNode = itCN->second;

                DataPtr pCN = boost::make_shared<DataItem>();
                cnVec.push_back(pCN.get());

                (*pCN)[ITEM_NAME] = DataValue(ConnNode.name, PtrVec());
                (*pCN)[ITEM_DESC] = DataValue(ConnNode.desc, PtrVec());

                data_ConnNode.push_back(pCN);
            }
            (*pBay)[ITEM_CONNECTIVITYNODE] = DataValue(ITEM_CONNECTIVITYNODE, cnVec);

            data_bay.push_back(pBay);
        }
        (*pVoltageLevel)[ITEM_BAY] = DataValue(ITEM_BAY, bay_vec);

        data_volLevel.push_back(pVoltageLevel);
    }
    (*pSubstation)[ITEM_VOLTAGELEVEL] = DataValue(ITEM_VOLTAGELEVEL, vl_vec);

    data_station.push_back(pSubstation);
}
