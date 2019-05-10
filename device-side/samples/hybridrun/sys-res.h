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

#ifndef HFCL_SAMPLES_SYS_RES_H__
#define HFCL_SAMPLES_SYS_RES_H__

#include <hfcl/resource/respkgmanager.h>
#include <hfcl/css.h>

#define RESPKGID     RESPKGID_NAME(sys)
#define RESID(name)  RESID_PKG(sys, name)

#include <hfcl/resource/resdefines.head.h>
#include "sys-res.inc"
#include <hfcl/resource/resundefines.h>

#include <hfcl/resource/resdefines.name.h>
#include "sys-res.inc"
#include <hfcl/resource/resundefines.h>

#undef RESPKGID
#undef RESID

#endif  /* HFCL_SAMPLES_SYS_RES_H__ */

