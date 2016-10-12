#ifndef DOWNHTTP_H
#define DOWNHTTP_H
#include <QString>
#include <QtCore/qglobal.h>
#include <QList>

class DownHTTP
{

public:
    DownHTTP();
    bool getFile(QString url, QString localFileName);
    bool unzip(QString fileName);
    bool moveFile(QString fileName);
    int load(QString url);
    int load(QString url, QString fileName);
    QString getFileNameAndSize(QString url);
    QString getLocalFileName();
    void setLocalFileName();
public:
    QString m_localFile;
	QList<QString> m_fileNameList;
	int m_fileSize;
	QString m_URLName;
	QString m_FileType;
	
};

#endif // DOWNHTTP_H
