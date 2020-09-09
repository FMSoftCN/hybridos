///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/* 
** wallpaper-dynamic.c: a daynamic wallpaper renderer.
**
** Copyright (C) 2019 ~ 2020 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgeff/mgeff.h>

#include <glib.h>

#include "librsvg/rsvg.h"

#include "../include/sysconfig.h"

#define WALLPAPER_FILE_TOP          "res/wallpaper-top.jpg"
#define WALLPAPER_FILE_BOTTOM       "res/wallpaper-bottom.jpg"

BITMAP bmpTop;
BITMAP bmpBottom;

int start = 100;
int end = 0;
int duration = 600;
enum EffMotionType motionType = OutQuart;
//enum EffMotionType motionType = OutQuint;

void loadBitmap(void)
{
    bmpTop.bmWidth = 0;
    bmpBottom.bmWidth = 0;
    LoadBitmapFromFile(HDC_SCREEN, &bmpTop, WALLPAPER_FILE_TOP);
    LoadBitmapFromFile(HDC_SCREEN, &bmpBottom, WALLPAPER_FILE_BOTTOM);
}

void clearBitmap(void)
{
    if (bmpTop.bmWidth > 0)
    {
        UnloadBitmap(&bmpTop);
    }

    if (bmpBottom.bmWidth > 0)
    {
        UnloadBitmap(&bmpBottom);
    }
}

int paintWallpaper (HDC hdc, int space)
{
    static int lastSpace = -1;
    int ret = 0;

    if (lastSpace == space)
    {
        return ret;
    }
    lastSpace = space;

    if (bmpTop.bmWidth > 0 && bmpTop.bmHeight > 0 && bmpBottom.bmWidth > 0 && bmpBottom.bmHeight > 0) {
        int x, y, w, h, y2;
        static int wp_w = 0, wp_h = 0, wp_half_h = 0;

        if (wp_w == 0) {
            wp_w = (int)GetGDCapability (hdc, GDCAP_HPIXEL);
            wp_h = (int)GetGDCapability (hdc, GDCAP_VPIXEL);
            wp_half_h = wp_h >> 1;
            start = wp_half_h - bmpTop.bmHeight;;
        }

        fprintf(stderr, "wp_w=%d|wp_h=%d\n", wp_w, wp_h);
        SetBrushColor(hdc, RGBA2Pixel(hdc, 0x00, 0x00, 0x0, 0x0));
        FillBox(hdc, 0, 0, wp_w, wp_h);

        w = bmpTop.bmWidth;
        h = bmpTop.bmHeight;

        x = (wp_w - w) >> 1;
        y = wp_half_h - h;
        y2 = wp_half_h;

        FillBoxWithBitmap(hdc, x, y - space, bmpTop.bmWidth, bmpTop.bmHeight, &bmpTop);
        FillBoxWithBitmap(hdc, x, y2 + space, bmpBottom.bmWidth, bmpBottom.bmHeight, &bmpBottom);

        SyncUpdateDC (hdc);
    }
    else {
        _ERR_PRINTF("Failed to get the size of wallpaper bitmap\n");
        ret = -2;
        goto ret;
    }

ret:
    return ret;
}

static void animated_cb(MGEFF_ANIMATION handle, HWND hwnd, int id, int *value)
{
    paintWallpaper(HDC_SCREEN, *value);
}

static void animated_end(MGEFF_ANIMATION handle)
{
}

void startAnimation (HWND hwnd)
{
    MGEFF_ANIMATION animation;
    animation = mGEffAnimationCreate((void *)hwnd, (void *)animated_cb, 1, MGEFF_INT);
    if (animation) {
        mGEffAnimationSetStartValue(animation, &start);
        mGEffAnimationSetEndValue(animation, &end);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationSetCurve(animation, motionType);
        mGEffAnimationSetFinishedCb(animation, animated_end);
        mGEffAnimationSyncRun(animation);
        mGEffAnimationDelete(animation);
    }
}

void doAnimationBack(HWND hwnd)
{
    MGEFF_ANIMATION animation;
    animation = mGEffAnimationCreate((void *)hwnd, (void *)animated_cb, 1, MGEFF_INT);
    if (animation) {
        mGEffAnimationSetStartValue(animation, &end);
        mGEffAnimationSetEndValue(animation, &start);
        mGEffAnimationSetDuration(animation, motionType);
        mGEffAnimationSetCurve(animation, motionType);
        mGEffAnimationSyncRun(animation);
        mGEffAnimationDelete(animation);
    }
}


static char *
get_output_file (const char *test_file,
                 const char *extension)
{
  const char *output_dir = g_get_tmp_dir ();
  char *result, *base;

  base = g_path_get_basename (test_file);

  if (g_str_has_suffix (base, ".svg"))
    base[strlen (base) - strlen (".svg")] = '\0';

  result = g_strconcat (output_dir, G_DIR_SEPARATOR_S, base, extension, NULL);
  g_free (base);

  return result;
}

static void
save_image (cairo_surface_t *surface,
            const char      *test_name,
            const char      *extension)
{
  char *filename = get_output_file (test_name, extension);

  fprintf(stderr, "=======================output filename=%s\n", filename);
  g_test_message ("Storing test result image at %s", filename);
  g_assert (cairo_surface_write_to_png (surface, filename) == CAIRO_STATUS_SUCCESS);

  g_free (filename);
}

static HDC create_memdc_from_image_surface (cairo_surface_t* image_surface)
{
    MYBITMAP my_bmp = {
        flags: MYBMP_TYPE_RGB | MYBMP_FLOW_DOWN,
        frames: 1,
        depth: 32,
    };

    my_bmp.w = cairo_image_surface_get_width (image_surface);
    my_bmp.h = cairo_image_surface_get_height (image_surface);
    my_bmp.pitch = cairo_image_surface_get_stride (image_surface);
    my_bmp.bits = cairo_image_surface_get_data (image_surface);
    my_bmp.size = my_bmp.pitch * my_bmp.h;

    return CreateMemDCFromMyBitmap(&my_bmp, NULL);
}

void loadSVG(const char* file)
{
    RsvgHandle *handle;
    GError *error = NULL;

    cairo_t *cr;
    cairo_surface_t *surface_a;
    RsvgDimensionData dimensions;
    
    handle = rsvg_handle_new_from_file (file, &error);
    rsvg_handle_get_dimensions (handle, &dimensions);

    int width = 100;// dimensions.width;
    int height = 100; //dimensions.height;

    surface_a = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create (surface_a);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);

#if 1
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_paint (cr);
#endif

    cairo_save(cr);
    cairo_scale(cr, 5.0, 5.0);

    cairo_push_group (cr);
    rsvg_handle_render_cairo (handle, cr);
    cairo_pattern_t *p = cairo_pop_group (cr);

    cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
    cairo_mask(cr, p);

    cairo_restore (cr);

    cairo_surface_type_t cst = cairo_surface_get_type (surface_a);

    HDC csdc = create_memdc_from_image_surface (surface_a);
    if (csdc != HDC_SCREEN && csdc != HDC_INVALID) {
        BitBlt(csdc, 0, 0, width, height,  HDC_SCREEN, 100, 100, 0);
    }
    DeleteMemDC (csdc);
    SyncUpdateDC (HDC_SCREEN);

    cairo_surface_destroy (surface_a);
    cairo_pattern_destroy (p);
    cairo_destroy (cr);
    g_object_unref (handle);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG msg;
    JoinLayer(NAME_DEF_LAYER , "wallpaper" , 0 , 0);

    loadBitmap();
    if (paintWallpaper(HDC_SCREEN, 0) < 0)
    {
        clearBitmap();
        exit (1);
    }

    mGEffInit();
    //startAnimation(NULL);
    //loadSVG("res/home.svg");
    loadSVG("res/arrow.svg");

    while (GetMessage (&msg, HWND_DESKTOP)) {
        if (msg.message == MSG_DYBKGND_DO_ANIMATION)
        {
    //        startAnimation(NULL);
        }
        DispatchMessage (&msg);
    }

    clearBitmap();
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

