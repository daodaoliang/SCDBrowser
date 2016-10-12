#include "downhttp.h"
#include <QDebug>
#include <QProcess>
#include <QDebug>
#include <QRegExp>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>
#include <QTextCodec>
#include <QDateTime>
#include "qdownload.h"
#pragma comment(lib,"QDownload.lib")

DownHTTP::DownHTTP()
{
	m_fileSize = 0;
}

QString getUTF8URLStrign(QString unicodeHanzi)
{
	QTextCodec * codecGB2312 = QTextCodec::codecForName("UTF-8");
	QByteArray byteArrayGB2312 = codecGB2312->fromUnicode(unicodeHanzi);
	//qDebug()<<byteArrayGB2312;
	QByteArray byteArrayPercentEncoded = byteArrayGB2312.toPercentEncoding(":/?@&=.|\\^*()[]{}<>-_+$%#!");
	return QString(byteArrayPercentEncoded);
}
#include <Windows.h>
#include <tchar.h>

bool DownHTTP::getFile(QString url, QString localFileName)
{			
	url.replace("scdbrower://","");

	QDownload down;
	QString strArgv1 = getUTF8URLStrign(url);
	QString strArgv2 = QCoreApplication::applicationDirPath() + "\\zip\\" + localFileName;
	qDebug()<<strArgv1<<"		"<<strArgv2<< "   "<<m_fileSize;
	int iDownload = down.download(strArgv1,strArgv2, m_fileSize);
	switch(iDownload)
	{
		case -1:
			QMessageBox::warning(NULL,"����","�ù�����δʵ��!");
			exit(0);
			break;
		case 0:
			QMessageBox::warning(NULL,"����",down.GetLastErr());
			exit(0);
			break;
		case 1:
			// QMessageBox::warning(NULL,"����","���سɹ�");
			break;
		case 2:
			QMessageBox::warning(NULL,"����","�û���ֹ");
			exit(0);
			break;
		case 3:
			QMessageBox::warning(NULL,"����","���سɹ��������ݰ�������");
			exit(0);
			break;
		default:
			QMessageBox::warning(NULL,"����","δ֪");
			exit(0);
	}
	return true;
}
bool DownHTTP::unzip(QString fileName)
{
	QDir dir(".\\zip\\" + m_URLName);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Size | QDir::Reversed);
	QString path = dir.currentPath();
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.count(); i++)
	{
		if (!dir.remove(list.value(i, QFileInfo()).fileName()))
		{
			QMessageBox::warning(NULL, "��ѹ��","��ǰ�������������У����Ժ����ԣ�");
			exit(0);
		}	
	}

    QString execmd = ".\\7z e .\\zip\\"+fileName +" -y -o.\\tmp\\" + m_URLName + " -aou";
    qDebug()<<execmd;
    if(QProcess::execute(execmd) ==0)
    {
        return true;
    }
    else
    {
		QMessageBox::warning(NULL, QObject::tr("��ѹ�ļ�"), QObject::tr("��ѹ�ļ�ʧ�ܣ�"));
        return false;
    }
}

int DownHTTP::load(QString url)
{
    return load(url,"");
}

int DownHTTP::load(QString url, QString fileName )
{
    if(fileName.isEmpty())
    {
        fileName = getFileNameAndSize(url);
		m_URLName = fileName;
    }
    if(!getFile(url,fileName))
    {
        return -1;
    }

	if(m_FileType == "zip" || m_FileType == "rar")
	{
		if (!unzip(fileName))
		{
			return -2;
		}
		setLocalFileName();
	}
	else if (m_FileType == "scd")
	{
		QDir dir;
		QString path = dir.currentPath();
		path += "/" + fileName;
		m_localFile = path;
	}
	else
	{
		// ToDo
	}

    return 0;
}

/** 
  * @brief    getFileName
  * 
  * ����URL��ѹ�����ĸ�ʽ������ѹ���ļ�
  * 
  * @class    DownHTTP
  * @param    QString url : 
  *     ʾ����	http://192.168.1.90:8089/com.roof.sfbm/archive.do?reqCode=downloadProcessFile&id=56b3ceecec5349a8930362a1a9561996&random=y7by9r6.rar&size=709473
  * @return   QString
  * @author   zhang-xch
  * @date     2014/11/19
  * @remarks  
  */ 
QString DownHTTP::getFileNameAndSize(QString url)
{
	int lastIndexOfPoint = url.lastIndexOf("."); //��λ��.rar��.��λ��
	int lastIndexOfAnd = url.length();
	QString suffix;
	if (url.contains("size="))
	{
		int lastIndexOfEtc = url.lastIndexOf("=");
		QString fileSize = url.right(url.length() - lastIndexOfEtc - 1);
		m_fileSize = fileSize.toInt();
		lastIndexOfAnd = url.lastIndexOf("&"); //�����size�Ļ���λ��&
	}
	suffix = url.mid(lastIndexOfPoint+1, lastIndexOfAnd - lastIndexOfPoint - 1);// ȡ�ú�׺
	m_FileType = suffix;
	QString zipFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh.mm.ss.zzz") + "." + suffix;
	return zipFileName.toAscii();
}

void DownHTTP::setLocalFileName()
{
    QDir dir(".\\tmp\\" + m_URLName);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
	QString path = dir.currentPath();

    QStringList list = dir.entryList();
	for (int i = 0; i < list.count(); i++)
	{
        QString fileName = list.at(i);
        QString f1 = QString(fileName.toStdString().data());
        QString f2 = QString(fileName.toLocal8Bit().data());

        qDebug() << "downLoad-----------fileName----------------- " << fileName << "f1          " << f1 <<  "f2          " << f2;
		m_fileNameList.append(path + "/tmp/" + m_URLName + "/"  + fileName);
        qDebug() << "downLoad-----------m_fileNameList[i];----------------- " << m_fileNameList[i];
	}

    /*QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.count(); i++)
    {
    QString fileName_1  = QString::fromLocal8Bit(list.at(i).fileName().toStdString().c_str());
    qDebug() << "downLoad-----------fileName_1----------------- " << fileName_1;
    QString fileName = list.at(i).fileName();
    qDebug() << "downLoad-----------fileName----------------- " << fileName;
    m_fileNameList.append(path + "/tmp/" + m_URLName + "/"  + fileName);
    qDebug() << "downLoad-----------m_fileNameList[i];----------------- " << m_fileNameList[i];
    }*/
}
