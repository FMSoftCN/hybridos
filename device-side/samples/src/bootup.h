/*
** $Id: BootupApp.h 2447 2012-01-11 04:03:00Z ylwang $
**
** BootupApp.h : TODO
**
** Copyright (C) 2002 ~ 2011 Beijing FMSoft Technology Co., Ltd.
**
** All rights reserved by FMSoft.
**
** Current Maintainer : ylwang
**
** Create Date : 2011-10-24
*/

#ifndef __NGUX_BOOTUPAPP_H__
#define __NGUX_BOOTUPAPP_H__

#include "appwithbar.h"

USE_NGUX_NAMESPACE


DECLARE_APP_FACTORY(BootupApp)

// app clients
enum {
    BOOTUP_CLIENT_BEGIN = APP_CLIENT_MAX,
    BOOTUP_CLIENT_POWREON_KEY,
    BOOTUP_CLIENT_GREET,
    BOOTUP_CLIENT_POWREON_CHARGEON,
    //BOOTUP_CLIENT_POWREON_ALARM,
    BOOTUP_CLIENT_POWREON_EXCEPTION,

    BOOTUP_CLIENT_TEXTDLG_INFO_TIP,
    BOOTUP_CLIENT_EDITDLG_PASSWD,
    BOOTUP_CLIENT_QUESTIONDLG_EMERGENCY,
    BOOTUP_CLIENT_TEXTDLG_NOINPUT_TIP,
    BOOTUP_CLIENT_TEXTDLG_PASSWD_ERROR_TIP,
    BOOTUP_CLIENT_TEXTDLG_PASSWD_PASS_TIP,
    BOOTUP_CLIENT_UNPLUGCHARGER,
    BOOTUP_CLIENT_FULL,
    BOOTUP_CLIENT_LOGO,
    BOOTUP_CLIENT_TEST_USIM,
    BOOTUP_CLIENT_SIMNAME,
};

// events and cmds
enum {
    BOOTUP_EVENT_BEGIN = APP_CMD_MAX,   // 24
    BOOTUP_CMD_ANIMATE_RESTART,         // 25
    BOOTUP_CMD_ANIMATE_FINISH,          // 26
    BOOTUP_CMD_ALARM_ANIMATE_FINISH,    // 27
    BOOTUP_CMD_GREET_FINISH,            // 28
    BOOTUP_CMD_SECURE_FINISH,           // 29
    BOOTUP_CMD_NEED_PIN,                // 30 // need pin1 while bootup
    BOOTUP_CMD_VRFD_PIN,                // 31
    BOOTUP_CMD_NEED_PUK,                // 32 // need puk1 while bootup
    BOOTUP_CMD_VRFD_PUK,                // 33
    BOOTUP_CMD_NEED_PNONELOCK,          // 34
    BOOTUP_CMD_VRFD_PNONELOCK,          // 35
    BOOTUP_CMD_BOOTUP_BATTERY,          // 36 // adp events
    BOOTUP_CMD_POWERON,                 // 37
    BOOTUP_CMD_POWEROFF,                // 38
    BOOTUP_CMD_BACKGROUND_ON,           // 39
    BOOTUP_CMD_BACKGROUND_OFF,          // 40
    BOOTUP_CMD_MODEBAR_SHOW,            // 41
    BOOTUP_CMD_MODEBAR_HIDE,            // 42
    BOOTUP_CMD_PUK_BLOCK,               // 43
    BOOTUP_CMD_NEED_SIMLOCK,
    BOOTUP_CMD_SIMNAME_FINISH,
};

enum {
    BOOTUP_FROM_PINLOCK = 1,     
    BOOTUP_FROM_PHONELOCK,
    BOOTUP_FROM_SIMLOCK,
    BOOTUP_FROM_PUKLOCK
};

enum {
	SIMCARD_NOT_INIT = 0x0,
	SIMCARD_ABSENT = 0x01,
	SIMCARD_VALID =0x02,
	SIMCARD_CHV1_REQUESTED = 0x04,
	SIMCARD_CHVBLOCKED =0x08,
};

class BootupApp : public AppWithBar {
    public:
        BootupApp();
        ~BootupApp();

		static BootupApp* getCurrInstance() { return instance; }
        
        enum {
            BOOTUP_KEY = 0,     // POWER_ON_KEYPAD
            BOOTUP_ALARM,       // POWER_ON_ALARM
            BOOTUP_CHARGER_IN,  // POWER_ON_CHARGER_IN
            BOOTUP_EXCEPTION,   // POWER_ON_EXCEPTION
            BOOTUP_USB,         // POWER_ON_USB
            BOOTUP_UNINIT       // POWER_ON_UNINIT
        };

        void onCreate(ContextStream* contextstream, Intent* intent);
		void onWakeup();

        NGBool onKey(NGInt keyCode, KeyEvent* event);
        virtual NGUInt onClientCommand(NGInt sender, NGUInt cmd_id, NGULong param1, NGULong param2);

		void showChargerUnplug(NGInt level);
        void showChargerFull(void);

        NGUInt showModalView(NGInt view_id, NGParam param1, NGParam param2);
        NGUInt showMsgTextDlg(NGCPStr text_info, NGUInt type, NGInt icon_id = 0, NGInt text_style = 0, NGInt second = 0);
        NGUInt showMsgEditDlg(NGCPStr text_info, NGInt length);
        void setTheme();
        
        NGChar* getEditDlgBuf() { return m_editDlgText; }
        void showErrorMsgDlg(NGInt count);
        NGBool m_fNeedCheckPIN[MMI_SIM_NUMBER];
        NGBool m_fNeedCheckPUK[MMI_SIM_NUMBER];
        NGBool m_fPhoneLockChecking;
        NGBool m_fNeedChekSim;
        NGBool m_LockCheck;
        NGBool m_PinFlag;	
        void CheckPhoneLockPass(void);
        void checkSimLock(void);
        NGBool m_PUKBlock;
        void showPUKblock(void);
        void exitTip(NGInt endCode);
        
        NGBool GetSOSCall(void) { return m_isSOSCalling; }
        void SetSOSCall(NGBool status) { m_isSOSCalling = status; }
		
        NGBool isPowerOnPlayed() {return mIsPowerOnPlayed; }
        NGInt getBootType() {return m_bootType; }   
        NGBool isTestUsimInserted(void);
        DECLARE_CONTROLLER_CLIENTS

    private:
        void boot(NGInt bootType);
        void EnterAnimate(NGInt bootType);
        static void callback(void* obj, void* data, NGInt data_bytes, NGInt type, NGBool result, NGInt reason);
        void EnterGreetwords(void);
        void EnterPhonelock(void);
        void EnterDesktop(void);
        void EnterAlarm(void);
        void EnterTestUsim(void);
        void EnterSIMName(void);

        void StopBootupTone(void);
        void PlayBootupTone(void);
        void CheckEarphone(void);
        void onPowerOnAlarm(void);
        
        AudioMediaPlayer* m_audioplayer;
        NGChar m_editDlgText[80];
		NGBool m_isSOSCalling;

    private:
        static BootupApp* instance;
        NGBool mIsPowerOnPlayed;
        NGInt m_bootType;
        
};

#endif /* __NGUX_BOOTUPAPP_H__ */

