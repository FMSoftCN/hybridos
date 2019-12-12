/*
 * eglinfo - like glxinfo but for EGL
 *
 * Brian Paul
 * 11 March 2005
 *
 * Copyright (C) 2005  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define EGL_EGLEXT_PROTOTYPES

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONFIGS 1000
#define MAX_MODES 1000
#define MAX_SCREENS 10

/* These are X visual types, so if you're running eglinfo under
 * something not X, they probably don't make sense. */
static const char *vnames[] = { "SG", "GS", "SC", "PC", "TC", "DC" };

/**
 * Print table of all available configurations.
 */
static void
PrintConfigs(EGLDisplay d)
{
   EGLConfig configs[MAX_CONFIGS];
   EGLint numConfigs, i;

   eglGetConfigs(d, configs, MAX_CONFIGS, &numConfigs);

   printf("Configurations:\n");
   printf("     bf lv colorbuffer dp st  ms    vis   cav bi  renderable  supported\n");
   printf("  id sz  l  r  g  b  a th cl ns b    id   eat nd gl es es2 vg surfaces \n");
   /*        ^  ^   ^  ^  ^  ^  ^ ^  ^  ^  ^    ^    ^   ^  ^  ^  ^   ^  ^
    *        |  |   |  |  |  |  | |  |  |  |    |    |   |  |  |  |   |  |
    *        |  |   |  |  |  |  | |  |  |  |    |    |   |  |  |  |   |  EGL_SURFACE_TYPE
    *        |  |   |  |  |  |  | |  |  |  |    |    |   |  EGL_RENDERABLE_TYPE
    *        |  |   |  |  |  |  | |  |  |  |    |    |   EGL_BIND_TO_TEXTURE_RGB/EGL_BIND_TO_TEXTURE_RGBA
    *        |  |   |  |  |  |  | |  |  |  |    |    EGL_CONFIG_CAVEAT
    *        |  |   |  |  |  |  | |  |  |  |    EGL_NATIVE_VISUAL_ID/EGL_NATIVE_VISUAL_TYPE
    *        |  |   |  |  |  |  | |  |  |  EGL_SAMPLE_BUFFERS
    *        |  |   |  |  |  |  | |  |  EGL_SAMPLES
    *        |  |   |  |  |  |  | |  EGL_STENCIL_SIZE
    *        |  |   |  |  |  |  | EGL_DEPTH_SIZE
    *        |  |   |  |  |  |  EGL_ALPHA_SIZE
    *        |  |   |  |  |  EGL_BLUE_SIZE
    *        |  |   |  |  EGL_GREEN_SIZE
    *        |  |   |  EGL_RED_SIZE
    *        |  |   EGL_LEVEL
    *        |  EGL_BUFFER_SIZE
    *        EGL_CONFIG_ID
    */
   printf("---------------------------------------------------------------------\n");
   for (i = 0; i < numConfigs; i++) {
      EGLint id, size, level;
      EGLint red, green, blue, alpha;
      EGLint depth, stencil;
      EGLint renderable, surfaces;
      EGLint vid, vtype, caveat, bindRgb, bindRgba;
      EGLint samples, sampleBuffers;
      char surfString[100] = "";

      eglGetConfigAttrib(d, configs[i], EGL_CONFIG_ID, &id);
      eglGetConfigAttrib(d, configs[i], EGL_BUFFER_SIZE, &size);
      eglGetConfigAttrib(d, configs[i], EGL_LEVEL, &level);

      eglGetConfigAttrib(d, configs[i], EGL_RED_SIZE, &red);
      eglGetConfigAttrib(d, configs[i], EGL_GREEN_SIZE, &green);
      eglGetConfigAttrib(d, configs[i], EGL_BLUE_SIZE, &blue);
      eglGetConfigAttrib(d, configs[i], EGL_ALPHA_SIZE, &alpha);
      eglGetConfigAttrib(d, configs[i], EGL_DEPTH_SIZE, &depth);
      eglGetConfigAttrib(d, configs[i], EGL_STENCIL_SIZE, &stencil);
      eglGetConfigAttrib(d, configs[i], EGL_NATIVE_VISUAL_ID, &vid);
      eglGetConfigAttrib(d, configs[i], EGL_NATIVE_VISUAL_TYPE, &vtype);

      eglGetConfigAttrib(d, configs[i], EGL_CONFIG_CAVEAT, &caveat);
      eglGetConfigAttrib(d, configs[i], EGL_BIND_TO_TEXTURE_RGB, &bindRgb);
      eglGetConfigAttrib(d, configs[i], EGL_BIND_TO_TEXTURE_RGBA, &bindRgba);
      eglGetConfigAttrib(d, configs[i], EGL_RENDERABLE_TYPE, &renderable);
      eglGetConfigAttrib(d, configs[i], EGL_SURFACE_TYPE, &surfaces);

      eglGetConfigAttrib(d, configs[i], EGL_SAMPLES, &samples);
      eglGetConfigAttrib(d, configs[i], EGL_SAMPLE_BUFFERS, &sampleBuffers);

      if (surfaces & EGL_WINDOW_BIT)
         strcat(surfString, "win,");
      if (surfaces & EGL_PBUFFER_BIT)
         strcat(surfString, "pb,");
      if (surfaces & EGL_PIXMAP_BIT)
         strcat(surfString, "pix,");
      if (surfaces & EGL_STREAM_BIT_KHR)
         strcat(surfString, "str,");
      if (strlen(surfString) > 0)
         surfString[strlen(surfString) - 1] = 0;

      printf("0x%02x %2d %2d %2d %2d %2d %2d %2d %2d %2d%2d 0x%02x%s ",
             id, size, level,
             red, green, blue, alpha,
             depth, stencil,
             samples, sampleBuffers, vid, vtype < 6 ? vnames[vtype] : "--");
      printf("  %c  %c  %c  %c  %c   %c %s\n",
             (caveat != EGL_NONE) ? 'y' : ' ',
             (bindRgba) ? 'a' : (bindRgb) ? 'y' : ' ',
             (renderable & EGL_OPENGL_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENGL_ES_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENGL_ES2_BIT) ? 'y' : ' ',
             (renderable & EGL_OPENVG_BIT) ? 'y' : ' ',
             surfString);
   }
}


static const char *
PrintExtensions(EGLDisplay d)
{
   const char *extensions, *p, *end, *next;
   int column;

   extensions = eglQueryString(d, EGL_EXTENSIONS);
   if (!extensions)
      return NULL;

#ifdef EGL_MESA_query_driver
   if (strstr(extensions, "EGL_MESA_query_driver")) {
      PFNEGLGETDISPLAYDRIVERNAMEPROC getDisplayDriverName =
         (PFNEGLGETDISPLAYDRIVERNAMEPROC)
            eglGetProcAddress("eglGetDisplayDriverName");
      printf("EGL driver name: %s\n", getDisplayDriverName(d));
   }
#endif

   puts(d == EGL_NO_DISPLAY ? "EGL client extensions string:" :
                              "EGL extensions string:");

   column = 0;
   end = extensions + strlen(extensions);

   for (p = extensions; p < end; p = next + 1) {
      next = strchr(p, ' ');
      if (next == NULL)
         next = end;

      if (column > 0 && column + next - p + 1 > 70) {
	 printf("\n");
	 column = 0;
      }
      if (column == 0)
	 printf("    ");
      else
	 printf(" ");
      column += next - p + 1;

      printf("%.*s", (int) (next - p), p);

      p = next + 1;
   }

   if (column > 0)
      printf("\n");

   return extensions;
}

static int
doOneDisplay(EGLDisplay d, const char *name)
{
   int maj, min;

   printf("%s:\n", name);
   if (!eglInitialize(d, &maj, &min)) {
      printf("eglinfo: eglInitialize failed\n\n");
      return 1;
   }

   printf("EGL API version: %d.%d\n", maj, min);
   printf("EGL vendor string: %s\n", eglQueryString(d, EGL_VENDOR));
   printf("EGL version string: %s\n", eglQueryString(d, EGL_VERSION));
#ifdef EGL_VERSION_1_2
   printf("EGL client APIs: %s\n", eglQueryString(d, EGL_CLIENT_APIS));
#endif

   PrintExtensions(d);

   PrintConfigs(d);
   eglTerminate(d);
   printf("\n");
   return 0;
}

int
main(int argc, char *argv[])
{
   int ret = 0;
   const char *clientext;

   clientext = PrintExtensions(EGL_NO_DISPLAY);
   printf("\n");

   if (strstr(clientext, "EGL_EXT_platform_base")) {
       PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplay =
           (PFNEGLGETPLATFORMDISPLAYEXTPROC)
           eglGetProcAddress("eglGetPlatformDisplayEXT");
       if (strstr(clientext, "EGL_KHR_platform_android"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_ANDROID_KHR,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "Android platform");
       if (strstr(clientext, "EGL_MESA_platform_gbm") ||
           strstr(clientext, "EGL_KHR_platform_gbm"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_GBM_MESA,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "GBM platform");
       if (strstr(clientext, "EGL_EXT_platform_wayland") ||
           strstr(clientext, "EGL_KHR_platform_wayland"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_WAYLAND_EXT,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "Wayland platform");
       if (strstr(clientext, "EGL_EXT_platform_x11") ||
           strstr(clientext, "EGL_KHR_platform_x11"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_X11_EXT,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "X11 platform");
       if (strstr(clientext, "EGL_MESA_platform_surfaceless"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "Surfaceless platform");
       if (strstr(clientext, "EGL_EXT_platform_minigui"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_MINIGUI_EXT,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "MiniGUI platform");
       if (strstr(clientext, "EGL_EXT_platform_device"))
           ret += doOneDisplay(getPlatformDisplay(EGL_PLATFORM_DEVICE_EXT,
                                                  EGL_DEFAULT_DISPLAY,
                                                  NULL), "Device platform");
   }
   else {
      ret = doOneDisplay(eglGetDisplay(EGL_DEFAULT_DISPLAY), "Default display");
   }

   return ret;
}
