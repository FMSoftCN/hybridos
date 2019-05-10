#!/bin/bash

sed -i 's/\<NAMESPACE_BEGIN\>/namespace hfcl {/g' `grep NAMESPACE_BEGIN -rl include/ src/`
sed -i 's/\<NAMESPACE_END\>/\} \/\/ namespace hfcl/g' `grep NAMESPACE_END -rl include/ src/`
sed -i 's/\<USE_NGUX_NAMESPACE\>/using namespace hfcl;/g' `grep USE_NGUX_NAMESPACE -rl include/ src/`
sed -i 's/NGUX_/HFCL_/g' `grep NGUX_ -rl include/ src/`

sed -i 's/\<KEYCODE_LEFT\>/KEYCODE_CURSOR_LEFT/g' `grep KEYCODE_LEFT -rl include/ src/`
sed -i 's/\<KEYCODE_UP\>/KEYCODE_CURSOR_UP/g' `grep KEYCODE_UP -rl include/ src/`
sed -i 's/\<KEYCODE_RIGHT\>/KEYCODE_CURSOR_RIGHT/g' `grep KEYCODE_RIGHT -rl include/ src/`
sed -i 's/\<KEYCODE_DOWN\>/KEYCODE_CURSOR_DOWN/g' `grep KEYCODE_DOWN -rl include/ src/`
sed -i 's/\<KEYCODE_SL\>/KEYCODE_SOFTKEY_LEFT/g' `grep KEYCODE_SL -rl include/ src/`
sed -i 's/\<KEYCODE_SR\>/KEYCODE_SOFTKEY_RIGHT/g' `grep KEYCODE_SR -rl include/ src/`

exit 0
