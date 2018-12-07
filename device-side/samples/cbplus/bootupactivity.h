
#ifndef HFCL_SAMPLES_BOOTUP_ACTIVITY_H_
#define HFCL_SAMPLES_BOOTUP_ACTIVITY_H_

#include <hfcl/activity.h>

using namespace hfcl;

DECLARE_ACTIVITY_FACTORY(BootupActivity)

class BootupActivity : public Activity {
public:
    BootupActivity();
    ~BootupActivity();

    // activity clients
    enum {
        BOOTUP_CLIENT_LOGO,
        BOOTUP_CLIENT_GREET,
    };

    void onCreate(ContextStream* contextstream, Intent* intent);

#if 0
    bool onKey(int keyCode, KeyEvent* event);
    virtual unsigned int onClientCommand(int sender, unsigned int cmd_id,
            HTData param1, HTData param2);
#endif

    DECLARE_CONTROLLER_CLIENTS

private:
    static BootupActivity* getCurrInstance() { return instance; }
    static BootupActivity* instance;
    int m_bootType;
};

#endif /* HFCL_SAMPLES_BOOTUP_ACTIVITY_H_ */

