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

#ifndef __HFCL_MGCLGRAPHICS_H__
#define __HFCL_MGCLGRAPHICS_H__

namespace hfcl {

/**
 * @brief mgclFontGetWidthFromName Get font width.
 *
 * @param name The string context.
 *
 * @returns string width on success, -1 on error.
 */
int mgclFontGetWidthFromName(const char* name);

/**
 * @brief mgclCreateLogFontByName Creates a logical font by a font name.
 *
 * @param font_name The name of the font.
 *
 * @returns The pointer to the logical font created, NULL on error.
 */
PLOGFONT mgclCreateLogFontByName(const char* font_name);

/**
 * @brief mgclDestroyLogFont Destroys a logical font.
 *
 * @param log_font The logical font will be destroied.
 */
void mgclDestroyLogFont(PLOGFONT log_font);

/**
 * @brief mgclSelectFont Selects a logical font into a DC.
 *
 * @param hdc The device context.
 * @param log_font The pointer to the logical font to be selected into the DC.
 *
 * @returns The old logical font of the DC.
 */
PLOGFONT mgclSelectFont(HDC hdc, PLOGFONT log_font);

/**
 * @brief mgclCreateBMPDevFont Create a bitmap device font.
 *
 * @param bmpfont_name The font name.
 * @param glyph_bmp The pointer to the bitmap object.
 * @param start_mchar The pointer to the first char of segment.
 * @param nr_glyphs The character count of the segment.
 * @param glyph_width The character width.
 *
 * @returns The pointer to the new device font on sucess, otherwise NULL.
 */
DEVFONT* mgclCreateBMPDevFont(const char *bmpfont_name, const BITMAP* glyph_bmp,
        const char* start_mchar, int nr_glyphs, int glyph_width);

/**
 * @brief mgclDestroyBMPFont Destroy the bitmap device font.
 *
 * @param dev_font The pointer to the device font which will be destroyed.
 */
void mgclDestroyBMPFont(DEVFONT* dev_font);

/**
 * @brief mgclAddGlyphsToBMPFont Add a new segment in device font.
 *
 * @param dev_font The pointer to the device font.
 * @param glyph_bmp The pointer to the segment glyph bitmap object.
 * @param start_mchar The pointer to the first char of the segment.
 * @param nr_glyphs The character count of the segment.
 * @param glyph_width The character width in the segment.
 *
 * @returns true if success, otherwise false.
 */
bool mgclAddGlyphsToBMPFont(DEVFONT* dev_font, BITMAP* glyph_bmp,
        const char* start_mchar, int nr_glyphs, int glyph_width);
/**
 * @brief mgclSetTextColor Sets the text color of a DC to a new value.
 *
 * @param hdc The device context.
 * @param color The new text color (pixel value).
 *
 * @returns The old text color (pixel value) of the DC \a hdc.
 */
Uint32 mgclSetTextColor(HDC hdc, Uint32 color);

/**
 * @brief mgclSetBrushColor Sets the brush color of a DC to a new value.
 *
 * @param hdc The device context.
 * @param color The new brush color (pixel value).
 *
 * @returns The old brush pixel value of the DC \a hdc.
 */
Uint32 mgclSetBrushColor(HDC hdc, Uint32 color);

/**
 * @brief mgclSetBkMode Sets the background color of a DC to a new mode.
 *
 * @param hdc The device context.
 * @param mode The new background mode, be can one of the following values:
 *
 *      - BM_TRANSPARENT \n
 *        Indicate that reserve the background untouched when drawing text. 
 *      - BM_OPAQUE \n
 *        Indicate that erase the background with background color when 
 *        drawing text.
 *
 * @returns The old background mode of the DC \a hdc.
 */
Uint32 mgclSetBkMode(HDC hdc, Uint32 mode);

/**
 * @brief mgclDrawText Draws a formatted text in a rectangle.
 *
 * @param hdc The device context.
 * @param pText The formatted text.
 * @param nCount The length of the text. If it is less than 0, MiniGUI will 
 *        treat it as a null-terminated string.
 * @param pRect The output rectangle.
 * @param nFormat The methods used to format the text.
 *
 * @returns The output height of the formatted text.
 */
int mgclDrawText(HDC hdc, const char* pText, int nCount, RECT* pRect, UINT nFormat);
int mgclTextOutOmitted(HDC hdc, int x, int y, const char *mtext, int len, int max_extent);

/**
 * \fn void GUIAPI GetBoundRect (PRECT pdrc, const RECT* psrc1, const RECT* psrc2)
 * \brief Gets the bound rectangle of two source rectangles.
 *
 * This function creates the bound rect (\a pdrc) of two rectangles 
 * (\a psrc1 and \a prsrc2). The bound rect is the smallest rectangle 
 * that contains both source rectangles.
 *
 * \param pdrc The destination rectangle.
 * \param psrc1 The first source rectangle.
 * \param psrc2 The second source rectangle.
 */
void mgclGetBoundRect(IntRect* pdrc, const IntRect* psrc1, const IntRect* psrc2);

/**
 * @brief mgclSelectClipRect Sets the visible region of a DC to be a rectangle. 
 *
 * @param hdc The device context.
 * @param prc Pointer to the rectangle.
 */
void mgclSelectClipRect(HDC hdc, const RECT* prc);

/**
 * @brief mgclSaveDC Saves the current state of a device context.
 *
 * @param hdc The handle to the DC whose state is to be saved.
 *
 * @returns  If the function succeeds, the return value identifies the saved 
 *         state. If the function fails, the return value is zero. 
 */
int mgclSaveDC(HDC hdc);

/**
 * @brief mgclRestoreDC Restores a device context (DC) to the specified state.
 *
 * @param hdc The handle to the DC.
 * @param saved_dc Specifies the saved state to be restored. 
 *
 * @returns If the function succeeds, the return value is nonzero. If the 
 *         function fails, the return value is zero. 
 */
bool mgclRestoreDC(HDC hdc, int saved_dc);

/**
 * @brief mgclSetMemDCAlpha Sets the alpha value for the entire surface of a
 *                DC, as opposed to using the alpha component of each pixel.
 *
 * @param mem_dc The device context.
 * @param flags The alpha value specific memory DC flags.
 * @param alpha The alpha value for the entire surface of the DC \a mem_dc.
 *
 * @returns  true on success, otherwise false.
 */
bool mgclSetMemDCAlpha(HDC mem_dc, DWORD flags, Uint8 alpha);

/**
 * @brief mgclBitBlt Performs a bit-block transfer from a device context into
 *        another device context.
 *
 * @param hsdc The source device context.
 * @param sx The x coordinate of the upper-left corner of the rectangle 
 *        in the source DC.
 * @param sy The y coordinate of the upper-left corner of the rectangle 
 *        in the source DC.
 * @param sw The width of the source rectangle.
 * @param sh The height of the source rectangle.
 * @param hddc The destination device context \a hddc.
 * @param dx The x coordinate of the upper-left corner of the rectangle 
 *        in the destination DC.
 * @param dy The y coordinate of the upper-left corner of the rectangle 
 *        in the destination DC.
 * @param dwRop The raster operation, currently ignored.
 */
void mgclBitBlt(HDC hsdc, int sx, int sy, int sw, int sh,
        HDC hddc, int dx, int dy, DWORD dwRop);

/**
 * @brief mgclFillBox Fills a rectangle box.
 *
 * @param hdc The device context.
 * @param x x,y: The coorinates of the upper-left corner of the box.
 * @param y x,y: The coorinates of the upper-left corner of the box.
 * @param w The width of the box.
 * @param h The height of the box.
 */
void mgclFillBox(HDC hdc, int x, int y, int w, int h);

/**
 * @brief mgclRectangle Draws a rectangle.
 *
 * @param hdc The device context.
 * @param x0 x0,y0: The coordinates of the upper-left corner of the rectangle.
 * @param y0 x0,y0: The coordinates of the upper-left corner of the rectangle.
 * @param x1 x1,y1: The coordinates of the lower-right corner of the rectangle.
 * @param y1 x1,y1: The coordinates of the lower-right corner of the rectangle.
 */
void mgclRectangle(HDC hdc, int x0, int y0, int x1, int y1);

/**
 * @brief mgclCreateMemDC Creates a memory DC.
 *
 * @param width The expected width of the result memory DC.
 * @param height The expected height of the result memory DC.
 * @param depth The expected color depth of the memory DC.
 * @param flags The memory DC flags, can be or'ed values of the following flags:
 *
 *   - MEMDC_FLAG_SWSURFACE\n
 *     Creates the surface of memory DC in the system memory.
 *   - MEMDC_FLAG_HWSURFACE\n
 *     Creates the surface of memory DC in the video memory.
 *   - MEMDC_FLAG_SRCCOLORKEY\n
 *     The created memory DC will use a source color key to blit to other DC.
 *   - MEMDC_FLAG_SRCALPHA\n
 *     The created memory DC will use a source alpha blending to blit to 
 *     other DC.
 *   - MEMDC_FLAG_RLEACCEL\n
 *     The memory DC will be RLE encoded.
 *
 * @param Rmask The bit-masks of the red components in a pixel value.
 * @param Gmask The bit-masks of the green components in a pixel value.
 * @param Bmask The bit-masks of the blue components in a pixel value.
 * @param Amask The bit-masks of the alpha components in a pixel value.
 *
 * @returns The handle to the memory DC, HDC_INVALID indicates an error.
 */
HDC mgclCreateMemDC(int width, int height, int depth, DWORD flags,
        Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

/**
 * @brief DeleteMemDC Deletes a memory DC.
 *
 * @param mem_dc The device context to be deleted.
 */
void mgclDeleteMemDC(HDC mem_dc);

/**
 * @brief mgclCreateCompatibleDCEx Creates a memory DC which is compatible
 *        with the specified reference DC.
 *
 * @param hdc The handle to the reference DC.
 * @param width The expected width of the result memory DC. If it is zero, 
 *        the width will be equal to the width of the reference DC.
 * @param height The expected height of the result memory DC. If it is zero, 
 *        the height will be equal to the height of the reference DC.
 *
 * @returns The handle to the memory DC, HDC_INVALID indicates an error.
 */
HDC mgclCreateCompatibleDCEx(HDC hdc, int width, int height);

/**
 * @brief DeleteMemDC Deletes a memory DC.
 *
 * @param mem_dc The device context to be deleted.
 */
void mgclDeleteCompatibleDCEx(HDC mem_dc);

/**
 * @brief mgclSetMemDCColorKey Sets the color key (transparent pixel) of a memory DC.
 *
 * @param mem_dc The device context.
 * @param flags The color key specific memory DC flags.
 * @param color_key The color_key of the memory DC.
 *
 * @returns true on success, otherwise false.
 */
bool mgclSetMemDCColorKey(HDC mem_dc, DWORD flags, Uint32 color_key);

/**
 * @brief mgclFillBoxWithBitmap Fills a box with a BITMAP object.
 *
 * @param hdc The device context.
 * @param x The x coordinate of the upper-left corner of the box.
 * @param y The y coordinate of the upper-left corner of the box.
 * @param w The width of the box. Can be zero, means the width
 *        or the height will be equal to the width or the height of 
 *        the BITMAP object.
 * @param h The height of the box. Can be zero, means the width
 *        or the height will be equal to the width or the height of 
 *        the BITMAP object.
 * @param bmp The pointer to the BITMAP object.
 *
 * @returns true on success, otherwise false.
 */
bool mgclFillBoxWithBitmap(HDC hdc, int x, int y, int w, int h,
        const BITMAP *bmp);

/**
 * @brief mgclLoadBitmapFromFile Loads a device-dependent bitmap from a file.
 *
 * @param hdc The device context.
 * @param pBitmap The pointer to the BITMAP object.
 * @param spFileName The bmp file name.
 *
 * @returns 0 on success, less than 0 on error.
 */
int mgclLoadBitmapFromFile(HDC hdc, PBITMAP pBitmap, const char* spFileName);

int mgclLoadBitmapFromMem(HDC hdc, PBITMAP *pBitmap, const void* data, int size, const char* extern_name);

/**
 * @brief mgclUnloadBitmap Unloads a bitmap.
 *
 * @param pBitmap The BITMAP object.
 */
void mgclUnloadBitmap(PBITMAP pBitmap);

/**
 * @brief mgclCreateMemDCFromBitmap Creates a memory DC from a reference DC
 *            and a BITMAP object.
 *
 * @param hdc The reference DC.
 * @param bmp The BITMAP object.
 *
 * @returns  The handle to the memory DC, HDC_INVALID indicates an error.
 */
HDC mgclCreateMemDCFromBitmap(HDC hdc, const BITMAP* bmp);

/**
 * @brief mgclInitBitmap Initializes a BITMAP object as a normal bitmap.
 *
 * @param hdc The device context.
 * @param w The width of the bitmap.
 * @param h The height of the bitmap.
 * @param pitch The pitch of the bitmap.
 * @param bits The pitch of the bitmap.
 * @param bmp The BITMAP object to be initialized.
 *
 * @returns  true on success, false on error.
 */
bool mgclInitBitmap(HDC hdc, Uint32 w, Uint32 h, Uint32 pitch,
        BYTE* bits, PBITMAP bmp);

/**
 * @brief mgclSaveScreenRectContent Saves content of a rectangle in the screen
 *        to a file.
 *
 * @param rc The RECT object defined the rectangle in the screen.
 * @param filename The name of the image file.
 *
 * @returns TRUE on success, otherwise FALSE.
 */
bool mgclSaveScreenRectContent(const RECT* rc, const char* filename);

void mgclDrawLine(HDC hdc, int x0, int y0, int x1, int y1,
				int width, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/**
 * \brief Fills an polygon.
 *
 * This function fills a polygon with the current brush in the DC \a hdc.
 *
 * \param hdc The device context.
 * \param pts The pointer to the vertex array of the polygon.
 * \param vertices The number of the vertices, i.e. the size of the
 *        vertex array.
 */
void mgclFillPolygon(HDC hdc, const POINT* pts, int vertices, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

static inline void mgclLPtoDP(HDC hdc, POINT *pt) {
	LPtoDP(hdc, pt);
}
static inline void mgclLPtoDP(HDC hdc, int *px, int *py) {
	POINT pt = { *px, *py };
	LPtoDP(hdc, &pt);
	*px = pt.x;
	*py = pt.y;
}

static inline void mgclExcludeClipRect(HDC hdc, const RECT *prc) 
{
	ExcludeClipRect(hdc, prc);
}

static inline void mgclSetWindowOrg(HDC hdc, int x, int y) 
{
	POINT pt = {x, y };
	SetWindowOrg(hdc, &pt);
}

static inline void mgclGetWindowOrg(HDC hdc, POINT *pt) {
	GetWindowOrg(hdc, pt);
}

static inline void mgclSetMapMode(HDC hdc, int mode) {
	SetMapMode(hdc, mode);
}

static inline int mgclGetMapMode(HDC hdc) {
	return GetMapMode(hdc);
}



/**
 * \brief Creates a clipping region.
 *
 * \return The pointer to the clip region.
 */
PCLIPRGN mgclCreateClipRgn(void);

/**
 * \brief Empties and destroys a clipping region.
 *
 * This function empties and destroys a clipping region pointed to by \a pRgn.
 *
 * \param pRgn The pointer to the region.
 */
void mgclDestroyClipRgn(PCLIPRGN pRgn);

/**
 * \brief Sets a region to contain only one rect.
 *
 * This function sets the region \a pRgn to contain only a rect pointed to
 * by \a pRect.
 *
 * \param pRgn The pointer to the region.
 * \param pRect The pointer to the rect.
 *
 * \return TRUE on success, otherwise FALSE.
 *
 * \note This function will empty the region \a pRgn first.
 */
bool mgclSetClipRgn(PCLIPRGN pRgn, const RECT* pRect);

/**
 * \brief Rotate a bitmap object.
 *
 * \param hdc The device context.
 * \param bmp The pointer of Bitmap object.
 * \param lx (lx,ty), the x coordinate of top left corner.
 * \param ty (lx,ty), the y coordinate of top left corner.
 * \param angle The specified rotated angle around its center.
 *              It must be in 1/64ths of a degree.
 */
void mgclRotateBitmap(HDC hdc, const BITMAP *bmp,
                int lx, int ty, int angle);

} // namespace hfcl
#endif /* __HFCL_MGCL_H__ */

