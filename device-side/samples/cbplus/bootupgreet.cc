
#include "bootupgreet.h"

#include "bootupactivity.h"

using namespace hfcl;

BootupGreet::BootupGreet(Controller* owner, int view_id, View* parent,
        HTData param1, HTData param2) : ControllerClient(owner, view_id, parent)
{
}

BootupGreet::~BootupGreet()
{
}

bool BootupGreet::handleEvent(Event* event)
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

BEGIN_SETVIEW(BootupGreet)
END_SETVIEW

BEGIN_GETHANDLE(BootupGreet)
END_GETHANDLE

