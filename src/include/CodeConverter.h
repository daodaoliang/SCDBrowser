#ifndef Code_converter_H
#define Code_converter_H

#include <string>
#include <qstring>
#include <QTextCodec>

using namespace std;

class CodeConverter
{
public:
	QString FromGBKToUTF8(QString strGBK)//��GBK����ת����UTF-8����
	{
		QTextCodec *utf8 = QTextCodec::codecForName("utf-8");
        return utf8->toUnicode(strGBK.toUtf8());	
	};
	
	string FromGBKToUTF8(string strGBK)//��GBK����ת����UTF-8����
	{
		QTextCodec *utf8 = QTextCodec::codecForName("utf-8");
        return utf8->toUnicode(QString(strGBK.c_str()).toUtf8()).toLocal8Bit().constData();				
	};
	
	QString FromUTF8ToGBK(QString strUtf8)//��UTF-8����ת����GBK����
	{	
		QTextCodec *gbk = QTextCodec::codecForName("gbk"); 
        return gbk->toUnicode(strUtf8.toLocal8Bit());
    };
  
	string FromUTF8ToGBK(string strUtf8)//��UTF-8����ת����GBK����
	{	
		QTextCodec *gbk = QTextCodec::codecForName("gbk"); 
        return gbk->toUnicode(QString(strUtf8.c_str()).toLocal8Bit()).toLocal8Bit().constData();
    }; 
};

#endif