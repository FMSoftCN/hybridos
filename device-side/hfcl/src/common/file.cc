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

#include "file.h"

NAMESPACE_BEGIN

File::File()
    : m_fileName(NULL)
    , m_filePointer(0)
    , m_fileSize(0)
{
}

File::File(NGCPStr filename)
    : m_fileName(filename)
    , m_filePointer(0)
    , m_fileSize(0)
{
    open(m_fileName);
}

File::~File()
{
#if 0
    m_fileName = NULL;
    if (m_filePointer != 0)
        close();
    m_filePointer = 0;
    m_fileSize = 0;
#endif
}

NGInt File::open(NGCPStr filename, NGInt mode)
{
    m_filePointer = mgclFopen(filename, mode);
    if (m_filePointer == 0)
        return -1;

    m_fileName = (NGPStr)filename;
    length();
    
	return m_filePointer;
}

NGInt File::close(void)
{
    NGInt ret = mgclFclose(m_filePointer);
    m_filePointer = 0;
    m_fileSize = 0;

    return ret;
}

NGInt File::close(NGFsHandle fsHandle)
{
    NGInt ret;
	if (fsHandle == 0)
        return -1;

    ret = mgclFclose(fsHandle);

    m_filePointer = 0;
    m_fileSize = 0;

    return ret;
}


NGBool File::isOpen(void) const
{
    return (m_filePointer != 0);
}

NGInt File::read(void* ptr, NGInt size, NGInt nmemb)
{
    return mgclFread(ptr, size, nmemb, m_filePointer);
}

NGInt File::write(const void* ptr, NGInt size, NGInt nmemb)
{
    return mgclFwrite(ptr, size, nmemb, m_filePointer);
}

NGInt File::puts(const char* ptr)
{
    return mgclFputs(ptr, m_filePointer);
}

NGPStr File::gets(char* ptr, NGInt size)
{
    return mgclFgets(ptr, size, m_filePointer);
}

NGInt File::eof(void)
{
    return mgclFeof(m_filePointer);
}

void File::seek(NGLong offset, FileSeekType fst_id)
{
    if (!isOpen()) {
        return;
    }

    switch ((NGInt)fst_id) {
        case SEEKSET:
            mgclFseek(m_filePointer, offset, SEEK_SET);
            break;

        case SEEKCUR:
            mgclFseek(m_filePointer, offset, SEEK_CUR);
            break;

        case SEEKEND:
            mgclFseek(m_filePointer, offset, SEEK_END);
            break;

        default:
            break;
    }
}

// BUG FIXME
NGLong File::length(void)
{
    if (!isOpen())
        return 0;

    NGLong curpos = tell();

    seek(0, SEEKEND);
    NGLong endpos = tell();

    seek(curpos, SEEKSET);
    m_fileSize = endpos;

    return endpos;
}

NGLong File::tell(void) const
{
    return mgclFtell(m_filePointer);
}

NGInt File::remove(NGCPStr pathname)
{
    return mgclRemove(pathname);
}

NGInt File::rename(NGCPStr oldpath, NGCPStr newpath)
{
    return mgclRename(oldpath, newpath);
}

NGInt File::truncate(NGUInt32 size)
{
    return mgclTruncate(m_filePointer, size);
}

//===================== begin private function define =========================

NGPStr File::openMode(FileModeType mode)
{
    static NGPStr fileOpenMode[] = { (NGPStr)"r", (NGPStr)"w", (NGPStr)"w+", (NGPStr)"r+", (NGPStr)"a", (NGPStr)"a+",
        (NGPStr)"rb", (NGPStr)"wb", (NGPStr)"wb+", (NGPStr)"rb+", (NGPStr)"ab", (NGPStr)"ab+" };

    return fileOpenMode[(NGInt)mode];
}

//====================== end private function define ==========================

NAMESPACE_END

