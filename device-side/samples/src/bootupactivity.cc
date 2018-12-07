
#include "bootupactivity.h"

#include <hfcl/resource.h>

#include "helloworld.h"
#include "bootup-res.h"
#include "bootuplogo.h"
#include "bootupgreet.h"

BootupActivity* BootupActivity::instance = NULL;

BootupActivity::BootupActivity()
{
}

BootupActivity::~BootupActivity()
{
    instance = NULL;
    UnregisterResPackage(RES_PKG_BOOTUP_ID);
}

void BootupActivity::onCreate(ContextStream* contextStream, Intent* intent)
{
    FRRegister_bootup_resource();
    Activity::onCreate(contextStream, intent);
}

BEGIN_CONTROLLER_CLIENTS(BootupActivity)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_GREET, BootupGreet)
    CONTROLLER_CLIENT(BOOTUP_CLIENT_LOGO, BootupLogo)
END_CONTROLLER_CLIENTS_EX(Activity)

BEGIN_DEFINE_ACTIVITY(BootupActivity)
    ACTIVITY_SET(name, "bootup")
    ACTIVITY_SET(position, POS_HIDE)
END_DEFINE_ACTIVITY

