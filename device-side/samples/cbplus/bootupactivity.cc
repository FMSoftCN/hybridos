
#include "bootupactivity.h"

#include <hfcl/resource.h>

#include "cbplus.h"
#include "bootup-res.h"
#include "bootuplogo.h"
#include "bootupgreet.h"

BootupActivity::BootupActivity()
{
    REGISTER_RESPKG(bootup);
}

BootupActivity::~BootupActivity()
{
    UnregisterResPackage(RES_PKG_BOOTUP_ID);
}

void BootupActivity::onCreate(ContextStream* contextStream, Intent* intent)
{
    FullScreenActivity::onCreate(contextStream, intent);

    if (intent->action ())
        showView (BOOTUP_CLIENT_GREET, 0, 0);
    else
        showView (BOOTUP_CLIENT_LOGO, 0, 0);
}

BEGIN_CONTROLLER_CLIENTS(BootupActivity)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_GREET, BootupGreet)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_LOGO, BootupLogo)
END_CONTROLLER_CLIENTS_EX(Activity)

BEGIN_DEFINE_ACTIVITY(BootupActivity)
    ACTIVITY_SET(name, "bootup")
    ACTIVITY_SET(position, POS_HIDE)
END_DEFINE_ACTIVITY

