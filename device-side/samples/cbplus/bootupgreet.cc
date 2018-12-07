
#include "bootupgreet.h"

using namespace hfcl;

BootupGreet::BootupGreet(Controller* owner, int view_id, View* parent,
        HTData param1, HTData param2) : ControllerClient(owner, view_id, parent)
{
}

BootupGreet::~BootupGreet()
{
}

BEGIN_SETVIEW(BootupGreet)
END_SETVIEW

BEGIN_GETHANDLE(BootupGreet)
END_GETHANDLE

