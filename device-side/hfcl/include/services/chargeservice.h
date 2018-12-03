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

/*
** $Id: RadioService.h 2522 2012-01-18 05:37:53Z dlzhong $
**
** RadioService.h:
**
** Copyright (C) 2002 ~ 2011 Beijing FMSoft Technology Co., Ltd.
**
** All rights reserved by FMSoft.
**
** Current maintainer: dlzhong
**
** Create Date : 2012-04-11
*/

#ifndef __HFCL_ChargeSERVICE_H__
#define __HFCL_ChargeSERVICE_H__

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace hfcl;

class ChargeService : public EventListener, public Service {
    public:
        static inline bool isExistInstance(void) {
            return (NULL != m_singleton);
        }

        static inline ChargeService* getInstance(void) {
            if (NULL == m_singleton) {
                m_singleton = HFCL_NEW_EX(ChargeService, ());
                m_singleton->start();
            }
            return m_singleton;
        }

        ~ChargeService();

        void releaseInstance(void);

        void start(void);
        void stop(void);
		enum EventType {
            BATTERY_STATUS,
            EARPHONE_DET,
			EARPHONE_PLUGOUT,
            EARPHONE_KEY,
        };


        void BatteryStatusHandle(void* info);
        void ChargingAbnormalHandler( Uint16 stringId );
        void  BatteryStatusLowBatteryHandler(Uint8 battery_voltage);

        void   SET_LOW_BATTERY_ALARM_THRESHOLD(Uint8 cnt);
        BOOL IS_LOW_BATTERY(void);
        BOOL CHECK_MO_ALLOW(void);
        void RESET_LOW_BATTERY_ALARM_CNT(void);

        Uint8 GetChargValueIndex(Uint8* isFullCharged);
        void  LowBatteryCommonAction();

        void addEventListener(EventListener* listener);
        void releaseEventListener(void);
        void raiseEvent(Event* event);
        bool handleEvent(Event* event);
        BOOL is_CheckedLowBatteryOnPowerOn(void);
		BOOL is_charge_connected(void){return isChargerConnected;}

    private:
        ChargeService();

        EventListener*    m_appListener;
        int             m_state;

        Uint8 	PowerOnChargingPeriod;			// Tells if charging is going on during power on charger
        Uint8 	ChargingAbnormal;				// If abnormal case of charging.
        Uint8 	ChargingComplete;				 

        Uint8 	chargerLevel;  
        Uint8 	batteryStatus; 
        Uint8 	isChargerConnected;				//Tells if charger is connected

        Uint8 	LowBatteryCutOffCallCnt;
        Uint8 	LowBatteryFlag;					//Tels if Battery is low.
        Uint8 	LowBatteryTxProhibitFlag;

        Uint8   batteryLowCount;
        Uint8   lowBatteryAlarmThreshold;

        static ChargeService* m_singleton;
        Uint8 	checkedLowbatteryOnPowerOn;
        int prevBatteryLevel;
};

#endif // __HFCL_ChargeSERVICE_H__

