#include "qxmlviewer.h"
#include "QXmlHighlighter.h"

#include <QString>
#include <QWebPage>
#include <QWebFrame>
#include <QWebView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
//#include <QPlainTextEdit>
#include <QRegExp>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QTextBlock>
#include <QMessageBox>
#include <QInputDialog>

QXmlViewer::QXmlViewer(QWidget *parent)
    : QWidget(parent),
    m_Height(0),
    m_UrlCompare("qrc:/mergely/Compare.html")
{
    m_verticalLayout = new QVBoxLayout(this);
    m_verticalLayout->setObjectName("verticalLayout");
    m_verticalLayout->setContentsMargins(1, 1, 1, 1);
    m_verticalLayout->setSpacing(0);
	
	//m_horizontalLayout = new QHBoxLayout(this);
	//m_horizontalLayout->setObjectName("horizontalLayout");
	//m_horizontalLayout->setContentsMargins(1, 1, 1, 1);
	//m_horizontalLayout->setSpacing(0);

	//m_pQueryLineEdit = new QLineEdit(this);
	//m_pQueryLineEdit->setObjectName("queryLineEdit");
	//m_pQueryLineEdit->setPlaceholderText("输入要搜索的关键字");

	//m_pQueryBtn = new QPushButton(this);
	//m_pQueryBtn->setObjectName("queryButton");
	//m_pQueryBtn->setText("查找");
	//connect(m_pQueryBtn, SIGNAL(clicked()), this, SLOT(findXML()));

	//m_horizontalLayout->addWidget(m_pQueryLineEdit);
	//m_horizontalLayout->addWidget(m_pQueryBtn);

    m_webView = new QWebView(this);
    m_webView->setObjectName("webView");

	//m_verticalLayout->addLayout(m_horizontalLayout);
    m_verticalLayout->addWidget(m_webView);
    m_webView->hide();
    connect(m_webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(JavaScriptWindowObjectCleared()));
    m_webView->setUrl(m_UrlCompare);
    m_pPlainText = new QPlainTextEdit(this);
    m_verticalLayout->addWidget(m_pPlainText);
    m_pPlainText->setReadOnly(true);
    m_pPlainText->setLineWrapMode(QPlainTextEdit::NoWrap);
    (void)new QXmlHighlighter(m_pPlainText->document());
    setLayout(m_verticalLayout);

	m_SearchDialog = new CSearchDialog();
	connect(m_SearchDialog, SIGNAL(SearchText()), this, SLOT(findXML()));
}

QXmlViewer::~QXmlViewer()
{
	delete m_SearchDialog;
}

void QXmlViewer::JavaScriptWindowObjectCleared()
{
    m_webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);
}

void QXmlViewer::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    m_Height = e->size().height() - 20;
    m_webView->page()->mainFrame()->evaluateJavaScript(QString("setHeight('%1')").arg(m_Height));
    e->accept();
}

void QXmlViewer::keyPressEvent(QKeyEvent *event)
{
	if (m_otherXml.isEmpty())	//非差异化下可用;
	{
		if(event->modifiers() & Qt::ControlModifier)
		{
			if (event->key() == Qt::Key_F)
			{
				showSearchDialog();
			}
		}
	}
}

void QXmlViewer::SetCmpXmlData(const QString &txt1, const QString &txt2)
{
    m_baseXml = txt1;
    m_otherXml = txt2;

    if (!m_webView->isVisible()) 
    {
        m_webView->show();
        m_pPlainText->hide();
    }

    m_webView->page()->mainFrame()->evaluateJavaScript("Refurbish()");
}
 
void QXmlViewer::SetXmlData(const QString &txt)
{
    QString tmp = txt;
    tmp.replace(QRegExp("\t"), "    ");
    if (!m_pPlainText->isVisible())
    {
        m_webView->hide();
        m_pPlainText->show();
    }
    m_pPlainText->setPlainText(tmp);
}

void QXmlViewer::showSearchDialog()
{
	if (m_SearchDialog->isHidden())
	{
		m_SearchDialog->show();
		m_SearchDialog->setTextEditFocus();
	}
}

void QXmlViewer::findXML()
{
	QString searchTxt = m_SearchDialog->getSearchText();
	bool isSensitive = m_SearchDialog->getIsSensitive();
	bool isBackWard = m_SearchDialog->getIsBackward();
	if (!searchTxt.isEmpty())
	{
		bool isFind = searchXML(searchTxt, isSensitive, isBackWard);
		
		if (!isFind)
		{
			QTextCursor tempCursor = m_pPlainText->textCursor();
			if (!isBackWard)
			{
				m_pPlainText->moveCursor(QTextCursor::Start);
			}else
			{
				m_pPlainText->moveCursor(QTextCursor::End);
			}
			
			isFind = searchXML(searchTxt, isSensitive, isBackWard);
			if (!isFind)
			{
				m_pPlainText->setTextCursor(tempCursor);
				QMessageBox::warning(m_SearchDialog, "查找", "未找到 \"" + searchTxt + "\"");
			}
		}
		this->activateWindow();		//激活当前窗口
	}
}

bool QXmlViewer::searchXML(const QString& searchTxt, const bool isSenst, const bool isBwd)
{
	bool isFind = false;
	if (isSenst && isBwd)
	{
		isFind = m_pPlainText->find(searchTxt, QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
	} 
	else if(isSenst)
	{
		isFind = m_pPlainText->find(searchTxt, QTextDocument::FindCaseSensitively);
	}
	else if (isBwd)
	{
		isFind = m_pPlainText->find(searchTxt, QTextDocument::FindBackward);
	}
	else
	{
		isFind = m_pPlainText->find(searchTxt);
	}
	return isFind;
}

QString QXmlViewer::GetBaseXml()
{
    return m_baseXml;
}

QString QXmlViewer::GetOtherXml()
{
    return m_otherXml;
}
