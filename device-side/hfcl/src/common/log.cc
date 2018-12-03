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

#include "common/log.h"

namespace hfcl {

Log *Log::m_log = NULL;

Log::Log (const char* logFile = NULL, bool benable = true)
    :m_enable(benable)
{
    m_fpLog = NULL;
    
    if (logFile)
        m_fpLog = fopen(logFile,"wt");
    
    if (m_fpLog == NULL)
        m_fpLog = stderr;
}

Log::~Log() 
{
    if (m_fpLog && m_fpLog != stderr)
        fclose(m_fpLog);
    releaseLog();
}

Log* Log::getLog(void) 
{
    if (Log::m_log == NULL){
		Log::m_log = HFCL_NEW_EX (Log, (HFCL_LOG_FILE, true));      
	}
    return Log::m_log;
}

void Log::releaseLog(void)
{
    if (NULL != Log::m_log)
        HFCL_DELETE (Log::m_log);
    Log::m_log = NULL;
}

int Log::logPrintf(const char* head, const char* format, va_list args)
{
    if (m_enable) {
		_MG_PRINTF("%s", head);
        _MG_PRINTF(format, args);
        return 0;
    } else {
        return 0;
    }
}

int Log::logPrintf(const char* head, const char* file, const int line,
        const char* func, const char* format, va_list args)
{
    if (m_enable) {
        _MG_PRINTF("%s%s %d %s() :  ", head, file, line, func);
        _MG_PRINTF(format, args);
        return 0;
    } else {
        return 0;
    }
}

void Log::logDebug(const char* format, ...)
{
    if (format == NULL)
        return;

    va_list ap;
    va_start(ap, format);
    Log::getLog()->logPrintf("HFCL DEBUG >> ", format, ap);
    va_end(ap);
}

void Log::logInfo(const char* format, ...)
{
    if (format == NULL)
        return;

    va_list args;
    va_start(args, format);
    Log::getLog()->logPrintf("HFCL INFO >> ", format, args);
    va_end(args);
}

void Log::logError(const char* format, ...)
{
    if (format == NULL)
        return;

    va_list args;
    va_start(args, format);
    Log::getLog()->logPrintf("HFCL ERROR >> ", format, args);
    va_end(args);
}

void Log::logMemory(const char* format, ...)
{
    if (format == NULL)
        return;

    va_list args;
    va_start(args, format);
    Log::getLog()->logPrintf("HFCL MEMORY >>", format, args);
    va_end(args);
}

typedef enum
{
    /// No error occured
    DBG_ERR_NO,
    /// Unsupported operation or parameter
    DBG_ERR_UNSUPPORTED,
    DBG_ERR_QTY
} DBG_ERR_T;

void Log::logHFLF(const char* head, const char* file, const int line, const char* func,
        const char* format, ...)
{
    if (format == NULL)
        return;

    va_list args;
    va_start(args, format);
    Log::getLog()->logPrintf(head, file, line, func, format, args);
    va_end(args);
}

void Log::dumpRect(const IntRect& rc)
{
    logDebug("rc.left is %d, rc.top is %d, rc.right is %d, rc.bottom is %d...\n",
             rc.left(), rc.top(), rc.right(), rc.bottom());
}

} // namespace hfcl

