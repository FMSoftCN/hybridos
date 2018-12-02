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

#ifndef __HFCL_DATETIME_H__
#define __HFCL_DATETIME_H__

#include "nguxcommon.h"
#include "log.h"
#include "tm.h"

namespace hfcl {

class NguxTime {
	public:
		NguxTime ()
			:year(0), month(0), day(0), hour(0), minute(0), second(0) { }

		NguxTime (int y, int m, int d, int h, int min, int s)
			:year(y), month(m), day(d), hour(h), minute(min), second(s) { }

		NguxTime (time_t timeInSec) {
			TM_SYSTEMTIME systime;
			TM_FILETIME fileTime;
			fileTime.DateTime = timeInSec;
			TM_FileTimeToSystemTime(fileTime, &systime);

			year  		 = systime.uYear;
			month 	  	 = systime.uMonth;
			day   		 = systime.uDay;
			dayOfWeek 	 = systime.uDayOfWeek;
			hour  		 = systime.uHour;
			minute 	  	 = systime.uMinute;
			second 	  	 = systime.uSecond;
			milliSeconds = systime.uMilliseconds;
		}

		~NguxTime() {}

	public:
		unsigned int  year;
		unsigned char month;
		unsigned char day;
		unsigned char hour;
		unsigned char minute;
		unsigned char second;
		unsigned int  milliSeconds;
		unsigned char dayOfWeek;

		static NguxTime * g_sys_time;
        static time_t g_date_time;

	public:

        static bool isToday(const NguxTime *nt)
        {
            NguxTime *st = getSysTime();

            return (st->year == nt->year
                    && st->month == nt->month
                    && st->day == nt->day) ? true : false;
        }

        static unsigned int nguxMktime(const NguxTime* nt)
        {
            if (NULL == nt)
                return 0;

			TM_SYSTEMTIME systime;
			TM_FILETIME fileTime;

			systime.uYear		 = nt->year  		;
			systime.uMonth		 = nt->month 	  	;
			systime.uDay		 = nt->day   		;
			systime.uDayOfWeek	 = nt->dayOfWeek 	;
			systime.uHour		 = nt->hour  		;
			systime.uMinute		 = nt->minute 	  	;
			systime.uSecond		 = nt->second 	  	;
			systime.uMilliseconds= nt->milliSeconds ;
			
			TM_SystemTimeToFileTime(&systime, &fileTime);
			
			return fileTime.DateTime;
        }

        unsigned int nguxMktime(void) const
        {
			return nguxMktime(this);
        }

		static unsigned int getSysTimeInSec(void)
        {
            /* VincentWei: call TM_GetSystemFileTime() instead of time(NULL) */
            TM_FILETIME tm_ft;
            TM_GetSystemFileTime (&tm_ft);
            g_date_time = tm_ft.DateTime;
            return g_date_time;
        }

		static unsigned int getNowMSecs(void)
        {
			static int c = 0;
			c += 53;
			c %= 1000;
			return c;
        }

		static NguxTime *getSysTime() 
		{
            /* VincentWei: call TM_GetSystemFileTime() instead of time(NULL) */
            TM_FILETIME tm_ft;
            TM_GetSystemFileTime (&tm_ft);
            g_date_time = tm_ft.DateTime;

			if (g_sys_time == NULL)
				g_sys_time = HFCL_NEW_EX(NguxTime, ());

            TM_SYSTEMTIME systime;
            TM_FILETIME fileTime;
            fileTime.DateTime = g_date_time;
            TM_FileTimeToSystemTime(fileTime, &systime);

            g_sys_time->year  		 = systime.uYear;
            g_sys_time->month 	  	 = systime.uMonth;
            g_sys_time->day   		 = systime.uDay;
            g_sys_time->dayOfWeek 	 = systime.uDayOfWeek;
            g_sys_time->hour  		 = systime.uHour;
            g_sys_time->minute 	  	 = systime.uMinute;
            g_sys_time->second 	  	 = systime.uSecond;
            g_sys_time->milliSeconds = systime.uMilliseconds;
			return g_sys_time;
		}
};

enum eTimeFormat {
	TIM_FMT_24H,
	TIM_FMT_12H 
};

enum eDateFormat {
	DATE_FMT_DMY, 
	DATE_FMT_MDY, 
	DATE_FMT_YMD
};

class Date
{
public:
    unsigned int year:16;
    unsigned int month:4;
    unsigned int wday:4; //week day
    unsigned int mday:8; //month day
    Date() {
        year = 0;
        month = 0;
        mday = 0;
        wday = 0;
    }

#ifdef __CALENDAR_ARABIC__
    static inline DATE_SYS_ENUM GetDateSystem()
    {
        DATE_SYS_ENUM dateSystem = GetCurrentDateSystem();
        return dateSystem;
    }

    static inline bool IsHijriLeapYear(int year)
    {
        UINT8 temp = (year % 30);
        if(temp == 2 || temp == 5 || temp == 7 || temp == 10 || temp == 13 || temp == 16 || temp == 18 || temp == 21 || temp == 24 || temp == 26 || temp == 29)
        {
            return TRUE;
        }
        return FALSE;
    }

    static inline bool IsShamsiLeapYear(int year)
    {
        if(((((((year - (474)) % 2820) + 474) + 38) * 682) % 2816) < 682)
            return TRUE;
        else 
            return FALSE;
    }
#endif
	
    static inline bool IsLeapYear(int year) {
        return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
    }
    
    inline bool  isLeapYear() {
        return IsLeapYear(year);
    }

    static int DaysOfMonth(int y, int m);

    inline int daysOfMonth() {
        return DaysOfMonth(year, month);
    }

    static int DayOfWeek(int y, int m, int d);
    
    inline int dayOfWeek() {
        return DayOfWeek(year, month, mday);
    }

    static int DayOfYear(int y, int m, int d);
    
    inline int dayOfYear() {
        return DayOfYear(year, month, mday);
    }

    inline void prevMonth() {
        if(month == 1)
        {
            month = 12;
            year --;
        }
        else {
            month --;
        }
    }

    inline void nextMonth() {
        if(month == 12) {
            month = 1;
            year ++;
        }
        else {
            month ++;
        }
    }
    
    inline void setDay(int d) {
        int days = daysOfMonth();
        if(d < 1) 
            d = 1;
        if(d > days)
            d = days;
        mday = d;
    }
    
    inline void setDate(int y, int m, int d) {
        year = y;
        month = m;
        if( m < 1)
            m = 1;
        else if(m > 12)
            m = 12;
        setDay(d);
    }

    static inline Date Now() {
        //FIXEDME: should use: get local time
        NguxTime* t = NguxTime::getSysTime();
        Date date;
        date.year = t->year;
        date.month = t->month;
        date.mday = t->day;
        date.wday = t->dayOfWeek;
        return date;
    }

    Date& operator = (const Date &d) {
        year = d.year;
        month = d.month;
        mday = d.mday;
        wday = d.wday;
        return *this;
    }
    
    Date(const Date& d){
        year = d.year;
        month = d.month;
        mday = d.mday;
        wday = d.wday;
    }
    
    unsigned int toInt() {
        return ((year)<<16) | (month <<8) | mday;
    }
    
    void fromInt(unsigned int i) {
        year = (i)>>16;
        month = (i&0xFF00) >> 8;
        mday = (i&0xFF);
        wday = 0;
    }
    
    Date(unsigned int i) {
        fromInt(i);
    }

    Date& operator=(unsigned int i) {
        fromInt(i);
        return *this;
    }

};

string FormatDateString (const NguxTime *pTime, 
		eDateFormat format, const char *split);

string FormatTimeString (const NguxTime *pTime, 
		eTimeFormat format, const char *split);

string FormatDateTimeString (const NguxTime *pTime, 
		eDateFormat dateFmt, const char *dateSplit,
		eTimeFormat timeFmt, const char *timeSplit);


/**
 * marco return string time hh:mm
 * eg 21:30
 */
#define TEXTTIME	FormatTimeString(NguxTime::getSysTime(), TIM_FMT_24H, ":")

#define TEXTTIME_12H	FormatTimeString(NguxTime::getSysTime(), TIM_FMT_12H, ":")
#define TEXTTIME_24H	FormatTimeString(NguxTime::getSysTime(), TIM_FMT_24H, ":")

/**
 * marco return string date mm/dd/yy
 * eg 05/30/11
 */
#define TEXTDATE    FormatDateString(NguxTime::getSysTime(), DATE_FMT_YMD, "/")

#define TEXTDATE_FMT_S_DMY    FormatDateString(NguxTime::getSysTime(), DATE_FMT_DMY, "-")
#define TEXTDATE_FMT_S_MDY    FormatDateString(NguxTime::getSysTime(), DATE_FMT_MDY, "-")
#define TEXTDATE_FMT_S_YMD    FormatDateString(NguxTime::getSysTime(), DATE_FMT_YMD, "-")
#define TEXTDATE_FMT_H_DMY    FormatDateString(NguxTime::getSysTime(), DATE_FMT_DMY, "/")
#define TEXTDATE_FMT_H_MDY    FormatDateString(NguxTime::getSysTime(), DATE_FMT_MDY, "/")
#define TEXTDATE_FMT_H_YMD    FormatDateString(NguxTime::getSysTime(), DATE_FMT_YMD, "/")

/**
 * marco return int date for system time in second
 * eg 12345678
 */
#define TIMEINSECONDS (NguxTime::getSysTimeInSec())

} // namespace hfcl {

#endif  /* __HFCL_DATETIME_H__ */

