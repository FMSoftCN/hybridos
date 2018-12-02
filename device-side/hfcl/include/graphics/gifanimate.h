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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HFCL_GifAnimate_h
#define _HFCL_GifAnimate_h

#include "nguxobject.h"
#include "graphicscontext.h"

namespace hfcl {

// !!! same as gen_incore_bitmap.c : BitmapFrame
typedef struct _tagGifAnimateFrame {
        int off_x;
        int off_y;
        int disposal;
        unsigned int delay_time;
        BITMAP bmp;
        struct _tagGifAnimateFrame* next;
        struct _tagGifAnimateFrame* prev;
}GifAnimateFrame;

#define MAXCOLORMAPSIZE         256
typedef struct tagGIFSCREEN {
    unsigned int Width;
    unsigned int Height;
    RGB ColorMap [MAXCOLORMAPSIZE];
    unsigned int BitPixel;
    unsigned int ColorResolution;
    unsigned int Background;
    unsigned int AspectRatio;
    int transparent;
    int delayTime;
    int inputFlag;
    int disposal;
} GIFSCREEN;

class GifAnimate : public RefCount 
{
    public:
        GifAnimate();
        GifAnimate(int ops);
        virtual ~GifAnimate();

        void drawFrame(void);

        void firstFrame(void);

        void nextFrame(void);

        // create from file system : file path
        void createGifAnimateFromFile (const char* file);

        // create from innner res : BitmapFrameArray *
        void createGifAnimateFromRes(BitmapFrameArray* bitmap_frame_array);
        void createGifAnimateFromMem(const char * data,unsigned int size);
        void loadGifAnimateNextFrameFromMem(void);
        void drawOneFrame(GraphicsContext* graphics, const IntRect &rect, GifAnimateFrame* frame = NULL);

        inline int currentFrameDelay(void) { return m_current_frame ? m_current_frame->delay_time : -1; }

        int frameCount(void) {return m_nr_frames;}

        bool isLastFrame(void) { return (m_last_frame == m_current_frame);}
	  unsigned int getNextFrameIndex(void){return m_nextFrameIndex;}

    protected:
        void restorePrevFrame(const IntRect &rect, GifAnimateFrame* frame);
        void restoreBkgColor(const IntRect &rect, GifAnimateFrame* frame);
        void drawFrameOnMem(const IntRect &rect, GifAnimateFrame* frame);

    private:
        GifAnimateFrame* m_frames;
        GifAnimateFrame* m_current_frame;
        GifAnimateFrame* m_last_frame;
        unsigned int m_nr_frames;
        unsigned int m_max_width;
        unsigned int m_max_height;
        bool m_is_scale;
        GraphicsContext* m_mem_gc;

        unsigned char m_c;
        MG_RWops* m_area;
        GIFSCREEN m_GifScreen;
        unsigned int m_nextFrameIndex;
};

} // namespace hfcl {


#endif  /* HFCL_GifAnimate_h */
