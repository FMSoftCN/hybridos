
#include <hfcl/resource.h>

#include "cbplus.h"
#include "sys-res.h"

using namespace hfcl;

#define RESPKGID      RPKG_sys
#define RESID(name)   R_sys_##name

#include <hfcl/resource/resdefines.source.h>
#include "sys-res.inc"
#include <hfcl/resource/resundefines.h>

#include <hfcl/resource/resdefines.init.h>
#include "sys-res.inc"
#include <hfcl/resource/resundefines.h>

#undef RESID
#undef RESPKGID

