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

#include "bootupactivity.h"

#include <hfcl/resource.h>

#include "hybridrun.h"
#include "bootup-res.h"
#include "bootuplogo.h"
#include "bootupgreet.h"

BootupActivity::BootupActivity()
{
    REGISTER_RESPKG(bootup);
}

BootupActivity::~BootupActivity()
{
    UnregisterResPackageById(RES_PKG_BOOTUP_ID);
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

