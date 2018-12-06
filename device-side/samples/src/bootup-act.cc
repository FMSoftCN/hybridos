/*
** $Id: BootupApp.cpp 2515 2012-01-13 09:25:51Z ylwang $
**
** BootupApp.cpp : TODO
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
#include "appmanager.h"
#include "appentries.h"
#include "cust_string_ids.h"

#include "bootupapp.h"
#include "bootupanimate.h"
#include "bootupgreet.h"
#include "chargeranimate.h"
//#include "alarmanimate.h"
#include "chargerunplug.h"
#include "chargerfull.h"
#include "bootupgreet.h"
#include "bootup_res.h"
#include "msgdlg.h"
#include "tipdlgcommon.h"
#include "globalcfg.h"
//#include "launcher_res.h"
#include "profile.h"
#include "alarmhandler.h"
#include "intentaction.h"
#include "settingsecurity.h"
#include "callapp.h"
#include "launcher.h"
#include "bootuphandler.h"
#include "logo.h"
#include "chargeservice.h"
#include "bootuptestusim.h"
#include "bootupsimname.h"

#ifdef ENABLE_FACTORYMODESAMSUNG
#include "factorymodeframe.h"
#endif

#include "device.h"
#include "mci_lcd.h"
#include "vibrate.h"

#include "cfw.h"
extern NGBool Has_SIM_Card_Insert(void);
NGUX_BEGIN_EXTERN_C
extern U8 PinEnterSimID;
extern U8 g_SimSuccessIndBuffered[MMI_SIM_NUMBER];
extern void VibratePlayOnce();
extern U8 GetBatteryLevel();
extern void SendMessageToCswForPowerOn();
#ifdef __MMI_SAMSUNG_GT_FEATURE__
extern void VibratePlayDelayOnce();
extern void VibratePlayOnce_oneTime();
#endif
extern VOID cfw_IMSItoASC(UINT8 *InPut,UINT8 *OutPut, UINT8 *OutLen);
extern BOOL nguxAutoFFSTest;
extern U8 GetHandsetInPhone();
NGUX_END_EXTERN_C

extern void setBootUpType(NGInt val);	
extern void registerCommunicationHandlers();

extern void turnOnLcd(char init);
extern void RefreshBattery(NGInt level);

BootupApp* BootupApp::instance = NULL;

BootupApp::BootupApp()
{
#ifdef __CALENDAR_ARABIC__
    NGInt dateSystem;
#endif
    instance = this;
    m_audioplayer = NULL;

    for (NGInt i = 0; i < MMI_SIM_NUMBER; i++) {
        m_fNeedCheckPIN[i] = false;
        m_fNeedCheckPUK[i] = false;
    }
    m_fPhoneLockChecking = true;
    m_LockCheck=false;
    m_isSOSCalling = false;
    m_PinFlag = false;
    m_PUKBlock = false;
    m_fNeedChekSim = false;
    memset(m_editDlgText, 0, sizeof(m_editDlgText));
    setTheme();

#ifdef __CALENDAR_ARABIC__
    dateSystem = ReadByteValue(NVRAM_SETTINGS_DATE_SYSTEM);
    TM_SetDateSystem((UINT8)dateSystem);
#endif

#if defined(ENABLE_NVRAM)
    if(ReadByteValue(NVRAM_AUTO_FFS_TEST) == 1)
        nguxAutoFFSTest = true;
    else
        nguxAutoFFSTest = false;
#endif

    Profile::getInstance()->blockKeyTone(TRUE);
    mIsPowerOnPlayed = false;
}

BootupApp::~BootupApp()
{
    instance = NULL;
    Profile::getInstance()->blockKeyTone(FALSE);
    UnregisterResPackage(RES_PKG_BOOTUP_ID);
}

void BootupApp::onCreate(ContextStream* contextStream, Intent* intent)
{
    FRRegister_bootup_resource();
    AppWithBar::onCreate(contextStream, intent);
    setFullScreen(true, true);
    
    boot(intent->action());
}

void BootupApp::onWakeup() 
{
	AppWithBar::onWakeup();

	// coming back from call ( SOS calling )
	if (m_isSOSCalling) {
		m_isSOSCalling = false;
		
		if(m_fPhoneLockChecking) {
			onClientCommand(0, BOOTUP_CMD_NEED_PNONELOCK, 0, 0); 
		}
        else if(m_PUKBlock) {
			onClientCommand(0, BOOTUP_CMD_PUK_BLOCK, 0, 0); 
        }
        else if(m_fNeedCheckPUK[0]) {
			onClientCommand(0, BOOTUP_CMD_NEED_PUK, 0, 0); 
        }
        else if(m_fNeedChekSim) {
        	onClientCommand(0, BOOTUP_CMD_NEED_SIMLOCK, 0, 0); 
        }
		else {
			for(NGInt i = 0; i < MMI_SIM_NUMBER; i++) {
				if(m_fNeedCheckPIN[i]) {
					onClientCommand(0, BOOTUP_CMD_NEED_PIN, 0, 0);
					break;
				}
				else if(m_fNeedCheckPUK[i]) {
					showErrorMsgDlg(0);
					onClientCommand(0, BOOTUP_CMD_NEED_PUK, 0, 0);
					break;	
				}
			}	
		}
	}

	AppClient* top_app = (AppClient*)getTop(0);
	if( top_app != NULL )
		top_app->active();
	
}

NGBool IsPlayAndVibrate(NGBool ring)
{
    unsigned char activeProfile = getActiveProfileMode();
    NGInt callAlertType = Profile::getInstance()->getCallAlertType();

    switch(activeProfile)
    {
        case PROFILE_SILENT:
            if(ring)
            {
                return false;
            }
            else
            {
                if(callAlertType == 1)
                    return false;
            }
            break;
        case PROFILE_NORMAL:
        case PROFILE_DRIVING:
        case PROFILE_MEETING:
        case PROFILE_OUTDOOR:
        case PROFILE_MY_PROFILE1:
        case PROFILE_MY_PROFILE2:
            if(ring)
            {
                if(callAlertType == 6 || callAlertType == 2)
                    return false;
            }
            else
            {
                if(callAlertType == 6)
                    return false;
            }
            break;
        default:
            break;
    }

    return true;
}

void BootupApp::boot(NGInt bootType)
{
    m_bootType = bootType;

    if(bootType >= 0 && bootType <= POWER_ON_UNINIT) {
		setBootUpType(bootType);
#ifdef __NGUX_PLATFORM__
	 	if((bootType == POWER_ON_KEYPAD) || (bootType == POWER_ON_EXCEPTION)) {
			registerCommunicationHandlers();
			CallAdp *calIns = CallAdp::getCallAdpInstance();
	    	if (NULL != calIns)
			{
				calIns->InitCallHandlers();
			//	calIns->OpenStackForCallApplication();
	    	}
	 		SendMessageToCswForPowerOn(); 				
	 	}
#endif
	}

    // check earphone 
    CheckEarphone();
	
    // Bootup Step 1 : Animate 
    EnterAnimate (bootType);
    
    // Bootup Step 1 : Music 
#ifdef __MMI_SAMSUNG_GT_FEATURE__
    if (bootType != BOOTUP_CHARGER_IN) {
        if(IsPlayAndVibrate(false))
            VibratePlayOnce_oneTime();

        if(ReadByteValue(NVRAM_SETTINGS_PHONE_LOCK) != 0 && ReadByteValue(NVRAM_SETTINGS_PIN_LOCK) != 0)
        {
            mIsPowerOnPlayed = true;
            if(IsPlayAndVibrate(true))
                PlayBootupTone();
        }
    }
#else	
    if(bootType==0)
        PlayBootupTone();
#endif

    /* VincentWei: The chance for the apps to load data from NVRAM here */
    AppManager::getInstance()->onBoot ();
}

void BootupApp::EnterAnimate(NGInt bootType)
{
    switch(bootType)
    {
        case BOOTUP_KEY: 
            showView(BOOTUP_CLIENT_POWREON_KEY, 0, 0);
            break;  
        case BOOTUP_ALARM:
            turnOnLcd(2);
            EnterAlarm();
            break;  
        case BOOTUP_CHARGER_IN: 
            showView(BOOTUP_CLIENT_POWREON_CHARGEON, 0, 0);
            break;  
        case BOOTUP_USB:          //TODO POWER_ON_USB
 	     	showView(BOOTUP_CLIENT_POWREON_CHARGEON, 0, 1);
            break; 	
        case BOOTUP_UNINIT:       //TODO POWER_ON_UNINIT
        case BOOTUP_EXCEPTION:    //TODO POWER_ON_EXCEPTION
        default: 
            showView(BOOTUP_CLIENT_POWREON_KEY, 0, 0);  
            break;  
    }
}

void BootupApp::showChargerUnplug(NGInt level)
{
	AppWithBar::showModalView(BOOTUP_CLIENT_UNPLUGCHARGER, level, 0);
}

void BootupApp::showChargerFull(void)
{
	AppWithBar::showModalView(BOOTUP_CLIENT_FULL, 0, 0);
}

void BootupApp::onPowerOnAlarm(void)
{
    NguxTime *  sysTime = NguxTime::getSysTime();
    NGChar      AlarmTimeStr[10];

    mci_LcdWakeup();

    exit();

    VibratePlayOnce();

    sprintf(AlarmTimeStr, "%02d:%02d",sysTime->hour, sysTime->minute);
    showMsgTextDlg((NGCPStr)AlarmTimeStr, BOOT_ALARM_ALERT, ICON_ALARM_ALERT , 0, 60);
}

void BootupApp::EnterGreetwords(void)
{
    showView(BOOTUP_CLIENT_GREET, 0, 0);
}

void BootupApp::EnterSIMName(void)
{
    showView(BOOTUP_CLIENT_SIMNAME, 0, 0);
}

void BootupApp::EnterPhonelock(void)
{
    //TODO
}

void BootupApp::EnterTestUsim(void)
{
    showView(BOOTUP_CLIENT_TEST_USIM, 0, 0);
}

NGBool BootupApp::isTestUsimInserted(void)
{
#if defined(ENABLE_NVRAM)
    UINT8 tmp[16] = {0,};
    UINT8 data[16] = {0,};
    UINT8 outLen = 0;
    UINT8 value = 0;

    if(Has_SIM_Card_Insert()) {
        CFW_CfgGetIMSI(tmp, CFW_SIM_0);
        cfw_IMSItoASC(tmp, data, &outLen);

        if(strcmp((char*)data, "999999999999999") == 0) {
            value = 1;
            WriteByteValue(NVRAM_INSERTED_TEST_USIM, value);
#ifdef ENABLE_FACTORYMODESAMSUNG
            FactorymodeFrameSamsung::factoryMode_SetTestDone(8);            
#endif
            return true;
        }
    }

    WriteByteValue(NVRAM_INSERTED_TEST_USIM, value);
#ifdef ENABLE_FACTORYMODESAMSUNG
    FactorymodeFrameSamsung::factoryMode_SetTestDone(8);
#endif
#endif
    return false;
}

void BootupApp::showPUKblock(void)
{
	NGUInt ret1;
	
	do {
		getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
		ret1 = BootupApp::getCurrInstance()->showMsgTextDlg(GetTextRes(SS_PUK_BLOCKED), SOSDLG, ICON_ERROR, 0);
		showMenuBar(false);
		getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_OFF, 0, 0); 

		if(ret1 == TipDlg::TIP_SOS) {
			getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
			NGInt ret2 = BootupApp::getCurrInstance()->showModalView(BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY, (NGParam)GetTextRes(SS_EMERGENCY_CALL_Q), YESNODLG);
    		getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_OFF, 0, 0); 

			if (ret2 == TipDlg::TIP_OK) {
				BootupApp::getCurrInstance()->m_isSOSCalling = true;
                BootupApp::getCurrInstance()->m_PUKBlock = true;
				RefreshBattery(GetBatteryLevel());
				Intent intent(CallApp::START_BY_EMERGENCY, "", BOOTUP_FROM_PUKLOCK);
				EntryCall(&intent);
                break;
			 }
		}
		
	} while(1);
}

NGUX_BEGIN_EXTERN_C
extern void  AT_SendBootupMessages();
NGUX_END_EXTERN_C

void BootupApp::EnterDesktop(void)
{
    CheckEarphone();
    exit();

   	AT_SendBootupMessages();

    EntryLauncher(NULL);
}

void BootupApp::EnterAlarm(void)
{
    NGUInt16 temp;
    NguxTime *sysTime = NguxTime::getSysTime();
	
    temp= ((sysTime->hour & 0x0FF)<<8) | (sysTime->minute & 0x0FF);	
    CheckEarphone();
    exit();

    Intent intent(ACTION_POWERON_ALARM,0,temp);
    EntryAlarm( &intent);
}

NGBool BootupApp::onKey(NGInt keyCode, KeyEvent* event)
{	  
	if ((event->eventType() == Event::KEY_LONGPRESSED)&&(keyCode==KeyEvent::KEYCODE_STOP)&& (m_LockCheck==true)) {
		EntryShutDown(NULL);
		return false;
	}
	else {
		return AppWithBar::onKey(keyCode,  event);
	}
}

void BootupApp::callback(void* obj, void* data, NGInt data_bytes, NGInt type, NGBool result, NGInt reason)
{
    BootupApp* ins = (BootupApp *)obj;
    // PIN1
    if (VRF_PIN1 == type)
    {
        if (!result && (VRF_USR_CANCEL == reason || VRF_NO_INPUT == reason||VRF_USER_EXITHOME == reason)) 
        {
            if(VRF_NO_INPUT == reason)
            {
	            ins->showModalView(BOOTUP_CLIENT_TEXTDLG_NOINPUT_TIP, (NGParam)GetTextRes(SS_PIN_TOO_SHORT), TIP);
                ins->onClientCommand(0, BOOTUP_CMD_NEED_PIN, 0, 0);
            }
            else if(VRF_USER_EXITHOME == reason)
            {
                ins->onClientCommand(0, BOOTUP_CMD_NEED_PIN, 0, 0);
            }
            else /*SOS*/
            {
                NGInt ret = ins->showModalView(BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY, (NGParam)GetTextRes(SS_EMERGENCY_CALL_Q), YESNODLG); // yes or no?
			    
                if (ret == TipDlg::TIP_OK) 
                {
                    ins->m_isSOSCalling = true;
                    RefreshBattery(GetBatteryLevel());
                    Intent intent(CallApp::START_BY_EMERGENCY, "", BOOTUP_FROM_PINLOCK);
                    EntryCall(&intent);
                    return;
                }
                else if((ret == TipDlg::TIP_CANCEL) || (ret == TipDlg::TIP_DESTROY))
                {
                    ins->onClientCommand(0, BOOTUP_CMD_NEED_PIN, 0, 0);
                }
            }
        }
    }
    else if (VRF_PUK1 == type) 
    {
        if (!result && (VRF_USR_CANCEL == reason || VRF_NO_INPUT == reason))
        {
            if(VRF_NO_INPUT == reason)
            {
                ins->showModalView(BOOTUP_CLIENT_TEXTDLG_NOINPUT_TIP, (NGParam)GetTextRes(SS_PUK_TOO_SHORT), TIP);
                ins->onClientCommand(0, BOOTUP_CMD_NEED_PUK, 0, 0);
            }
            else/*SOS*/
            {
                NGInt ret = ins->showModalView(BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY, (NGParam)GetTextRes(SS_EMERGENCY_CALL_Q), YESNODLG); // yes or no?
                if (ret == TipDlg::TIP_OK) 
                {
                    // call emergency
                    RefreshBattery(GetBatteryLevel());
                    Intent intent(CallApp::START_BY_EMERGENCY, "", BOOTUP_FROM_PUKLOCK);
                    EntryCall(&intent);
                    return;
                }
                else if((ret == TipDlg::TIP_CANCEL)||(ret == TipDlg::TIP_DESTROY))
                    ins->onClientCommand(0, BOOTUP_CMD_NEED_PUK, 0, 0);
            }                
        }
    }
    
    if (result) {
        ins->onClientCommand(0, BOOTUP_CMD_VRFD_PIN, 0, 0);
    }
}


NGUInt BootupApp::onClientCommand(NGInt sender, NGUInt cmd_id, NGULong param1, NGULong param2)
{
    NGUInt i;
	AppManager *am = AppManager::getInstance();	
    AppClient* top = ((AppWithBar*)am->getCurrentApp())->getTopAppClient();
	AppClient *_topBooting = BootupApp::getCurrInstance()->getTopAppClient();
	
    if (top == NULL || _topBooting == NULL)
        return 0;
		
    switch (cmd_id) 
    {
        case BOOTUP_CMD_MODEBAR_HIDE:
            _topBooting->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
            //setFullScreen(true, false);
            showMenuBar(false);
        	break;

        case BOOTUP_CMD_MODEBAR_SHOW:
            //setFullScreen(true, true);
            _topBooting->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
            showMenuBar(true);
        	break;
        
        // pin need while bootup
        case BOOTUP_CMD_NEED_PIN: {
            m_LockCheck = true;
            RefreshLcdService();
            top->onControllerCommand(BOOTUP_CMD_NEED_PIN, 0, 0);
#ifdef ENABLE_SETTINGS_SECURITY
            SecurityVerify::getInstance()->setFromBootLock(true);
#endif
            m_PinFlag = true;

            RefreshLcdService();
#ifdef ENABLE_SETTINGS_SECURITY
            VerifyPassPin(VRF_PIN1, this, (VerifyCb)&callback);
#endif

            break;
        }
        
        // puk need while bootup
        case BOOTUP_CMD_NEED_PUK: {
            //setFullScreen(true, false);
            showMenuBar(true);
            _topBooting->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
            
            RefreshLcdService();
#ifdef ENABLE_SETTINGS_SECURITY
            SecurityVerify::getInstance()->setFromBootLock(true);
#endif

            RefreshLcdService();
#ifdef ENABLE_SETTINGS_SECURITY
            VerifyPassPin(VRF_PUK1, this, (VerifyCb)&callback);
#endif

            break;
        }

        case BOOTUP_CMD_VRFD_PIN: {
            //setFullScreen(true, false);
            showMenuBar(true);
            _topBooting->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
        
            RefreshLcdService();
#ifdef ENABLE_SETTINGS_SECURITY
            SecurityVerify::getInstance()->setFromBootLock(false);
#endif
			
    		for(i = 0; i < MMI_SIM_NUMBER; i++) {
    			if(g_SimSuccessIndBuffered[i]) {
    				CodeVerifiedPopupCallback();
    				break;
    			}
    		}
    		
#ifdef __MMI_SAMSUNG_GT_FEATURE__	
            showMenuBar(false);
            _topBooting->onControllerCommand(BOOTUP_CMD_BACKGROUND_OFF, 0, 0); 
    		_topBooting->onControllerCommand(BOOTUP_CMD_ANIMATE_RESTART, 0, 0); 
#else
    		onClientCommand(0, BOOTUP_CMD_ANIMATE_FINISH, 0, 0); 
#endif
            break;
        }

        case BOOTUP_CMD_VRFD_PUK:
#ifdef ENABLE_SETTINGS_SECURITY
            SecurityVerify::getInstance()->setFromBootLock(false);
#endif
            break;

        case BOOTUP_CMD_ANIMATE_FINISH: {
#ifndef __MMI_SAMSUNG_GT_FEATURE__		
            StopBootupTone();
#endif
	        //////////////////// Bootup Step 2 : Greet Words /////////////////
            // open or close : greet words
#ifdef ENABLE_NVRAM
            NGBool bOpen = ReadByteValue(NVRAM_SETTINGS_GREETWORDS_STATE) == 0 ? false : true;
			if (bOpen) {
				XLOGD("BOOTUP_CMD_ANIMATE_FINISH|greetwords is open\n");
			}
#endif

            if (ChargeService::getInstance()->is_CheckedLowBatteryOnPowerOn()) {
                LcdService* lcd_service = NULL;
                lcd_service = LcdService::getInstance();
                if (NULL != lcd_service) {
                    lcd_service->changeLowPowerMode(true);
                }
                //Profile::getInstance()->startPlayAudio(BATTERY_LOW_TONE);
                showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_LOW_BATTERY), TIP);
            }
            
            setFullScreen(true, true);
            showMenuBar(false);

            if(isTestUsimInserted())
                EnterTestUsim();
            else
                //EnterGreetwords();
                EnterDesktop();
            break;
        }

        case BOOTUP_CMD_ALARM_ANIMATE_FINISH: {
            EnterAlarm();
            break;
        }

        case BOOTUP_CMD_GREET_FINISH:
            ///////////////////// Bootup Step 3 : Show SIMName ////////////////
            if(Has_SIM_Card_Insert())
                EnterSIMName();
            else                
                EnterDesktop();
            break;

        case BOOTUP_CMD_SIMNAME_FINISH:
            ///////////////////// Bootup Step 4 : Show Desktop ////////////////
            EnterDesktop();
            break;

        // adp event : save bootup battery
        case BOOTUP_CMD_BOOTUP_BATTERY:
            if(top->getId() == BOOTUP_CLIENT_POWREON_CHARGEON)
                top->onControllerCommand( cmd_id,  param1, param2);
            break;

        case BOOTUP_CMD_NEED_PNONELOCK:            
          //  top->onControllerCommand(BOOTUP_CMD_NEED_PNONELOCK, 0, 0);
            CheckPhoneLockPass();
            break;

        case BOOTUP_CMD_VRFD_PNONELOCK:
           // top->onControllerCommand(BOOTUP_CMD_VRFD_PNONELOCK, 0, 0);
            break;

        case BOOTUP_CMD_PUK_BLOCK:
            showPUKblock();
            break;

        case BOOTUP_CMD_NEED_SIMLOCK:
            checkSimLock();
            break;

        default:
            break;
    }

    return 0;
}

void BootupApp::StopBootupTone(void)
{
    Profile::getInstance()->stopPlayAudio();
}

void BootupApp::PlayBootupTone(void)
{
    Profile::getInstance()->startPlayAudio(POWER_ON_TONE);
}

void BootupApp::CheckEarphone(void)
{
    if(GetHandsetInPhone()) {    
        Profile::getInstance()->enterEarPhoneOrBlueToothMode();
	 	//ShowEarphone(true);	
    } else {
        Profile::getInstance()->exitEarPhoneOrBlueToothMode();
	 	//ShowEarphone(false);	
    }
}

NGUInt BootupApp::showMsgTextDlg(NGCPStr text_info, NGUInt type, NGInt icon_id, NGInt text_style, NGInt second)
{
    MsgTextDlgCreateInfo dlgcreate = {
        text_info,
        second,
        icon_id,
        type,   
        (NGBool)text_style,
        UNIT_1_SECOND
    };  
    return showTipView(TIPDLGAPP_CLIENT_ID_TEXTDLG, (NGParam )&dlgcreate, 0);
}

NGUInt BootupApp::showMsgEditDlg(NGCPStr text_info, NGInt length)
{
	MsgTextEditDlgCreateInfo dlgcreate = {
        text_info,
        NULL,
        1,
#ifdef __MMI_T9__ 
        false,
#endif
        OKSOSDLG,
        m_editDlgText,
        length,
        false
    };
	return showTipView(TIPDLGAPP_CLIENT_ID_TEXTEDITDLG, (NGParam )&dlgcreate, 0);
}
    
NGUInt BootupApp::showModalView(NGInt view_id, NGParam param1, NGParam param2)
{
    switch(view_id)
    {
        case BOOTUP_CLIENT_TEXTDLG_PASSWD_PASS_TIP:
            return showMsgTextDlg((NGCPStr)param1, param2, ICON_CHECKING, 0, 1);
        
        case BOOTUP_CLIENT_TEXTDLG_INFO_TIP:
            return showMsgTextDlg((NGCPStr)param1, param2, ICON_ERROR, 0, 2);
            
        case BOOTUP_CLIENT_EDITDLG_PASSWD:
            return showMsgEditDlg((NGCPStr)param1, param2);

        case BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY:
            return showMsgTextDlg((NGCPStr)param1, param2, ICON_QUESTION);

        case BOOTUP_CLIENT_TEXTDLG_NOINPUT_TIP:
            return showMsgTextDlg((NGCPStr)param1, param2, ICON_ERROR, 0, 1);

        case BOOTUP_CLIENT_TEXTDLG_PASSWD_ERROR_TIP:
            return showMsgTextDlg((NGCPStr)param1, param2, ICON_ERROR, 0, 1);
            
        default:
            break;
    }
    return AppWithBar::showModalView( view_id, param1, param2);
}

void BootupApp::setTheme()
{
    /* VincentWei: Always set Retro theme for Nokia */
    GetSystemPackage()->setTheme(R_sys_theme_Retro);			
    return;

    NGInt theme = ReadByteValue(NVRAM_SETTINGS_THEME);
    if (0 == theme) {
        GetSystemPackage()->setTheme(R_sys_theme_Emerald);
    }
    else if (1 == theme) {
        GetSystemPackage()->setTheme(R_sys_theme_Skyline);
    }
    else if (2 == theme) {
        GetSystemPackage()->setTheme(R_sys_theme_Ocean);			
    }
}

void BootupApp::showErrorMsgDlg(NGInt count)
{
    showMenuBar(true);
    
    switch(count)
    {
        case 0:/*PIN blocked*/
            showModalView(BOOTUP_CLIENT_TEXTDLG_PASSWD_ERROR_TIP, (NGParam)GetTextRes(SS_PIN_LOCKED), TIP);
            break;        
        case 1:/*1 attempt left"*/
        case 2:/*2 attempts left"*/
            {
            	char msg1[100] = {0,};
            	char msg2[100] = {0,};
            	
				strcpy(msg1, GetTextRes(SS_INCORRECT_PIN));
				
				if(count==1)
            	    sprintf(msg2, GetTextRes(SS_PD_1_ATTEMPTS_LEFT), count);
            	else
            	    sprintf(msg2, GetTextRes(SS_PD_ATTEMPTS_LEFT), count);
            	
				strcat(msg1, "\n");
            	strcat(msg1, msg2);
            	
                showModalView(BOOTUP_CLIENT_TEXTDLG_PASSWD_ERROR_TIP, (NGParam)msg1, TIP);
            }
            break;
            
        default:
            break;
    }

    showMenuBar(false);
}

void BootupApp::CheckPhoneLockPass(void)
{
    _DBG_PRINTF ("BootupApp :: PhoneLock");
    NGInt passwdValue = 0;
    NGChar* editbuf = getEditDlgBuf();

#if defined(ENABLE_NVRAM)
    BOOL bFound = FALSE;
//    NGInt nSimId = 0;
 /*   nSimId =*/ MTPNP_AD_get_protocol_event_src_mode();
	if(ReadByteValue(NVRAM_SETTINGS_PHONE_LOCK)== 0) /* 0 is locked, 1 is unlock */
#endif
    {        
		NGChar password[SETTINGS_SIM_PASSWORD];
		m_fPhoneLockChecking = true;
		m_LockCheck=true;
		NGInt16 error = 0;
		ReadRecord(NVRAM_SETTINGS_PASSWORD, 0, password, SETTINGS_SIM_PASSWORD, &error);

        showMenuBar(true);
        getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
		
        do
        {
            RefreshLcdService();
            // "Phone password" change to "Phone lock"			
            NGInt ret = showModalView(BOOTUP_CLIENT_EDITDLG_PASSWD, (NGParam)GetTextRes(SS_PHONE_LOCK), 8);
            if(ret == TipDlg::TIP_OK)
            {
                RefreshLcdService();
                if(strlen(editbuf) < 4)
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_PASSWORD_TOO_SHORT), TIP_WARNING);
                    passwdValue = BOOTUP_PASSWD_WRONG;
                }
                else if(strcmp(editbuf, password))
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_WRONG_PASSWORD), TIP_WARNING);
                    passwdValue = BOOTUP_PASSWD_WRONG;
                }
                else // passwd is right
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_PASSWD_PASS_TIP, (NGParam)GetTextRes(SS_PHONE_LOCK_PASSED), TIP);
                    passwdValue = BOOTUP_PASSWD_RIGHT;
                }
            }
            else if(ret == TipDlg::TIP_CANCEL) //SOS call
            { 
                passwdValue = BOOTUP_PASSWD_WRONG;
                RefreshLcdService();
                // press cancel or endkey
                ret = showModalView(BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY, (NGParam)GetTextRes(SS_EMERGENCY_CALL_Q), YESNODLG); // yes or no?
                if (ret == TipDlg::TIP_OK) 
                {
                    // call emergency
                    m_isSOSCalling = true;
                    RefreshBattery(GetBatteryLevel());
                    Intent intent(CallApp::START_BY_EMERGENCY, "", BOOTUP_FROM_PHONELOCK);
                    editbuf [0] = '\0';
                    EntryCall(&intent);
                    return;
                }
            }

            editbuf [0] = '\0';
        }
        while(!passwdValue);

        showMenuBar(false);
        getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_OFF, 0, 0); 
    }

  	m_fPhoneLockChecking = false;
  	m_fNeedCheckPUK[0] = false;
  	

  	if(Has_SIM_Card_Insert()) {
        NGUInt i;	
        for(i = 0; i < MMI_SIM_NUMBER; i++)
        {
            if(g_SimSuccessIndBuffered[i])
            {
                bFound = TRUE;
                CheckSimPin(i);
                break;
            }
        }
        
        if(!bFound && !m_fNeedCheckPIN && !m_fNeedCheckPUK)
        {
#ifdef __MMI_SAMSUNG_GT_FEATURE__
            getTopAppClient()->onControllerCommand(BOOTUP_CMD_ANIMATE_RESTART, 0, 0); 
#else
            onClientCommand(0, BOOTUP_CMD_ANIMATE_FINISH, 0, 0); 
#endif
        }
    }
    else
    {
#ifdef __MMI_SAMSUNG_GT_FEATURE__
        getTopAppClient()->onControllerCommand(BOOTUP_CMD_ANIMATE_RESTART, 0, 0); 
#else
        onClientCommand(0, BOOTUP_CMD_ANIMATE_FINISH, 0, 0); 
#endif
    }
}

void BootupApp::checkSimLock(void)
{
    NGInt passwdValue = -1;
    NGChar* editbuf = getEditDlgBuf();

#ifdef ENABLE_SETTINGS_SECURITY
    if(SecurityVerify::getInstance()->isSimLockNeedVerify(0) && Has_SIM_Card_Insert())
#endif
    {
        NGChar password[SETTINGS_SIM_PASSWORD];

        m_fNeedChekSim = true;
        NGInt16 error = 0;
        ReadRecord(NVRAM_SETTINGS_SIM_PASSWORD, 0, password, SETTINGS_SIM_PASSWORD, &error);
        //m_end_iamge->show();
        showMenuBar(true);
        getTopAppClient()->onControllerCommand(BOOTUP_CMD_BACKGROUND_ON, 0, 0); 
        do
        {
            RefreshLcdService();
            NGInt ret = showModalView(BOOTUP_CLIENT_EDITDLG_PASSWD, (NGParam)GetTextRes(SS_SIM_LOCK), 8);
            if(ret == TipDlg::TIP_OK)
            {
                RefreshLcdService();
                if(strlen(editbuf) < 4)
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_PASSWORD_TOO_SHORT), TIP_WARNING);
                    passwdValue = BOOTUP_PASSWD_WRONG;
                }
                else if(strcmp(editbuf, password))
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_INFO_TIP, (NGParam)GetTextRes(SS_WRONG_PASSWORD), TIP_WARNING);
                    passwdValue = BOOTUP_PASSWD_WRONG;
                }
                else // passwd is right
                {
                    showModalView(BOOTUP_CLIENT_TEXTDLG_PASSWD_PASS_TIP, (NGParam)GetTextRes(SS_SIM_LOCK_PASSED), TIP);
                    passwdValue = BOOTUP_PASSWD_RIGHT;
                }
            }
            else if(ret == TipDlg::TIP_CANCEL)
            { 
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
                    m_isSOSCalling = true;
                    editbuf [0] = '\0';
                    EntryCall(&intent);
                    break;
                }
            }
            else if(ret == TipDlg::TIP_CANCEL)
            {
                // do nothing!
            }

            editbuf [0] = '\0';
        } while(!passwdValue);

	//showMenuBar(false);
	//m_end_iamge->hide();
	//sendCommand(BOOTUP_CMD_MODEBAR_HIDE, 0, 0);
    }

    if (passwdValue == BOOTUP_PASSWD_RIGHT ||passwdValue == -1)
    {
        m_fNeedChekSim = false;
        onClientCommand(0, BOOTUP_CMD_ANIMATE_FINISH, 0, 0); 
    }
}

void BootupApp::exitTip(NGInt endCode)
{
    if(endCode == TipDlg::TIP_DESTROY && (getTop()->getId() == BOOTUP_CLIENT_EDITDLG_PASSWD))
            return;
	
    AppWithBar::exitTip(endCode);
}

BEGIN_CONTROLLER_CLIENTS(BootupApp)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_POWREON_KEY, BootupAnimate) 
    //CONTROLLER_CLIENT(BOOTUP_CLIENT_POWREON_ALARM, AlarmAnimate)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_POWREON_CHARGEON, ChargerAnimate)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_GREET, BootupGreet)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_UNPLUGCHARGER, ChargerUnplug)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_FULL, ChargerFull)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_LOGO, Logo)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_TEST_USIM, BootupTestUsim)    
    CONTROLLER_CLIENT(BOOTUP_CLIENT_SIMNAME, BootupSIMName)
END_CONTROLLER_CLIENTS_EX(AppWithBar)

BEGIN_DEFINE_APP(BootupApp)
    APP_SET(name, "bootup")
    APP_SET(position, POS_HIDE)
END_DEFINE_APP

