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

    QMap<QString, QString> m_mapCfgFileList; // �Զ��������ļ��б�KeyΪ�ļ�����valueΪ�ļ�·��

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

    QString                m_NewFile; // �������ļ���·��
    QString                m_NewName;
    QString                m_strCaption;

    QStringList checkComboBoxList;
    QStringList levelComboBoxList;

    QList<QTreeWidgetItem *> vecCheckItem;
};

#endif // CCHECKCONFIGDLG_H
