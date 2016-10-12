#include <QTextCodec>
#include <QtGui/QApplication>
#include "QTranslator.h"
#include "GraphicBrowserFrm.h"
#include <WinBase.h>
#include "downhttp.h"
#include "TrivalVersionCheck.h"
#include "qtsingleapplication/qtsingleapplication.h"
#include "logsrv.h"
#include "regmanager.h"

//#include <QDebug>
#include <tchar.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

void outputMessage(QtMsgType type, const char* msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    //QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1----%2-----%3").arg(text).arg(current_date).arg(msg);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

void InitTextCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("GB18030");  
    QTextCodec::setCodecForLocale(codec);   
    QTextCodec::setCodecForCStrings(codec);   
    QTextCodec::setCodecForTr(codec);
}

int GetHostId(CHAR *p)
{
    CHAR str[100];

    if (WinExec((LPCSTR)"cmd.exe /c dir c:\\*.* > c:\\temp.wz",SW_MINIMIZE) < 32)
        return 0;

    string szChar =""; 
    szChar = setlocale( LC_CTYPE, ("chs")); 
    FILE *f = fopen("c:\\temp.wz","r");
    if (f == NULL)   return 0;

    fgets(str,100,f);
    fgets(str,100,f);

    strcpy(p,str + strlen(str)-10);
    p[strlen(p) -1 ] = 0;
    fclose(f);

    f = fopen("c:\\temp.wz","w+");
    if(f == NULL)  return 0;
    fclose(f);
    return 1;
}        

int    CheckHostId(CHAR *hostid)
{

    FILE    *fp;
    CHAR    buffer[16];
    int        retval;
    int        key[2],kkk;

    WCHAR filename[MAX_PATH];

    WCHAR path[_MAX_PATH];
    GetModuleFileName( GetModuleHandle(L"UIBasic.dll"), path, _MAX_PATH );
    wstring sModulePath(path);
    
    wstring sIniPath(sModulePath.begin(),sModulePath.begin()+sModulePath.rfind('\\'));
    

    wsprintf( filename, L"%s\\Support\\hostid.sys", sIniPath.c_str());
    string szChar =""; 
    szChar = setlocale( LC_CTYPE, ("chs")); 
    fp = _wfopen(filename, L"rb") ;
    if(fp == NULL)  return 0;


    buffer[0] = '0' ;
    buffer[1] = 'x' ;

    buffer[2] = hostid[0] ;
    buffer[3] = hostid[5] ;
    buffer[4] = hostid[1] ;
    buffer[5] = hostid[6] ;

    buffer[6] = hostid[2] ;
    buffer[7] = hostid[7] ;
    buffer[8] = hostid[3] ;
    buffer[9] = hostid[8] ;

    buffer[10] = 0;
    sscanf(buffer,"%x",&kkk);

    if(kkk > 0xf9000000)  kkk = kkk - 0x10000000 ;
    kkk = kkk + 0x6135322;

    while(!feof(fp))
    {
        retval = fread(key,1,2*sizeof(int),fp);
        if(retval <= 0)  break;

        if(kkk != key[0])  continue; 

        fclose(fp);
        return 1;
    }
    fclose(fp);
    return  0;
}


/** 
  * @brief    CheckTriVerHostID
  * 
  * 检测计算的验证码与注册的是否一致
  * 
  * @param    int hostID : 计算的验证码
  * @return   int
  * @author   zhang-xch
  * @date    2014/7/28
  * @remarks  
  */ 
int CheckTriVerHostID(int hostID)
{
    FILE    *fp;
    int        retval;
    int        key[2];

    WCHAR filename[MAX_PATH];

    WCHAR path[_MAX_PATH];
    GetModuleFileName( GetModuleHandle(L"UIBasic.dll"), path, _MAX_PATH );
    wstring sModulePath(path);

    wstring sIniPath(sModulePath.begin(),sModulePath.begin()+sModulePath.rfind('\\'));

    wsprintf( filename, L"%s\\Support\\hostid.sys", sIniPath.c_str());
    string szChar =""; 
    szChar = setlocale( LC_CTYPE, ("chs")); 
    fp = _wfopen(filename, L"rb") ;
    if(fp == NULL)  return 0;

    while(!feof(fp))
    {
        retval = fread(key,1,2*sizeof(int),fp);
        if(retval <= 0)  break;

        if(hostID != key[0])  continue; 

        fclose(fp);
        return 1;
    }
    fclose(fp);
    return  0;

}

handle logsrv;

int main(int argc, char *argv[])
{
    TCHAR szFilename[MAX_PATH] = {0};
    DWORD lpcstr = GetModuleFileName( GetModuleHandle(_T("SCDBrowser.exe")),szFilename,MAX_PATH);
    PathRemoveFileSpec(szFilename);
    SetCurrentDirectory(szFilename);

    //EventLog 日志, 打开注释后即可正常输出;
    //logsrv = logsrv_acquire(_T("EventLog"), 1024000, 15);
    logsrv_scoped_output(logsrv, _T("进入main调用"), _T("退出main调用"), _T("main"));
    //注册MessageHandler, 打开注释后即可正常输出;
    qInstallMsgHandler(outputMessage);

    QApplication a(argc, argv);
    a.addLibraryPath("plugins");
    a.addLibraryPath("./plugins");
    a.addLibraryPath("../plugins");
    QString exePath = a.applicationFilePath();

    //将标准对话框设置成中文
    InitTextCodec();
    QTranslator translator;
    if(!translator.load("qt_zh_CN.qm"))
    {
        QMessageBox::warning(NULL,"装载中文失败","装载中文库",QMessageBox::Ok);
    }
    a.installTranslator(&translator);

//#ifndef GRAND_DOG 
//    CTrivalVersionCheck TrivalVersionCheck;
//    if (!TrivalVersionCheck.CheckDate())
//    {
//        int retVal = TrivalVersionCheck.CheckHostID();
//        if (1 != retVal)
//        {
//            if (!retVal)
//                MessageBox(NULL,L"请确信您有[SCD可视化工具]使用证书! \n ",L"",MB_OK);
//            else
//                MessageBox(NULL,L"您的[SCD可视化工具]使用证书非法! \n ",L"",MB_OK);
//            exit(0);
//        }
//        else
//        {
//            TrivalVersionCheck.GetLastHostID();
//            TrivalVersionCheck.AddDate();
//        }        
//    }
//
//#else
//    CHAR        hostid[32];
//    int retVal = GetHostId(hostid);    
//    retVal = CheckHostId(hostid);
//    if( retVal != 1)
//    {
//        if (!retVal)
//            MessageBox(NULL,L"请确信您有[SCD可视化工具]使用证书! \n ",L"",MB_OK);
//        else
//            MessageBox(NULL,L"您的[SCD可视化工具]使用证书非法! \n ",L"",MB_OK);
//        return FALSE;
//    }
//#endif

#ifndef WORK_STATION
    HWND hwnWindow = FindWindowA("QWidget", "{617278E1-A464-40A5-A87A-4AF62D5421F2}");
    if (hwnWindow)
    {
        qDebug() << "Find Window    Find!!!";
        QStringList cmd = a.arguments();
        QString line = cmd.join(" ");
        RegManager reg;
        reg.WriteRegValue(line);
        exit(0);
    }
#else
    HWND hwnWindow = FindWindowA("QWidget", "{3A8D23B6-F91F-4624-9829-0263E2BD74BD}");
    if (hwnWindow)
    {
        qDebug() << "Find Window    Find!!!";
        QStringList cmd = a.arguments();
        QString line = cmd.join(" ");
        RegManager reg;
        reg.WriteRegValue(line);
        exit(0);
    }
#endif
    qDebug() << "Find Window    Not Find!!!";
    GraphicBrowserFrm *frame = new GraphicBrowserFrm(NULL);
    QString temp = exePath.left(exePath.lastIndexOf("/"));
    frame->exePath = temp;
    frame->show();


    // 如果参数个数大于，表明通过CMD启动；
    if (argc > 1)
    {
		QString cmd = argv[1]; 
		if (cmd == "--import")
		{
			QString fileName = argv[2];
			frame->argc1 = fileName;
			frame->StartFromCMD();
		}
		else 
		{
			QString url = argv[1];
			for (int i = 2; i < argc; i++)
			{
				qDebug()<<argv[i]<<endl;
				url = url + " " + argv[i];
			}
			qDebug()<<url;

			DownHTTP b;
			int ans = b.load(url);
			if (ans == -1)
			{
				QMessageBox::warning(NULL,
					"文件下载",
					"文件下载失败！");
				exit(0);
			}else if (ans == -2)
			{
				exit(0);
			}

            InitTextCodec();
			if (b.m_fileNameList.count() == 1)
			{
				QString fileName = b.m_fileNameList.at(0);
				QString houzhui = fileName.right(4);
				if (houzhui.toLower().compare(".scd") == 0)
				{
					frame->argc1 = fileName;
					frame->StartFromCMD();
				}
			}
			else
			{
				for (int i= 0; i < b.m_fileNameList.count(); i++)
				{
					QString fileName = b.m_fileNameList.at(i);
					QString houzhui = fileName.right(4);
					if (houzhui.toLower().compare(".scd") == 0)
					{
						frame->fileNameList.append(fileName);
					}
				}
				frame->StartMoreFromCMD();
			}
		}
       
    }

    return a.exec();
}
