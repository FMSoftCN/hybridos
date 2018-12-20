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

#ifndef HFCL_SAMPLES_BOOTUP_RES_H__
#define HFCL_SAMPLES_BOOTUP_RES_H__

#include "hybridrun.h"
#include "sys-res.h"

#define RESPKGID      RPKG_bootup
#define RESID(name)   R_bootup_##name

#include <hfcl/resource/resdefines.head.h>
#include "bootup-res.inc"
#include <hfcl/resource/resundefines.h>

#include <hfcl/resource/resdefines.name.h>
#include "bootup-res.inc"
#include <hfcl/resource/resundefines.h>

#undef RESID
#undef RESPKGID

#endif  /* HFCL_SAMPLES_BOOTUP_RES_H__ */

