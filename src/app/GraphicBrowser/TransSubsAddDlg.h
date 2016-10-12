#ifndef TransSubsAdd_DIALOG_H
#define TransSubsAdd_DIALOG_H

#include <QDialog>
#include "Ui_TransSubsAddDlg.h"

class TransSubsAddDlg : public QDialog, public Ui_TransSubsAddDlg
{
    Q_OBJECT
public:
	TransSubsAddDlg(QWidget *parent,QString transSubsName);
	QString GetFilePath();
private slots:
	void on_filePathButton_clicked();
    void on_transtaNameEdit_textChanged();
    void on_okButton_clicked();

private:
	QString filePath;
};

#endif