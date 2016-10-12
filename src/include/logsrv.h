#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _LOGSRV_H_
#define _LOGSRV_H_

#ifdef LOGOUPT_EXPORTS
#define LOGSRV_API __declspec(dllexport)
#else
#define LOGSRV_API __declspec(dllimport)
#endif

typedef int handle;

enum log_encoding
{
	log_encoding_gbk = 0,
	log_encoding_utf8 = 1,
	log_encoding_unicode = 2,

#if defined(UNICODE) && defined(_UNICODE)
#define log_encoding_auto log_encoding_unicode
#else
#define log_encoding_auto log_encoding_gbk
#endif
};

enum log_level
{
	lv_error = 0x01,
	lv_warning = 0x02,
	lv_info = 0x04,
	lv_debug = 0x08,
	lv_none = 0x10,
	lv_all = 0x1F
};


extern "C" LOGSRV_API handle __stdcall logsrv_acquireA
(
	char const * path, 
	int single_log_file_max_size, 
	int reserved_log_date_count
);

extern "C" LOGSRV_API handle __stdcall logsrv_acquireW
(
	wchar_t const * path,
	int single_log_file_max_size,
	int reserved_log_date_count
);

extern "C" LOGSRV_API bool __stdcall logsrv_outputA
(
	handle logsrv_handle,
	char const * log_info,
	char const * module_name,
	char const * file,
	int line,
	log_encoding encoding,
	log_level level,
	bool also_output_to_ui
);

extern "C" LOGSRV_API bool __stdcall logsrv_outputW
(
	handle logsrv_handle,
	wchar_t const * log_info,
	wchar_t const * module_name,
	char const * file,
	int line,
	log_encoding encoding,
	log_level level,
	bool also_output_to_ui
);

extern "C" LOGSRV_API bool __stdcall logsrv_set_output_levels
(
	handle logsrv_handle, 
	int levels
);

extern "C" LOGSRV_API void __stdcall logsrv_release
(
	handle logsrv_handle
);



#include <string>

class scoped_log_output
{
public:
	scoped_log_output(
		handle log_handle, 
		char const * log_info_enter, 
		char const * log_info_leave, 
		char const * module, 
		char const * file, 
		int line)
		: m_is_unicode(false)
		, m_log_handle(log_handle)
		, m_module_nameA(module)
		, m_log_info_leaveA(log_info_leave)
	{
		logsrv_outputA(log_handle, log_info_enter, module, file, line, log_encoding_gbk, lv_info, false);
	}

	scoped_log_output(
		handle log_handle, 
		wchar_t const * log_info_enter, 
		wchar_t const * log_info_leave, 
		wchar_t const * module, 
		char const * file, 
		int line)
		: m_is_unicode(true)
		, m_log_handle(log_handle)
		, m_module_nameW(module)
		, m_log_info_leaveW(log_info_leave)
	{
		logsrv_outputW(log_handle, log_info_enter, module, file, line, log_encoding_auto, lv_info, false);
	}

	~scoped_log_output(void)
	{
		if (m_is_unicode)
		{
			logsrv_outputW(m_log_handle, m_log_info_leaveW.c_str(), m_module_nameW.c_str(), NULL, 0, log_encoding_auto, lv_info, false);
		}else
		{
			logsrv_outputA(m_log_handle, m_log_info_leaveA.c_str(), m_module_nameA.c_str(), NULL, 0, log_encoding_gbk, lv_info, false);
		}
	}
private:
	bool m_is_unicode;
	handle m_log_handle;
	std::string m_module_nameA;
	std::string m_log_info_leaveA;
	std::wstring m_module_nameW;
	std::wstring m_log_info_leaveW;
};


#define logsrv_scoped_output(logsrv_handle, enter_log, leave_log, module_name) \
	scoped_log_output __scoped_log_object(logsrv_handle, enter_log, leave_log, module_name, __FILE__, __LINE__)


#if defined(_UNICODE) && defined(UNICODE)

#define logsrv_acquire logsrv_acquireW
#define logsrv_output logsrv_outputW

#define logsrv_output_ex(logsrv_handle, log_info, module_name, level) \
	logsrv_outputW(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, level, false)

#define logsrv_output_error(logsrv_handle, log_info, module_name) \
	logsrv_outputW(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_error, false)

#define logsrv_output_warning(logsrv_handle, log_info, module_name) \
	logsrv_outputW(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_warning, false)

#define logsrv_output_info(logsrv_handle, log_info, module_name) \
	logsrv_outputW(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_info, false)

#define logsrv_output_debug(logsrv_handle, log_info, module_name) \
	logsrv_outputW(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_debug, false)

#define logsrv_output_normal(logsrv_handle, log_info, module_name) \
	logsrv_outputW(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_none, false)

#else

#define logsrv_acquire logsrv_acquireA
#define logsrv_output logsrv_outputA

#define logsrv_output_ex(logsrv_handle, log_info, module_name, level) \
	logsrv_outputA(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_gbk, level, false)

#define logsrv_output_error(logsrv_handle, log_info, module_name) \
	logsrv_outputA(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_error, false)

#define logsrv_output_warning(logsrv_handle, log_info, module_name) \
	logsrv_outputA(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_warning, false)

#define logsrv_output_info(logsrv_handle, log_info, module_name) \
	logsrv_outputA(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_info, false)

#define logsrv_output_debug(logsrv_handle, log_info, module_name) \
	logsrv_outputA(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_debug, false)

#define logsrv_output_normal(logsrv_handle, log_info, module_name) \
	logsrv_outputA(logsrv_handle, log_info, module_name, __FILE__, __LINE__, log_encoding_auto, lv_none, false)

#endif



#endif
