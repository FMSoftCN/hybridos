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

#include "lcdservice.h"
#include "services/timerservice.h"
#include "nguxdevlcd.h"
#include "chargeservice.h"

#include "tm.h"
#include "hal_device.h"

extern bool mGetKeypadLightOnOff(void);

static bool bKeepLightOn = false;

void KeepAlwaysLightOn(bool ON)
{
	LcdService* lcd_service = LcdService::getInstance();   

	lcd_service->EnableAlwaysOn(ON);
}
 
bool getKeeponLightOn(void)
{
    return bKeepLightOn;
}

bool setKeeponLightOn(bool keeponLightOn)
{
    bKeepLightOn = keeponLightOn;
    return bKeepLightOn;
}

void KeeponLightOn(bool ON)
{
    LcdService* lcd_service = LcdService::getInstance();   

    if(lcd_service == NULL) return;

    setKeeponLightOn(ON);

    if(ON )
        lcd_service->stopService(true);
    else  
        lcd_service->startService(true);
}

namespace hfcl {

LcdService* LcdService::m_singleton = NULL;

LcdService::LcdService()
{
    init();
}

LcdService::~LcdService()
{
}

void LcdService::init(void)
{
    m_lcd_status = Wakeup;
    m_counter_500ms = 0;
    m_timer_id = -1;
    // FIXME 50 means 500ms on coolsand platform
    m_timer_freq = 500;
    // by default, we delay 10s(500ms*20) to sleep
    m_delay_500ms_to_sleep= 100*2;
    m_service_status = Service_Stop;
    power_saving_mode = false;
    m_lcdLowPower = false;
    m_dimming_status = false;
    prevTicks = 0;
    m_dimmingDurationInPowerSavingMode = 10;    
}

void LcdService::reinit(void)
{
    m_lcd_status = Wakeup;
    m_counter_500ms = 0;
    m_timer_id = -1;
}

void LcdService::reset(bool autostart)
{
    stopService(false);
    reinit();
    if (autostart)
        startService(true);
}

void LcdService::ResetKeypadLight()
{
    bool kepad_status;
    if(power_saving_mode || isLowPowerMode()) return;

    kepad_status = mGetKeypadLightOnOff();

    if(kepad_status)
    {
        DRV_SetKeyBacklight(1); 
        DM_KeyBackLightOpen();
    } 
    else 
    {
        DRV_SetKeyBacklight(0); 
        DM_KeyBackLightClose();
    }
}

void LcdService::setBacklightTime(unsigned int time, unsigned int dimmingDurationInPowerSavingMode, bool needTostartService)
{
    if(m_lcd_status != Sleep)
        stopService(false);

    m_counter_500ms = 0;
    m_delay_500ms_to_sleep = time;

    if(dimmingDurationInPowerSavingMode != 0)
        m_dimmingDurationInPowerSavingMode = dimmingDurationInPowerSavingMode;
    else
        m_dimmingDurationInPowerSavingMode = 10;

    if(needTostartService)
        startService(true);
}

void LcdService::backlightRestart(void)
{

    m_counter_500ms = 0;
    prevTicks = csw_TMGetTick();

    if(isLowPowerMode())
        m_counter_500ms = m_delay_500ms_to_sleep;
	
    if(m_lcd_status == Sleep)
    {
        startService(true);
    }
    else if(m_lcd_status == Dimming)
    {
        if(!power_saving_mode && !isLowPowerMode())
        {
            nguxWakeupLcd();
            m_lcd_status = Wakeup;
        }
    }
   else
   	     ResetKeypadLight();

}

void LcdService::start(void)
{
    startService(true);
}

void LcdService::startService(bool openlcd)
{
    // we are already started.
    if (m_service_status== Service_Start) {
        return ;
    }

    // register a timer
    if (m_timer_id <= 0) {
   	 	m_timer_id = registerTimer(m_timer_freq, "LcdService",TIMER_PRIORITY_HIGH);
	}
    prevTicks = csw_TMGetTick();

    // open lcd
    if (openlcd) {
    	if(power_saving_mode || isLowPowerMode())
    	{
    		nguxActive();
    		nguxHalfLightLcd();
    		if(isLowPowerMode())
    		    m_counter_500ms = m_delay_500ms_to_sleep;
    		m_lcd_status = Dimming;
    	}
    	else
    	{
    		nguxTurnOnLcd();
    		m_lcd_status = Wakeup;
    	}
    }
    m_service_status = Service_Start;

}

void LcdService::stop(void)
{
    stopService(true);
}

void LcdService::stopService(bool openlcd)
{

    // we are not started.
    if (m_service_status == Service_Stop)
    {
        return ;
    }


    // remove the timer we use
    removeTimer(m_timer_id);
    m_timer_id = -1;

    // open lcd
    if (openlcd) {
    	if(power_saving_mode || isLowPowerMode())
    	{
    		nguxActive();
    		nguxHalfLightLcd();
    		if(isLowPowerMode())
    		    m_counter_500ms = m_delay_500ms_to_sleep;
    		m_lcd_status = Dimming;
    	}
    	else
    	{
    		nguxWakeupLcd();
    		m_lcd_status = Wakeup;
    	}
    }
	    m_service_status = Service_Stop;

}

bool LcdService::handleEvent(NGUX::Event* event)
{
    TimerEvent *te = (TimerEvent *)event;

    if (NULL != event && event->eventType() == Event::TIMER && te->timerID() == m_timer_id) {
        ++m_counter_500ms;

        // sleep now
        if (m_enableAlwaysOn) //to block that lcd is off in all screen
        {
            m_counter_500ms = 0;
        }        
        else if ((m_counter_500ms == m_delay_500ms_to_sleep)&&(m_lcd_status != Dimming))
        {

                if(((csw_TMGetTick() - prevTicks) >> 14) < (m_delay_500ms_to_sleep/2))
                {
                    m_counter_500ms--;
                }
                else
                {
                    prevTicks = csw_TMGetTick();
                    nguxHalfLightLcd();
                    m_lcd_status = Dimming;
                }
        }

        if ((m_counter_500ms == (m_delay_500ms_to_sleep+10)) 
        	|| ((m_lcd_status == Dimming) && power_saving_mode && !isLowPowerMode())) {
            if(m_dimming_status) {
                m_counter_500ms = m_delay_500ms_to_sleep;
            }
            else {
                unsigned int offset = 0;
                if((m_lcd_status == Dimming) && power_saving_mode && !isLowPowerMode())
                    offset = m_dimmingDurationInPowerSavingMode;
                else
                    offset = 5;

                if(((csw_TMGetTick() - prevTicks) >> 14) < offset) {
                    m_counter_500ms--;
                }
                else {
                    stopService(false);
                    m_lcd_status = Sleep;
                    nguxSleepinLcd();
                }
            }
        }
    }

    return true; 
}

void LcdService::EnableAlwaysOn(bool enable)
{
	m_enableAlwaysOn = enable;
}

void LcdService::changeLowPowerMode(bool power_low, bool needLcdOn)
{
    //if(m_lcdLowPower == power_low) return ;
    m_lcdLowPower = power_low;

    if(m_lcdLowPower && needLcdOn)
    {                
        prevTicks = csw_TMGetTick();
        m_counter_500ms = m_delay_500ms_to_sleep;	
        m_lcd_status = Dimming;
        nguxActive();
        nguxHalfLightLcd();
    }
}

bool LcdService::isLowPowerMode(void)
{
    ChargeService* chargeService = ChargeService::getInstance();

    if(chargeService != NULL && chargeService->is_charge_connected())
        return false;

    if(chargeService != NULL  && chargeService->IS_LOW_BATTERY())
        return true;

    return m_lcdLowPower;
}

} // namespace hfcl

