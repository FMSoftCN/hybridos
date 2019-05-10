#!/bin/bash

sed -i 's/\<HResTypeID\>/HIDResType/g' `grep HResTypeID src/ include/ -rl`
sed -i 's/\<GetTextRes\>/GetText/g' `grep GetTextRes src/ include/ -rl`
sed -i 's/\<HLangID\>/HIDLanguage/g' `grep HLangID src/ include/ -rl`
sed -i 's/\<HEncodingID\>/HIDEncoding/g' `grep HEncodingID src/ include/ -rl`

exit 0

sed -i 's/\<CONST_PTR_TResourceEntry\>/CPTResourceEntry/g' `grep CONST_PTR_TResourceEntry src/ include/ -rl`
sed -i 's/TResourceEntry/ResourceEntry/g' `grep TResourceEntry src/ include/ -rl`

sed -i 's/\<ResEntry\>/ResourceBucket/g' `grep ResEntry src/ include/ -rl`

sed -i 's/\<NGBool\>/bool/g' `grep NGBool -rl include/ src/`

sed -i 's/\<NGChar\>/char/g' `grep NGChar -rl include/ src/`
sed -i 's/\<NGByte\>/unsigned char/g' `grep NGByte -rl include/ src/`

sed -i 's/\<NGShort\>/short/g' `grep NGShort -rl include/ src/`
sed -i 's/\<NGSShort\>/signed short/g' `grep NGSShort -rl include/ src/`
sed -i 's/\<NGUShort\>/unsigned short/g' `grep NGUShort -rl include/ src/`

sed -i 's/\<NGInt\>/int/g' `grep NGInt -rl include/ src/`
sed -i 's/\<NGSInt\>/signed int/g' `grep NGSInt -rl include/ src/`
sed -i 's/\<NGUInt\>/unsigned int/g' `grep NGUInt -rl include/ src/`

sed -i 's/\<NGLong\>/long/g' `grep NGLong -rl include/ src/`
sed -i 's/\<NGSLong\>/signed long/g' `grep NGSLong -rl include/ src/`
sed -i 's/\<NGULong\>/unsigned long/g' `grep NGULong -rl include/ src/`
sed -i 's/\<NGULongLong\>/unsigned long long/g' `grep NGUInt -rl include/ src/`

sed -i 's/\<NGPStr\>/char */g' `grep NGPStr -rl include/ src/`
sed -i 's/\<NGCPStr\>/const char */g' `grep NGCPStr -rl include/ src/`

sed -i 's/\<NGPointer\>/void */g' `grep NGPointer -rl include/ src/`
sed -i 's/\<NGSize_t\>/size_t/g' `grep NGSize_t -rl include/ src/`
sed -i 's/\<NGSize\>/size_t/g' `grep NGSize -rl include/ src/`
sed -i 's/\<NGOff\>/off_t/g' `grep NGOff -rl include/ src/`

sed -i 's/\<NGInt8\>/Sint8/g' `grep NGInt8 -rl include/ src/`
sed -i 's/\<NGUInt8\>/Uint8/g' `grep NGUInt8 -rl include/ src/`
sed -i 's/\<NGInt16\>/Sint16/g' `grep NGInt16 -rl include/ src/`
sed -i 's/\<NGUInt16\>/Uint16/g' `grep NGUInt16 -rl include/ src/`
sed -i 's/\<NGInt32\>/Sint32/g' `grep NGInt32 -rl include/ src/`
sed -i 's/\<NGUInt32\>/Uint32/g' `grep NGUInt32 -rl include/ src/`
sed -i 's/\<NGInt64\>/Sint64/g' `grep NGInt64 -rl include/ src/`
sed -i 's/\<NGUInt64\>/Uint64/g' `grep NGUInt64 -rl include/ src/`

sed -i 's/\<NGFsHandle\>/HTHandle/g' `grep NGFsHandle -rl include/ src/`
sed -i 's/\<ResID\>/HTResId/g' `grep ResID -rl include/ src/`
sed -i 's/\<NGDword\>/HTData/g' `grep NGDword -rl include/ src/`
sed -i 's/\<NGReal\>/HTReal/g' `grep NGReal -rl include/ src/`

sed -i 's/\<NGParam\>/HTData/g' `grep NGParam -rl include/ src/`

sed -i 's/\<BaseApp\>/BaseActivity/g' `grep BaseApp src/ include/ -rl`
sed -i 's/\<AppInfo\>/ActivityInfo/g' `grep AppInfo src/ include/ -rl`
sed -i 's/\<AppFactory\>/ActivityFactory/g' `grep AppFactory src/ include/ -rl`
sed -i 's/\<AppManager\>/ActivityManager/g' `grep AppManager src/ include/ -rl`
sed -i 's/\<AppInfo\>/ActivityInfo/g' `grep AppInfo src/ include/ -rl`
sed -i 's/\<AppStack\>/ActivityStack/g' `grep AppStack src/ include/ -rl`
sed -i 's/\<Application\>/Activity/g' `grep Application src/ include/ -rl`
sed -i 's/\<AppInformation\>/AppData/g' `grep AppInformation src/ include/ -rl`
sed -i 's/\<FullScreenApp\>/FullScreenActivity/g' `grep FullScreenApp src/ include/ -rl`

sed -i 's/\<U8\>/Uint8/g' `grep U8 src/ include/ -rl`
sed -i 's/\<U16\>/Uint16/g' `grep U16 src/ include/ -rl`

sed -i 's/\<enumResType\>/HFCLResType/g' `grep enumResType src/ include/ -rl`
sed -i 's/\<ResLang\>/HFCLResLang/g' `grep ResLang src/ include/ -rl`
sed -i 's/\<ResEncoding\>/HFCLResEncoding/g' `grep ResEncoding src/ include/ -rl`

exit 0
