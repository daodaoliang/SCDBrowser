#include "SearchDialog.h"

/** 
  * @brief    CSearchDialog
  * 
  * 构造函数,初始化对话框属性;
  * 
  * @class    CSearchDialog
  * @param    void :
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2015/1/22
  * @remarks  
  */ 
CSearchDialog::CSearchDialog(void)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	setWindowTitle("查找/搜索");
}

/** 
  * @brief    ~CSearchDialog
  * 
  * 析构函数;
  * 
  * @class    CSearchDialog
  * @param    void :
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2015/1/22
  * @remarks  
  */ 
CSearchDialog::~CSearchDialog(void)
{
}

/** 
  * @brief    getSearchText
  * 
  * 获取要查找的字符串;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   QString
  * @author   牛子明;
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
  * 获取是否大小写敏感;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   bool
  * @author   牛子明;
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
  * 获取是否向前搜索;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   bool
  * @author   牛子明;
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
  * 使lineEdit获得焦点;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   void
  * @author   牛子明
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
  * "查找"键点击时的相应函数;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2015/1/22
  * @remarks  
  */ 
void CSearchDialog::on_okButton_clicked()
{
	emit SearchText();		//发送查找信号;
	ui.okButton->setFocus();
}

/** 
  * @brief    on_cancelButton_clicked
  * 
  * "取消"键点击时的相应函数;
  * 
  * @class    CSearchDialog
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2015/1/22
  * @remarks  
  */ 
void CSearchDialog::on_cancelButton_clicked()
{
	hide();
}
