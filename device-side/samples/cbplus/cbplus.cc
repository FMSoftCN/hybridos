/*
** HFCL Samples - Samples for HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL Samples.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "cbplus.h"

#include <cstdlib>

#include <hfcl/hfcl.h>
#include <hfcl/common.h>
#include <hfcl/activity.h>
#include <hfcl/view.h>
#include <hfcl/drawable.h>
#include <hfcl/resource.h>

#include "bootupactivity.h"
#include "sys-res.h"

int main (int argc, const char** argv)
{
    Initialize (argc, argv);

    // register system resource
    FRRegister_sys_resource();

    if (!SetResourceLanguage (R_LANG_zh_CN)) {
        _ERR_PRINTF ("cbplus: Failed when calling SetResourceLanguage with %d\n",
                R_LANG_zh_CN);
        return 1;
    }

    REGISTER_ACTIVITY("bootup", BootupActivity);

    ActivityManager* act_mgr = ActivityManager::getInstance();

    int boot_type = 0;
    if (argc > 1) {
        boot_type = atoi (argv [1]);
    }

    Intent intent(boot_type, 0, 0);
    act_mgr->startActivity ("bootup", &intent);
    act_mgr->run();

    Terminate (0);
    return 0;
}

