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

#include "resource/textres.h"

#include "resource/resloader.h"

namespace hfcl {

bool TextResZipped::load ()
{
#if 0
    if (m_l10n_zipped_str_table == NULL ||
            m_l10n_zipped_str_table [langId].zipped_bytes == NULL) {
        _ERR_PRINTF ("ResPkgManager::setCurrentLang: no strings for language: %d.\n", langId);
        return false;
    }

    if (m_string_bucket == NULL) {
        size_t max_bucket_size = 0;
        for (int i = 0; i < MAX_LANGID; i++) {
            size_t current_size = m_l10n_zipped_str_table [i].origin_size;
            if (current_size > max_bucket_size) {
                max_bucket_size = current_size;
            }
        }

        if (max_bucket_size == 0) {
            _ERR_PRINTF ("ResPkgManager::setCurrentLang: no any language defined.\n");
            return false;
        }

        m_string_bucket = (char*)malloc (max_bucket_size);
        if (m_string_bucket == NULL) {
            _ERR_PRINTF ("ResPkgManager::setCurrentLang: failed to allocate memory for bucket.\n");
            return false;
        }

        m_raw_strings = (const char**)malloc (sizeof (char*) * (m_max_text_id + 1));
        if (m_string_bucket == NULL) {
            _ERR_PRINTF ("ResPkgManager::setCurrentLang: failed to allocate memory for string table.\n");
            return false;
        }
    }

    uLongf len_uncompressed = m_l10n_zipped_str_table [langId].origin_size;
    int ret = uncompress ((Bytef*)m_string_bucket, &len_uncompressed,
            (Bytef*)m_l10n_zipped_str_table [langId].zipped_bytes,
            m_l10n_zipped_str_table [langId].zipped_size);
    if (ret != Z_OK) {
        _ERR_PRINTF ("ResPkgManager::setCurrentLang: failed when calling uncompress: %d.\n", ret);
        return false;
    }

#ifdef _DEBUG
    if (len_uncompressed != m_l10n_zipped_str_table [langId].origin_size) {
        _DBG_PRINTF ("ResPkgManager::setCurrentLang: length not matched: %lu, %u.\n",
            len_uncompressed, m_l10n_zipped_str_table [langId].origin_size);
        return false;
    }
#endif

    size_t offset = 0;
    for (HTResId i = 0; i < m_max_text_id; i++) {
        m_raw_strings [i] = m_string_bucket + offset;
        offset += strlen (m_raw_strings [i]);
        offset += 1;
    }

#ifdef _DEBUG
    if (offset > len_uncompressed) {
        _DBG_PRINTF ("ResPkgManager::setCurrentLang: buffer overflow: %lu, %u.\n",
            len_uncompressed, offset);
        return false;
    }
#endif
#endif
    return true;
}

void TextResZipped::release ()
{
    if (m_loaded) {
        free (m_string_bucket);
        m_loaded = false;
    }
    free (m_raw_strings);
    m_string_bucket = NULL;
    m_raw_strings = NULL;
}

const char* TextResGnuMsg::getText (const char* msgid)
{
    if (m_text_map.size () == 0) {
        return msgid;
    }

    TextMsgMap::iterator it = m_text_map.find (msgid);
    if (it == m_text_map.end ()) {
        return msgid;
    }

    return m_text_map [msgid];
}

/*
    load text from a GNU MO file.

    Please refer to

https://www.gnu.org/software/gettext/manual/gettext.html#MO-Files

    for the format of GNU MO files, or see the following chart:

        byte
             +------------------------------------------+
          0  | magic number = 0x950412de                |
             |                                          |
          4  | file format revision = 0                 |
             |                                          |
          8  | number of strings                        |  == N
             |                                          |
         12  | offset of table with original strings    |  == O
             |                                          |
         16  | offset of table with translation strings |  == T
             |                                          |
         20  | size of hashing table                    |  == S
             |                                          |
         24  | offset of hashing table                  |  == H
             |                                          |
             .                                          .
             .    (possibly more entries later)         .
             .                                          .
             |                                          |
          O  | length & offset 0th string  ----------------.
      O + 8  | length & offset 1st string  ------------------.
              ...                                    ...   | |
O + ((N-1)*8)| length & offset (N-1)th string           |  | |
             |                                          |  | |
          T  | length & offset 0th translation  ---------------.
      T + 8  | length & offset 1st translation  -----------------.
              ...                                    ...   | | | |
T + ((N-1)*8)| length & offset (N-1)th translation      |  | | | |
             |                                          |  | | | |
          H  | start hash table                         |  | | | |
              ...                                    ...   | | | |
  H + S * 4  | end hash table                           |  | | | |
             |                                          |  | | | |
             | NUL terminated 0th string  <----------------' | | |
             |                                          |    | | |
             | NUL terminated 1st string  <------------------' | |
             |                                          |      | |
              ...                                    ...       | |
             |                                          |      | |
             | NUL terminated 0th translation  <---------------' |
             |                                          |        |
             | NUL terminated 1st translation  <-----------------'
             |                                          |
              ...                                    ...
             |                                          |
             +------------------------------------------+
*/
static void read_mo_data (MG_RWops* src, Uint32* dst, int count, bool be)
{
    if (be) {
        for (int i = 0; i < count; i++) {
            dst [i] = MGUI_ReadBE32 (src);
        }
    }
    else {
        for (int i = 0; i < count; i++) {
            dst [i] = MGUI_ReadLE32 (src);
        }
    }
}

int TextResGnuMsg::loadFromFile (const char* mo_file)
{
    MG_RWops* src = MGUI_RWFromFile (mo_file, "r");
    bool be = false;

    if (src == NULL) {
        _ERR_PRINTF ("MyGetText::loadText: failed to open mo file: %s\n", mo_file);
        return -1;
    }

    Uint32 magic_number = MGUI_ReadLE32 (src);
    Uint32 revision;
    if (magic_number == 0x950412de) {
        be = false;
        revision = MGUI_ReadLE32 (src);
    }
    else if (magic_number == 0xde120495) {
        be = true;
        revision = MGUI_ReadBE32 (src);
    }
    else {
        MGUI_FreeRW (src);
        _ERR_PRINTF ("MyGetText::loadText: wrong magic number: 0x%X (%s)\n", magic_number, mo_file);
        return -2;
    }

    /* TODO check revision here */
    _MG_PRINTF ("MyGetText::loadText: revision number: 0x%X (%s)\n", revision, mo_file);

    Uint32 N;
    Uint32 O;
    Uint32 T;
    if (!be) {
        N = MGUI_ReadLE32 (src);
        O = MGUI_ReadLE32 (src);
        T = MGUI_ReadLE32 (src);
    }
    else {
        N = MGUI_ReadBE32 (src);
        O = MGUI_ReadBE32 (src);
        T = MGUI_ReadBE32 (src);
    }

    if (N == 0) {
        MGUI_FreeRW (src);
        _MG_PRINTF ("MyGetText::loadText: there is no any string: %s\n", mo_file);
        return 0;
    }

    m_text_map.clear ();

    Uint32* Olist = new Uint32 [N * 2];
    Uint32* Tlist = new Uint32 [N * 2];

    MGUI_RWseek (src, O, SEEK_SET);
    read_mo_data (src, Olist, N * 2, be);
    MGUI_RWseek (src, T, SEEK_SET);
    read_mo_data (src, Tlist, N * 2, be);

    for (unsigned i = 0; i < N; i++) {
        Uint32 orig_len = Olist [i * 2];
        char* orig_str = new char[orig_len + 1];
        if (orig_len != 0) {
            MGUI_RWseek (src, Olist [i * 2 + 1], SEEK_SET);
            MGUI_RWread (src, orig_str, sizeof (char), orig_len + 1);

        } else {
            orig_str [0] = '\0';
        }

        Uint32 tran_len = Tlist [i * 2];
        char* tran_str = new char[tran_len + 1];
        if (tran_str != 0) {
            MGUI_RWseek (src, Tlist[i * 2 + 1], SEEK_SET);
            MGUI_RWread (src, tran_str, sizeof (char), tran_len + 1);
        }
        else {
            tran_str [0] = '\0';
        }

        /* store to map */
        m_text_map [orig_str] = tran_str;

        /* check plural */
        size_t str_len = strlen (orig_str);
        if (orig_len > str_len) {
            char* plural = orig_str + str_len + 1;
            _MG_PRINTF ("MyGetText::loadText, skip plural %s in %s\n", plural, mo_file);
        }

        delete [] orig_str;
        delete [] tran_str;
    }

    delete [] Olist;
    delete [] Tlist;

    MGUI_FreeRW (src);
    return N;
}

bool TextResGnuMsg::load ()
{
    return true;
}

} // namespace hfcl

