#ifndef CCHECKCONFIGDLG_H
#define CCHECKCONFIGDLG_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QLineEdit>
#include <QTreeWidgetItem>

namespace Ui {
class CCheckConfigDlg;
}

class CCheckConfigDlg : public QWidget
{
    Q_OBJECT
public:
    explicit CCheckConfigDlg(QWidget *parent = 0);
    ~CCheckConfigDlg();

    QMap<QString, QString> m_mapCfgFileList; // 自定义配置文件列表，Key为文件名，value为文件路径

    void UpdateComBoxInfo();
    void CreateSaveAsDialog();
    void SaveConfig();
    void OnInitDialog();
    bool LoadConfigXml(QString sXml);

public slots:
    void OnBnClickedButtonCancel();
    void OnBnClickedButtonSaveAs();
    void OnBnClickedButtonDel();
    void OnBnClickedButtonSave();

    void OnSaveAsOkButton();
    void OnSaveAsCancelButton();

    void OnComboxIndxeChanged(const QString &text);

signals:
    void DialogClose(QString configName);

private:
    Ui::CCheckConfigDlg *ui;

    QDialog  *m_SaveAsDialog;
    QLineEdit *m_LineEdit;

    QString                m_NewFile; // 新配置文件的路径
    QString                m_NewName;
    QString                m_strCaption;

    QStringList checkComboBoxList;
    QStringList levelComboBoxList;

    QList<QTreeWidgetItem *> vecCheckItem;
};

#endif // CCHECKCONFIGDLG_H
