#!/bin/bash

sed -i 's/#include "nguxcommon.h"/#include "common\/common.h"/g' `grep '#include "nguxcommon.h"' -rl include/ src/`
sed -i 's/#include "nguxevent.h"/#include "common\/event.h"/g' `grep '#include "nguxevent.h"' -rl include/ src/`
sed -i 's/#include "nguxobject.h"/#include "common\/object.h"/g' `grep '#include "nguxobject.h"' -rl include/ src/`
sed -i 's/#include "log.h"/#include "common\/log.h"/g' `grep '#include "log.h"' -rl include/ src/`
sed -i 's/#include "intrect.h"/#include "common\/intrect.h"/g' `grep '#include "intrect.h"' -rl include/ src/`

sed -i 's/#include "nguxwindow.h"/#include "activity\/window.h"/g' `grep '#include "nguxwindow.h"' -rl include/ src/`

sed -i 's/#include "mgcl.h"/#include "mgcl\/mgcl.h"/g' `grep '#include "mgcl.h"' -rl include/ src/`

sed -i 's/#include "timerservice.h"/#include "services\/timerservice.h"/g' `grep '#include "timerservice.h"' -rl include/ src/`

sed -i 's/#include "respkgmanager.h"/#include "resource\/respkgmanager.h"/g' `grep '#include "respkgmanager.h"' -rl include/ src/`
sed -i 's/#include "respackage.h"/#include "resource\/respackage.h"/g' `grep '#include "respackage.h"' -rl include/ src/`
sed -i 's/#include "resloader.h"/#include "resource\/resloader.h"/g' `grep '#include "resloader.h"' -rl include/ src/`

sed -i 's/#include "graphicscontext.h"/#include "graphics\/graphicscontext.h"/g' `grep '#include "graphicscontext.h"' -rl include/ src/`
sed -i 's/#include "font.h"/#include "graphics\/font.h"/g' `grep '#include "font.h"' -rl include/ src/`
sed -i 's/#include "image.h"/#include "graphics\/image.h"/g' `grep '#include "image.h"' -rl include/ src/`
sed -i 's/#include "gifanimate.h"/#include "graphics\/gifanimate.h"/g' `grep '#include "gifanimate.h"' -rl include/ src/`

sed -i 's/#include "view.h"/#include "view\/view.h"/g' `grep '#include "view.h"' -rl include/ src/`
sed -i 's/#include "viewcontext.h"/#include "view\/viewcontext.h"/g' `grep '#include "viewcontext.h"' -rl include/ src/`
sed -i 's/#include "animateimageview.h"/#include "view\/animateimageview.h"/g' `grep '#include "animateimageview.h"' -rl include/ src/`

sed -i 's/#include "nguxanimation.h"/#include "animation\/animation.h"/g' `grep '#include "nguxanimation.h"' -rl include/ src/`

sed -i 's/#include "color.h"/#include "graphics\/color.h"/g' `grep '#include "color.h"' -rl include/ src/`

sed -i 's/#include "drawable.h"/#include "drawable\/drawable.h"/g' `grep '#include "drawable.h"' -rl include/ src/`
sed -i 's/#include "drawable-id-namemap.h"/#include "drawable\/drawable-id-namemap.h"/g' `grep '#include "drawable-id-namemap.h"' -rl include/ src/`
sed -i 's/#include "common-drawable-ids.h"/#include "drawable\/common-drawable-ids.h"/g' `grep '#include "common-drawable-ids.h"' -rl include/ src/`
sed -i 's/#include "colordrawable.h"/#include "drawable\/colordrawable.h"/g' `grep '#include "colordrawable.h"' -rl include/ src/`
sed -i 's/#include "textdrawable.h"/#include "drawable\/textdrawable.h"/g' `grep '#include "textdrawable.h"' -rl include/ src/`
sed -i 's/#include "imagedrawable.h"/#include "drawable\/imagedrawable.h"/g' `grep '#include "imagedrawable.h"' -rl include/ src/`

sed -i 's/#include "common\/alternativestl.h"/#include "common\/stlalternative.h"/g' `grep '#include "common\/alternativestl.h"' -rl include/ src/`

exit 0
