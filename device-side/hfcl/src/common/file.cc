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

namespace hfcl {

File::File()
    : m_fileName(NULL)
    , m_filePointer(0)
    , m_fileSize(0)
{
}

File::File(const char * filename)
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

int File::open(const char * filename, int mode)
{
    m_filePointer = mgclFopen(filename, mode);
    if (m_filePointer == 0)
        return -1;

    m_fileName = (char *)filename;
    length();
    
	return m_filePointer;
}

int File::close(void)
{
    int ret = mgclFclose(m_filePointer);
    m_filePointer = 0;
    m_fileSize = 0;

    return ret;
}

int File::close(HTHandle fsHandle)
{
    int ret;
	if (fsHandle == 0)
        return -1;

    ret = mgclFclose(fsHandle);

    m_filePointer = 0;
    m_fileSize = 0;

    return ret;
}


bool File::isOpen(void) const
{
    return (m_filePointer != 0);
}

int File::read(void* ptr, int size, int nmemb)
{
    return mgclFread(ptr, size, nmemb, m_filePointer);
}

int File::write(const void* ptr, int size, int nmemb)
{
    return mgclFwrite(ptr, size, nmemb, m_filePointer);
}

int File::puts(const char* ptr)
{
    return mgclFputs(ptr, m_filePointer);
}

char * File::gets(char* ptr, int size)
{
    return mgclFgets(ptr, size, m_filePointer);
}

int File::eof(void)
{
    return mgclFeof(m_filePointer);
}

void File::seek(long offset, FileSeekType fst_id)
{
    if (!isOpen()) {
        return;
    }

    switch ((int)fst_id) {
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
long File::length(void)
{
    if (!isOpen())
        return 0;

    long curpos = tell();

    seek(0, SEEKEND);
    long endpos = tell();

    seek(curpos, SEEKSET);
    m_fileSize = endpos;

    return endpos;
}

long File::tell(void) const
{
    return mgclFtell(m_filePointer);
}

int File::remove(const char * pathname)
{
    return mgclRemove(pathname);
}

int File::rename(const char * oldpath, const char * newpath)
{
    return mgclRename(oldpath, newpath);
}

int File::truncate(Uint32 size)
{
    return mgclTruncate(m_filePointer, size);
}

//===================== begin private function define =========================

char * File::openMode(FileModeType mode)
{
    static char * fileOpenMode[] = { (char *)"r", (char *)"w", (char *)"w+", (char *)"r+", (char *)"a", (char *)"a+",
        (char *)"rb", (char *)"wb", (char *)"wb+", (char *)"rb+", (char *)"ab", (char *)"ab+" };

    return fileOpenMode[(int)mode];
}

//====================== end private function define ==========================

} // namespace hfcl

