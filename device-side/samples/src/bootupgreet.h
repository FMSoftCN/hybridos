#ifndef HFCL_SAMPLES_BOOTUPGREET_H_
#define HFCL_SAMPLES_BOOTUPGREET_H_

#include <hfcl/activity.h>

using namespace hfcl;

class BootupGreet : public ControllerClient, public TimerEventListener {
public:
    BootupGreet (Controller* owner, int view_id, View* parent,
        HTData param1, HTData param2);
    virtual ~BootupGreet();

    DECLARE_VIEWCONTEXT

private:
    bool onKey(int keyCode, KeyEvent* event);
    bool handleEvent(Event* event);

    int m_timer_id;
};

#endif /* HFCL_SAMPLES_BOOTUPGREET_H_ */
