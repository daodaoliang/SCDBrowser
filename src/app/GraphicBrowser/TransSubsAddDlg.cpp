#include <QtGui>
#include "TransSubsAddDlg.h"
TransSubsAddDlg::TransSubsAddDlg(QWidget *parent,QString transSubsName)
    :QDialog(parent)
{
    setupUi(this);
	scdNameEdit->setText("");
}


void TransSubsAddDlg::on_transtaNameEdit_textChanged()
{

}

void TransSubsAddDlg::on_filePathButton_clicked()
{
     QFileDialog::Options options;
     options |= QFileDialog::DontUseNativeDialog;
     QString selectedFilter;
     QString fileName = QFileDialog::getOpenFileName(NULL,
                                 ("QFileDialog::getOpenFileName()"),
                                 "",
                                 ("All Files (*);;Text Files (*.txt)"),
                                 &selectedFilter,
                                 options);
     if (!fileName.isEmpty())
	 {
		 scdNameEdit->setText(fileName);
	 }
	 filePath = fileName;
}

QString TransSubsAddDlg::GetFilePath()
{
	return filePath;
}

void TransSubsAddDlg::on_okButton_clicked()
{
	if(scdNameEdit->text().isEmpty())
	{
		QMessageBox::warning(this,"增加变电站模型","未选择SCD文件",QMessageBox::Yes);
		return;
	}
	accept();
}