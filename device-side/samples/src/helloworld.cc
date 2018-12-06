#include <hfcl/hfcl.h>

#include "bootup.h"
#include "helloworld.h"

int main (int argc, const char** argv)
{
    if (!SetResourceLanguage (LANGID_ENGLISH_US)) {
        _ERR_PRINTF ("main: Faield calling SetResourceLanguage with %d\n",
                LANGID_ENGLISH_US);
        return 1;
    }

    // register system resource
    FRRegister_sys_resource();

    REGISTER_ACTIVITY("helloworld", HelloWorld);
    REGISTER_ACTIVITY("bootup", Bootup);

    ActivityManager* act_mgr = ActivityManager::getInstance();

    Intent intent(0);
    act_mgr->startActivity ("bootup", intent);
    act_mgr->run();

    return 0;
}

