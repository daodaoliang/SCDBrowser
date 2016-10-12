#ifndef QDOWNLOAD_H
#define QDOWNLOAD_H

#include <QtCore/qglobal.h>


#include <QtNetwork>
#include <QNetworkReply>
#include <QByteArray>
#include <QEventLoop>



#ifdef QDOWNLOAD_LIB
# define QDOWNLOAD_EXPORT Q_DECL_EXPORT
#else
# define QDOWNLOAD_EXPORT Q_DECL_IMPORT
#endif

enum QDOWNLOAD_TYPE
{
	HTTP = 1,
	FTP
};

class CProgressDlg;
class QDOWNLOAD_EXPORT QDownload : public QObject
{
	Q_OBJECT
public:
	QDownload();
	~QDownload();
	

private:
	
	void Clean(void);

	QByteArray				m_localbuff;
	QString					m_strLocalFile;
	CProgressDlg*			m_progressdlg;
	QEventLoop*				m_loophead;
	QEventLoop*				m_loopdown;
	int						m_nDownloadReturn;

	QNetworkAccessManager*	m_downmanager;
	QNetworkAccessManager*	m_headmanager;
	QNetworkReply*			m_downreply;
	QNetworkReply*			m_headreply;
	qint64					m_nTotalSize;
	qint64					m_nRecvSize;

	QString					m_strRemoteFile;
	QString					m_strLastErr;
	

public:
	int download(QString strRemoteFile,QString strLocalFile,int nTotalSize = 0,QDOWNLOAD_TYPE type = HTTP);
	QString GetLastErr();

private slots:
	//void OnHeadManagerFinished(QNetworkReply* reply);
	void OnHeadReplyFinished();

	void OnDownReplyProgress(qint64 bytesReceived, qint64 bytesTotal);
	void OnDownReplyRead();
	//void OnDownManagerFinished(QNetworkReply* reply);
	void OnDownReplyFinished();
	void OnDownReplyError(QNetworkReply::NetworkError err);
	void OnCancle(void);

	
};

#endif // QDOWNLOAD_H
