
#include <hfcl/resouce.h>

#include "sys_res.h"

using namespace hfcl;

#define RESPKGID      RPKG_sys
#define RESID(name)   R_sys_##name

#include <hfcl/resource/resdefines.source.h>
#include "sys_res.inc"
#include <hfcl/resource/resundefines.h>

#include <hfcl/resource/resdefines.init.h>
#include "sys_res.inc"
#include <hfcl/resource/resundefines.h>

#undef RESID
#undef RESPKGID

