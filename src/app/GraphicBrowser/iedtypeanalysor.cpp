#include "iedtypeanalysor.h"
#include <QDebug>
#include <QFile>

IEDTypeAnalysor::IEDTypeAnalysor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);
	setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	setWindowTitle("装置类型分析器配置");
	//connect(ui.pushButton,SIGNAL(on))
	filename="TypeAnalysor.txt";
	prefixP="";
	prefixC="";
	prefixM="";
	prefixI="";
	prefixBC_1="";
	prefixBC_2="";
	prefixHZ_1="";
	prefixHZ_2="";

}

IEDTypeAnalysor::~IEDTypeAnalysor()
{

}
IEDType IEDTypeAnalysor::parseIEDType(QString name,QString desc)
{
	QFile file(filename);
	if(!file.exists())
	{
		setInitalVal();
	}else 
	{  
		file.open(QIODevice::ReadOnly);
		QDataStream stream(&file);   
		stream >>prefixP;
		stream >>prefixC;
		stream >>prefixM;
		stream >>prefixI;
		stream >>prefixBC_1;
		stream >>prefixBC_2;
		stream >>prefixHZ_1;
		stream >>prefixHZ_2;

		stream >>prefixPKey_1;
		stream >>prefixPKey_2;
		stream >>prefixCKey_1;
		stream >>prefixCKey_2;
		stream >>prefixMKey_1;
		stream >>prefixMKey_2;
		stream >>prefixIKey_1;
		stream >>prefixIKey_2;
		stream >>prefixBCKey_1;
		stream >>prefixBCKey_2;
		stream >>prefixHZKey_1;
		stream >>prefixHZKey_2;

		file.close();
	}
	if(desc.isEmpty())
	{ 
		if(name.startsWith(prefixP))
			return P;
		else if(name.startsWith(prefixC))
			return C;
		else if(name.startsWith(prefixM))
			return MU;
		else if(name.startsWith(prefixI))
			return ST;
		return NO;
	}
	else
	{
		if(desc.contains(prefixPKey_1)&&!desc.contains(prefixIKey_2)&&name.startsWith(prefixP))
			return P;
		else if(desc.contains(prefixCKey_1)&&!desc.contains(prefixCKey_2)&&name.startsWith(prefixC))
			return C;
		else if(desc.contains(prefixBCKey_1)&&desc.contains(prefixBCKey_2)&&(name.startsWith(prefixBC_1)||name.startsWith(prefixBC_2)))
			return PC;
		else if(desc.contains(prefixMKey_1)&&!desc.contains(prefixMKey_2)&&name.startsWith(prefixM))
			return MU;
		else if(desc.contains(prefixIKey_1)&&!desc.contains(prefixIKey_2)&&name.startsWith(prefixI))
			return ST;
		else if(desc.contains(prefixHZKey_1)&&desc.contains(prefixHZKey_2)&&(name.startsWith(prefixHZ_1)||name.startsWith(prefixHZ_2)))
			return MS; 
		return NO;
	}
}
void IEDTypeAnalysor::setParsePrefix()
{
	QFile file(filename);
	if(!file.exists())
	{
		setInitalVal();
		setLineVal();
	}else 
	{
		file.open(QIODevice::ReadOnly);
		QDataStream stream(&file);
		stream >>prefixP;
		stream >>prefixC;
		stream >>prefixM;
		stream >>prefixI;
		stream >>prefixBC_1;
		stream >>prefixBC_2;
		stream >>prefixHZ_1;
		stream >>prefixHZ_2;

		stream >>prefixPKey_1;
		stream >>prefixPKey_2;
		stream >>prefixCKey_1;
		stream >>prefixCKey_2;
		stream >>prefixMKey_1;
		stream >>prefixMKey_2;
		stream >>prefixIKey_1;
		stream >>prefixIKey_2;
		stream >>prefixBCKey_1;
		stream >>prefixBCKey_2;
		stream >>prefixHZKey_1;
		stream >>prefixHZKey_2;
		setLineVal();
		file.close();
	}
	show();
}
void IEDTypeAnalysor::setInitalVal()
{
	prefixP="P";
	prefixC="C";
	prefixM="M";
	prefixI="I";
	prefixBC_1="P";
	prefixBC_2="C";
	prefixHZ_1="M";
	prefixHZ_2="I";

	prefixPKey_1="保";
	prefixPKey_2="测";
	prefixCKey_1="测";
	prefixCKey_2="保";
	prefixMKey_1="合";
	prefixMKey_2="智";
	prefixIKey_1="智";
	prefixIKey_2="合";
	prefixBCKey_1="保";
	prefixBCKey_2="测";
	prefixHZKey_1="智";
	prefixHZKey_2="合";

}
void IEDTypeAnalysor::setLineVal()
{
	ui.line_P->setText(prefixP);
	ui.line_C->setText(prefixC);
	ui.line_MU->setText(prefixM);
	ui.line_ST->setText(prefixI);
	ui.line_PC->setText(prefixBC_1);
	ui.line_PC2->setText(prefixBC_2);
	ui.line_MS->setText(prefixHZ_1);
	ui.line_MS2->setText(prefixHZ_2);

	ui.line_PKey->setText(prefixPKey_1);
	ui.line_PKey2->setText(prefixPKey_2);
	ui.line_CKey->setText(prefixCKey_1);
	ui.line_CKey2->setText(prefixCKey_2);
	ui.line_MUKey->setText(prefixMKey_1);
	ui.line_MUKey2->setText(prefixMKey_2);
	ui.line_STKey->setText(prefixIKey_1);
	ui.line_STKey2->setText(prefixIKey_2);
	ui.line_PCKey->setText(prefixBCKey_1);
	ui.line_PCKey2->setText(prefixBCKey_2);
	ui.line_MSKey->setText(prefixHZKey_1);
	ui.line_MSKey2->setText(prefixHZKey_2);

}
void  IEDTypeAnalysor::on_pushButton_clicked()
{
	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QDataStream stream(&file);    

/*	QString a=prefixBCKey_1+"或"+prefixBCKey_2;
	ui.line_PCKey->setText(a);
	//ui.line_PCKey2->setText();
	QString b=	prefixHZKey_1 +"或"+prefixHZKey_2;
	ui.line_MSKey->setText(b);*/

	stream <<ui.line_P->text();
	stream <<ui.line_C->text();     
	stream <<ui.line_MU->text();
	stream <<ui.line_ST->text();
	stream <<ui.line_PC->text();
	stream <<ui.line_PC2->text();
	stream <<ui.line_MS->text();
	stream <<ui.line_MS2->text();

	stream <<ui.line_PKey->text();
	stream <<ui.line_PKey2->text();
	stream <<ui.line_CKey->text();
	stream <<ui.line_CKey2->text();
	stream <<ui.line_MUKey->text();
	stream <<ui.line_MUKey2->text();
	stream <<ui.line_STKey->text();
	stream <<ui.line_STKey2->text();

    stream <<ui.line_PCKey->text();
	stream <<ui.line_PCKey2->text();
	stream <<ui.line_MSKey->text();
	stream <<ui.line_MSKey2->text();
	file.close();
	hide();

}
void  IEDTypeAnalysor::on_pushButton_2_clicked()
{
	  hide();
}