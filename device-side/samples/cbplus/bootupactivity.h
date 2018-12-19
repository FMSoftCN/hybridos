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

#ifndef HFCL_SAMPLES_BOOTUP_ACTIVITY_H_
#define HFCL_SAMPLES_BOOTUP_ACTIVITY_H_

#include <hfcl/activity.h>

using namespace hfcl;

DECLARE_ACTIVITY_FACTORY(BootupActivity)

class BootupActivity : public FullScreenActivity {
public:
    BootupActivity();
    ~BootupActivity();

    // activity clients
    enum {
        BOOTUP_CLIENT_LOGO,
        BOOTUP_CLIENT_GREET,
    };

    void onCreate(ContextStream* contextstream, Intent* intent);

#if 0
    bool onKey(int keyCode, KeyEvent* event);
    virtual unsigned int onClientCommand(int sender, unsigned int cmd_id,
            HTData param1, HTData param2);
#endif

    DECLARE_CONTROLLER_CLIENTS

private:
    int m_bootType;
};

#endif /* HFCL_SAMPLES_BOOTUP_ACTIVITY_H_ */

