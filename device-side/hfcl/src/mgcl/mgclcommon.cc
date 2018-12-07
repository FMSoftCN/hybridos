/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
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

#include "mgcl/mgcl.h"
#include "common/common.h"

#if 0
namespace hfcl {

HTHandle mgclFopen(const char* path, const int mode)
{
    //char * unicodeFileName[256];
    //memset(unicodeFileName, 0, sizeof(unicodeFileName));
    //strcpy(unicodeFileName,path);
    //NGUtf8ToUnicode((unsigned char*)path, (unsigned char*)unicodeFileName);

    return  MMI_FS_Open((const Uint8*)path, (INT32)mode);
}

int mgclFclose(HTHandle fPointer)
{
    return MMI_FS_Close((FS_HANDLE)fPointer);
}

int mgclFread(void* ptr, int size, int nmemb, HTHandle stream)
{
    int len = 0;
    MMI_FS_Read((FS_HANDLE)stream, ptr, (size*nmemb), (INT32*)&len);
    return len;
}

int mgclFwrite(const void* ptr, int size, int nmemb, HTHandle stream)
{
    int len = 0;
    MMI_FS_Write((FS_HANDLE)stream, (void*)ptr, size * nmemb, (INT32*)&len);
    return len;
}

int mgclFputs(const char* ptr, HTHandle stream)
{
    int len = 0;
    int slen = strlen (ptr);
    if (slen < 0 || slen > 1024) // FIXME, not support large string.
        return -1;
    MMI_FS_Write((FS_HANDLE)stream, (void*)ptr, strlen(ptr) + 1, (INT32*)&len);
    return len;
}

char* mgclFgets(char* ptr, int size, HTHandle stream)
{
    int len;
    //int x_pos;

    MMI_FS_Read((FS_HANDLE)stream, (void *)ptr, size, (INT32*)&len);
    if (len < 0){
        return NULL;
    }

    for(int i = 0; i < len; i++){
        if (ptr[i] == '\n' || ptr[i] == '\r'){
            ptr[i+1] = '\0';
            MMI_FS_Seek((FS_HANDLE)stream, - (len - (i + 1)), FS_FILE_CURRENT);
            return ptr;
        }
    }
    ptr[size - 1] = '\0';
    return ptr;
}

int mgclFeof(HTHandle stream)
{
    long pos = MMI_FS_Seek((FS_HANDLE)stream, 0, FS_FILE_CURRENT);
    long tell = MMI_FS_Seek((FS_HANDLE)stream, 0, FS_FILE_END);

    MMI_FS_Seek((FS_HANDLE)stream, pos, FS_FILE_BEGIN);

    return (pos == tell) ? 0 : 1;
}

int mgclFseek(HTHandle stream, long offset, int whence)
{
    return MMI_FS_Seek((FS_HANDLE)stream, offset, whence);
}

long mgclFtell(HTHandle stream)
{
    return MMI_FS_Seek((FS_HANDLE)stream, 0, FS_FILE_CURRENT);
}

int mgclRemove(const char* pathname)
{
    //char * unicodeFileName[256];
    //memset(unicodeFileName, 0, sizeof(unicodeFileName));

    //NGUtf8ToUnicode((unsigned char*)pathname, (unsigned char*)unicodeFileName);
    return MMI_FS_Delete((const Uint8 *)pathname);
}

int mgclRename(const char* oldpath, const char* newpath)
{
    return MMI_FS_Rename((const Uint8 *)oldpath, (const Uint8 *)newpath);
}

int mgclTruncate(HTHandle stream, unsigned int nFileSize)
{
    return MMI_FS_Truncate(stream, nFileSize);
}

} // namespace hfcl

#endif

