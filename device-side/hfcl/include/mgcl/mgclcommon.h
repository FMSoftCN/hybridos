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

#ifndef __HFCL_MGCLCOMMON_H__
#define __HFCL_MGCLCOMMON_H__

namespace hfcl {

/**
 * @brief mgclFopen Open file.
 *
 * @param path The file path and name.
 * @param mode The open file mode.
 *
 * @returns success return file pointer, error return NULL.
 */
HTHandle mgclFopen(const char* path, const int mode);

/**
 * @brief mgclFclose Close file.
 *
 * @param fPointer The file pointer.
 *
 * @returns success return 0, error return EOF.
 */
int mgclFclose(HTHandle fPointer);

/**
 * @brief mgclFread Read data from the file.
 *
 * @param ptr The store data's buffer.
 * @param size The record size.
 * @param nmemb The record count.
 * @param stream The file pointer.
 *
 * @returns success return nmemb, error used 'feof()' if.
 */
int mgclFread(void* ptr, int size, int nmemb, HTHandle stream);

/**
 * @brief mgclFwrite Write data to the file.
 *
 * @param ptr The store data's buffer.
 * @param size The record size.
 * @param nmemb The record count.
 * @param stream The file pointer.
 *
 * @returns reality write nmemb.
 */
int mgclFwrite(const void* ptr, int size, int nmemb, HTHandle stream);

/**
 * @brief mgclFputs Write a-line data to the file.
 *
 * @param ptr The store data's buffer.
 * @param stream The file pointer.
 *
 * @returns reality write nmemb.
 */
int mgclFputs(const char* ptr, HTHandle stream);

/**
 * @brief mgclFgets Read a-line data to the file.
 *
 * @param ptr The store data's buffer.
 * @param size The record size.
 * @param stream The file pointer.
 *
 * @returns char * string prt
 */
char* mgclFgets(char* ptr, int size, HTHandle stream);

/**
 * @brief mgclFeof Detect whether the end of the file.
 *
 * @param stream The file pointer.
 *
 * @returns Non-zero on success, otherwise return 0.
 */
int mgclFeof(HTHandle stream);

/**
 * @brief mgclFseek The move current location of a file.
 *
 * @param stream The file pointer.
 * @param offset The move offset.
 * @param whence The move mark.
 *
 * @returns 0 on success, -1 on error.
 */
int mgclFseek(HTHandle stream, long offset, int whence);

/**
 * @brief mgclFtell The current location of a file.
 *
 * @param stream The file pointer.
 *
 * @returns current location of file, -1 on error.
 */
long mgclFtell(HTHandle stream);

/**
 * @brief remove delete file.
 *
 * @param pathname The delete file path name.
 *
 * @returns 0 on success, -1 on error.
 */
int mgclRemove(const char* pathname);

/**
 * @brief mgclRename oldpath name to newpath name.
 *
 * @param oldpath The old file name.
 * @param newpath The new file name.
 *
 * @returns 0 on success, -1 on error.
 */
int mgclRename(const char* oldpath, const char* newpath);

/**
 * @brief mgclTruncate to truncate file stream to new length.
 *
 * @returns 0 on success, -1 on error.
 */
int mgclTruncate(HTHandle stream, unsigned int nFileSize);

} // namespace hfcl

#endif /* __HFCL_MGCLCOMMON_H__ */

