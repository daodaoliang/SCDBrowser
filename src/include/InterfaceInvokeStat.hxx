/*
 *  @file  : InterfaceInvokeStat.hxx
 *  @author: 陆腾
 *  @date  : 2014/10/23
 *  @note  : 接口调用统计库
 */

#ifndef _INTERFACEINVOKESTAT_HXX
#define _INTERFACEINVOKESTAT_HXX

#ifdef SCLDATAACCESSCOMPONENT_EXPORTS

// 原始记录接口
int InvokeRecordBegin(const char *interface_name, const char *content);
void InvokeRecordEnd(int key);

#ifdef __cplusplus

//接口内部使用的调用记录助手类，用于给接口调用记录耗时
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

// 是否开启了以下宏的功能
#define SIISH_ENABLED               1

// 记录宏
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

// 外部控制函数
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// 开启或关闭统计功能
void WINAPI SiisEnableStat(bool bEnabled);

// 重置所有统计数据
void WINAPI SiisResetStat();

// 开启或关闭系统调试输出
void WINAPI SiisEnableOutput(bool bEnabled);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SCLDATAACCESSCOMPONENT_EXPORTS */

#endif /* _INTERFACEINVOKESTAT_HXX */
