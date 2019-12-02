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
#include <EGL/egl.h>

#include "eglut.h"

int
main(int argc, char *argv[])
{
   /* Initialize the window */
   eglutInitWindowSize(400, 300);
   eglutInitAPIMask(EGLUT_OPENGL_ES1_BIT);
   eglutInit(argc, argv);

   eglutCreateWindow("es1info");

   eglutPrintInfo();

   eglutMainLoop();

   return 0;
}
