#!/bin/bash

sed -i 's/\<utf8_to_ucs2\>/UTF8ToUCS2/g' `grep utf8_to_ucs2 -rl include/ src/`
sed -i 's/\<CreateMemGc\>/CreateMemGC/g' `grep CreateMemGc -rl include/ src/`
sed -i 's/\<DeleteMemGc\>/DeleteMemGC/g' `grep DeleteMemGc -rl include/ src/`

sed -i 's/\<isNumberChar\>/IsNumberUCS2Char/g' `grep isNumberChar -rl include/ src/`
sed -i 's/\<isSymbolChar\>/IsSymbolUCS2Char/g' `grep isSymbolChar -rl include/ src/`
sed -i 's/\<isArabicSymbol\>/IsArabicSymbolUCS2Char/g' `grep isArabicSymbol -rl include/ src/`

exit 0
