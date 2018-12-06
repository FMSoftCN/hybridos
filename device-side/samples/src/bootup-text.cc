/*
 ** $Id: BootupAnimate.cpp 2458 2012-01-11 08:20:38Z wangjian $
 **
 ** BootupAnimate.cpp : TODO
 **
 ** Copyright (C) 2002 ~ 2011 Beijing FMSoft Technology Co., Ltd.
 **
 ** All rights reserved by FMSoft.
 **
 ** Current Maintainer : ylwang
 **
 ** Create Date : 2011-10-24
 */
#include "ngux.h"
#include "appentries.h"
#include "cust_string_ids.h"

#include "globalcfg.h"
#include "bootupapp.h"
#include "bootup_res.h"
#include "bootupanimate.h"
#include "intent.h"
#include "tipdlgcommon.h"
#include "callapp.h"
#include "settingsecurity.h"
#include "launcher.h"
#include "bootuphandler.h"
#include "timerservice.h"

#include "hal_device.h"
#include "launcher.h"


#include "chargeservice.h"
#include "profile.h"

extern "C" {
void mci_LcdWakeup(void);
extern U8 GetBatteryLevel();
}

extern NGBool IsPlayAndVibrate(NGBool ring);

USE_NGUX_NAMESPACE
	
extern void turnOnLcd(char init);
extern void RefreshBattery(NGInt level);

BootupAnimate::BootupAnimate(Controller* owner, NGInt view_id, View* parent, NGParam param1, NGParam param2)
    : AppClient(owner, view_id, parent)
    , m_animateview(NULL)
    , m_CheckEmergency(false)
{
    m_baseView = CreateViewFromRes(R_bootup_ui_bootup_ui_animate, parent, this, NULL);
    m_timer_id = 0;
    init(param1, param2);
}

BootupAnimate::~BootupAnimate()
{
	if (m_timer_id != 0)
	{
	    removeTimer(m_timer_id);
	}
}

void BootupAnimate::active(void)
{
    AppClient::active();
	if(m_CheckEmergency)
	{
		m_CheckEmergency = false;
		checkSimLock();
	}
}

void BootupAnimate::inactive(void)
{
	_DBG_PRINTF ("BootupAnimate::inactive(void)");
    AppClient::inactive();
}

void BootupAnimate::init(NGParam param1, NGParam param2)
{
    //NGInt id = ReadByteValue(NVRAM_SETTINGS_POWER_ON_ANIM);

	// FIXME max limits // use default ani
    //if (id < 0 || id > 1) id = 0;
    
	m_animateview->setGifAnimate(GetGifAnimateRes(R_bootup_img_bootup_img_animate));
#ifdef __MMI_SAMSUNG_GT_FEATURE__	
	m_timer_id = registerTimer (10, "BootupAnimate", TIMER_PRIORITY_HIGH);
	m_timer_CheckPhoneLock_flag = false;
#endif

#ifdef __DISPLAY_POWERON_LOGO__
	turnOnLcd(0);
#else
	mci_LcdWakeup();
	DRV_SetKeyBacklight(1); 
	DM_KeyBackLightOpen();
#endif
}

NGUInt BootupAnimate::onControllerCommand(NGUInt cmd_id, NGParam param1, NGParam param2)
{
	switch (cmd_id) 
	{
    	case BOOTUP_CMD_ANIMATE_RESTART: 
    	{
            KeeponLightOn(true); //block to enter dimming on boot
            if(BootupApp::getCurrInstance()->getBootType() != 2 && !BootupApp::getCurrInstance()->isPowerOnPlayed())
            {
                if(IsPlayAndVibrate(true))
                    Profile::getInstance()->startPlayAudio(POWER_ON_TONE);
            }
    	    m_first_image->hide();
    	    m_end_iamge->hide();
            m_animateview->resume();
        }
    	break;
    	
    	case BOOTUP_CMD_BACKGROUND_OFF:
    	{
    		m_end_iamge->hide();
    	}
    	break;
        
    	case BOOTUP_CMD_BACKGROUND_ON:
    	{
    		m_end_iamge->show();
    	}
    	break;
    	
    	default: break;
	}
    return 0;
}

void BootupAnimate::onMenuCommand(CustomEvent* event)
{
    if (event->customWparam() != Menu::CUSTOM_MENU_KEY_SL) return;
/*
    NGInt id = ((CustomEvent *)event)->customLparam();

    switch (id) 
    {
        default: break;
    }
*/
}

void BootupAnimate::stopAnimate(void)
{
    if(ChargeService::getInstance()->is_CheckedLowBatteryOnPowerOn()) {
        m_end_iamge->show();
    }
    BootupApp::getCurrInstance()->checkSimLock();	
    //sendCommand(BOOTUP_CMD_ANIMATE_FINISH, 0, 0);
}

NGBool BootupAnimate::onGifAnimateNotify(CustomEvent* event)
{
    if (event->customWparam() != CustomEvent::CUS_GIFANIMATE_STOP)
        return false;
#ifndef __MMI_SAMSUNG_GT_FEATURE__	
	if(((BootupApp *)m_owner)->getTopAppClient()->getId() == BOOTUP_CLIENT_POWREON_KEY)
	{
        SIM_Status_Init();
        if (Is_All_SIM_Init() || !Has_SIM_Card_Insert()) {
            BootupApp::getCurrInstance()->CheckPhoneLockPass();
        }
        else {
            m_animateview->start();
        }
	}
	else
#endif

	stopAnimate();
    return true;
}

void BootupAnimate::checkSimLock(void)
{
    NGInt passwdValue = 0;
    NGChar* editbuf = ((BootupApp *)m_owner)->getEditDlgBuf();
    
#ifdef ENABLE_SETTINGS_SECURITY
    if(SecurityVerify::getInstance()->isSimLockNeedVerify(0))
#else
    if(0)
#endif
    {
#ifdef ENABLE_NVRAM
    	NGInt16 error = 0;
		NGChar password[SETTINGS_SIM_PASSWORD];
		ReadRecord(NVRAM_SETTINGS_SIM_PASSWORD, 0, password, SETTINGS_SIM_PASSWORD, &error);
#endif
		m_end_iamge->show();
        showMenuBar(true);
        
        do
        {
            RefreshLcdService();
            NGInt ret = showModalView(BOOTUP_CLIENT_EDITDLG_PASSWD, (NGParam)GetTextRes(SS_SIM_LOCK), 8);
            if(ret == TipDlg::TIP_OK)
            {
                RefreshLcdService();
                if(strlen(editbuf) < 4)
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_PASSWORD_TOO_SHORT), TIP);
                    passwdValue = BOOTUP_PASSWD_WRONG;
                }
                else if(strcmp(editbuf, password))
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_WRONG_PASSWORD), TIP);
                    passwdValue = BOOTUP_PASSWD_WRONG;
                }
                else // passwd is right
                    passwdValue = BOOTUP_PASSWD_RIGHT;
            }
            else if(ret == TipDlg::TIP_CANCEL) { 
                RefreshLcdService();
                passwdValue = BOOTUP_PASSWD_WRONG;
                // press cancel or endkey
                NGInt ret = showModalView(BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY, (NGParam)GetTextRes(STR_DIAL_EMERGENCY_NUMBER), YESNODLG); // yes or no?
                if (ret == TipDlg::TIP_OK) 
                {
                    // call emergency
                    RefreshBattery(GetBatteryLevel());
                    Intent intent(CallApp::START_BY_EMERGENCY, "", BOOTUP_FROM_SIMLOCK);
                    //Intent intent(CallApp::START_BY_EMERGENCY, "", 0);
                    m_CheckEmergency=true;
                    editbuf[0] = '\0';
                    EntryCall(&intent);
                    break;
                }
            }
            else if(ret == TipDlg::TIP_CANCEL) {
                // do nothing!
            }

            editbuf[0] = '\0';

        } while(!passwdValue);

        showMenuBar(false);
		m_end_iamge->hide();
		sendCommand(BOOTUP_CMD_MODEBAR_HIDE, 0, 0);
		
        if(BOOTUP_PASSWD_RIGHT == passwdValue)
        {
#ifdef __MMI_SAMSUNG_GT_FEATURE__
			onControllerCommand(BOOTUP_CMD_ANIMATE_RESTART, 0, 0); 
#else
			sendCommand(BOOTUP_CMD_ANIMATE_FINISH, 0, 0);
#endif
        }
    }
    else
    {
#ifdef __MMI_SAMSUNG_GT_FEATURE__
		onControllerCommand(BOOTUP_CMD_ANIMATE_RESTART, 0, 0); 
#else
		sendCommand(BOOTUP_CMD_ANIMATE_FINISH, 0, 0);
#endif

    }
}

NGBool BootupAnimate::onKey(NGInt keyCode, KeyEvent* event)
{
	//stopAnimate();
	switch(event->eventType())
	{
		case Event::CUSTOM_NOTIFY:
			m_animateview->pause();
			m_first_image->show(); 
			BootupApp::getCurrInstance()->CheckPhoneLockPass();
			break;
		default :
			break;
	}
	return DISPATCH_CONTINUE_MSG;
}


bool BootupAnimate::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER && m_timer_id == ((TimerEvent *)event)->timerID())
	{
#ifdef __MMI_SAMSUNG_GT_FEATURE__	
		if(((BootupApp *)m_owner)->getTopAppClient() == NULL) return false;

		NGInt id = ((BootupApp *)m_owner)->getTopAppClient()->getId();
		if((id == BOOTUP_CLIENT_POWREON_KEY)&& (m_animateview->gifAnimate()->getNextFrameIndex() >= 0))
		{
			if((m_animateview->gifAnimate()->getNextFrameIndex() == 0) && (!m_timer_CheckPhoneLock_flag))
			{
				if (m_timer_id != 0)
				{
				    removeTimer(m_timer_id);
					m_timer_id = 0;
				}
				m_timer_id = registerTimer(/*m_animateview->gifAnimate()->currentFrameDelay()*2/3*/10, "BootupAnimate",TIMER_PRIORITY_HIGH);
				m_timer_CheckPhoneLock_flag = true;
			}
			else
			{
#ifdef __MMI_SAMSUNG_GT_FEATURE__
				if(1)
#else
		        SIM_Status_Init();
                if(Is_All_SIM_Init() || !Has_SIM_Card_Insert() )
#endif
		        {
		            if (m_timer_id != 0)
				    {
					    removeTimer(m_timer_id);
					    m_timer_id = 0;
				    }
//					m_animateview->pause();
					//checkSimLock();
//		            BootupApp::getCurrInstance()->CheckPhoneLockPass();
					PostMessage(((BootupApp*)m_owner)->viewWindow(),MSG_USER,0,0);
		        }
			}
		}
#endif
	}

    return DISPATCH_CONTINUE_MSG;
}


BEGIN_SETVIEW(BootupAnimate)
    MAP_VIEW(m_animateview, R_bootup_var_bootup_view_animate_view, GifAnimateView)
    MAP_VIEW(m_end_iamge, R_bootup_var_bootup_img_animate_end_view, ImageView)
    MAP_VIEW(m_first_image, R_bootup_var_bootup_img_animate_first_view, ImageView)    
END_SETVIEW


BEGIN_GETHANDLE(BootupAnimate)
    MAP_HANDLE(BootupAnimate, R_bootup_var_bootup_event_gifanimate_view, onGifAnimateNotify)
END_GETHANDLE

