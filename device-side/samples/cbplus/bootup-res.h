
#ifndef HFCL_SAMPLES_BOOTUP_RES_H__
#define HFCL_SAMPLES_BOOTUP_RES_H__

#include "cbplus.h"
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

