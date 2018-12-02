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

#ifndef __NGUX_NGUXFILE_H__
#define __NGUX_NGUXFILE_H__

#include "nguxcommon.h"
#include "log.h"
#include "mgcl.h"
#include "hal_fs.h"

NAMESPACE_BEGIN

class File {
    public:
        typedef enum {
            SEEKSET,    // Beginning of file
            SEEKCUR,    // Current position of file pointer
            SEEKEND     // End of file
        } FileSeekType;

#ifndef __LINUX__
        typedef enum {
            MODE_TXT_READ = FS_READ_ONLY,
            MODE_TXT_WRITE = FS_READ_WRITE,
            MODE_TXT_WPLUS = FS_CREATE | FS_READ_WRITE,
            MODE_TXT_RW = FS_READ_WRITE,
            // FIXME
            MODE_TXT_APPEND,
            // FIXME
            MODE_TXT_PLUS,
            // FIXME
            MODE_BIN_READ,
            // FIXME
            MODE_BIN_WRITE,
            // FIXME
            MODE_BIN_WPlUS,
            // FIXME
            MODE_BIN_RW,
            // FIXME
            MODE_BIN_APPEND,
            // FIXME
            MODE_BIN_PLUS,
            MODE_FS_CREATE = FS_CREATE,
        } FileModeType;
#else
		typedef enum {
			MODE_TXT_READ,
			MODE_TXT_WRITE,
			MODE_TXT_WPLUS,
			MODE_TXT_RW,
			MODE_TXT_APPEND,
			MODE_TXT_PLUS,
			MODE_BIN_READ,
			MODE_BIN_WRITE,
			MODE_BIN_WPlUS,
			MODE_BIN_RW,
			MODE_BIN_APPEND,
			MODE_BIN_PLUS
		} FileModeType;
#endif

        File();
        File(const char * filename);

        virtual ~File();

        int open(const char * filename, int mode = File::MODE_TXT_READ);
        int close(void);
        int close(HTHandle fsHandle);

        int read(void* ptr, int size, int nmemb);
        int write(const void* ptr, int size, int nmemb);

        int puts(const char* ptr);
        char * gets(char* ptr, int size);

        bool isOpen(void) const;

        int  eof(void);
        void seek(long offset, FileSeekType fst = File::SEEKSET);
        long tell(void) const;
        long length(void);
	 	int truncate(Uint32 size);
        static int remove(const char * pathname);
        static int rename(const char * oldpath, const char * newpath);

        inline const char * name(void){return m_fileName;}
    private:
        char * openMode(FileModeType mode);

        const char * m_fileName;
        HTHandle m_filePointer;
        long m_fileSize;
};

NAMESPACE_END

#endif /* __NGUX_NGUXFILE_H__ */

