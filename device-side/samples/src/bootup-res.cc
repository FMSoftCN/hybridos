
#include <hfcl/view.h>
#include <hfcl/resource.h>

#include "helloworld.h"
#include "bootup-res.h"

using namespace hfcl;

#define RESPKGID      RPKG_bootup
#define RESID(name)   R_bootup_##name

#include <hfcl/resource/resdefines.source.h>
#include "bootup-res.inc"
#include <hfcl/resource/resundefines.h>

#include <hfcl/resource/resdefines.init.h>
#include "bootup-res.inc"
#include <hfcl/resource/resundefines.h>

#undef RESID
#undef RESPKGID

