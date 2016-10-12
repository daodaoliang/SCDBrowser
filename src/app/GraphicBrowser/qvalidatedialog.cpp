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
  * ���õ��������и�����Ŀ������
  * 
  * @class    QValidateDialog
  * @param    const QModelIndex &index : ��ǰ˫��ѡ����
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

	// ��д��ǰ��ѡ����ĸ�������
	for (int i = 0; i < columnCount; i++)
	{
		QModelIndex temp = model->index(r, i, QModelIndex());
		QString tempStr = model->data(temp, 0).toString();

		switch(i)
		{
		case 0: // ���
			dialog->labelNum_2->setText(tempStr);
			break;
		case 1: // ��������
			dialog->labelType_2->setText(tempStr);
			break;
        case 2: // �����
            dialog->labelName_2->setText(tempStr);
            break; 
		case 3: // λ��
			dialog->labelPos_2->setText(tempStr);
			break;           
		case 4: // �����
            dialog->textEdit_1->setPlainText(tempStr);
            break;
		case 5: // ��׼
			dialog->textEdit_2->setPlainText(tempStr);
			break;
        case 6: // ԭ������
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
  * ���ݼ�����е�λ����Ϣ��ȡ����ǰ�������кţ��Ӷ�ȡ��XMLƬ��
  * 
  * @class    QValidateDialog
  * @param    QString pos : ��ǰ������λ����Ϣ
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
  * �����ļ�·��ȡ��XML���ݣ�����չʾXMLƬ��
  * 
  * @class    QValidateDialog
  * @param    QString filePath : ����ļ���·��
  * @return   void
  * @author   zhang-xch
  * @date     2014/10/29
  * @remarks  
  */ 
void QValidateDialog::SetFilePath(QString filePath)
{
	m_FileLineList.clear();		// ����б�
	m_FilePath = filePath;
	QString line;
	QFile file(m_FilePath);
	if ( file.open( QIODevice::ReadOnly ) ) 
	{
		QTextStream stream( &file );
		stream.setCodec(QTextCodec::codecForName("UTF-8")) ;
		while ( !stream.atEnd()) 
		{
			line = stream.readLine(); // ��������\n����һ���ı�
			m_FileLineList.append(line);
		}
		file.close();
	}
}
