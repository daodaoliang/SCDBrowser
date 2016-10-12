#include "DiffInfoDialog.h"
#include <QDebug>

CDiffInfoDialog::CDiffInfoDialog(void)
{
	m_DiffInfo = "";
	m_Label = new QTextEdit();
	m_Label->setReadOnly(true);
	m_VLayout = new QVBoxLayout();
	m_VLayout->addWidget(m_Label);
	setLayout(m_VLayout);
	setModal(false);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);
	setMinimumSize(300, 300);
	resize(300, 300);
}

CDiffInfoDialog::CDiffInfoDialog(QString diffInfo)
{
	m_DiffInfo = diffInfo;
	m_Label = new QTextEdit(diffInfo);
	m_Label->setReadOnly(true);
	m_VLayout = new QVBoxLayout();
	m_VLayout->addWidget(m_Label);
	setLayout(m_VLayout);
	setModal(false);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);
	setMinimumSize(300, 300);
	resize(300, 300);
}

CDiffInfoDialog::~CDiffInfoDialog(void)
{
	delete m_Label;
	delete m_VLayout;
}

void CDiffInfoDialog::setDiffInfo(const QString diffInfo)
{
	m_DiffInfo = diffInfo;
	m_Label->setPlainText(diffInfo);
}

bool CDiffInfoDialog::compareInfo(QString compareinfo)
{
    if(m_DiffInfo==compareinfo)
		return true;
	else
		return false;
}

void CDiffInfoDialog::closeEvent(QCloseEvent * event)
{
	emit CLose();
}