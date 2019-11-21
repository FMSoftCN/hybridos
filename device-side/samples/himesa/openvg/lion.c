#include <stdio.h>
#include <VG/openvg.h>
#include <EGL/egl.h>

#include "lion-render.h"
#include "eglut.h"

static VGint width, height;
struct lion *lion = 0;
VGfloat angle = 0;

static void
draw(void)
{
   static double t0 = -1.0;
   static int num_frames;
   double now;

   vgClear(0, 0, width, height);

   vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
   vgLoadIdentity();
   vgTranslate(width/2, height/2);
   vgRotate(angle);
   vgTranslate(-width/2, -height/2);

   lion_render(lion);
   vgFlush();
   num_frames++;

   if (t0 < 0.0) {
      t0 = eglutGet(EGLUT_ELAPSED_TIME) / 1000.0;
      num_frames = 0;
   }

   now = eglutGet(EGLUT_ELAPSED_TIME) / 1000.0;
   if (now - t0 > 5.0) {
      /* just a rough estimate */
      printf("%d frames in %3.1f seconds = %6.3f FPS\n",
            num_frames, now - t0, num_frames / (now - t0));

      t0 = now;
      num_frames = 0;
   }

   ++angle;
   eglutPostRedisplay();
}


/* new window size or exposure */
static void
reshape(int w, int h)
{
   width  = w;
   height = h;
}


static void
init(void)
{
   float clear_color[4] = {1.0, 1.0, 1.0, 1.0};
   vgSetfv(VG_CLEAR_COLOR, 4, clear_color);

   lion = lion_create();
}


int
main(int argc, char *argv[])
{
   eglutInitWindowSize(350, 450);
   eglutInitAPIMask(EGLUT_OPENVG_BIT);
   eglutInit(argc, argv);

   eglutCreateWindow("Lion Example");

   eglutReshapeFunc(reshape);
   eglutDisplayFunc(draw);

   init();

   eglutMainLoop();

   return 0;
}
