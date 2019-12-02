/*
 * Copyright (C) 2008  Tunsgten Graphics,Inc.   All Rights Reserved.
 */

/*
 * List OpenGL ES extensions.
 * Print ES 1 or ES 2 extensions depending on which library we're
 * linked with: libGLESv1_CM.so vs libGLESv2.so
 */

#define GL_GLEXT_PROTOTYPES

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <EGL/egl.h>

/*
 * Print a list of extensions, with word-wrapping.
 */
static void
print_extension_list(const char *ext)
{
   const char indentString[] = "    ";
   const int indent = 4;
   const int max = 79;
   int width, i, j;

   if (!ext || !ext[0])
      return;

   width = indent;
   printf("%s", indentString);
   i = j = 0;
   while (1) {
      if (ext[j] == ' ' || ext[j] == 0) {
         /* found end of an extension name */
         const int len = j - i;
         if (width + len > max) {
            /* start a new line */
            printf("\n");
            width = indent;
            printf("%s", indentString);
         }
         /* print the extension name between ext[i] and ext[j] */
         while (i < j) {
            printf("%c", ext[i]);
            i++;
         }
         /* either we're all done, or we'll continue with next extension */
         width += len + 1;
         if (ext[j] == 0) {
            break;
         }
         else {
            i++;
            j++;
            if (ext[j] == 0)
               break;
            printf(", ");
            width += 2;
         }
      }
      j++;
   }
   printf("\n");
}


static void
info(EGLDisplay egl_dpy)
{
   const char *s;

   s = eglQueryString(egl_dpy, EGL_VERSION);
   printf("EGL_VERSION: %s\n", s);

   s = eglQueryString(egl_dpy, EGL_VENDOR);
   printf("EGL_VENDOR: %s\n", s);

   s = eglQueryString(egl_dpy, EGL_EXTENSIONS);
   printf("EGL_EXTENSIONS: %s\n", s);
   print_extension_list((char *) s);

   s = eglQueryString(egl_dpy, EGL_CLIENT_APIS);
   printf("EGL_CLIENT_APIS: %s\n", s);

const char *glGetString (int name);

#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03

   printf("GL_VERSION: %s\n", (char *) glGetString(GL_VERSION));
   printf("GL_RENDERER: %s\n", (char *) glGetString(GL_RENDERER));
   printf("GL_EXTENSIONS:\n");
   print_extension_list((char *) glGetString(GL_EXTENSIONS));
}

static int
make_minigui_window(EGLDisplay egl_dpy,
              const char *name,
              int x, int y, int width, int height, int es_ver,
              HWND *winRet,
              EGLContext *ctxRet,
              EGLSurface *surfRet)
{
   EGLint attribs[] = {
      EGL_RENDERABLE_TYPE, 0x0,
      EGL_RED_SIZE, 1,
      EGL_GREEN_SIZE, 1,
      EGL_BLUE_SIZE, 1,
      EGL_NONE
   };
   EGLint ctx_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 0,
      EGL_NONE
   };

   MAINWINCREATE create_info;
   HWND win;

   unsigned long mask;
   EGLContext ctx;
   EGLConfig config;
   EGLint num_configs;
   EGLint vid;

   if (es_ver == 1)
      attribs[1] = EGL_OPENGL_ES_BIT;
   else
      attribs[1] = EGL_OPENGL_ES2_BIT;
   ctx_attribs[1] = es_ver;

   if (!eglChooseConfig( egl_dpy, attribs, &config, 1, &num_configs)) {
      printf("Error: couldn't get an EGL visual config\n");
      return 1;
   }

   assert(config);
   assert(num_configs > 0);

   if (!eglGetConfigAttrib(egl_dpy, config, EGL_NATIVE_VISUAL_ID, &vid)) {
      printf("Error: eglGetConfigAttrib() failed\n");
      return 1;
   }

   create_info.dwStyle = WS_VISIBLE;
   create_info.dwExStyle = WS_EX_USEPRIVATECDC;
   create_info.spCaption = name;
   create_info.hMenu = 0;
   create_info.hCursor = 0;
   create_info.hCursor = GetSystemCursor(0);
   create_info.hIcon = 0;
   create_info.MainWindowProc = DefaultMainWinProc;
   create_info.lx = x;
   create_info.ty = y;
   create_info.rx = x + width;
   create_info.by = y + height;
   create_info.iBkColor = COLOR_lightwhite;
   create_info.dwAddData = (DWORD)0;
   create_info.hHosting = HWND_DESKTOP;
   win = CreateMainWindow(&create_info);

   if (win == HWND_INVALID) {
      printf ("fatal: failed to create a window\n");
      return 1;
   }
   ShowWindow(win, SW_SHOWNORMAL);

   eglBindAPI(EGL_OPENGL_ES_API);

   ctx = eglCreateContext(egl_dpy, config, EGL_NO_CONTEXT, ctx_attribs);
   if (!ctx) {
      printf("Error: eglCreateContext failed\n");
      return 1;
   }

   *surfRet = eglCreateWindowSurface(egl_dpy, config, (EGLNativeWindowType)win, NULL);

   if (!*surfRet) {
      printf("Error: eglCreateWindowSurface failed\n");
      return 1;
   }

   *winRet = win;
   *ctxRet = ctx;

   return 0;
}


int
main(int argc, char *argv[])
{
   const int winWidth = 400, winHeight = 300;
   HWND win;
   EGLSurface egl_surf;
   EGLContext egl_ctx;
   EGLDisplay egl_dpy;
   char *dpyName = NULL;
   EGLint egl_major, egl_minor, es_ver;
   int i;

   if (InitGUI (argc, (const char**)argv) != 0) {
      printf("Error: InitGUI() failed\n");
      return 1;
   }

   egl_dpy = eglGetDisplay(GetVideoHandle(HDC_SCREEN));
   if (!egl_dpy) {
      printf("Error: eglGetDisplay() failed\n");
      goto fail_sys;
   }

   if (!eglInitialize(egl_dpy, &egl_major, &egl_minor)) {
      printf("Error: eglInitialize() failed\n");
      goto fail_dpy;
   }

   es_ver = 1;
   /* decide the version from the executable's name */
   if (argc > 0 && argv[0] && strstr(argv[0], "es2"))
      es_ver = 2;

   if (make_minigui_window(egl_dpy,
                 "ES info", 0, 0, winWidth, winHeight, es_ver,
                 &win, &egl_ctx, &egl_surf)) {
      printf("Error: make_minigui_window() failed\n");
      goto fail_dpy;
   }


   if (!eglMakeCurrent(egl_dpy, egl_surf, egl_surf, egl_ctx)) {
      printf("Error: eglMakeCurrent() failed\n");
      goto fail_win;
   }

   info(egl_dpy);

fail_win:
   eglDestroyContext(egl_dpy, egl_ctx);
   eglDestroySurface(egl_dpy, egl_surf);

   DestroyMainWindow(win);
   MainWindowThreadCleanup(win);

fail_dpy:
   eglTerminate(egl_dpy);

fail_sys:
   ExitGUISafely(0);
   TerminateGUI(0);

   return 0;
}
