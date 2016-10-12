#ifndef QVALIDAEDIALOG_H
#define QVALIDAEDIALOG_H
#include <QDialog>
#include "ui_ValidateDialog.h"

class QValidateDialog : public QDialog
{
	Q_OBJECT
public:
	QValidateDialog(QWidget *parent = 0);
	~QValidateDialog();

	void SetModelIndexValue(const QModelIndex &index);
	void SetFilePath(QString filePath);

private:
	Ui::ValidateDialog* dialog;
	QString m_FilePath;
	QString m_Text;
	QList<QString> m_FileLineList;

	void GetLineNum(QString pos);
	int lineNum;
	
};

#endif // QVALIDAEDIALOG_H
