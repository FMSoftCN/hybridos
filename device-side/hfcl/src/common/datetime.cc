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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/datetime.h"

namespace hfcl {
time_t NguxTime::g_date_time = 0;
NguxTime* NguxTime::g_sys_time = NULL;

static inline BOOL _format_date_time(const NguxTime *pNguxTime, char *dateStr,
        char *timeStr, UINT16 fmt)
{
    TM_FILETIME filetime;
    char *pDate = dateStr;
    char *pTime = timeStr;
    char tmpDate[16], tmpTime[16];
    UINT8 dateLen = 14, timeLen = 10;
    BOOL err;
    UINT32 ret;

    if (pNguxTime) {
        filetime.DateTime = pNguxTime->g_date_time;
    }
    else {
        TM_GetSystemFileTime(&filetime);
    }

    if(!dateStr) {
        pDate = tmpDate;
        fmt |= (TM_GetTimeFormat() & 0x7F0);
    }
    if (!timeStr) {
        pTime = tmpTime;
        fmt |= (TM_GetTimeFormat() & 0xF);
    }

    err = TM_FormatFileTime(filetime, fmt, 0, (char *)pDate, dateLen, (char *)pTime, timeLen);

    if (!err) {
        TM_GetLastErr(&ret);
    }

    if (pDate)
        *(pDate + 10) = '\0';

    return err;
}

static inline UINT16 _get_time_flag(eTimeFormat format, const char *split)
{
    UINT16 flag = 0;

    if (*split == '.') {
        flag |= TM_FMT_DOT_TIME;
    } else {
        flag |= TM_FMT_COLON_TIME;
    }

    switch (format) {
        case TIM_FMT_12H:
            flag |= TM_FMT_TWELVE_TIME;
            break;
        case TIM_FMT_24H:
        default:
            flag |= TM_FMT_TWENTYFOUR_TIME;
            break;
    }

    return flag;
}

static inline UINT16 _get_date_flag(eDateFormat format, const char *split)
{
    UINT16 flag = 0;

    if (*split == '.') {
        flag |= TM_FMT_DOT_DATE;
    } else if (*split == ':') {
        flag |= TM_FMT_COLON_DATE;
    } else if (*split == '/') {
        flag |= TM_FMT_SLASH_DATE;
    } else {
        flag |= TM_FMT_BASELINE_DATE;
    }

    switch (format) {
        case DATE_FMT_DMY:
            flag |= TM_FMT_DMY_DATE;
            break;
        case DATE_FMT_MDY:
            flag |= TM_FMT_MDY_DATE;
            break;
        case DATE_FMT_YMD:
            flag |= TM_FMT_YMD_DATE;
            break;
        default:
            flag |= TM_FMT_YMD_DATE;
            break;
    }

    return flag;
}

string FormatDateString (const NguxTime *pTime, eDateFormat format, const char *split)
{
    char datestr[32];
    if (_format_date_time(pTime, datestr, NULL, _get_date_flag(format, split)))
        return string((char*)datestr);
   else
        return "2018/09/06";
}

string FormatTimeString (const NguxTime *pTime, eTimeFormat format, const char *split)
{
    char timestr[32];
    //UINT32 err = 0;
    if (_format_date_time(pTime, NULL, timestr, _get_time_flag(format, split)))
    {
        return string((char*)timestr);
    }
    else
        return "10:34";
}

string FormatDateTimeString (const NguxTime *pTime,
        eDateFormat dateFmt, const char *dateSplit,
        eTimeFormat timeFmt, const char *timeSplit)
{
    char timestr[32];
    char datestr[32];

    if (_format_date_time(pTime, datestr, timestr,
                _get_date_flag(dateFmt, dateSplit)|_get_time_flag(timeFmt, timeSplit))) {
        char datetime[64];
        sprintf(datetime, "%s %s", datestr, timestr);
        return string(datetime);
    }
    else
        return "";
}

const static unsigned char daysInMonth[12]= {31,28,31,30,31,30,31,31,30,31,30,31};

int Date::DaysOfMonth(int y, int m)
{
#ifdef __CALENDAR_ARABIC__
    DATE_SYS_ENUM dateSystem = GetDateSystem();
    if(dateSystem == DATE_HIJRI || dateSystem == DATE_SHAMSI)
    {
        return TM_GetDaysOneMonthByDateSystem(y, m, dateSystem);
    }
    else
#endif
    {
        return (m == 2 && IsLeapYear(y) ? 29 : daysInMonth[m - 1]);
    }
}

int Date::DayOfWeek(int y, int m, int d)
{
#ifdef __CALENDAR_ARABIC__
    DATE_SYS_ENUM dateSystem = GetDateSystem();
    if(dateSystem == DATE_HIJRI ||dateSystem == DATE_SHAMSI)
    {
        int w = 0;
        int daysOfYear = 0;
        int baseYear = 0;
        int weekOfBase = 0;

        if(dateSystem == DATE_HIJRI)
        {
            baseYear = 1389;
            weekOfBase = 4; //Thurthday
        }
        else
        {
            baseYear = 1348;
            weekOfBase = 5; //Friday
        }

        for(int i = 0; i < y-baseYear; i++)
        {
            daysOfYear = 0;
            for(int j = 0; j < 12 ; j++)
            {
                daysOfYear += TM_GetDaysOneMonthByDateSystem(i+baseYear, j+1, dateSystem);
            }
            weekOfBase = ((daysOfYear % 7) + weekOfBase) % 7;
        }
        w = DayOfYear(y,m,d);
        w = (w%7) + weekOfBase;
        if(d>0) w = w -1;

        return w;
    }
    else
#endif
    {
        // optimize 1 : w int to register int
        register int w = 1;
        y = (y-1)%400 + 1;
        register int ly = (y-1)/4;
        ly = ly - (y-1)/100;
        ly = ly + (y-1)/400;
        register int ry = y - 1 - ly;
        w = w + ry;
        w = w + 2*ly;
        w = w + DayOfYear(y,m,d);
        w = (w-1)%7 + 1;
        return w;
    }
}

int Date::DayOfYear(int y, int m, int d)
{
    register int c = 0;
    for (register int i = 1; i< m; i++)
    {
        c = c + DaysOfMonth(y,i);
    }
    c = c + d;
    return c;
}

} // namespace hfcl
