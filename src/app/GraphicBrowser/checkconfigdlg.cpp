#include "checkconfigdlg.h"
#include "ui_checkconfigdlg.h"
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QString>

#include "CMarkUp/Markup.h"

CCheckConfigDlg::CCheckConfigDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCheckConfigDlg)
{
    ui->setupUi(this);
    m_NewName = "默认方案";

    setWindowTitle("检测项配置");
    resize(1100, 600);
    ui->ConfigItemTreeWidget->setColumnWidth(0, 650);    

    checkComboBoxList << "启用" << "禁用";
    levelComboBoxList << "错误" << "警告" << "提示";

    CreateSaveAsDialog();
    UpdateComBoxInfo();

    connect(ui->comboBox_ConfigFile, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(OnComboxIndxeChanged(const QString &)));
    connect(ui->pushButton_Del, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonDel()));
    connect(ui->pushButton_Save, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonSave()));
    connect(ui->pushButton_SaveAs, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonSaveAs()));
    connect(ui->pushButton_Cancel, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonCancel()));
}

CCheckConfigDlg::~CCheckConfigDlg()
{
    delete ui;
}

void CCheckConfigDlg::OnBnClickedButtonCancel()
{
    emit DialogClose(m_NewName);
    close();
}

void CCheckConfigDlg::OnBnClickedButtonSaveAs()
{
    m_LineEdit->setText("");
    m_SaveAsDialog->exec();

    if ((m_NewName = m_LineEdit->text()) != "")
    {
        QString sNewName = m_NewName + ".xml";

        QString appPath = QApplication::applicationDirPath();
        m_NewFile = appPath + "/../cfg/private/" + sNewName;
        SaveConfig();
        UpdateComBoxInfo();
        emit DialogClose(m_NewName);
        close();
    }
}

void CCheckConfigDlg::OnBnClickedButtonDel()
{
    int iRes = QMessageBox::question(NULL, "另存为", "是否删除此方案?", QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No);
    if (iRes == QMessageBox::StandardButton::Yes)
    {
        QString currentConfig = ui->comboBox_ConfigFile->currentText();
        QString filePath = m_mapCfgFileList[currentConfig];
        if (QFile::exists(filePath))
        {
            QFile::remove(filePath);
        }
        UpdateComBoxInfo();
        m_NewName = "默认方案";
    }
    return;    
}

void CCheckConfigDlg::OnBnClickedButtonSave()
{
    QString sNewName = m_NewName + ".xml";
    QString appPath = QApplication::applicationDirPath();
    m_NewFile = appPath + "/../cfg/private/" + sNewName;
    SaveConfig();
    emit DialogClose(m_NewName);
    close();
}

void CCheckConfigDlg::CreateSaveAsDialog()
{
    m_SaveAsDialog = new QDialog();
    m_SaveAsDialog->setModal(true);
    m_SaveAsDialog->setWindowTitle(tr("方案另存为"));
    
    QLabel *saveLabel = new QLabel("请输入方案名称");
    saveLabel->setFixedSize(saveLabel->sizeHint());
    m_LineEdit = new QLineEdit();

    QPushButton *okButton = new QPushButton("确定");
    connect(okButton, SIGNAL(clicked()), this, SLOT(OnSaveAsOkButton()));
    QPushButton *cancelButton = new QPushButton("取消");
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnSaveAsCancelButton()));

    QHBoxLayout *hBox = new QHBoxLayout();
    hBox->addStretch();
    hBox->addWidget(okButton);
    hBox->addWidget(cancelButton);

    QHBoxLayout *hMainBox = new QHBoxLayout();
    hMainBox->addWidget(saveLabel);
    hMainBox->addWidget(m_LineEdit);

    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addLayout(hMainBox);
    vBox->addLayout(hBox);
      
    m_SaveAsDialog->setLayout(vBox);
}

void CCheckConfigDlg::OnSaveAsOkButton()
{
    QString newName = m_LineEdit->text();
    if (newName.isEmpty())
    {
        QMessageBox::warning(NULL, "另存为", "方案名称不能为空，请重新输入！");
        return;
    }
    else 
    {
        QMap<QString, QString>::iterator it = m_mapCfgFileList.find(newName);
        if (it != m_mapCfgFileList.end())
        {
            int iRes = QMessageBox::question(NULL, "另存为", "方案已存在,是否保存此次修改?", QMessageBox::Yes, QMessageBox::No);
            if (iRes == QMessageBox::Yes)
            {
                m_NewName = newName;
                SaveConfig();
                emit DialogClose(m_NewName);
                this->close();
            }
            else
            {
                return;
            }
        }
        else
        {
            m_NewName = newName;
        }
    }
    m_SaveAsDialog->close();
}

void CCheckConfigDlg::OnSaveAsCancelButton()
{
    m_LineEdit->setText("");
    m_SaveAsDialog->close();
}


void CCheckConfigDlg::SaveConfig()
{
    QMap<QString, QString> mapMsgC2E;
    mapMsgC2E["错误"] = "E";
    mapMsgC2E["提示"] = "R";
    mapMsgC2E["警告"] = "W";

    CMarkup xml;
    xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    xml.AddElem(L"SCLConfig");
    xml.IntoElem();
    for (int i = 0; i < vecCheckItem.size(); i++)
    {
        QTreeWidgetItem * item = vecCheckItem.at(i);
        xml.AddElem(L"CheckClass");
        xml.AddAttrib(L"key", item->data(0, Qt::UserRole).toString().toStdWString());
        xml.AddAttrib(L"desc",item->data(0, Qt::DisplayRole).toString().toStdWString());

        int itemChildCount = item->childCount();
        for (int j = 0; j < itemChildCount; j++)
        {
            xml.IntoElem();
            QTreeWidgetItem* childItem = item->child(j);
            QString key = childItem->data(0, Qt::UserRole).toString();
            QString name = childItem->data(0, Qt::UserRole+1).toString();
            QString desc = childItem->data(0, Qt::DisplayRole).toString();
            QComboBox *checkComboBox = (QComboBox*)ui->ConfigItemTreeWidget->itemWidget(childItem, 1);
            QString isCheck = checkComboBox->currentText()=="启用"?"true":"false";
            QComboBox *levelComboBox = (QComboBox*)ui->ConfigItemTreeWidget->itemWidget(childItem, 2);
            QString flag = mapMsgC2E[levelComboBox->currentText()];
            QString standard = childItem->data(3, Qt::DisplayRole).toString();

            xml.AddElem(L"CheckItem");
            xml.AddAttrib(L"key", key.toStdWString());
            xml.AddAttrib(L"name", name.toStdWString());
            xml.AddAttrib(L"desc", desc.toStdWString());
            xml.AddAttrib(L"flag", flag.toStdWString());
            xml.AddAttrib(L"isCheck", isCheck.toStdWString());
            xml.AddAttrib(L"standard", standard.toStdWString());

            int childItemChildCount = childItem->childCount();
            for (int k = 0; k < childItemChildCount; k++)
            {
                xml.IntoElem();
                QTreeWidgetItem* subChildItem = childItem->child(k);
                QString subKey = subChildItem->data(0, Qt::UserRole).toString();
                QString subName = subChildItem->data(0, Qt::UserRole+1).toString();
                QString subDesc = subChildItem->data(0, Qt::DisplayRole).toString();

                QComboBox *subCheckComboBox = (QComboBox*)ui->ConfigItemTreeWidget->itemWidget(subChildItem, 1);
                QString subIsCheck = subCheckComboBox->currentText()=="启用"?"true":"false";
                QComboBox *subLevelComboBox = (QComboBox*)ui->ConfigItemTreeWidget->itemWidget(subChildItem, 2);
                QString subFlag = mapMsgC2E[subLevelComboBox->currentText()];
                QString subStandard = subChildItem->data(3, Qt::DisplayRole).toString();
                xml.AddElem(L"SubCheckItem");
                xml.AddAttrib(L"key", subKey.toStdWString());
                xml.AddAttrib(L"name", subName.toStdWString());
                xml.AddAttrib(L"desc", subDesc.toStdWString());
                xml.AddAttrib(L"flag", subFlag.toStdWString());
                xml.AddAttrib(L"isCheck", subIsCheck.toStdWString());
                xml.AddAttrib(L"standard", subStandard.toStdWString());
                xml.OutOfElem();
            }

            xml.OutOfElem();
        }
    }
    xml.OutOfElem();
    xml.Save(m_NewFile.toStdWString());
}

void CCheckConfigDlg::OnInitDialog()
{

}

void CCheckConfigDlg::UpdateComBoxInfo()
{
    QString appPath = QApplication::applicationDirPath();
    QString configDir = appPath + "/../cfg/private";

    QDir dirPrivate(configDir);
    if (dirPrivate.exists())
    {
        int index = 0;
        m_mapCfgFileList.clear();
        ui->comboBox_ConfigFile->clear();

        dirPrivate.setFilter(QDir::Files);
        dirPrivate.setSorting(QDir::Time);
        QFileInfoList fileList = dirPrivate.entryInfoList();
        foreach(QFileInfo fileInfo, fileList)
        {
            QString absPath = fileInfo.absoluteFilePath();
            QString fileName = fileInfo.baseName();
            m_mapCfgFileList.insert(fileName, absPath);
            ui->comboBox_ConfigFile->addItem(fileName);
        }     
        ui->comboBox_ConfigFile->setCurrentIndex(ui->comboBox_ConfigFile->findText("默认方案"));
        OnComboxIndxeChanged("默认方案");
    }
}

void CCheckConfigDlg::OnComboxIndxeChanged(const QString &text)
{
    if (text.isEmpty())
    {
        return;
    }

    if (text == "默认方案")
    {
        ui->pushButton_Del->hide();
        ui->pushButton_Save->hide();
        ui->pushButton_SaveAs->show();
    }
    else if (text == "SSD检测")
    {
        ui->pushButton_Del->hide();
        ui->pushButton_Save->show();
        ui->pushButton_SaveAs->hide();
    }
    else
    {
        ui->pushButton_Del->show();
        ui->pushButton_Save->show();
        ui->pushButton_SaveAs->show();
    }
    m_NewName = text;
    LoadConfigXml(m_mapCfgFileList[text]);
}

bool CCheckConfigDlg::LoadConfigXml(QString sXml)
{
    // 加载 xml文件
    CMarkup xml;
    if (!xml.Load(sXml.toStdWString()))
    {
        return false;
    }

    ui->ConfigItemTreeWidget->clear();
    vecCheckItem.clear();

    QMap<QString, QString> mapMsgE2C;
    mapMsgE2C["E"] = "错误";
    mapMsgE2C["R"] = "提示";
    mapMsgE2C["W"] = "警告";

    while (xml.FindChildElem(L"CheckClass"))
    {
        xml.IntoElem();
        QTreeWidgetItem *ci = new QTreeWidgetItem();
        ci->setData(0, Qt::UserRole, QString::fromStdWString(xml.GetAttrib(L"key")));
        ci->setData(0, Qt::ToolTipRole, QString::fromStdWString(xml.GetAttrib(L"desc")));
        ci->setData(0, Qt::DisplayRole, QString::fromStdWString(xml.GetAttrib(L"desc")));

        while (xml.FindChildElem(L"CheckItem"))
        {
            xml.IntoElem();
            QTreeWidgetItem *subCi = new QTreeWidgetItem();
            subCi->setData(0, Qt::UserRole, QString::fromStdWString(xml.GetAttrib(L"key")));
            subCi->setData(0, Qt::UserRole+1, QString::fromStdWString(xml.GetAttrib(L"name")));
            subCi->setData(0, Qt::DisplayRole, QString::fromStdWString(xml.GetAttrib(L"desc")));
            subCi->setData(0, Qt::ToolTipRole, QString::fromStdWString(xml.GetAttrib(L"desc")));
            subCi->setData(1, Qt::DisplayRole, QString::fromStdWString((xml.GetAttrib(L"isCheck") == L"true")?L"启用":L"禁用"));
            subCi->setData(2, Qt::DisplayRole, mapMsgE2C[QString::fromStdWString(xml.GetAttrib(L"flag"))]);
            subCi->setData(3, Qt::DisplayRole, QString::fromStdWString(xml.GetAttrib(L"standard")));
            
            while(xml.FindChildElem(L"SubCheckItem"))
            {
                xml.IntoElem();
                QTreeWidgetItem *lastItem = new QTreeWidgetItem();
                lastItem->setData(0, Qt::UserRole, QString::fromStdWString(xml.GetAttrib(L"key")));
                lastItem->setData(0, Qt::UserRole+1, QString::fromStdWString(xml.GetAttrib(L"name")));
                lastItem->setData(0, Qt::DisplayRole, QString::fromStdWString(xml.GetAttrib(L"desc")));
                lastItem->setData(0, Qt::ToolTipRole, QString::fromStdWString(xml.GetAttrib(L"desc")));
                lastItem->setData(1, Qt::DisplayRole, QString::fromStdWString((xml.GetAttrib(L"isCheck") == L"true")?L"启用":L"禁用"));
                lastItem->setData(2, Qt::DisplayRole, mapMsgE2C[QString::fromStdWString(xml.GetAttrib(L"flag"))]);
                lastItem->setData(3, Qt::DisplayRole, QString::fromStdWString(xml.GetAttrib(L"standard")));
                
                subCi->addChild(lastItem);
                xml.OutOfElem();
            }

            ci->addChild(subCi);
            xml.OutOfElem();
        }
        vecCheckItem.push_back(ci);
        xml.OutOfElem();
    }

    ui->ConfigItemTreeWidget->addTopLevelItems(vecCheckItem);
    
    for (int i = 0; i < vecCheckItem.size(); i++)
    {
        QTreeWidgetItem * item = vecCheckItem.at(i);

        int itemChildCount = item->childCount();
        for (int j = 0; j < itemChildCount; j++)
        {
            QTreeWidgetItem* childItem = item->child(j);
            QVariant checkContent = childItem->data(1, Qt::DisplayRole);
            QVariant levelContent = childItem->data(2, Qt::DisplayRole);

            QComboBox *checkComboBox = new QComboBox();
            checkComboBox->addItems(checkComboBoxList);
            int checkIndex = checkComboBox->findData(checkContent, Qt::DisplayRole);
            checkComboBox->setCurrentIndex(checkIndex);

            QComboBox *levelComboBox = new QComboBox();
            levelComboBox->addItems(levelComboBoxList);
            int levelIndex = levelComboBox->findData(levelContent, Qt::DisplayRole);
            levelComboBox->setCurrentIndex(levelIndex);
            
            ui->ConfigItemTreeWidget->setItemWidget(childItem, 1, checkComboBox);
            ui->ConfigItemTreeWidget->setItemWidget(childItem, 2, levelComboBox);

            int childItemChildCount = childItem->childCount();
            for (int k = 0; k < childItemChildCount; k++)
            {
                QTreeWidgetItem* subChildItem = childItem->child(k);
                QVariant subCheckContent = subChildItem->data(1, Qt::DisplayRole);
                QVariant subLevelContent = subChildItem->data(2, Qt::DisplayRole);

                QComboBox *checkComboBox1 = new QComboBox();
                checkComboBox1->addItems(checkComboBoxList);
                checkComboBox1->setCurrentIndex(checkComboBox1->findData(subCheckContent, Qt::DisplayRole));

                QComboBox *levelComboBox1 = new QComboBox();
                levelComboBox1->addItems(levelComboBoxList);
                levelComboBox1->setCurrentIndex(levelComboBox1->findData(subLevelContent, Qt::DisplayRole));

                ui->ConfigItemTreeWidget->setItemWidget(subChildItem, 1, checkComboBox1);
                ui->ConfigItemTreeWidget->setItemWidget(subChildItem, 2, levelComboBox1);
            }
        }
    }

    ui->ConfigItemTreeWidget->expandAll();

    return TRUE;
}