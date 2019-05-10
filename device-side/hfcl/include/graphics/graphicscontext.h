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

#ifndef HFCL_GRAPHICS_GRAPHICSCONTEXT_H_
#define HFCL_GRAPHICS_GRAPHICSCONTEXT_H_

#include <string>

#include "../common/stlalternative.h"
#include "../common/object.h"
#include "../common/intrect.h"
#include "../mgcl/mgcl.h"

namespace hfcl {

typedef BITMAP  Bitmap;
typedef DEVFONT Devfont;
typedef POINT   Point;
typedef LOGFONT Logfont;

// for inner resource
typedef struct _tagBitmapFrame {
    int off_x;
    int off_y;
    int disposal;
    unsigned int delay_time;
    PBITMAP bmp;
    struct _tagBitmapFrame* next;
    struct _tagBitmapFrame* prev;
} BitmapFrame;

typedef struct _tagBitmapFrameArray
{
    int nr_frames;
    BitmapFrame* frames;
} BitmapFrameArray;

#define HFCL_BMP_TYPE_NORMAL         0x00
#define HFCL_BMP_TYPE_RLE            0x01
#define HFCL_BMP_TYPE_ALPHA          0x02
#define HFCL_BMP_TYPE_ALPHACHANNEL   0x04
#define HFCL_BMP_TYPE_COLORKEY       0x10
#define HFCL_BMP_TYPE_ALPHA_MASK     0x20
#define HFCL_BMP_TYPE_PRIV_PIXEL     0x00

class Image;
class View;
class GraphicsContextPrivate;

class GraphicsContext : public Object {
public:
    GraphicsContext(HDC);
    ~GraphicsContext();

    static GraphicsContext* screenGraphics();
    static void deleteScreenGraphics();
    static int px2dots(HTReal px);
    static HTReal dots2px(int dots);

    void translation(int offx, int offy);
    void getTranslation(int& offx, int & offy);

    Logfont* getLogFont();

    void clip(const IntRect&);
    void clipOut(const IntRect&);
    void clipIn(const IntRect&);
    bool rectVisible(IntRect&);

    void save();
    void restore();

    int setTextCharacterExtra(int extra) {
        return SetTextCharacterExtra (context(), extra);
    }
    int setTextAboveLineExtra(int extra) {
        return SetTextAboveLineExtra (context(), extra);
    }
    int setTextBellowLineExtra(int extra) {
        return SetTextBellowLineExtra(context(), extra);
    }
    int getTextCharacterExtra() {
        return GetTextCharacterExtra (context());
    }
    int getTextAboveLineExtra() {
        return GetTextAboveLineExtra (context());
    }
    int getTextBellowLineExtra() {
        return GetTextBellowLineExtra(context());
    }

    void fillRect(const IntRect& rc, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);
    void fillBox(int x, int y, int w, int h);
    void bitBlt(GraphicsContext* src, int sx, int sy, int sw, int sh,
            int dx, int dy, Uint32 op);
    void rectangle(int x0, int y0, int x1, int y1);
    void rectangle(int x0, int y0, int x1, int y1,
            Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
    void drawLine(int x0, int y0, int x1, int y1, int width,
            Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
    void drawHVDotLine(int x, int y, int wh, bool isHorz,
            Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawPolygonLine(Point *pts, int nums, int width,
            Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
    void fillPolygon(const Point* pts, int vertices,
            Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);

    void getTextDrawSize(const std::string text, Logfont *f, int *w, int *h);
    void getTextDrawSize(const char *text, Logfont *f, int *w, int *h);

    bool getBiDiFlag();
    void setBiDiFlag(bool bidi);
    int  getBiDiFirstChType ();
    void setBiDiFirstChType (int type);

    void textOut(int x, int y, const char * text, int len,
            RGBCOLOR color, Logfont * logfont = NULL);
    void textOut(int x, int y, std::string text,
            RGBCOLOR color, Logfont * logfont) {
        textOut(x, y, text.c_str(), text.size(), color, logfont);
    }

    void textOutOmitted(int x, int y, const char* text, int len,
            int max_extent);
    void textOutOmitted(int x, int y, const char* text, int len,
            int max_extent, RGBCOLOR color, Logfont * logfont);
    void textOutOmitted(int x, int y, const std::string& text,
            int max_extent) {
        textOutOmitted(x, y, text.c_str(), text.size(), max_extent);
    }
    void textOutOmitted(int x, int y, const std::string& text,
            int max_extent, RGBCOLOR color, Logfont * logfont) {
        textOutOmitted(x, y, text.c_str(), text.size(),
                max_extent, color, logfont);
    }

    void drawText(const std::string& text, const IntRect& rc);
    void drawText(const char * text, const IntRect& rc,
            RGBCOLOR color, Logfont * logfont, unsigned int format);
    void drawText(const std::string& text, const IntRect& rc,
            RGBCOLOR color, Logfont * logfont, unsigned int format) {
        drawText(text.c_str(), rc, color, logfont, format);
    }
    int drawTextToCalcRect(const std::string& text, IntRect& rect,
            unsigned int format, Logfont *font = NULL);
    int calcTextRectOnDrawing(const char *text, Logfont *f,
            unsigned int format, IntRect *rect,
            DTFIRSTLINE *firstline = NULL, Point *txtPos = NULL);
    int calcTextRectOnDrawing(const std::string& text, Logfont *f,
            unsigned int format, IntRect *rect,
            DTFIRSTLINE *firstline = NULL, Point *txtPos = NULL) {
        return calcTextRectOnDrawing(text.c_str(), f, format, rect,
            firstline, txtPos);
    }
    int drawTextToGetLenght(const std::string& text);
    int getFontHeight(Logfont *f = NULL);
    int getFontWidth(Logfont *f = NULL);

    //Bitmap
    bool fillBoxWithBitmap(int x, int y, int w, int h, const Bitmap* pBitmap);
    bool setReplaceColor(const RGBCOLOR color);
    bool drawRotateBitmap(const Bitmap* pBitmap,int lx, int ty, int angle);
    bool fillBoxWithBitmapPart(int x, int y, int w, int h,
            const Bitmap* pBitmap, int xo, int yo);
    int loadBitmap(Bitmap* bmp, const char * file_name);
    int loadBitmap(Bitmap** bmp, const void* data, int size,
            const char * extern_name);
    void unloadBitmap(Bitmap* pBitmap);
    bool getBitmapSize(Bitmap* pBitmap, int* w, int* h);
    GraphicsContext* createGraphicsFromBitmap(Bitmap *pbmp);
    bool initBitmap(int w, int h, Bitmap* pbmp);

    //Font
    Logfont* getCurFont();
    Logfont* createLogFontByName(const char * fontname);

    void deleteLogFont(Logfont* logfont);

    void rotateBitmap(const Bitmap *pBitmap, int lx, int ty, int angle);
    HDC context();
    bool captureScreen2Bitmap(Bitmap* pbmp);

    void setMapView(View *view);
    void map(int x, int y, int &x2, int &y2);
    void mapRect(RECT &rc);

    // create memdc
    GraphicsContext* createMemGc(int w, int h);

private:
    GraphicsContextPrivate *m_data;
    static GraphicsContext *screen_graphics_context;
    static int screen_dpi;
};

GraphicsContext* CreateMemGC(int w, int h);
void DeleteMemGC(GraphicsContext *memGc);

bool SaveScreenToFile(const char* bmpFile);

} // namespace hfcl

#endif /* HFCL_GRAPHICS_GRAPHICSCONTEXT_H_ */

