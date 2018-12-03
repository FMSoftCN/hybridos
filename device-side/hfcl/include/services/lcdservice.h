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

#ifndef _HFCL_LcdService_h
#define _HFCL_LcdService_h

#include "service.h"
#include "common/event.h"
#include "services/timerservice.h"

void KeeponLightOn(bool ON);
void KeepAlwaysLightOn(bool ON);
bool getKeeponLightOn(void);
bool setKeeponLightOn(bool keeponLightOn);

namespace hfcl {

class LcdService : public Service , public TimerEventListener
{
public:
    static inline LcdService* getInstance()
    {
        if (NULL == m_singleton)
            m_singleton = HFCL_NEW_EX(LcdService, ());

        return m_singleton;
    }
    virtual ~LcdService();

    /***
     * init
     */
    void init(void);
    void reinit(void);

    /***
     * Lcd status
     */
    typedef enum __tag_LcdStatus {
        LCDON_Sleep,
        Sleep,
        Dimming,
        Wakeup,
       
    }LcdStatus;
    /***
     * Service Lstatus
     */
    typedef enum __tag_ServiceStatus {
        Service_Start,
        Service_Stop
    }ServiceStatus;

    /***
     * query current lcd status
     *
     * @return LcdStatus current lcd status
     */
    LcdStatus lcdStatus(void) {
        return m_lcd_status;
    }

    /***
     * query current service status
     *
     * @return ServiceStatus current service status
     */
    ServiceStatus serviceStatus(void) {
        return m_service_status;
    }

    /***
     *  start the service, we need to stop lcd sleep while call app , so forth.
     *
     *  @param openlcd, true to open lcd, false not
     */
    void startService(bool openlcd);
    void start(void);

    /***
     * stop the service, we need to stop lcd sleep while call app , so forth.
     */
    void stop(void);
    void stopService(bool openlcd);
    void EnableAlwaysOn(bool enalbe);

    /***
     * reset it, when we reset the service, it means we count the timer that cause lcd sleep from the very beginning.
     * 
     * @param autostart - true to start auto, false not start, just reset the counter.
     */
    void reset(bool autostart = true);


    void PowerSavingMode(bool status){
        power_saving_mode = status;
    }
    
    
    void DimmingConsecutive(bool delay)
    {
        m_dimming_status = delay;
    }

    /***
     * handleEvent interface from EventListener parent class
     */
    bool handleEvent(NGUX::Event*event);
    /* 
    **LCD backlight time setting
    */
    void ResetKeypadLight();    
    void setBacklightTime(unsigned int time, unsigned int dimmingDurationInPowerSavingMode = 0, bool needTostartService = true);
    void backlightRestart(void);
    void changeLowPowerMode(bool power_low, bool needLcdOn = true);
    bool isLowPowerMode(void);
private:
    LcdService();
    static LcdService* m_singleton;
    LcdStatus m_lcd_status;
    Uint32 m_counter_500ms;
    Uint32 prevTicks;
    Sint32 m_timer_id;
    Uint32 m_timer_freq;
    Uint32 m_delay_500ms_to_sleep;
    ServiceStatus m_service_status;
    bool power_saving_mode;
    bool m_lcdLowPower;
    bool m_dimming_status;
    bool m_enableAlwaysOn;
    Uint32 m_dimmingDurationInPowerSavingMode;
};

} // namespace hfcl

#endif //_HFCL_LcdService_h

