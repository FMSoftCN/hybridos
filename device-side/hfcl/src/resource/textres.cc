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

#include <zlib.h>

#include "resource/resloader.h"

namespace hfcl {

bool TextResRaw::load ()
{
    m_raw_strings = (const char**)ResLoader::getInstance()->loadData (
            m_res_name, &m_is_incore);

    if (m_raw_strings == NULL) {
        _ERR_PRINTF ("TextResRaw::load: "
                "failed to load raw text resource\n");
        return false;
    }

    return true;
}

void TextResRaw::release ()
{
    if (m_is_incore && m_raw_strings) {
        free (m_raw_strings);
    }

    m_is_incore = false;
    m_raw_strings = NULL;
}

bool TextResZipped::load ()
{
    bool is_incore = false;
    HFCL_ZIPPED_STRINGS* zipped_str
        = (HFCL_ZIPPED_STRINGS*)ResLoader::getInstance()->loadData (
            m_res_name, &is_incore);

    if (zipped_str == NULL) {
        _ERR_PRINTF ("TextResZipped::load: "
                "failed to load zipped text resource\n");
        return false;
    }

    uLongf len_uncompressed = zipped_str->origin_size;
    size_t offset = 0;
    int ret;

    if (m_string_bucket)
        free (m_string_bucket);

    m_string_bucket = (char*)malloc (zipped_str->origin_size);
    if (m_string_bucket == NULL) {
        _ERR_PRINTF ("TextResZipped::load: "
                "failed to allocate memory for bucket\n");
        goto error;
    }

    m_raw_strings
        = (const char**)malloc (sizeof (char*) * zipped_str->nr_strings);
    if (m_raw_strings == NULL) {
        _ERR_PRINTF ("TextResZipped::load: "
                "failed to allocate memory for string table\n");
        goto error;
    }

    ret = uncompress ((Bytef*)m_string_bucket, &len_uncompressed,
            (Bytef*)zipped_str->zipped_bytes, zipped_str->zipped_size);
    if (ret != Z_OK) {
        _ERR_PRINTF ("TextResZipped::load: "
                "failed when calling uncompress: %d\n", ret);
        goto error;
    }

#ifdef _DEBUG
    if (len_uncompressed != zipped_str->origin_size) {
        _DBG_PRINTF ("TextResZipped::load: "
                "length not matched: %lu, %u\n",
                len_uncompressed, zipped_str->origin_size);
        goto error;
    }
#endif

    for (Uint32 i = 0; i < zipped_str->nr_strings; i++) {
        m_raw_strings [i] = m_string_bucket + offset;
        offset += strlen (m_raw_strings [i]);
        offset += 1;
    }

#ifdef _DEBUG
    if (offset > len_uncompressed) {
        _DBG_PRINTF ("TextResZipped::load: buffer overflow: %lu, %u\n",
            len_uncompressed, offset);
        goto error;
    }
#endif

    if (!is_incore)
        free (zipped_str);
    return true;

error:
    if (!is_incore)
        free (zipped_str);
    return false;
}

void TextResZipped::release ()
{
    if (m_string_bucket) {
        free (m_string_bucket);
    }

    if (m_raw_strings) {
        free (m_raw_strings);
    }

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

    return m_text_map [msgid].c_str ();
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

int TextResGnuMsg::doLoad (MG_RWops* src)
{
    bool be = false;

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
        _ERR_PRINTF ("TextResGnuMsg::doLoad: wrong magic number: 0x%X\n",
                magic_number);
        return -2;
    }

    /* TODO check revision here */
    _MG_PRINTF ("TextResGnuMsg::doLoad: revision number: 0x%X\n", revision);

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
        _MG_PRINTF ("TextResGnuMsg::doLoad: there is no any string\n");
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
            _MG_PRINTF ("TextResGnuMsg::doLoad: skip plural %s\n", plural);
        }

        delete [] orig_str;
        delete [] tran_str;
    }

    delete [] Olist;
    delete [] Tlist;

    return N;
}

bool TextResGnuMsg::load ()
{
    MG_RWops* src;
    const char* mo_file = m_res_name;
    const HFCL_INCORE_RES* res_info
        = ResLoader::getInstance()->getIncoreData (mo_file);

    if (res_info) {
        src = MGUI_RWFromMem (res_info->data, res_info->size);
    }
    else {
        src = MGUI_RWFromFile (mo_file, "r");
    }

    if (src == NULL) {
        _ERR_PRINTF ("TextResGnuMsg::load: failed to open mo file: %s\n",
                mo_file);
        return false;
    }

    doLoad (src);

    MGUI_FreeRW (src);

    return true;
}

void TextResGnuMsg::release ()
{
    m_text_map.clear ();
}

} // namespace hfcl

