#pragma  once
/*************************************************
 【接口介绍】

  SCL提取 ： SAXSCLImport   
  ICD校验 ： SAXICDValidate 
  SCD校验 ： SAXSCDValidate 
  释放    ： SAXFree

  【注意事项】

   * SCL提取包括SCD、ICD、CID的提取；
   * 每个接口都要与释放函数配合使用；

*************************************************/
#ifndef RCSCLOPERATOR_H
#define RCSCLOPERATOR_H

#define EXLSIZE 400   // extra large
#define MAXSIZE 200   // 中文名称、描述等
#define MIDSIZE  96
#define MINSIZE  64 
#define TINSIZE  32

// 校验所依据的大项（标准规范的级别）
//#define RL_SCHEMA       L"Schema校验及语法校验"
//#define RL_61850        L"61850校验"
//#define RL_GDW          L"国家电网相关标准"
//#define RL_SD           L"山东相关标准"
//#define RL_OTHER        L"其他"

#define RL_SCHEMA			L"语法语义检查"
#define RL_PROPERTIES		L"属性规范性检查"
#define RL_UNIQUENESS		L"唯一性检查"
#define RL_MODELINTEGRITY	L"模型完整性校验"
#define RL_REFERENCE		L"引用有效性校验"
#define RL_STRUCT			L"结构规范性校验"

// 相关标准规范
//#define REF_61850_1     L"DL/T 860.1/IEC 61850-1:2003"
//#define REF_61850_2     L"DL/T 860.2/IEC 61850-2:2003"
//#define REF_61850_3     L"DL/T 860.3/IEC 61850-3:2002"
//#define REF_61850_4     L"DL/T 860.4/IEC 61850-4:2002"
//#define REF_61850_5     L"DL/T 860.5/IEC 61850-5:2003"
//#define REF_61850_6     L"DL/T 860.6/IEC 61850-6:2004"
//#define REF_61850_7_1   L"DL/T 860.71/IEC 61850-7-1:2003"
//#define REF_61850_7_2   L"DL/T 860.72/IEC 61850-7-2:2003"
//#define REF_61850_7_3   L"DL/T 860.73/IEC 61850-7-3:2003"
//#define REF_61850_7_4   L"DL/T 860.74/IEC 61850-7-4:2003"
//#define REF_61850_8_1   L"DL/T 860.81/IEC 61850-8-1:2004"
//#define REF_61850_9_1   L"DL/T 860.91/IEC 61850-9-1:2003"
//#define REF_61850_9_2   L"DL/T 860.92/IEC 61850-9-2:2004"
//#define REF_61850_10    L"DL/T 860.10/IEC 61850-10:2004"
//#define REF_396         L"Q/GDW 396-2009"
//#define REF_SD_YTH      L"智能变电站二次系统配置文件检测方案研讨会"
//#define REF_OTHER       L"其他"

#define REF_DEFAULT			L"DL/T 860.6"
#define REF_SCHEMA			L"SCL.xsd"

#define REF_DL6932			L"DL/T 860.6 第9.3.2节"
#define REF_DL6934			L"DL/T 860.6 第9.3.4节"
#define REF_DL6937			L"DL/T 860.6 第9.3.7节"
#define REF_DL6938			L"DL/T 860.6 第9.3.8节"
#define REF_DL69310			L"DL/T 860.6 第9.3.10节"
#define REF_DL69311			L"DL/T 860.6 第9.3.11节"
#define REF_DL6942			L"DL/T 860.6 第9.4.2节"
#define REF_DL6943			L"DL/T 860.6 第9.4.3节"
#define REF_DL745			L"DL/T 860.74 第5章"
#define REF_DL81B			L"DL/T 860.81 附录B"
#define REF_DL81C			L"DL/T 860.81 附录C"

#define REF_DL6938_11		L"DL/T 860.6 第9.3.8-9.3.11节"
#define REF_DL7214229		L"DL/T 860.72 第14.2.2.9节"
#define REF_DL721421		L"DL/T 860.72 第14.2.1节"
#define REF_DL7214227		L"DL/T 860.72 第14.2.2.7节"
#define REF_DL72142212		L"DL/T 860.72 第14.2.2.12节"
#define REF_DL9285			L"DL/T 860.92 第8.5节"
#define REF_DL72164			L"DL/T 860.72 第16.4节"

#define REF_Q396			L"Q/GDW 396-2009 第6.1.1节"
#define REF_DL6944_SMV945	L"DL/T 860.6 第9.4.4节（GSE 9.4.4节，SMV第9.4.5节）"
#define	REF_DL6994_SMV946	L"DL/T 860.6 第9.4.4节（GSE 9.4.4节，SMV第9.4.6节）"

// 2014.04.04修改
#define REF_DL6936			L"DL/T 860.6 第9.3.6节 "
#define REF_Q396_911E_1011G L"Q/GDW 396-2009 第9.1.1节 e)，第10.1.1节 g)"
#define REF_Q396_5A			L"Q/GDW 396-2009 第5节 a)"
#define REF_Q396_1011		L"Q/GDW 396-2009 第10.1.1节"
#define REF_Q396_613		L"Q/GDW 396-2009 第6.1.3节"
#define REF_Q396_1011		L"Q/GDW 396-2009 第10.1.1节"
#define REF_Q396_652		L"Q/GDW 396-2012 第6.5.2节 c)"
#define REF_PROEX_FACTOR    L"工程经验(部分厂家的设备不支持32个字符以上的内容)"

//---------------------------------------------------------
//                   提取返回的信息  
//---------------------------------------------------------


// 【输入\输出信号】
typedef struct SignalStruW            // Unicode
{
	int     iOrd;                     // 信号编号
	int     iOutSigOrd;               // 记录输出信号在DataSet中的序号，仅输出信号使用
	wchar_t wBriefName[MAXSIZE];      // 信号简称
	wchar_t wGroupName[MAXSIZE];      // 信号组合名称
	wchar_t wDataSet[MINSIZE];        // 数据集 (进输出信号有)
	wchar_t wLnPrefix[MINSIZE];       // ln前缀
	wchar_t wLnClass[MINSIZE];        // ln类型
	wchar_t wLnInst[MINSIZE];         // ln实例
	wchar_t wDoName[MINSIZE];         // Do名称
	wchar_t wDaName[MINSIZE];         // Da名称
	wchar_t wFc[MINSIZE];             // 功能
	wchar_t wPath[MIDSIZE];           // 信号路径
	wchar_t wSigType[MINSIZE];        // 信号类型 GSEControl、SampledValueControl
	wchar_t wStrpName[MINSIZE];       // 压板名称
	wchar_t wStrpDesc[MINSIZE];       // 压板描述
	bool    bIsStrp;                  // 是否为压板信号

	//// 记录信号对应的物理端口 2014.1.16 zhang-xch
	//wchar_t wIedName[MINSIZE];  
	//wchar_t wPhyPortIndex[TINSIZE];

	// 新增内容 2014-4.28  根据《SCD可视化概要设计说明书》进行更改
	 wchar_t sBDAType[MINSIZE]; // BDA数据类型 未知
	 wchar_t sPlugType[MINSIZE]; // 类型（单点、双点、时标） 未知

	// 在信号中添加Ln描述，DOI描述，dU值
	wchar_t sLnDesc[MAXSIZE];
	wchar_t sDOIDesc[MAXSIZE];
	wchar_t sdUVal[MAXSIZE];

	struct SignalStruW* pNext;
}SIGNALSTRUW, *PSIGNALSTRUW;

typedef struct SignalStruA            // Ansi
{
	int  iOrd;                        // 信号编号
	int  iOutSigOrd;               // 记录输出信号在DataSet中的序号，仅输出信号使用
	char chBriefName[MAXSIZE];        // 信号简称
	char chGroupName[MAXSIZE];        // 信号组合名称
	char chDataSet[MINSIZE];          // 数据集 (进输出信号有)
	char chLnPrefix[MINSIZE];         // ln前缀
	char chLnClass[MINSIZE];          // ln类型
	char chLnInst[MINSIZE];           // ln实例
	char chDoName[MINSIZE];           // Do名称
	char chDaName[MINSIZE];           // Da名称
	char chFc[MINSIZE];               // 功能
	char chPath[MIDSIZE];             // 信号路径
	char chSigType[MINSIZE];          // 信号类型 GSEControl、SampledValueControl
	char chStrpName[MINSIZE];         // 压板名称
	char chStrpDesc[MINSIZE];         // 压板描述
	bool bIsStrp;                     // 是否为压板信号

	//// 记录信号对应的物理端口 2014.1.16 zhang-xch
	// char wIedName[MINSIZE];
	// char wPhyPortIndex[TINSIZE];

	// 新增内容 2014-4.28  根据《SCD可视化概要设计说明书》进行更改
	char sBDAType[MINSIZE]; // BDA数据类型 未知
	char sPlugType[MINSIZE]; // 类型（单点、双点、时标） 未知

	// 在信号中添加Ln描述，DOI描述，dU值
	char sLnDesc[MAXSIZE];
	char sDOIDesc[MAXSIZE];
	char sdUVal[MAXSIZE];

	struct SignalStruA* pNext;
}SIGNALSTRUA, *PSIGNALSTRUA;

// 【控制块】
typedef struct CBStruW                // Unicode
{
	wchar_t wName[MINSIZE];           // 控制块名称
	wchar_t wType[MINSIZE];           // 控制块类型 GSEControl、SampledValueControl
	wchar_t wDataSet[MINSIZE];        // 数据集

	wchar_t wAppID[MINSIZE]; // 可视化时使用 2014.4.16 zhang-xch

	// 新增内容 2014-4.28  根据《SCD可视化概要设计说明书》进行更改
	wchar_t sConfRev[MINSIZE]; // 控制块配置版本号 SCL\IED\AccessPoint\Server\LDevice\Ln0\GSEControl(SimpleValueControl)\confRev
	int sASDU;    // ASDU数目  SCL\IED\AccessPoint\Server\LDevice\Ln0\DataSet\FCDA 
	wchar_t sSmpRate[MINSIZE]; // 采样率（SV）     SCL\IED\AccessPoint\Server\LDevice\Ln0\SimpleValueControl\smpRate

	wchar_t sDataSetAddr[MINSIZE]; // 数据集地址 未知
	wchar_t sGocbRef[MINSIZE]; // gocbRef（GOOSE） 未知

	struct CBStruW* pNext;
}CBSTRUW, *PCBSTRUW;

typedef struct CBStruA                // Ansi
{
	char chName[MINSIZE];              // 控制块名称
	char chType[MINSIZE];              // 控制块类型 GSEControl、SampledValueControl
	char chDataSet[MINSIZE];           // 数据集

	char wAppID[MINSIZE]; // 可视化时使用 2014.4.16 zhang-xch

	// 新增内容 2014-4.28  根据《SCD可视化概要设计说明书》进行更改
	char sConfRev[MINSIZE]; // 控制块配置版本号 SCL\IED\AccessPoint\Server\LDevice\Ln0\GSEControl(SimpleValueControl)\confRev
	int sASDU;    // ASDU数目  SCL\IED\AccessPoint\Server\LDevice\Ln0\DataSet\FCDA 
	char sSmpRate[MINSIZE]; // 采样率（SV）     SCL\IED\AccessPoint\Server\LDevice\Ln0\SimpleValueControl\smpRate

	char sDataSetAddr[MINSIZE]; // 数据集地址 未知
	char sGocbRef[MINSIZE]; // gocbRef（GOOSE） 未知

	struct CBStruA* pNext;
}CBSTRUA, *PCBSTRUA;

// 【LD】
typedef struct LDStruW                // Unicode
{
	wchar_t wInst[TINSIZE];           // LD inst

	int          iCBSize;             // CB的数量
	PCBSTRUW     pCBStru;             // CB
	int          iInsigSize;          // 输入信号的数量
	PSIGNALSTRUW pInsigStru;          // 输入信号
	int          iOutsigSize;         // 输出信号的数量
	PSIGNALSTRUW pOutsigStru;         // 输出信号

	struct LDStruW* pNext;
}LDSTRUW, *PLDSTRUW;

typedef struct LDStruA                // Ansi
{
	char chInst[TINSIZE];              // LD inst

	int          iCBSize;             // CB的数量
	PCBSTRUA     pCBStru;             // CB
	int          iInsigSize;          // 输入信号的数量
	PSIGNALSTRUA pInsigStru;          // 输入信号
	int          iOutsigSize;         // 输出信号的数量
	PSIGNALSTRUA pOutsigStru;         // 输出信号

	struct LDStruA* pNext;
}LDSTRUA, *PLDSTRUA;

// 【物理端口】  2014.09.03 按照国网标准添加物理端口
typedef struct PhyPortStruW                // Unicode
{
	wchar_t PhysConnType[TINSIZE];
	wchar_t Cable[TINSIZE];
	wchar_t Plug[TINSIZE];
	wchar_t Port[TINSIZE];
	wchar_t Type[TINSIZE];        

	struct PhyPortStruW* pNext;
}PHYPORTSTRUW, *PPHYPORTSTRUW;

typedef struct PhyPortStruA                // Ansi
{
	char PhysConnType[TINSIZE];
	char Cable[TINSIZE];
	char Plug[TINSIZE];
	char Port[TINSIZE];
	char Type[TINSIZE];

	struct PhyPortStruA* pNext;
}PHYPORTSTRUA, *PPHYPORTSTRUA;


// 【AP】
typedef struct APStruW                // Unicode
{ 
	wchar_t wName[TINSIZE];

	int      iLDSize;                 // LD的数量
	PLDSTRUW pLDStru;                 // LD

	// 【物理端口】  2014.09.03 按照国网标准添加物理端口
	PPHYPORTSTRUW pPhyPort;

	struct APStruW* pNext;
}APSTRUW, *PAPSTRUW;

typedef struct APStruA                // Ansi
{
	char chName[TINSIZE];          

	int      iLDSize;                 // LD的数量
	PLDSTRUA pLDStru;                 // LD

	// 【物理端口】  2014.09.03 按照国网标准添加物理端口
	PPHYPORTSTRUA pPhyPort;

	struct APStruA* pNext;
}APSTRUA, *PAPSTRUA;

// 【ICD 信息】
typedef struct ICDStruW               // Unicode
{
	wchar_t wName[MINSIZE];           // 名称
	wchar_t wDesc[MAXSIZE];           // 描述
	wchar_t wType[MINSIZE];           // 设备型号 （type属性第一个@前的部分）
	wchar_t wManu[MINSIZE];           // 生产厂家
	wchar_t wfunVer[MINSIZE];         // 功能版本 （type属性第一个@后的部分）
	wchar_t wConfigVer[MINSIZE];      // ICD版本

	wchar_t chCRC[TINSIZE];				// 增加CRC校验码	2014.07.07

	int      iAPSize;                 // AP的数量
	PAPSTRUW pAPStru;                 // AP

	struct ICDStruW* pNext;

}ICDSTRUW, *PICDSTRUW;

typedef struct ICDStruA               // Ansi
{
	char chName[MINSIZE];              // 名称
	char chDesc[MAXSIZE];              // 描述
	char chType[MINSIZE];              // 设备型号 （type属性第一个@前的部分）
	char chManu[MINSIZE];              // 生产厂家
	char chfunVer[MINSIZE];            // 功能版本 （type属性第一个@后的部分）
	char chConfigVer[MINSIZE];         // ICD版本

	char chCRC[TINSIZE];				// 增加CRC校验码	2014.07.07

	int      iAPSize;                 // AP的数量
	PAPSTRUA pAPStru;                 // AP

	struct ICDStruA* pNext;

}ICDSTRUA, *PICDSTRUA;

// 【回路信息】
typedef struct InputsStruW            // Unicode
{
	int iInputsOrd; // 记录回路在Inputs中的序号
	wchar_t wINIedName[MINSIZE];
	wchar_t wINIedDesc[MAXSIZE];
	wchar_t wINSigDesc[MAXSIZE];      // 信号描述
	wchar_t wINLdInst[MINSIZE];
	wchar_t wINPrefix[MINSIZE];
	wchar_t wINLnClass[MINSIZE];
	wchar_t wINLnInst[MINSIZE];
	wchar_t wINDoName[MINSIZE];
	wchar_t wINDaName[MINSIZE];
	wchar_t wINPhyPort[MINSIZE]; // 2014.09.03根据国网标准添加物理端口

	wchar_t wOUTIedName[MINSIZE];
	wchar_t wOUTIedDesc[MAXSIZE];
	wchar_t wOUTSigDesc[MAXSIZE];
	wchar_t wOUTLdInst[MINSIZE];
	wchar_t wOUTPrefix[MINSIZE];
	wchar_t wOUTLnClass[MINSIZE];
	wchar_t wOUTLnInst[MINSIZE];
	wchar_t wOUTDoName[MINSIZE];
	wchar_t wOUTDaName[MINSIZE];

	struct InputsStruW* pNext;
}INPUTSSTRUW, *PINPUTSSTRUW;

typedef struct InputsStruA            // Ansi
{
	int iInputsOrd; // 记录回路在Inputs中的序号
	char chINIedName[MINSIZE];
	char chINIedDesc[MAXSIZE];
	char chINSigDesc[MAXSIZE];
	char chINLdInst[MINSIZE];
	char chINPrefix[MINSIZE];
	char chINLnClass[MINSIZE];
	char chINLnInst[MINSIZE];
	char chINDoName[MINSIZE];
	char chINDaName[MINSIZE];
	char chINPhyPort[MINSIZE]; // 2014.09.03根据国网标准添加物理端口

	char chOUTIedName[MINSIZE];
	char chOUTIedDesc[MAXSIZE];
	char chOUTSigDesc[MAXSIZE];
	char chOUTLdInst[MINSIZE];
	char chOUTPrefix[MINSIZE];
	char chOUTLnClass[MINSIZE];
	char chOUTLnInst[MINSIZE];
	char chOUTDoName[MINSIZE];
	char chOUTDaName[MINSIZE];

	struct InputsStruA* pNext;
}INPUTSSTRUA, *PINPUTSSTRUA;

// 【IP地址】
typedef struct IPStruW                // Unicode
{
	wchar_t wIedName[TINSIZE];
	wchar_t wIedDesc[MAXSIZE];
	wchar_t wApName[TINSIZE];
	wchar_t wIpAddr[TINSIZE];         // IP地址
	wchar_t wIpMask[TINSIZE];         // 掩码

	struct IPStruW* pNext;
}IPSTRUW, *PIPSTRUW;

typedef struct IPStruA                // Ansi
{
	char chIedName[TINSIZE];
	char chIedDesc[MAXSIZE];
	char chApName[TINSIZE];
	char chIpAddr[TINSIZE];            // IP地址
	char chIpMask[TINSIZE];            // 掩码

	struct IPStruA* pNext;
}IPSTRUA, *PIPSTRUA;

// 【MAC地址】
typedef struct MacStruW               // Unicode
{
	wchar_t wIedName[TINSIZE];
	wchar_t wIedDesc[MAXSIZE];
	wchar_t wApName[TINSIZE];
	wchar_t wLdInst[TINSIZE];
	wchar_t wCBName[TINSIZE];
	wchar_t wMacAddr[MINSIZE];
	wchar_t wAPPID[TINSIZE];
	wchar_t wVLANID[TINSIZE];

	wchar_t wVlanPriority[TINSIZE]; // 可视化时使用 2014.4.16 zhang-xch

	// 新增内容 2014-4.28  根据《SCD可视化概要设计说明书》进行更改
	wchar_t sMaxTime[MINSIZE]; // MaxTime（GOOSE） SCL\communication\subNetWork\ConnectAP\address\GSE\MaxTime\Text
	wchar_t sMinTime[MINSIZE]; // MinTime（GOOSE） SCL\communication\subNetWork\ConnectAP\address\GSE\MinTime\Text

	struct MacStruW* pNext;
}MACSTRUW, *PMACSTRUW;

typedef struct MacStruA               // Ansi
{
	char chIedName[TINSIZE];
	char chIedDesc[MAXSIZE];
	char chApName[TINSIZE];
	char chLdInst[TINSIZE];
	char chCBName[TINSIZE];
	char chMacAddr[MINSIZE];
	char chAPPID[TINSIZE];
	char chVLANID[TINSIZE];

	char wVlanPriority[TINSIZE]; // 可视化时使用 2014.4.16 zhang-xch

	// 新增内容 2014-4.28  根据《SCD可视化概要设计说明书》进行更改
	char sMaxTime[MINSIZE]; // MaxTime（GOOSE） SCL\communication\subNetWork\ConnectAP\address\GSE\MaxTime\Text
	char sMinTime[MINSIZE]; // MinTime（GOOSE） SCL\communication\subNetWork\ConnectAP\address\GSE\MinTime\Text

	struct MacStruA* pNext;
}MACSTRUA, *PMACSTRUA;

// 【子网】
typedef struct SubNetStruW            // Unicode
{
	wchar_t wName[MINSIZE];
	wchar_t wDesc[MAXSIZE];
	wchar_t wType[TINSIZE];

	int       iIPSize;                // IP地址的数量
	PIPSTRUW  pIpStru;
	int       iMacSize;               // MAC地址的数量
	PMACSTRUW pMacstru;

	struct SubNetStruW* pNext;
}SUBNETSTRUW, *PSUBNETSTRUW;

typedef struct SubNetStruA            // Ansi
{
	char chName[MINSIZE];
	char chDesc[MAXSIZE]; 
	char chType[TINSIZE];

	int       iIPSize;                // IP地址的数量
	PIPSTRUA  pIpStru;
	int       iMacSize;               // MAC地址的数量
	PMACSTRUA pMacstru;

	struct SubNetStruA* pNext;
}SUBNETSTRUA, *PSUBNETSTRUA;

// 【历史记录项】
typedef struct HitemStruW             // Unicode         
{
	wchar_t wVersion[MINSIZE];
	wchar_t wRevision[MINSIZE];
	wchar_t wWhen[MINSIZE];
	wchar_t wWho[TINSIZE];
	wchar_t wWhat[MAXSIZE];
	wchar_t wWhy[MAXSIZE];

	struct HitemStruW* pNext;
}HITEMSTRUW, *PHITEMSTRUW;

typedef struct HitemStruA             // Ansi
{
	char chVersion[MINSIZE];
	char chRevision[MINSIZE];
	char chWhen[MINSIZE];
	char chWho[TINSIZE];
	char chWhat[MAXSIZE];
	char chWhy[MAXSIZE];

	struct HitemStruA* pNext;
}HITEMSTRUA, *PHITEMSTRUA;

// 【SCD 信息】
typedef struct SCDStruW               // Unicode
{
	wchar_t wName[MAXSIZE];
	wchar_t wVersion[TINSIZE];
	wchar_t wRevision[TINSIZE];
	wchar_t chCRC[TINSIZE];				// 增加CRC校验码	2014.07.07

	wchar_t chToolID[MINSIZE];				// 增加ToolID	2014.09.11

	int          iHitemSize;          // 历史记录数
	PHITEMSTRUW  pHitemStru;          // 历史记录
	int          iSubNetSize;         // 网络的数量
	PSUBNETSTRUW pSubNetStru;         // 网络信息
	int          iICDSize;            // 装置的数量
	PICDSTRUW    pICDStru;            // IED及虚端子信息
	int          iInputsSize;         // 回路数
	PINPUTSSTRUW pInputs;             // 回路信息

}SCDSTRUW, *PSCDSTRUW;

typedef struct SCDStruA               // Ansi
{
	char chName[MAXSIZE];
	char chVersion[TINSIZE];
	char chRevision[TINSIZE];
	char chCRC[TINSIZE];				// 增加CRC校验码	2014.07.07

	char chToolID[MINSIZE];				// 增加ToolID	2014.09.11

	int          iHitemSize;          // 历史记录数
	PHITEMSTRUA  pHitemStru;          // 历史记录
	int          iSubNetSize;         // 网络的数量
	PSUBNETSTRUA pSubNetStru;         // 网络信息
	int          iICDSize;            // 装置的数量
	PICDSTRUA    pICDStru;            // IED及虚端子信息
	int          iInputsSize;         // 回路数
	PINPUTSSTRUA pInputs;             // 回路信息

}SCDSTRUA, *PSCDSTRUA;

//---------------------------------------------------------
//                   校验返回的信息  
//---------------------------------------------------------

// 【错误、警告信息】
typedef struct ValidInfoW 
{
	wchar_t wName[MAXSIZE] ;
	wchar_t wValue[EXLSIZE] ;
	wchar_t wLevel[MAXSIZE];
	wchar_t wRef[MAXSIZE];

	struct ValidInfoW* pNext;
}VALIDINFOSTRUW, *PVALIDINFOSTRUW;
typedef struct ValidInfoA
{
	char chName[MINSIZE];
	char chValue[EXLSIZE];
	char chLevel[MAXSIZE];
	char chRef[MAXSIZE];

	struct ValidInfoA* pNext;
}VALIDINFOSTRUA, *PVALIDINFOSTRUA;
// ICD校验返回
typedef struct ICDValidInfoStruW
{
	wchar_t wName[MAXSIZE];           // 名称
	wchar_t wDesc[MAXSIZE];           // 描述
	wchar_t wType[MINSIZE];           // 设备型号 （type属性第一个@前的部分）
	wchar_t wManu[MINSIZE];           // 生产厂家
	wchar_t wFunVer[MINSIZE];         // 功能版本 （type属性第一个@后的部分）
	wchar_t wConfigVer[MINSIZE];      // ICD版本
	// 2014.9.10 读取文件本身的CRC码
	wchar_t wCRC[TINSIZE];

	int     iInsigNum;
	int     iOutsigNum;
	int     iErroSize;
	int     iWarnSize;
	PVALIDINFOSTRUW pErro;
	PVALIDINFOSTRUW pWarn;
}ICDVALIDINFOSTRUW, *PICDVALIDINFOSTRUW;
typedef struct ICDValidInfoStruA
{
	char chName[MAXSIZE];
	char chDesc[MAXSIZE];
	char chType[MINSIZE];
	char chManu[MINSIZE];
	char chFunVer[MINSIZE];
	char chConfigVer[MINSIZE];
	// 2014.9.10 读取文件本身的CRC码
	char chCRC[TINSIZE];

	int  iInsigNum;
	int  iOutsigNum;
	int  iErroSize;
	int  iWarnSize;
	PVALIDINFOSTRUA pErro;
	PVALIDINFOSTRUA pWarn;
}ICDVALIDINFOSTRUA, *PICDVALIDINFOSTRUA;
// SCD 校验返回值
typedef struct SCDValidInfoStruW
{
	wchar_t wName[MAXSIZE];
	wchar_t wVersion[MINSIZE];
	wchar_t wRevision[MINSIZE];

	int iErroSize;
	int iWarnSize;

	int RL_SCHEMA_ErrorCount; 
	int RL_PROPERTIES_ErrorCount; 
	int	RL_UNIQUENESS_ErrorCount;
	int	RL_MODELINTEGRITY_ErrorCount; 
	int	RL_REFERENCE_ErrorCount; 
	int	RL_STRUCT_ErrorCount;

	int RL_SCHEMA_WarnCount; 
	int RL_PROPERTIES_WarnCount; 
	int	RL_UNIQUENESS_WarnCount;
	int	RL_MODELINTEGRITY_WarnCount; 
	int	RL_REFERENCE_WarnCount; 
	int	RL_STRUCT_WarnCount;

	PVALIDINFOSTRUW pErro;
	PVALIDINFOSTRUW pWarn;
}SCDVALIDINFOSTRUW, *PSCDVALIDINFOSTRUW;
typedef struct SCDValidInfoStruA
{
	char chName[MAXSIZE];
	char chVersion[MINSIZE];
	char chRevision[MINSIZE];

	int iErroSize;
	int iWarnSize;

	int RL_SCHEMA_ErrorCount; 
	int RL_PROPERTIES_ErrorCount; 
	int	RL_UNIQUENESS_ErrorCount;
	int	RL_MODELINTEGRITY_ErrorCount; 
	int	RL_REFERENCE_ErrorCount; 
	int	RL_STRUCT_ErrorCount;

	int RL_SCHEMA_WarnCount; 
	int RL_PROPERTIES_WarnCount; 
	int	RL_UNIQUENESS_WarnCount;
	int	RL_MODELINTEGRITY_WarnCount; 
	int	RL_REFERENCE_WarnCount; 
	int	RL_STRUCT_WarnCount;

	PVALIDINFOSTRUA pErro;
	PVALIDINFOSTRUA pWarn;
}SCDVALIDINFOSTRUA, *PSCDVALIDINFOSTRUA;

// Schema 校验返回值
typedef struct SChemaValidInfoStruW
{
	int iErroSize;
	PVALIDINFOSTRUW pErro;
}SCHEMAVALIDINFOSTRUW, *PSCHEMAVALIDINFOSTRUW;
typedef struct SchemaValidInfoStruA
{
	int iErroSize;
	PVALIDINFOSTRUA pErro;
}SCHEMAVALIDINFOSTRUA, *PSCHEMAVALIDINFOSTRUA;


#ifdef _UNICODE
#define SAXSCLImport   SAXSCLImportW
#define SAXICDValidate SAXICDValidateW
#define SAXSCDValidate SAXSCDValidateW
#define SAXXMLValidate SAXXMLValidateW
#define SAXFree        SAXFreeW
#else
#define SAXSCLImport   SAXSCLImportA
#define SAXICDValidate SAXICDValidateA
#define SAXSCDValidate SAXSCDValidateA
#define SAXFree        SAXFreeA
#endif

//---------------------------||  Unicode  ||---------------------------------

// SCL 提取接口(同时支持ICD、CID、SCD的提取) / 释放
bool SAXSCLImportW(const wchar_t* sPath, PSCDSTRUW* ppSCDStru);
bool SAXSCLImportW(const wchar_t* sPath, const wchar_t* sIEDName, PSCDSTRUW* ppSCDStru);
bool SAXFreeW(PSCDSTRUW pSCDStru);

// SCD校验 / 释放
bool SAXICDValidateW(const wchar_t* sPath, PICDVALIDINFOSTRUW* ppIcdValid);
bool SAXICDValidateToFileW(const wchar_t* sPath, PICDVALIDINFOSTRUW* ppIcdValid);
bool SAXFreeW(PICDVALIDINFOSTRUW pICDValid);

 // SCD校验 / 释放 
bool SAXSCDValidateW(const wchar_t* sPath, PSCDVALIDINFOSTRUW* ppScdValid);
bool SAXSCDValidateToFileW(const wchar_t* sPath, PSCDVALIDINFOSTRUW* ppScdValid);
bool SAXSCDValidateToFileA(const char* sPath, PSCDVALIDINFOSTRUA* ppScdValid);
bool SAXFreeW(PSCDVALIDINFOSTRUW pSCDValid);

//---------------------------||    Ansi   ||----------------------------------

// SCL 提取接口(同时支持ICD、CID、SCD的提取) / 释放
bool SAXSCLImportA(const char* sPath, PSCDSTRUA* ppSCDStru);
bool SAXFreeA(PSCDSTRUA pSCDStru);

// ICD 校验 / 释放 
bool SAXICDValidateA(const char* sPath, PICDVALIDINFOSTRUA* ppIcdValid);
bool SAXFreeA(PICDVALIDINFOSTRUA pICDValid);

// SCD校验 / 释放 
bool SAXSCDValidateA(const char* sPath, PSCDVALIDINFOSTRUA* ppScdValid);
bool SAXFreeA(PSCDVALIDINFOSTRUA pSCDValid);

// Schema 校验
bool SAXXMLValidateW(const wchar_t* sXMLPath, const wchar_t* sXSDPath, PSCHEMAVALIDINFOSTRUW *pXMLValid);
bool SAXFreeW(PSCHEMAVALIDINFOSTRUW pXMLValid);
// Schema校验
bool SAXXMLValidateA(const char* sXMLPath, const char* sXSDPath, PSCHEMAVALIDINFOSTRUA *pXMLValid);
bool SAXFreeA(PSCHEMAVALIDINFOSTRUA pXMLValid);

#endif