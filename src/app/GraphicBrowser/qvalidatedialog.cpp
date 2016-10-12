#include "qvalidatedialog.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

QValidateDialog::QValidateDialog(QWidget *parent)
	: QDialog(parent)
{
	dialog = new Ui::ValidateDialog;
	dialog->setupUi(this);
}

QValidateDialog::~QValidateDialog()
{
	delete dialog;
}

/** 
  * @brief    SetModelIndexValue
  * 
  * 设置弹出窗口中各个项目的内容
  * 
  * @class    QValidateDialog
  * @param    const QModelIndex &index : 当前双击选中项
  * @return   void
  * @author   zhang-xch
  * @date     2014/10/29
  * @remarks  
  */ 
void QValidateDialog::SetModelIndexValue(const QModelIndex &index)
{
	const QAbstractItemModel *model = index.model();
	int columnCount = model->columnCount();
	int r = index.row();

	// 填写当前的选中项的各项内容
	for (int i = 0; i < columnCount; i++)
	{
		QModelIndex temp = model->index(r, i, QModelIndex());
		QString tempStr = model->data(temp, 0).toString();

		switch(i)
		{
		case 0: // 序号
			dialog->labelNum_2->setText(tempStr);
			break;
		case 1: // 错误类型
			dialog->labelType_2->setText(tempStr);
			break;
        case 2: // 检测项
            dialog->labelName_2->setText(tempStr);
            break; 
		case 3: // 位置
			dialog->labelPos_2->setText(tempStr);
			break;           
		case 4: // 检测结果
            dialog->textEdit_1->setPlainText(tempStr);
            break;
		case 5: // 标准
			dialog->textEdit_2->setPlainText(tempStr);
			break;
        case 6: // 原文内容
            dialog->textEdit_3->setPlainText(tempStr);
            break;
		default:
			break;
		}
	}
}

/** 
  * @brief    GetLineNum
  * 
  * 根据检测项中的位置信息截取出当前检测项的行号，从而取出XML片段
  * 
  * @class    QValidateDialog
  * @param    QString pos : 当前检测项的位置信息
  * @return   void
  * @author   zhang-xch
  * @date     2014/10/29
  * @remarks  
  */ 
void QValidateDialog::GetLineNum(QString pos)
{
	int start = pos.indexOf(":");
	int end = pos.indexOf(",");
	QString temp = pos.mid(start+1, end-start-1);
	lineNum = temp.toInt();
}

/** 
  * @brief    SetFilePath
  * 
  * 根据文件路径取出XML内容，用于展示XML片段
  * 
  * @class    QValidateDialog
  * @param    QString filePath : 检测文件的路径
  * @return   void
  * @author   zhang-xch
  * @date     2014/10/29
  * @remarks  
  */ 
void QValidateDialog::SetFilePath(QString filePath)
{
	m_FileLineList.clear();		// 清空列表
	m_FilePath = filePath;
	QString line;
	QFile file(m_FilePath);
	if ( file.open( QIODevice::ReadOnly ) ) 
	{
		QTextStream stream( &file );
		stream.setCodec(QTextCodec::codecForName("UTF-8")) ;
		while ( !stream.atEnd()) 
		{
			line = stream.readLine(); // 不包括“\n”的一行文本
			m_FileLineList.append(line);
		}
		file.close();
	}
}
