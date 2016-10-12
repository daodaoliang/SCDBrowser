#include "SearchDialog.h"

/** 
  * @brief    CSearchDialog
  * 
  * ���캯��,��ʼ���Ի�������;
  * 
  * @class    CSearchDialog
  * @param    void :
  * @param    
  * @return   
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
CSearchDialog::CSearchDialog(void)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	setWindowTitle("����/����");
}

/** 
  * @brief    ~CSearchDialog
  * 
  * ��������;
  * 
  * @class    CSearchDialog
  * @param    void :
  * @param    
  * @return   
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
CSearchDialog::~CSearchDialog(void)
{
}

/** 
  * @brief    getSearchText
  * 
  * ��ȡҪ���ҵ��ַ���;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   QString
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
QString CSearchDialog::getSearchText()
{
	return ui.lineEdit->text();
}

/** 
  * @brief    getIsSensitive
  * 
  * ��ȡ�Ƿ��Сд����;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   bool
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
bool CSearchDialog::getIsSensitive()
{
	if (ui.Upper_Sensitive_CheckBox->checkState() == Qt::Unchecked)
	{
		return false;
	}
	else if (ui.Upper_Sensitive_CheckBox->checkState() == Qt::Checked)
	{
		return true;
	}
	return false;
}

/** 
  * @brief    getIsBackward
  * 
  * ��ȡ�Ƿ���ǰ����;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   bool
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
bool CSearchDialog::getIsBackward()
{
	if (ui.Backward_CheckBox->checkState() == Qt::Checked)
	{
		return true;
	} 
	else if(ui.Backward_CheckBox->checkState() == Qt::Unchecked)
	{
		return false;
	}
	return false;
}

/** 
  * @brief    setTextEditFocus
  * 
  * ʹlineEdit��ý���;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   void
  * @author   ţ����
  * @date     2015/2/5
  * @remarks  
  */ 
void CSearchDialog::setTextEditFocus()
{
	ui.lineEdit->setFocus();
}

/** 
  * @brief    on_okButton_clicked
  * 
  * "����"�����ʱ����Ӧ����;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
void CSearchDialog::on_okButton_clicked()
{
	emit SearchText();		//���Ͳ����ź�;
	ui.okButton->setFocus();
}

/** 
  * @brief    on_cancelButton_clicked
  * 
  * "ȡ��"�����ʱ����Ӧ����;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/22
  * @remarks  
  */ 
void CSearchDialog::on_cancelButton_clicked()
{
	hide();
}
