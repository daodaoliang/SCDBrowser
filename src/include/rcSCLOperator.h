#pragma  once
/*************************************************
 ���ӿڽ��ܡ�

  SCL��ȡ �� SAXSCLImport   
  ICDУ�� �� SAXICDValidate 
  SCDУ�� �� SAXSCDValidate 
  �ͷ�    �� SAXFree

  ��ע�����

   * SCL��ȡ����SCD��ICD��CID����ȡ��
   * ÿ���ӿڶ�Ҫ���ͷź������ʹ�ã�

*************************************************/
#ifndef RCSCLOPERATOR_H
#define RCSCLOPERATOR_H

#define EXLSIZE 400   // extra large
#define MAXSIZE 200   // �������ơ�������
#define MIDSIZE  96
#define MINSIZE  64 
#define TINSIZE  32

// У�������ݵĴ����׼�淶�ļ���
//#define RL_SCHEMA       L"SchemaУ�鼰�﷨У��"
//#define RL_61850        L"61850У��"
//#define RL_GDW          L"���ҵ�����ر�׼"
//#define RL_SD           L"ɽ����ر�׼"
//#define RL_OTHER        L"����"

#define RL_SCHEMA			L"�﷨������"
#define RL_PROPERTIES		L"���Թ淶�Լ��"
#define RL_UNIQUENESS		L"Ψһ�Լ��"
#define RL_MODELINTEGRITY	L"ģ��������У��"
#define RL_REFERENCE		L"������Ч��У��"
#define RL_STRUCT			L"�ṹ�淶��У��"

// ��ر�׼�淶
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
//#define REF_SD_YTH      L"���ܱ��վ����ϵͳ�����ļ���ⷽ�����ֻ�"
//#define REF_OTHER       L"����"

#define REF_DEFAULT			L"DL/T 860.6"
#define REF_SCHEMA			L"SCL.xsd"

#define REF_DL6932			L"DL/T 860.6 ��9.3.2��"
#define REF_DL6934			L"DL/T 860.6 ��9.3.4��"
#define REF_DL6937			L"DL/T 860.6 ��9.3.7��"
#define REF_DL6938			L"DL/T 860.6 ��9.3.8��"
#define REF_DL69310			L"DL/T 860.6 ��9.3.10��"
#define REF_DL69311			L"DL/T 860.6 ��9.3.11��"
#define REF_DL6942			L"DL/T 860.6 ��9.4.2��"
#define REF_DL6943			L"DL/T 860.6 ��9.4.3��"
#define REF_DL745			L"DL/T 860.74 ��5��"
#define REF_DL81B			L"DL/T 860.81 ��¼B"
#define REF_DL81C			L"DL/T 860.81 ��¼C"

#define REF_DL6938_11		L"DL/T 860.6 ��9.3.8-9.3.11��"
#define REF_DL7214229		L"DL/T 860.72 ��14.2.2.9��"
#define REF_DL721421		L"DL/T 860.72 ��14.2.1��"
#define REF_DL7214227		L"DL/T 860.72 ��14.2.2.7��"
#define REF_DL72142212		L"DL/T 860.72 ��14.2.2.12��"
#define REF_DL9285			L"DL/T 860.92 ��8.5��"
#define REF_DL72164			L"DL/T 860.72 ��16.4��"

#define REF_Q396			L"Q/GDW 396-2009 ��6.1.1��"
#define REF_DL6944_SMV945	L"DL/T 860.6 ��9.4.4�ڣ�GSE 9.4.4�ڣ�SMV��9.4.5�ڣ�"
#define	REF_DL6994_SMV946	L"DL/T 860.6 ��9.4.4�ڣ�GSE 9.4.4�ڣ�SMV��9.4.6�ڣ�"

// 2014.04.04�޸�
#define REF_DL6936			L"DL/T 860.6 ��9.3.6�� "
#define REF_Q396_911E_1011G L"Q/GDW 396-2009 ��9.1.1�� e)����10.1.1�� g)"
#define REF_Q396_5A			L"Q/GDW 396-2009 ��5�� a)"
#define REF_Q396_1011		L"Q/GDW 396-2009 ��10.1.1��"
#define REF_Q396_613		L"Q/GDW 396-2009 ��6.1.3��"
#define REF_Q396_1011		L"Q/GDW 396-2009 ��10.1.1��"
#define REF_Q396_652		L"Q/GDW 396-2012 ��6.5.2�� c)"
#define REF_PROEX_FACTOR    L"���̾���(���ֳ��ҵ��豸��֧��32���ַ����ϵ�����)"

//---------------------------------------------------------
//                   ��ȡ���ص���Ϣ  
//---------------------------------------------------------


// ������\����źš�
typedef struct SignalStruW            // Unicode
{
	int     iOrd;                     // �źű��
	int     iOutSigOrd;               // ��¼����ź���DataSet�е���ţ�������ź�ʹ��
	wchar_t wBriefName[MAXSIZE];      // �źż��
	wchar_t wGroupName[MAXSIZE];      // �ź��������
	wchar_t wDataSet[MINSIZE];        // ���ݼ� (������ź���)
	wchar_t wLnPrefix[MINSIZE];       // lnǰ׺
	wchar_t wLnClass[MINSIZE];        // ln����
	wchar_t wLnInst[MINSIZE];         // lnʵ��
	wchar_t wDoName[MINSIZE];         // Do����
	wchar_t wDaName[MINSIZE];         // Da����
	wchar_t wFc[MINSIZE];             // ����
	wchar_t wPath[MIDSIZE];           // �ź�·��
	wchar_t wSigType[MINSIZE];        // �ź����� GSEControl��SampledValueControl
	wchar_t wStrpName[MINSIZE];       // ѹ������
	wchar_t wStrpDesc[MINSIZE];       // ѹ������
	bool    bIsStrp;                  // �Ƿ�Ϊѹ���ź�

	//// ��¼�źŶ�Ӧ������˿� 2014.1.16 zhang-xch
	//wchar_t wIedName[MINSIZE];  
	//wchar_t wPhyPortIndex[TINSIZE];

	// �������� 2014-4.28  ���ݡ�SCD���ӻ���Ҫ���˵���顷���и���
	 wchar_t sBDAType[MINSIZE]; // BDA�������� δ֪
	 wchar_t sPlugType[MINSIZE]; // ���ͣ����㡢˫�㡢ʱ�꣩ δ֪

	// ���ź������Ln������DOI������dUֵ
	wchar_t sLnDesc[MAXSIZE];
	wchar_t sDOIDesc[MAXSIZE];
	wchar_t sdUVal[MAXSIZE];

	struct SignalStruW* pNext;
}SIGNALSTRUW, *PSIGNALSTRUW;

typedef struct SignalStruA            // Ansi
{
	int  iOrd;                        // �źű��
	int  iOutSigOrd;               // ��¼����ź���DataSet�е���ţ�������ź�ʹ��
	char chBriefName[MAXSIZE];        // �źż��
	char chGroupName[MAXSIZE];        // �ź��������
	char chDataSet[MINSIZE];          // ���ݼ� (������ź���)
	char chLnPrefix[MINSIZE];         // lnǰ׺
	char chLnClass[MINSIZE];          // ln����
	char chLnInst[MINSIZE];           // lnʵ��
	char chDoName[MINSIZE];           // Do����
	char chDaName[MINSIZE];           // Da����
	char chFc[MINSIZE];               // ����
	char chPath[MIDSIZE];             // �ź�·��
	char chSigType[MINSIZE];          // �ź����� GSEControl��SampledValueControl
	char chStrpName[MINSIZE];         // ѹ������
	char chStrpDesc[MINSIZE];         // ѹ������
	bool bIsStrp;                     // �Ƿ�Ϊѹ���ź�

	//// ��¼�źŶ�Ӧ������˿� 2014.1.16 zhang-xch
	// char wIedName[MINSIZE];
	// char wPhyPortIndex[TINSIZE];

	// �������� 2014-4.28  ���ݡ�SCD���ӻ���Ҫ���˵���顷���и���
	char sBDAType[MINSIZE]; // BDA�������� δ֪
	char sPlugType[MINSIZE]; // ���ͣ����㡢˫�㡢ʱ�꣩ δ֪

	// ���ź������Ln������DOI������dUֵ
	char sLnDesc[MAXSIZE];
	char sDOIDesc[MAXSIZE];
	char sdUVal[MAXSIZE];

	struct SignalStruA* pNext;
}SIGNALSTRUA, *PSIGNALSTRUA;

// �����ƿ顿
typedef struct CBStruW                // Unicode
{
	wchar_t wName[MINSIZE];           // ���ƿ�����
	wchar_t wType[MINSIZE];           // ���ƿ����� GSEControl��SampledValueControl
	wchar_t wDataSet[MINSIZE];        // ���ݼ�

	wchar_t wAppID[MINSIZE]; // ���ӻ�ʱʹ�� 2014.4.16 zhang-xch

	// �������� 2014-4.28  ���ݡ�SCD���ӻ���Ҫ���˵���顷���и���
	wchar_t sConfRev[MINSIZE]; // ���ƿ����ð汾�� SCL\IED\AccessPoint\Server\LDevice\Ln0\GSEControl(SimpleValueControl)\confRev
	int sASDU;    // ASDU��Ŀ  SCL\IED\AccessPoint\Server\LDevice\Ln0\DataSet\FCDA 
	wchar_t sSmpRate[MINSIZE]; // �����ʣ�SV��     SCL\IED\AccessPoint\Server\LDevice\Ln0\SimpleValueControl\smpRate

	wchar_t sDataSetAddr[MINSIZE]; // ���ݼ���ַ δ֪
	wchar_t sGocbRef[MINSIZE]; // gocbRef��GOOSE�� δ֪

	struct CBStruW* pNext;
}CBSTRUW, *PCBSTRUW;

typedef struct CBStruA                // Ansi
{
	char chName[MINSIZE];              // ���ƿ�����
	char chType[MINSIZE];              // ���ƿ����� GSEControl��SampledValueControl
	char chDataSet[MINSIZE];           // ���ݼ�

	char wAppID[MINSIZE]; // ���ӻ�ʱʹ�� 2014.4.16 zhang-xch

	// �������� 2014-4.28  ���ݡ�SCD���ӻ���Ҫ���˵���顷���и���
	char sConfRev[MINSIZE]; // ���ƿ����ð汾�� SCL\IED\AccessPoint\Server\LDevice\Ln0\GSEControl(SimpleValueControl)\confRev
	int sASDU;    // ASDU��Ŀ  SCL\IED\AccessPoint\Server\LDevice\Ln0\DataSet\FCDA 
	char sSmpRate[MINSIZE]; // �����ʣ�SV��     SCL\IED\AccessPoint\Server\LDevice\Ln0\SimpleValueControl\smpRate

	char sDataSetAddr[MINSIZE]; // ���ݼ���ַ δ֪
	char sGocbRef[MINSIZE]; // gocbRef��GOOSE�� δ֪

	struct CBStruA* pNext;
}CBSTRUA, *PCBSTRUA;

// ��LD��
typedef struct LDStruW                // Unicode
{
	wchar_t wInst[TINSIZE];           // LD inst

	int          iCBSize;             // CB������
	PCBSTRUW     pCBStru;             // CB
	int          iInsigSize;          // �����źŵ�����
	PSIGNALSTRUW pInsigStru;          // �����ź�
	int          iOutsigSize;         // ����źŵ�����
	PSIGNALSTRUW pOutsigStru;         // ����ź�

	struct LDStruW* pNext;
}LDSTRUW, *PLDSTRUW;

typedef struct LDStruA                // Ansi
{
	char chInst[TINSIZE];              // LD inst

	int          iCBSize;             // CB������
	PCBSTRUA     pCBStru;             // CB
	int          iInsigSize;          // �����źŵ�����
	PSIGNALSTRUA pInsigStru;          // �����ź�
	int          iOutsigSize;         // ����źŵ�����
	PSIGNALSTRUA pOutsigStru;         // ����ź�

	struct LDStruA* pNext;
}LDSTRUA, *PLDSTRUA;

// ������˿ڡ�  2014.09.03 ���չ�����׼�������˿�
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


// ��AP��
typedef struct APStruW                // Unicode
{ 
	wchar_t wName[TINSIZE];

	int      iLDSize;                 // LD������
	PLDSTRUW pLDStru;                 // LD

	// ������˿ڡ�  2014.09.03 ���չ�����׼�������˿�
	PPHYPORTSTRUW pPhyPort;

	struct APStruW* pNext;
}APSTRUW, *PAPSTRUW;

typedef struct APStruA                // Ansi
{
	char chName[TINSIZE];          

	int      iLDSize;                 // LD������
	PLDSTRUA pLDStru;                 // LD

	// ������˿ڡ�  2014.09.03 ���չ�����׼�������˿�
	PPHYPORTSTRUA pPhyPort;

	struct APStruA* pNext;
}APSTRUA, *PAPSTRUA;

// ��ICD ��Ϣ��
typedef struct ICDStruW               // Unicode
{
	wchar_t wName[MINSIZE];           // ����
	wchar_t wDesc[MAXSIZE];           // ����
	wchar_t wType[MINSIZE];           // �豸�ͺ� ��type���Ե�һ��@ǰ�Ĳ��֣�
	wchar_t wManu[MINSIZE];           // ��������
	wchar_t wfunVer[MINSIZE];         // ���ܰ汾 ��type���Ե�һ��@��Ĳ��֣�
	wchar_t wConfigVer[MINSIZE];      // ICD�汾

	wchar_t chCRC[TINSIZE];				// ����CRCУ����	2014.07.07

	int      iAPSize;                 // AP������
	PAPSTRUW pAPStru;                 // AP

	struct ICDStruW* pNext;

}ICDSTRUW, *PICDSTRUW;

typedef struct ICDStruA               // Ansi
{
	char chName[MINSIZE];              // ����
	char chDesc[MAXSIZE];              // ����
	char chType[MINSIZE];              // �豸�ͺ� ��type���Ե�һ��@ǰ�Ĳ��֣�
	char chManu[MINSIZE];              // ��������
	char chfunVer[MINSIZE];            // ���ܰ汾 ��type���Ե�һ��@��Ĳ��֣�
	char chConfigVer[MINSIZE];         // ICD�汾

	char chCRC[TINSIZE];				// ����CRCУ����	2014.07.07

	int      iAPSize;                 // AP������
	PAPSTRUA pAPStru;                 // AP

	struct ICDStruA* pNext;

}ICDSTRUA, *PICDSTRUA;

// ����·��Ϣ��
typedef struct InputsStruW            // Unicode
{
	int iInputsOrd; // ��¼��·��Inputs�е����
	wchar_t wINIedName[MINSIZE];
	wchar_t wINIedDesc[MAXSIZE];
	wchar_t wINSigDesc[MAXSIZE];      // �ź�����
	wchar_t wINLdInst[MINSIZE];
	wchar_t wINPrefix[MINSIZE];
	wchar_t wINLnClass[MINSIZE];
	wchar_t wINLnInst[MINSIZE];
	wchar_t wINDoName[MINSIZE];
	wchar_t wINDaName[MINSIZE];
	wchar_t wINPhyPort[MINSIZE]; // 2014.09.03���ݹ�����׼�������˿�

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
	int iInputsOrd; // ��¼��·��Inputs�е����
	char chINIedName[MINSIZE];
	char chINIedDesc[MAXSIZE];
	char chINSigDesc[MAXSIZE];
	char chINLdInst[MINSIZE];
	char chINPrefix[MINSIZE];
	char chINLnClass[MINSIZE];
	char chINLnInst[MINSIZE];
	char chINDoName[MINSIZE];
	char chINDaName[MINSIZE];
	char chINPhyPort[MINSIZE]; // 2014.09.03���ݹ�����׼�������˿�

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

// ��IP��ַ��
typedef struct IPStruW                // Unicode
{
	wchar_t wIedName[TINSIZE];
	wchar_t wIedDesc[MAXSIZE];
	wchar_t wApName[TINSIZE];
	wchar_t wIpAddr[TINSIZE];         // IP��ַ
	wchar_t wIpMask[TINSIZE];         // ����

	struct IPStruW* pNext;
}IPSTRUW, *PIPSTRUW;

typedef struct IPStruA                // Ansi
{
	char chIedName[TINSIZE];
	char chIedDesc[MAXSIZE];
	char chApName[TINSIZE];
	char chIpAddr[TINSIZE];            // IP��ַ
	char chIpMask[TINSIZE];            // ����

	struct IPStruA* pNext;
}IPSTRUA, *PIPSTRUA;

// ��MAC��ַ��
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

	wchar_t wVlanPriority[TINSIZE]; // ���ӻ�ʱʹ�� 2014.4.16 zhang-xch

	// �������� 2014-4.28  ���ݡ�SCD���ӻ���Ҫ���˵���顷���и���
	wchar_t sMaxTime[MINSIZE]; // MaxTime��GOOSE�� SCL\communication\subNetWork\ConnectAP\address\GSE\MaxTime\Text
	wchar_t sMinTime[MINSIZE]; // MinTime��GOOSE�� SCL\communication\subNetWork\ConnectAP\address\GSE\MinTime\Text

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

	char wVlanPriority[TINSIZE]; // ���ӻ�ʱʹ�� 2014.4.16 zhang-xch

	// �������� 2014-4.28  ���ݡ�SCD���ӻ���Ҫ���˵���顷���и���
	char sMaxTime[MINSIZE]; // MaxTime��GOOSE�� SCL\communication\subNetWork\ConnectAP\address\GSE\MaxTime\Text
	char sMinTime[MINSIZE]; // MinTime��GOOSE�� SCL\communication\subNetWork\ConnectAP\address\GSE\MinTime\Text

	struct MacStruA* pNext;
}MACSTRUA, *PMACSTRUA;

// ��������
typedef struct SubNetStruW            // Unicode
{
	wchar_t wName[MINSIZE];
	wchar_t wDesc[MAXSIZE];
	wchar_t wType[TINSIZE];

	int       iIPSize;                // IP��ַ������
	PIPSTRUW  pIpStru;
	int       iMacSize;               // MAC��ַ������
	PMACSTRUW pMacstru;

	struct SubNetStruW* pNext;
}SUBNETSTRUW, *PSUBNETSTRUW;

typedef struct SubNetStruA            // Ansi
{
	char chName[MINSIZE];
	char chDesc[MAXSIZE]; 
	char chType[TINSIZE];

	int       iIPSize;                // IP��ַ������
	PIPSTRUA  pIpStru;
	int       iMacSize;               // MAC��ַ������
	PMACSTRUA pMacstru;

	struct SubNetStruA* pNext;
}SUBNETSTRUA, *PSUBNETSTRUA;

// ����ʷ��¼�
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

// ��SCD ��Ϣ��
typedef struct SCDStruW               // Unicode
{
	wchar_t wName[MAXSIZE];
	wchar_t wVersion[TINSIZE];
	wchar_t wRevision[TINSIZE];
	wchar_t chCRC[TINSIZE];				// ����CRCУ����	2014.07.07

	wchar_t chToolID[MINSIZE];				// ����ToolID	2014.09.11

	int          iHitemSize;          // ��ʷ��¼��
	PHITEMSTRUW  pHitemStru;          // ��ʷ��¼
	int          iSubNetSize;         // ���������
	PSUBNETSTRUW pSubNetStru;         // ������Ϣ
	int          iICDSize;            // װ�õ�����
	PICDSTRUW    pICDStru;            // IED���������Ϣ
	int          iInputsSize;         // ��·��
	PINPUTSSTRUW pInputs;             // ��·��Ϣ

}SCDSTRUW, *PSCDSTRUW;

typedef struct SCDStruA               // Ansi
{
	char chName[MAXSIZE];
	char chVersion[TINSIZE];
	char chRevision[TINSIZE];
	char chCRC[TINSIZE];				// ����CRCУ����	2014.07.07

	char chToolID[MINSIZE];				// ����ToolID	2014.09.11

	int          iHitemSize;          // ��ʷ��¼��
	PHITEMSTRUA  pHitemStru;          // ��ʷ��¼
	int          iSubNetSize;         // ���������
	PSUBNETSTRUA pSubNetStru;         // ������Ϣ
	int          iICDSize;            // װ�õ�����
	PICDSTRUA    pICDStru;            // IED���������Ϣ
	int          iInputsSize;         // ��·��
	PINPUTSSTRUA pInputs;             // ��·��Ϣ

}SCDSTRUA, *PSCDSTRUA;

//---------------------------------------------------------
//                   У�鷵�ص���Ϣ  
//---------------------------------------------------------

// �����󡢾�����Ϣ��
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
// ICDУ�鷵��
typedef struct ICDValidInfoStruW
{
	wchar_t wName[MAXSIZE];           // ����
	wchar_t wDesc[MAXSIZE];           // ����
	wchar_t wType[MINSIZE];           // �豸�ͺ� ��type���Ե�һ��@ǰ�Ĳ��֣�
	wchar_t wManu[MINSIZE];           // ��������
	wchar_t wFunVer[MINSIZE];         // ���ܰ汾 ��type���Ե�һ��@��Ĳ��֣�
	wchar_t wConfigVer[MINSIZE];      // ICD�汾
	// 2014.9.10 ��ȡ�ļ������CRC��
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
	// 2014.9.10 ��ȡ�ļ������CRC��
	char chCRC[TINSIZE];

	int  iInsigNum;
	int  iOutsigNum;
	int  iErroSize;
	int  iWarnSize;
	PVALIDINFOSTRUA pErro;
	PVALIDINFOSTRUA pWarn;
}ICDVALIDINFOSTRUA, *PICDVALIDINFOSTRUA;
// SCD У�鷵��ֵ
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

// Schema У�鷵��ֵ
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

// SCL ��ȡ�ӿ�(ͬʱ֧��ICD��CID��SCD����ȡ) / �ͷ�
bool SAXSCLImportW(const wchar_t* sPath, PSCDSTRUW* ppSCDStru);
bool SAXSCLImportW(const wchar_t* sPath, const wchar_t* sIEDName, PSCDSTRUW* ppSCDStru);
bool SAXFreeW(PSCDSTRUW pSCDStru);

// SCDУ�� / �ͷ�
bool SAXICDValidateW(const wchar_t* sPath, PICDVALIDINFOSTRUW* ppIcdValid);
bool SAXICDValidateToFileW(const wchar_t* sPath, PICDVALIDINFOSTRUW* ppIcdValid);
bool SAXFreeW(PICDVALIDINFOSTRUW pICDValid);

 // SCDУ�� / �ͷ� 
bool SAXSCDValidateW(const wchar_t* sPath, PSCDVALIDINFOSTRUW* ppScdValid);
bool SAXSCDValidateToFileW(const wchar_t* sPath, PSCDVALIDINFOSTRUW* ppScdValid);
bool SAXSCDValidateToFileA(const char* sPath, PSCDVALIDINFOSTRUA* ppScdValid);
bool SAXFreeW(PSCDVALIDINFOSTRUW pSCDValid);

//---------------------------||    Ansi   ||----------------------------------

// SCL ��ȡ�ӿ�(ͬʱ֧��ICD��CID��SCD����ȡ) / �ͷ�
bool SAXSCLImportA(const char* sPath, PSCDSTRUA* ppSCDStru);
bool SAXFreeA(PSCDSTRUA pSCDStru);

// ICD У�� / �ͷ� 
bool SAXICDValidateA(const char* sPath, PICDVALIDINFOSTRUA* ppIcdValid);
bool SAXFreeA(PICDVALIDINFOSTRUA pICDValid);

// SCDУ�� / �ͷ� 
bool SAXSCDValidateA(const char* sPath, PSCDVALIDINFOSTRUA* ppScdValid);
bool SAXFreeA(PSCDVALIDINFOSTRUA pSCDValid);

// Schema У��
bool SAXXMLValidateW(const wchar_t* sXMLPath, const wchar_t* sXSDPath, PSCHEMAVALIDINFOSTRUW *pXMLValid);
bool SAXFreeW(PSCHEMAVALIDINFOSTRUW pXMLValid);
// SchemaУ��
bool SAXXMLValidateA(const char* sXMLPath, const char* sXSDPath, PSCHEMAVALIDINFOSTRUA *pXMLValid);
bool SAXFreeA(PSCHEMAVALIDINFOSTRUA pXMLValid);

#endif