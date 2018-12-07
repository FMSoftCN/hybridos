#ifndef HFCL_SAMPLES_BOOTUPLOGO_H_
#define HFCL_SAMPLES_BOOTUPLOGO_H_

#include <hfcl/activity.h>
#include <hfcl/view.h>

using namespace hfcl;

class BootupLogo : public ControllerClient, public TimerEventListener {
public:
    BootupLogo (Controller* owner, int view_id, View* parent,
        HTData param1, HTData param2);
    virtual ~BootupLogo();

    DECLARE_VIEWCONTEXT

private:
#if 0
    bool onKey(int keyCode, KeyEvent* event);
    unsigned int onControllerCommand(unsigned int cmd_id,
        HTData param1, HTData param2);
#endif

    bool handleEvent(Event* event);

    void onMenuCommand(CustomEvent* event);
    bool onGifAnimateNotify(CustomEvent* event);

    GifAnimateView* m_animateview;
    int m_timer_id;
};

#endif /* HFCL_SAMPLES_BOOTUPLOGO_H_ */
