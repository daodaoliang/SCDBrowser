/*
 *  @file  : InterfaceInvokeStat.hxx
 *  @author: ½��
 *  @date  : 2014/10/23
 *  @note  : �ӿڵ���ͳ�ƿ�
 */

#ifndef _INTERFACEINVOKESTAT_HXX
#define _INTERFACEINVOKESTAT_HXX

#ifdef SCLDATAACCESSCOMPONENT_EXPORTS

// ԭʼ��¼�ӿ�
int InvokeRecordBegin(const char *interface_name, const char *content);
void InvokeRecordEnd(int key);

#ifdef __cplusplus

//�ӿ��ڲ�ʹ�õĵ��ü�¼�����࣬���ڸ��ӿڵ��ü�¼��ʱ
class CScopedInterfaceInvokeStatHelper
{
public:
    CScopedInterfaceInvokeStatHelper(const char *interface_name);
    CScopedInterfaceInvokeStatHelper(const char *interface_name, const char *fmt, ...);
    ~CScopedInterfaceInvokeStatHelper();

private:
    CScopedInterfaceInvokeStatHelper(const CScopedInterfaceInvokeStatHelper &);
    CScopedInterfaceInvokeStatHelper &operator=(const CScopedInterfaceInvokeStatHelper &);

private:
    int m_key;
};

// �Ƿ��������º�Ĺ���
#define SIISH_ENABLED               1

// ��¼��
#if SIISH_ENABLED

#define SIISH_NORMAL(name)          CScopedInterfaceInvokeStatHelper __unused((name))
#if _MSC_VER >= 1400
#define SIISH_SIMPLE                CScopedInterfaceInvokeStatHelper __unused(__FUNCTION__)
#define SIISH_ARGS(name, fmt, ...)  CScopedInterfaceInvokeStatHelper __unused((name), (fmt), __VA_ARGS__)
#endif /* _MSC_VER >= 1400 */

#else /* SIISH_ENABLED */

#define SIISH_NORMAL(name)          do {} while (false)
#if _MSC_VER >= 1400
#define SIISH_SIMPLE                do {} while (false)
#define SIISH_ARGS(name, fmt, ...)  do {} while (false)
#endif /* _MSC_VER >= 1400 */

#endif /* SIISH_ENABLED */

#endif /* __cplusplus */

#else /* SCLDATAACCESSCOMPONENT_EXPORTS */

#include <Windows.h>

// �ⲿ���ƺ���
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// ������ر�ͳ�ƹ���
void WINAPI SiisEnableStat(bool bEnabled);

// ��������ͳ������
void WINAPI SiisResetStat();

// ������ر�ϵͳ�������
void WINAPI SiisEnableOutput(bool bEnabled);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SCLDATAACCESSCOMPONENT_EXPORTS */

#endif /* _INTERFACEINVOKESTAT_HXX */
