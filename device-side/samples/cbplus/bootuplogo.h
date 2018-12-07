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
    bool onKey(int keyCode, KeyEvent* event);
    bool handleEvent(Event* event);
    unsigned int onControllerCommand(unsigned int cmd_id,
        HTData param1, HTData param2);

    void onMenuCommand(CustomEvent* event);
    void stopAnimate(void);
    bool onGifAnimateNotify(CustomEvent* event);

    GifAnimateView* m_animateview;
    int m_timer_id;
};

#endif /* HFCL_SAMPLES_BOOTUPLOGO_H_ */
