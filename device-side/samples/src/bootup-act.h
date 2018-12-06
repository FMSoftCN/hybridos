
#ifndef HFCL_SAMPLES_BOOTUP_ACTIVITY_H_
#define HFCL_SAMPLES_BOOTUP_ACTIVITY_H_

using namespace hfcl;

DECLARE_ACTIVITY_FACTORY(BootupActivity)

// activity clients
enum {
    BOOTUP_CLIENT_BEGIN = ACTIVITY_CLIENT_MAX,
    BOOTUP_CLIENT_LOGO,
    BOOTUP_CLIENT_GREET,
};

class BootupActivity : public ActivityWithBar {
public:
    BootupActivity();
    ~BootupActivity();

    static BootupActivity* getCurrInstance() { return instance; }

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

    DECLARE_CONTROLLER_CLIENTS

private:
    void boot(NGInt bootType);
    void EnterAnimate(NGInt bootType);
    void EnterGreetwords(void);

private:
    static BootupActivity* instance;
    NGInt m_bootType;
};

#endif /* HFCL_SAMPLES_BOOTUP_ACTIVITY_H_ */

