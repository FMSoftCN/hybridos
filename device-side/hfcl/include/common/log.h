/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HFCL_COMMON_LOG_H_
#define HFCL_COMMON_LOG_H_

#include "common/common.h"
#include "common/stlalternative.h"
#include "common/intrect.h"

// TUNNING CONDITION: 
#define HFCL_LOG_FILE   "/dev/null"

namespace hfcl {

class Log {
public:
    ~Log();
    static Log* getLog(void);
    static void releaseLog(void);

    bool enable(void) const { return m_enable; }
    void setEnable(bool benable) { m_enable = benable; }

    int logPrintf(const char* head, const char* format, va_list args);
    int logPrintf(const char* head, const char* file, const int line, const char* func,
            const char* format, va_list args);

    void logDebug(const char* format, ...);
    void logInfo(const char* format, ...);
    void logError(const char* format, ...);
    void logMemory(const char* format, ...);
    void logHFLF(const char* head, const char* file, const int line, const char* func,
            const char* format, ...);
    void dumpRect(const IntRect& rc);

private:
    Log(const char* logFile, bool benable);

    FILE* m_fpLog;
    bool  m_enable;

    static Log* m_log;
};

} // namespace hfcl

#ifndef __CC_ARM
#define LOGDEBUG Log::getLog()->logDebug
#define LOGINFO  Log::getLog()->logInfo
#define LOGERROR Log::getLog()->logError

#define LOGMEMORY   Log::getLog()->logMemory
#define LOGHFLF     Log::getLog()->logHFLF
#define DUMPRECT    Log::getLog()->dumpRect

/**
 * Normally we strip LOGV (VERBOSE messages) from release builds.
 * You can modify this (for example with "#define HFCL_NDEBUG 1"
 * at the top of your source file) to change that behavior.
 */
#define HFCL_NDEBUG 1
#ifndef	HFCL_LOG_NDEBUG
#if defined(HFCL_NDEBUG) && !defined(MMI_NO_TRACE)
#define	HFCL_LOG_NDEBUG	1
#else
#define	HFCL_LOG_NDEBUG	0
#endif
#endif

// debug  print, macro difine
#ifndef	LOGD
#if	HFCL_LOG_NDEBUG
#define	LOGD(...)   LOGHFLF("HFCL DEBUG >> ", __FILE__, __LINE__, __func__, __VA_ARGS__);
#else
#define	LOGD(...)   ((void)0)
#endif
#endif

// info print, macro difine
#ifndef	LOGI
#if	HFCL_LOG_NDEBUG
#define	LOGI(...)   LOGHFLF("HFCL INFO >> ", __FILE__, __LINE__, __func__, __VA_ARGS__);
#else
#define	LOGI(...)   ((void)0)
#endif
#endif

// error  print, macro difine
#ifndef	LOGE
#if	HFCL_LOG_NDEBUG
#define	LOGE(...)   LOGHFLF("HFCL Error >> ", __FILE__, __LINE__, __func__, __VA_ARGS__);
#else
#define	LOGE(...)   ((void)0)
#endif
#endif

// if memory leak print, macro difine
#ifndef	LOGMEM
#if	1
#define	LOGMEM(...)   LOGMEMORY(__VA_ARGS__);
#else
#define	LOGMEM(...)   ((void)0)
#endif
#endif

//#define Log_Debug Log::getLog()->logDebug

#else /* !__CC_ARM */

#define LOGINFO  _MG_PRINTF
#define LOGDEBUG _DBG_PRINTF
#define LOGERROR _ERR_PRINTF

#define	LOGI    _MG_PRINTF
#define	LOGD    _DBG_PRINTF
#define	LOGE    _ERR_PRINTF

#endif /* __CC_ARM */

#endif  /* HFCL_COMMON_LOG_H_ */

