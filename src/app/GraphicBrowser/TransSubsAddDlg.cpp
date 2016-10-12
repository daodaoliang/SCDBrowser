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
		QMessageBox::warning(this,"���ӱ��վģ��","δѡ��SCD�ļ�",QMessageBox::Yes);
		return;
	}
	accept();
}