#ifndef QXMLVIEWER_H
#define QXMLVIEWER_H

#include <QUrl>
#include <QWidget>
#include <QTextCursor>
#include <QPlainTextEdit>
#include "SearchDialog.h"

class QWebView;
class QVBoxLayout;
class QHBoxLayout;
class QWebPage;
class QResizeEvent;
class QPlainTextEdit;
class QLineEdit;
class QPushButton;

class QXmlViewer : public QWidget
{
    Q_OBJECT
public:
    QXmlViewer(QWidget *parent);
    ~QXmlViewer();

public slots:
    void SetCmpXmlData(const QString &txt1, const QString &txt2);
    void SetXmlData(const QString &txt);
    void JavaScriptWindowObjectCleared();
    QString GetBaseXml();
    QString GetOtherXml();
	void findXML();

private:
	void showSearchDialog();
	bool searchXML(const QString& searchTxt, const bool isSenst, const bool isBwd);

    // QWidget interface
protected:
	void keyPressEvent (QKeyEvent *event);
    void resizeEvent(QResizeEvent *);

private:
    QVBoxLayout     *m_verticalLayout;
	QHBoxLayout		*m_horizontalLayout;
    QWebView        *m_webView;
    QPlainTextEdit  *m_pPlainText;
	QLineEdit		*m_pQueryLineEdit;
	QPushButton		*m_pQueryBtn;
    QString         m_baseXml;
    QString         m_otherXml;
    int             m_Height;
    QString         m_UrlCompare;
	//QTextCursor		m_TextCursor;

	CSearchDialog	*m_SearchDialog;
};

#endif // QXMLVIEWER_H
