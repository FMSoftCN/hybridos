
#include "bootuplogo.h"

#include <hfcl/common.h>

#include "bootup-res.h"
#include "bootupactivity.h"

using namespace hfcl;

#define HRESNAME(pkg, type, name) R_##pkg##_##type##_##name

BootupLogo::BootupLogo(Controller* owner, int view_id, View* parent,
        HTData param1, HTData param2)
    : ControllerClient (owner, view_id, parent)
    , m_animateview(NULL)
{
    m_baseView = CreateViewFromRes (HRESNAME(bootup, ui, logo),
            parent, this, NULL);

    m_animateview->setGifAnimate (
            GetGifAnimateRes (HRESNAME (bootup, img, animation_logo)));

    m_timer_id = 0;
}

BootupLogo::~BootupLogo()
{
    if (m_timer_id != 0) {
        removeTimer(m_timer_id);
    }
}

bool BootupLogo::onGifAnimateNotify(CustomEvent* event)
{
    if (event->customWparam() != CustomEvent::CUS_GIFANIMATE_STOP)
        return false;

    stopAnimate();
    return true;
}

bool BootupLogo::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER
            && m_timer_id == ((TimerEvent *)event)->timerID()) {

        if (((BootupActivity *)m_owner)->getTop() == NULL)
            return false;

        if (m_timer_id != 0) {
            removeTimer(m_timer_id);
            m_timer_id = 0;
        }
    }

    return DISPATCH_CONTINUE_MSG;
}

BEGIN_SETVIEW(BootupLogo)
    MAP_VIEW(m_animateview, HRESNAME (bootup, var, view_animation_logo), GifAnimateView)
END_SETVIEW

BEGIN_GETHANDLE(BootupLogo)
    MAP_HANDLE(BootupLogo, HRESNAME (bootup, var, event_animation_logo), onGifAnimateNotify)
END_GETHANDLE

