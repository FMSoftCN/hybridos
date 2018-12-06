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

//#undef _DEBUG

#include "graphics/gifanimate.h"

#include <string.h>

namespace hfcl {

#define MAXCOLORMAPSIZE         256
#define MAX_LWZ_BITS            12
#define INTERLACE               0x40
#define LOCALCOLORMAP           0x80

#define CM_RED                  0
#define CM_GREEN                1
#define CM_BLUE                 2

#define BitSet(byte, bit)               (((byte) & (bit)) == (bit))
#define ReadOK(file,buffer,len)         MGUI_RWread(file, buffer, len, 1)
#define LM_to_uint(a,b)                 (((b)<<8)|(a))
#define PIX2BYTES(n)                    (((n)+7)/8)

typedef struct tagIMAGEDESC {
    int Top;
    int Left;
    int Width;
    int Height;
    BOOL haveColorMap;
    int bitPixel;
    int grayScale;
    RGB ColorMap [MAXCOLORMAPSIZE];
    BOOL interlace;
} IMAGEDESC;

static int ZeroDataBlock = 0;

static int bmpComputePitch (int bpp, Uint32 width, Uint32* pitch, BOOL does_round);
static int LWZReadByte (MG_RWops *area, int flag, int input_code_size);
static int GetCode (MG_RWops *area, int code_size, int flag);
static int GetDataBlock (MG_RWops *area, unsigned char *buf);
static int DoExtension (MG_RWops *area, int label, GIFSCREEN* GifScreen);
static int ReadColorMap (MG_RWops *area, int number, RGB* ColorMap);
static int ReadImageDesc (MG_RWops *area, IMAGEDESC* ImageDesc, GIFSCREEN* GifScreen);

static int bmpComputePitch (int bpp, Uint32 width, Uint32* pitch, BOOL does_round)
{
    Uint32 linesize;
    int bytespp = 1;

    if(bpp == 1)
        linesize = PIX2BYTES (width);
    else if(bpp <= 4)
        linesize = PIX2BYTES (width << 2);
    else if (bpp <= 8)
        linesize = width;
    else if(bpp <= 16) {
        linesize = width * 2;
        bytespp = 2;
    } else if(bpp <= 24) {
        linesize = width * 3;
        bytespp = 3;
    } else {
        linesize = width * 4;
        bytespp = 4;
    }

    /* rows are DWORD right aligned*/
    if (does_round)
        *pitch = (linesize + 3) & -4;
    else
        *pitch = linesize;
    return bytespp;
}

static int LWZReadByte (MG_RWops *area, int flag, int input_code_size)
{
    int code, incode;
    register int i;
    static int fresh = FALSE;
    static int code_size, set_code_size;
    static int max_code, max_code_size;
    static int firstcode, oldcode;
    static int clear_code, end_code;
    static int table[2][(1 << MAX_LWZ_BITS)];
    static int stack[(1 << (MAX_LWZ_BITS)) * 2], *sp;

    if (flag) {
        set_code_size = input_code_size;
        code_size = set_code_size + 1;
        clear_code = 1 << set_code_size;
        end_code = clear_code + 1;
        max_code_size = 2 * clear_code;
        max_code = clear_code + 2;

        GetCode(area, 0, TRUE);

        fresh = TRUE;

        for (i = 0; i < clear_code; ++i) {
            table[0][i] = 0;
            table[1][i] = i;
        }
        for (; i < (1 << MAX_LWZ_BITS); ++i)
            table[0][i] = table[1][0] = 0;

        sp = stack;

        return 0;
    } else if (fresh) {
        fresh = FALSE;
        do {
            firstcode = oldcode = GetCode(area, code_size, FALSE);
        } while (firstcode == clear_code);
        return firstcode;
    }

    if (sp > stack)
        return *--sp;

    while ((code = GetCode(area, code_size, FALSE)) >= 0) {
        if (code == clear_code) {
            for (i = 0; i < clear_code; ++i) {
                table[0][i] = 0;
                table[1][i] = i;
            }
            for (; i < (1 << MAX_LWZ_BITS); ++i)
                table[0][i] = table[1][i] = 0;
            code_size = set_code_size + 1;
            max_code_size = 2 * clear_code;
            max_code = clear_code + 2;
            sp = stack;
            firstcode = oldcode = GetCode(area, code_size, FALSE);
            return firstcode;
        } else if (code == end_code) {
            int count;
            unsigned char buf[260];

            if (ZeroDataBlock)
                return -2;

            while ((count = GetDataBlock(area, buf)) > 0);
            if (count != 0) {
                /*
                 * _DBG_PRINTF ("missing EOD in data stream (common occurence)\n");
                 */
            }
            return -2;
        }
        incode = code;

        if (code >= max_code) {
            *sp++ = firstcode;
            code = oldcode;
        }
        while (code >= clear_code) {
            *sp++ = table[1][code];
            if (code == table[0][code]) {
                return -1;
            }
            code = table[0][code];
        }

        *sp++ = firstcode = table[1][code];

        if ((code = max_code) < (1 << MAX_LWZ_BITS)) {
            table[0][code] = oldcode;
            table[1][code] = firstcode;
            ++max_code;
            if ((max_code >= max_code_size) &&
                (max_code_size < (1 << MAX_LWZ_BITS))) {
                max_code_size *= 2;
                ++code_size;
            }
        }
        oldcode = incode;

        if (sp > stack)
            return *--sp;
    }
    return code;
}


static int GetCode(MG_RWops *area, int code_size, int flag)
{
    static unsigned char buf[280];
    static int curbit, lastbit, done, last_byte;
    int i, j, ret;
    unsigned char count;

    if (flag) {
        curbit = 0;
        lastbit = 0;
        done = FALSE;
        return 0;
    }
    if ((curbit + code_size) >= lastbit) {
        if (done) {
            if (curbit >= lastbit)
            return -1;
        }
        buf[0] = buf[last_byte - 2];
        buf[1] = buf[last_byte - 1];

        if ((count = GetDataBlock(area, &buf[2])) == 0)
            done = TRUE;

        last_byte = 2 + count;
        curbit = (curbit - lastbit) + 16;
        lastbit = (2 + count) * 8;
    }
    ret = 0;
    for (i = curbit, j = 0; j < code_size; ++i, ++j)
        ret |= ((buf[i / 8] & (1 << (i % 8))) != 0) << j;

    curbit += code_size;

    return ret;
}

static int GetDataBlock (MG_RWops *area, unsigned char *buf)
{
    unsigned char count;

    if (!ReadOK(area, &count, 1))
        return 0;
    ZeroDataBlock = (count == 0);

    if ((count != 0) && (!ReadOK(area, buf, count)))
        return 0;
    return count;
}

static int DoExtension (MG_RWops *area, int label, GIFSCREEN* GifScreen)
{
    static unsigned char buf[256];

    switch (label) {
    case 0x01:                        /* Plain Text Extension */
        while (GetDataBlock (area, (unsigned char *) buf) != 0);
        break;
    case 0xff:                        /* Activity Extension */
        while (GetDataBlock (area, (unsigned char *) buf) != 0);
        break;
    case 0xfe:                        /* Comment Extension */
        while (GetDataBlock (area, (unsigned char *) buf) != 0);
        return 0;
    case 0xf9:                        /* Graphic Control Extension */
        GetDataBlock (area, (unsigned char *) buf);
        GifScreen->disposal = (buf[0] >> 2) & 0x7;//000 000 0 0 the middle 2 bit is disposal
        GifScreen->inputFlag = (buf[0] >> 1) & 0x1;//000 000 0 0 the secand last bit
                            //is user input flag
        GifScreen->delayTime = LM_to_uint(buf[1], buf[2]);
        if ((buf[0] & 0x1) != 0)// 000 000 0 0 the last bit is transparent flag
            GifScreen->transparent = buf[3];
        else
            GifScreen->transparent = -1;

        while (GetDataBlock (area, (unsigned char *) buf) != 0);
        return 0;
    default:
        while (GetDataBlock (area, (unsigned char *) buf) != 0);
        break;
    }

    return 0;
}

static int ReadColorMap (MG_RWops *area, int number, RGB* ColorMap)
{
    int i;
    unsigned char rgb[3];

    for (i = 0; i < number; ++i) {
        if (!ReadOK (area, rgb, sizeof(rgb))) {
            return -1;
        }

        ColorMap [i].r = rgb[0];
        ColorMap [i].g = rgb[1];
        ColorMap [i].b = rgb[2];
    }

    return 0;
}

static int ReadGIFGlobal (MG_RWops *area, GIFSCREEN* GifScreen)
{
    unsigned char buf[9];
    unsigned char version[4];

    if (!ReadOK (area, buf, 6))
        return -1;                /* not gif image*/

    if (strncmp((char *) buf, "GIF", 3) != 0)
        return -1;

    strncpy ((char*)version, (char *) buf + 3, 3);
    version [3] = '\0';

    if (strcmp ((const char*)version, "87a") != 0 && strcmp ((const char*)version, "89a") != 0) {
        return -1;                /* image loading error*/
    }

    GifScreen->Background = -1;
    GifScreen->transparent = -1;
    GifScreen->delayTime = -1;
    GifScreen->inputFlag = -1;
    GifScreen->disposal = 0;

    if (!ReadOK (area, buf, 7)) {
        return -1;                /* image loading error*/
    }
    GifScreen->Width = LM_to_uint (buf[0], buf[1]);
    GifScreen->Height = LM_to_uint (buf[2], buf[3]);
    GifScreen->BitPixel = 2 << (buf[4] & 0x07);
    GifScreen->ColorResolution = (((buf[4] & 0x70) >> 3) + 1);
    GifScreen->Background = buf[5];
    GifScreen->AspectRatio = buf[6];

    if (BitSet(buf[4], LOCALCOLORMAP)) {        /* Global Colormap */
        if (ReadColorMap (area, GifScreen->BitPixel, GifScreen->ColorMap)) {
            return -1;                /* image loading error*/
        }
    }

    return 0;
}

static int ReadImageDesc (MG_RWops *area, IMAGEDESC* ImageDesc, GIFSCREEN* GifScreen)
{
    unsigned char buf[16];
    if (!ReadOK (area, buf, 9)) {
        return -1;
    }

    ImageDesc->Top = LM_to_uint (buf[0], buf[1]);
    ImageDesc->Left = LM_to_uint (buf[2], buf[3]);
    ImageDesc->Width = LM_to_uint (buf[4], buf[5]);
    ImageDesc->Height = LM_to_uint (buf[6], buf[7]);
    ImageDesc->haveColorMap = BitSet (buf[8], LOCALCOLORMAP);

    ImageDesc->bitPixel = 1 << ((buf[8] & 0x07) + 1);

    ImageDesc->interlace = BitSet(buf[8], INTERLACE);

    if (ImageDesc->haveColorMap) {
        if (ReadColorMap (area, ImageDesc->bitPixel, ImageDesc->ColorMap) < 0) {
            return -1;
        }
    } else {
        memcpy (ImageDesc->ColorMap, GifScreen->ColorMap, MAXCOLORMAPSIZE*sizeof (RGB));
    }

    return 0;
}

static int ReadImage (MG_RWops* area, MYBITMAP* bmp, IMAGEDESC* ImageDesc, GIFSCREEN* GifScreen, int ignore)
{

    unsigned char c;
    int v;
    int xpos = 0, ypos = 0, pass = 0;

    /*
     * initialize the compression routines
     */
    if (!ReadOK (area, &c, 1)) {
        return -1;
    }

    if (LWZReadByte (area, TRUE, c) < 0) {
        return -1;
    }

    /*
     * if this is an "uninteresting picture" ignore it.
     */
    if (ignore) {
        while (LWZReadByte (area, FALSE, c) >= 0);
        return 0;
    }

    bmp->w = ImageDesc->Width;
    bmp->h = ImageDesc->Height;

    bmp->flags = MYBMP_FLOW_DOWN;
    if (GifScreen->transparent >= 0) {
        bmp->flags |= MYBMP_TRANSPARENT;
        bmp->transparent = GifScreen->transparent;
    }
    bmp->frames = 1;
    bmp->depth = 8;
    bmpComputePitch (bmp->depth, bmp->w, &bmp->pitch, TRUE);
    // bmp->bits = (BYTE *)malloc (bmp->h * bmp->pitch);
    bmp->bits = HFCL_NEW_ARR(BYTE, (bmp->h * bmp->pitch));

    if(!bmp->bits)
        return -1;

    while ((v = LWZReadByte (area, FALSE, c)) >= 0) {
        bmp->bits[ypos * bmp->pitch + xpos] = v;
        ++xpos;
        if (xpos == ImageDesc->Width) {
            xpos = 0;
            if (ImageDesc->interlace) {
                switch (pass) {
                case 0:
                case 1:
                    ypos += 8;
                    break;
                case 2:
                    ypos += 4;
                    break;
                case 3:
                    ypos += 2;
                    break;
                }

                if (ypos >= ImageDesc->Height) {
                    ++pass;
                    switch (pass) {
                    case 1:
                        ypos = 4;
                        break;
                    case 2:
                        ypos = 2;
                        break;
                    case 3:
                        ypos = 1;
                        break;
                    default:
                        goto fini;
                    }
                }
            } else {
                ++ypos;
            }
        }
        if (ypos >= ImageDesc->Height)
            break;
    }

fini:
    if (v >= 0) return 0;
    return -1;
}

void GifAnimate::createGifAnimateFromRes(BitmapFrameArray* bitmap_frame_array)
{
    GifAnimateFrame* frame = NULL, *current = NULL;
    BitmapFrame* curren_bitmap = NULL;
    unsigned int max_width = 0, max_height = 0;
    int i = 0, c = 0;

    if (NULL == bitmap_frame_array)
        return;
    if (bitmap_frame_array->nr_frames < 1)
        return;

    c = bitmap_frame_array->nr_frames;
    //curren_bitmap = bitmap_frame_array->frames;

    for (i = 0; i < c; ++i)
    {
        // frame = (GifAnimateFrame*) calloc(1, sizeof(GifAnimateFrame));

        frame = HFCL_NEW(GifAnimateFrame);
        curren_bitmap = bitmap_frame_array->frames + i; // add this
        if(!frame)
            return;

        frame->next = NULL;
        frame->off_y = curren_bitmap->off_y;
        frame->off_x = curren_bitmap->off_x;
        frame->disposal = curren_bitmap->disposal;
        frame->delay_time = curren_bitmap->delay_time;
        frame->bmp = *(curren_bitmap->bmp);

        if(frame->bmp.bmWidth > max_width)
            max_width = frame->bmp.bmWidth;

        if(frame->bmp.bmHeight > max_height)
            max_height = frame->bmp.bmHeight;

        if(m_frames == NULL)
        {
            m_frames = frame;
            current = frame;
            current->prev = NULL;
        }
        else
        {
            frame->prev = current;
            current->next = frame;
            current = current->next;
        }

        m_last_frame = frame;

        m_nr_frames++;
        //curren_bitmap = ++bitmap_frame_array->frames;// + i; //bug fix
        //curren_bitmap = bitmap_frame_array->frames + i; //bug fix
    }
    m_current_frame = m_frames;
    m_max_width = max_width;
    m_max_height = max_height;

    if(m_mem_gc ) {
        DeleteMemGC(m_mem_gc);
    }

    m_mem_gc = CreateMemGC(m_max_width, m_max_height);
#ifdef _DEBUG
    if (NULL == m_mem_gc) {
        _ERR_PRINTF ("Error -- GifAnimate::createGifAnimateFromRes CreateMemGC Error. 1\n");
    }
#endif
}

void GifAnimate::createGifAnimateFromMem(const char * data, unsigned int size)
{
    unsigned char c;
    int ok = 0;
    MYBITMAP mybmp;

    IMAGEDESC ImageDesc;
    GifAnimateFrame *frame = NULL;
    GifAnimateFrame *current = NULL;

    if (!(m_area = MGUI_RWFromMem ((void*)data, size))) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (m_area == NULL).\n");
#endif
        return;
    }

    if (ReadGIFGlobal (m_area, &m_gif_screen) < 0) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (ReadGIFGlobal < 0).\n");
#endif
        return;
    }

    if ((ok = ReadOK (m_area, &c, 1)) == 0) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (ReadOK == 0).\n");
#endif
        return;
    }

    while (c != ';' && ok > 0) {
        switch (c) {
        case '!':
            if ( (ok = ReadOK (m_area, &c, 1)) == 0) {
#ifdef _DEBUG
                _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (case '!' ReadOK == 0).\n");
#endif
                return;
            }
            DoExtension (m_area, c, &m_gif_screen);
            break;

        case ',':
            if (ReadImageDesc (m_area, &ImageDesc, &m_gif_screen) < 0) {
#ifdef _DEBUG
                _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (ReadImageDesc < 0).\n");
#endif
                return;
            }
            else {
                if (ReadImage (m_area, &mybmp, &ImageDesc, &m_gif_screen, 0) < 0) {
#ifdef _DEBUG
                    _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (ReadImage < 0).\n");
#endif
                    return;
                }
            }

            frame = HFCL_NEW(GifAnimateFrame);

            if(!frame) {
#ifdef _DEBUG
                _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (new frame).\n");
#endif
                return;
            }
            frame->next = NULL;
            frame->off_y = ImageDesc.Left;
            frame->off_x = ImageDesc.Top;
            frame->disposal = m_gif_screen.disposal;

            frame->delay_time = (m_gif_screen.delayTime>10)?m_gif_screen.delayTime*10:10;

            if(ExpandMyBitmap(HDC_SCREEN, &frame->bmp, &mybmp, ImageDesc.ColorMap, 0) != 0)
            {
                if(frame)
                    HFCL_DELETE(frame);
                frame = NULL;
                if(mybmp.bits)
                    HFCL_DELETE_ARR(mybmp.bits);
                mybmp.bits = NULL;
#ifdef _DEBUG
                _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem error (ExpandMyBitmap).\n");
#endif
                return;
            }

            if(m_frames == NULL)
            {
                m_frames = frame;
                current = frame;
                current->prev = NULL;
            }
            else
            {
                frame->prev = current;
                current->next = frame;
                current = current->next;
            }

            m_nr_frames++;

            if(mybmp.bits)
                HFCL_DELETE_ARR(mybmp.bits);

            if(m_nr_frames >= 2)
            {
                ok = ReadOK (m_area, &c, 1);
                m_current_frame = m_frames;
                m_max_width = m_gif_screen.Width;
                m_max_height = m_gif_screen.Height;
                m_c = c;
                if(!(c != ';' && ok > 0))
                {
                    m_last_frame = frame;
                }

                goto ret_tag_0;
            }

            break;
        }
        ok = ReadOK (m_area, &c, 1);
    }

    m_current_frame = m_frames;
    m_last_frame = m_current_frame;
    m_max_width = m_gif_screen.Width;
    m_max_height = m_gif_screen.Height;

ret_tag_0:
    if (m_mem_gc) {
        DeleteMemGC (m_mem_gc);
    }

    m_mem_gc = CreateMemGC(m_max_width, m_max_height);

#ifdef _DEBUG
    if(NULL == m_mem_gc) {
        _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem CreateMemGC Error. 0\n");
    }
#endif
}

void GifAnimate::loadGifAnimateNextFrameFromMem()
{
    unsigned char c;
    int ok = 1;
    MYBITMAP mybmp;
    IMAGEDESC ImageDesc;
    GifAnimateFrame*frame, *current = NULL;

    frame = NULL;
    c = m_c;

    if(NULL != (current = m_frames)) {
        m_frames = m_frames->next;
        UnloadBitmap(&current->bmp);
        HFCL_DELETE(current);
        current = m_frames;
        if(current)
                 current->prev = NULL;
    }

    while (c != ';' && ok > 0) {
        switch (c) {
        case '!':
            if ( (ok = ReadOK (m_area, &c, 1)) == 0) {
                return;
            }
            DoExtension (m_area, c, &m_gif_screen);
            break;

        case ',':
            if (ReadImageDesc (m_area, &ImageDesc, &m_gif_screen) < 0) {
                return;
            }
            else {
                if (ReadImage (m_area, &mybmp, &ImageDesc, &m_gif_screen, 0) < 0)
                    return;
            }

            frame = HFCL_NEW(GifAnimateFrame);


            if(!frame)
                return;

            frame->next = NULL;
            frame->off_y = ImageDesc.Left;
            frame->off_x = ImageDesc.Top;
            frame->disposal = m_gif_screen.disposal;

            frame->delay_time = (m_gif_screen.delayTime>10)?m_gif_screen.delayTime*10:10;

            if(ExpandMyBitmap(HDC_SCREEN, &frame->bmp, &mybmp, ImageDesc.ColorMap, 0) != 0)
            {
                if(frame)
                    HFCL_DELETE(frame);
                frame = NULL;
                if(mybmp.bits)
                    HFCL_DELETE_ARR(mybmp.bits);
                mybmp.bits = NULL;
                return;
            }

            if(m_frames == NULL)
            {
                m_frames = frame;
                current = frame;
                current->prev = NULL;
            }
            else
            {
                frame->prev = current;
                current->next = frame;
                current = current->next;
            }

            m_nr_frames++;

            if(mybmp.bits)
                HFCL_DELETE_ARR(mybmp.bits);

            if(m_nr_frames >= 2)
            {
                ok = ReadOK (m_area, &c, 1);
                m_current_frame = frame;
                m_max_width = m_gif_screen.Width;
                m_max_height = m_gif_screen.Height;
                m_c = c;
                if(!(c != ';' && ok > 0))
                {
                    m_last_frame = m_current_frame;
                }

                goto ret_tag;
            }
            break;
        }
        ok = ReadOK (m_area, &c, 1);
    }

    m_last_frame = m_current_frame;

ret_tag:
    if (m_max_width != m_gif_screen.Width || m_max_height != m_gif_screen.Height) {
        m_max_width = m_gif_screen.Width;
        m_max_height = m_gif_screen.Height;

        if (m_mem_gc) {
            DeleteMemGC(m_mem_gc);
        }

        m_mem_gc = CreateMemGC(m_max_width, m_max_height);
#ifdef _DEBUG
        if(NULL == m_mem_gc) {
            _DBG_PRINTF ("GifAnimate::createGifAnimateFromMem CreateMemGC Error. 0\n");
        }
#endif
    }
}

void GifAnimate::createGifAnimateFromFile (const char* file)
{
    unsigned char c;
    int ok = 0;
    MYBITMAP mybmp;

    IMAGEDESC ImageDesc;
    GifAnimateFrame *frame = NULL;
    GifAnimateFrame *current = NULL;

#ifdef _MGRWOPS_RTOSIO
    if (!(m_area = MGUI_RWFromFileRTOS (file, RTOS_FS_O_RDONLY))) {
        return;
    }
#else
    if (!(m_area = MGUI_RWFromFile (file, "rb"))) {
        return;
    }
#endif

    if (ReadGIFGlobal (m_area, &m_gif_screen) < 0)
        return;

    if ((ok = ReadOK (m_area, &c, 1)) == 0) {
        return;
    }

    while (c != ';' && ok > 0) {
        switch (c) {
        case '!':
            if ( (ok = ReadOK (m_area, &c, 1)) == 0) {
                return;
            }
            DoExtension (m_area, c, &m_gif_screen);
            break;

        case ',':
            if (ReadImageDesc (m_area, &ImageDesc, &m_gif_screen) < 0) {
                return;
            }
            else {
                if (ReadImage (m_area, &mybmp, &ImageDesc, &m_gif_screen, 0) < 0)
                    return;
            }

            frame = HFCL_NEW(GifAnimateFrame);

            if(!frame)
                return;

            frame->next = NULL;
            frame->off_y = ImageDesc.Left;
            frame->off_x = ImageDesc.Top;
            frame->disposal = m_gif_screen.disposal;

            frame->delay_time = (m_gif_screen.delayTime>10)?m_gif_screen.delayTime*10:10;

            if(ExpandMyBitmap(HDC_SCREEN, &frame->bmp, &mybmp, ImageDesc.ColorMap, 0) != 0)
            {
                if(frame)
                    HFCL_DELETE(frame);
                frame = NULL;
                if(mybmp.bits)
                    HFCL_DELETE_ARR(mybmp.bits);
                mybmp.bits = NULL;
                return;
            }

            if(m_frames == NULL)
            {
                m_frames = frame;
                current = frame;
                current->prev = NULL;
            }
            else
            {
                frame->prev = current;
                current->next = frame;
                current = current->next;
            }

            m_nr_frames++;
            if(mybmp.bits)
                HFCL_DELETE_ARR(mybmp.bits);
            if(m_nr_frames >= 2)
            {
                ok = ReadOK (m_area, &c, 1);
                m_current_frame = m_frames;
                m_max_width = m_gif_screen.Width;
                m_max_height = m_gif_screen.Height;
                m_c = c;
                if(!(c != ';' && ok > 0))
                {
                    m_last_frame = frame;
                }

                goto ret_tag_0;
            }

            break;
        }
        ok = ReadOK (m_area, &c, 1);
    }

    m_current_frame = m_frames;
    m_last_frame = m_current_frame;
    m_max_width = m_gif_screen.Width;
    m_max_height = m_gif_screen.Height;

ret_tag_0:
    if (m_mem_gc) {
        DeleteMemGC(m_mem_gc);
    }

    m_mem_gc = CreateMemGC(m_max_width, m_max_height);
#ifdef _DEBUG
    if(NULL == m_mem_gc) {
        _DBG_PRINTF ("Error -- GifAnimate::createGifAnimateFromMem CreateMemGC Error. 0\n");
    }
#endif
}

void GifAnimate::nextFrame(void)
{
    m_next_frame_idx++;
    if(m_next_frame_idx >= 2) {
        loadGifAnimateNextFrameFromMem();
        if(m_last_frame == m_current_frame)
        {
            return;
        }
    }
    else {
        if (NULL != m_current_frame)
        {
            if (NULL != m_current_frame->next)
                m_current_frame = m_current_frame->next;
            else
                m_current_frame = m_frames;
        }
    }
}

void GifAnimate::firstFrame(void)
{
    int ok = 0;
    m_area->seek(m_area,0,SEEK_SET);

    if (ReadGIFGlobal (m_area, &m_gif_screen) < 0)
        return;

    if ((ok = ReadOK (m_area, &m_c, 1)) == 0) {
        return;
    }
    m_next_frame_idx = 0;
    m_nr_frames = 0;
    loadGifAnimateNextFrameFromMem();
    m_current_frame = m_frames;
    m_last_frame = NULL;
}

GifAnimate::GifAnimate() : RefCount (0)
{
    m_current_frame = NULL;
    m_frames = NULL;
    m_nr_frames = 0;
    m_max_width = 0;
    m_max_height = 0;
    m_is_scale = false;
    m_next_frame_idx = 0;
    m_mem_gc = NULL;
}

#if 0 // VW: remove this constructor
GifAnimate::GifAnimate(int ops) : RefCount(0)
{
    m_current_frame = NULL;
    m_frames = NULL;
    m_nr_frames = 0;
    m_max_width = 0;
    m_max_height = 0;
    m_is_scale = false;
    m_next_frame_idx = 0;

    if (m_mem_gc) {
        _DBG_PRINTF ("GifAnimate::GifAnimate some where not free\n");
    }

    m_mem_gc = CreateMemGC(_ngux_screen_w, _ngux_screen_h);

#ifdef _DEBUG
    if (NULL == m_mem_gc) {
        _DBG_PRINTF ("GifAnimate::GifAnimate CreateMemGC Error. 2\n");
    }
#endif
}
#endif

GifAnimate::~GifAnimate()
{
    GifAnimateFrame *f = NULL;

    while(NULL != (f = m_frames)) {
        m_frames = m_frames->next;
        UnloadBitmap(&f->bmp);
        HFCL_DELETE(f);
#ifdef _DEBUG
        _DBG_PRINTF ("GifAnimate::GifAnimate delete f=%x", f);
#endif
    }
    MGUI_RWclose (m_area);

    if (NULL != m_mem_gc) {
        DeleteMemGC(m_mem_gc);
        m_mem_gc = NULL;
    }
}

void GifAnimate::restoreBkgColor(const IntRect &rect, GifAnimateFrame* frame)
{
    gal_pixel old_color = -1, bg_color = 0xFFC08000;

    if (NULL == m_mem_gc)
        return;

    bg_color = GetWindowElementPixel(HWND_DESKTOP, WE_BGC_DESKTOP);

    SetMemDCColorKey(m_mem_gc->context(), MEMDC_FLAG_SRCCOLORKEY, bg_color);

    old_color = SetBrushColor(m_mem_gc->context(), bg_color);

    if(frame)
        m_mem_gc->fillBox(frame->off_x, frame->off_y,
                frame->bmp.bmWidth, frame->bmp.bmHeight);
    else
        m_mem_gc->fillBox(0, 0, m_max_width, m_max_height);

    SetBrushColor(m_mem_gc->context(), old_color);
}

void GifAnimate::restorePrevFrame(const IntRect &rect, GifAnimateFrame* frame)
{
    if(frame == NULL)
        return;

    switch(frame->disposal) {
    case 3:
        restorePrevFrame(rect, frame->prev);
        break;
    case 2:
        restoreBkgColor(rect, frame->prev);
        break;

    default:
        break;
    }

    drawFrameOnMem(rect, frame);
}

void GifAnimate::drawFrameOnMem(const IntRect &rect, GifAnimateFrame* frame)
{
    if (NULL == frame) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error :: GifAnimate::drawFrameOnMem ... frame == NULL \n");
#endif
        return;
    }
    if (NULL == m_mem_gc) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error :: GifAnimate::drawFrameOnMem ... m_mem_gc == NULL \n");
#endif
        return;
    }

    if(m_is_scale)
    {
        m_mem_gc->fillBoxWithBitmap(0, 0,
                rect.width(), rect.height(), &frame->bmp);
    }
    else
    {
        m_mem_gc->fillBoxWithBitmap(frame->off_x,
                frame->off_y, -1, -1, &frame->bmp);
    }
}

void GifAnimate::drawOneFrame(GraphicsContext* graphics,
        const IntRect &rect, GifAnimateFrame* frame)
{
    if (NULL == graphics) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error :: GifAnimate::drawOneFrame ... graphics == NULL \n");
#endif
        return;
    }

    if (NULL == m_current_frame) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error :: GifAnimate::drawOneFrame ... m_current_frame == NULL \n");
#endif
        return;
    }

    if (NULL == m_mem_gc) {
#ifdef _DEBUG
        _DBG_PRINTF ("Error :: GifAnimate::drawOneFrame ... m_mem_gc == NULL \n");
#endif
        return;
    }

    restorePrevFrame(rect, frame == NULL ? m_current_frame : frame);

    graphics->bitBlt(m_mem_gc, 0, 0,
            m_max_width, m_max_height, rect.left(), rect.top(), 0);
}

} // namespace hfcl


