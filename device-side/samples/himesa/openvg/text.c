#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <VG/openvg.h>

#include "eglut.h"

#define MAX_GLYPHS 256
static struct glyph {
   VGboolean is_path;
   VGHandle handle;
   VGfloat origin[2];
   VGfloat escapement[2];
} glyph_string[MAX_GLYPHS];

static int glyph_string_len;
static VGHandle glyph_string_font = VG_INVALID_HANDLE;

static VGint width, height;

static VGint
glyph_string_add_path(VGPath path, const VGfloat origin[2], VGfloat escapement[2])
{
   struct glyph *g;

   if (glyph_string_len >= MAX_GLYPHS)
      return -1;

#ifdef OPENVG_VERSION_1_1
   if (glyph_string_font != VG_INVALID_HANDLE) {
      vgSetGlyphToPath(glyph_string_font, glyph_string_len,
            path, VG_TRUE, origin, escapement);
      return glyph_string_len++;
   }
#endif

   g = &glyph_string[glyph_string_len];
   g->is_path = VG_TRUE;
   g->handle = (VGHandle) path;
   g->origin[0] = origin[0];
   g->origin[1] = origin[1];
   g->escapement[0] = escapement[0];
   g->escapement[1] = escapement[1];

   return glyph_string_len++;
}

static VGint
glyph_string_add_image(VGImage image, const VGfloat origin[2], VGfloat escapement[2])
{
   struct glyph *g;

   if (glyph_string_len >= MAX_GLYPHS)
      return -1;

#ifdef OPENVG_VERSION_1_1
   if (glyph_string_font != VG_INVALID_HANDLE) {
      vgSetGlyphToImage(glyph_string_font, glyph_string_len,
            image, origin, escapement);
      return glyph_string_len++;
   }
#endif

   g = &glyph_string[glyph_string_len];
   g->is_path = VG_FALSE;
   g->handle = (VGHandle) image;
   g->origin[0] = origin[0];
   g->origin[1] = origin[1];
   g->escapement[0] = escapement[0];
   g->escapement[1] = escapement[1];

   return glyph_string_len++;
}

static void
glyph_string_draw(VGfloat x, VGfloat y)
{
   VGfloat pen[2];
   int i;

#ifdef OPENVG_VERSION_1_1
   if (glyph_string_font != VG_INVALID_HANDLE) {
      VGuint indices[MAX_GLYPHS];

      for (i = 0; i < glyph_string_len; i++)
         indices[i] = i;

      pen[0] = x;
      pen[1] = y;
      vgSetfv(VG_GLYPH_ORIGIN, 2, pen);
      vgDrawGlyphs(glyph_string_font, glyph_string_len, indices,
            NULL, NULL, VG_FILL_PATH, VG_TRUE);

      return;
   }
#endif

   pen[0] = (VGint) (x + 0.5f);
   pen[1] = (VGint) (y + 0.5f);

   for (i = 0; i < glyph_string_len; i++) {
      const struct glyph *g = &glyph_string[i];

      if (g->handle == VG_INVALID_HANDLE)
         continue;

      vgSeti(VG_MATRIX_MODE, (glyph_string[i].is_path) ?
            VG_MATRIX_PATH_USER_TO_SURFACE : VG_MATRIX_IMAGE_USER_TO_SURFACE);
      vgLoadIdentity();
      vgTranslate(pen[0] - (VGint) g->origin[0], pen[1] - (VGint) g->origin[1]);

      if (glyph_string[i].is_path)
         vgDrawPath((VGPath) glyph_string[i].handle, VG_FILL_PATH);
      else
         vgDrawImage((VGImage) glyph_string[i].handle);

      pen[0] += (VGint) (g->escapement[0] + 0.5f);
      pen[1] += (VGint) (g->escapement[1] + 0.5f);
   }
}

static int
path_append(VGPath path, VGubyte segment, const FT_Vector **vectors)
{
   VGfloat coords[6];
   int i, num_vectors;

   switch (segment) {
   case VG_MOVE_TO:
   case VG_LINE_TO:
      num_vectors = 1;
      break;
   case VG_QUAD_TO:
      num_vectors = 2;
      break;
   case VG_CUBIC_TO:
      num_vectors = 3;
      break;
   default:
      return -1;
      break;
   }

   for (i = 0; i < num_vectors; i++) {
      coords[2 * i + 0] = (float) vectors[i]->x / 64.0f;
      coords[2 * i + 1] = (float) vectors[i]->y / 64.0f;
   }

   vgAppendPathData(path, 1, &segment, (const void *) coords);

   return 0;
}

static int
decompose_move_to(const FT_Vector *to, void *user)
{
   VGPath path = (VGPath) (long) user;

   return path_append(path, VG_MOVE_TO, &to);
}

static int
decompose_line_to(const FT_Vector *to, void *user)
{
   VGPath path = (VGPath) (long) user;

   return path_append(path, VG_LINE_TO, &to);
}

static int
decompose_conic_to(const FT_Vector *control, const FT_Vector *to, void *user)
{
   VGPath path = (VGPath) (long) user;
   const FT_Vector *vectors[2] = { control, to };

   return path_append(path, VG_QUAD_TO, vectors);
}

static int
decompose_cubic_to(const FT_Vector *control1, const FT_Vector *control2,
                   const FT_Vector *to, void *user)
{
   VGPath path = (VGPath) (long) user;
   const FT_Vector *vectors[3] = { control1, control2, to };

   return path_append(path, VG_CUBIC_TO, vectors);
}

static VGHandle
convert_outline_glyph(FT_GlyphSlot glyph)
{
   FT_Outline_Funcs funcs = {
      decompose_move_to,
      decompose_line_to,
      decompose_conic_to,
      decompose_cubic_to,
      0, 0
   };
   VGPath path;

   path = vgCreatePath(VG_PATH_FORMAT_STANDARD,
         VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, glyph->outline.n_points,
         VG_PATH_CAPABILITY_ALL);

   if (FT_Outline_Decompose(&glyph->outline, &funcs, (void *) (long) path)) {
      vgDestroyPath(path);
      path = VG_INVALID_HANDLE;
   }

   return (VGHandle) path;
}

static VGHandle
convert_bitmap_glyph(FT_GlyphSlot glyph)
{
   VGImage image;
   VGint width, height, stride;
   unsigned char *data;
   int i, j;

   switch (glyph->bitmap.pixel_mode) {
   case FT_PIXEL_MODE_MONO:
   case FT_PIXEL_MODE_GRAY:
      break;
   default:
      return VG_INVALID_HANDLE;
      break;
   }

   data = glyph->bitmap.buffer;
   width = glyph->bitmap.width;
   height = glyph->bitmap.rows;
   stride = glyph->bitmap.pitch;

   /* mono to gray, and flip if needed */
   if (glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
      data = malloc(width * height);
      if (!data)
         return VG_INVALID_HANDLE;

      for (i = 0; i < height; i++) {
         char *dst = &data[width * i];
         const unsigned char *src;

         if (stride > 0)
            src = glyph->bitmap.buffer + stride * (height - i - 1);
         else
            src = glyph->bitmap.buffer - stride * i;

         for (j = 0; j < width; j++) {
            if (src[j / 8] & (1 << (7 - (j % 8))))
               dst[j] = 0xff;
            else
               dst[j] = 0x0;
         }
      }
      stride = -width;
   }

   image = vgCreateImage(VG_A_8, width, height,
         VG_IMAGE_QUALITY_NONANTIALIASED);

   if (stride < 0) {
      stride = -stride;
      vgImageSubData(image, data, stride, VG_A_8,
            0, 0, width, height);
   }
   else {
      /* flip vertically */
      for (i = 0; i < height; i++) {
         const char *row = data + stride * i;

         vgImageSubData(image, row, stride, VG_A_8,
               0, height - i - 1, width, 1);
      }
   }

   if (data != glyph->bitmap.buffer)
      free(data);

   return (VGHandle) image;
}

static void
glyph_string_init(const char *font, int size, const char *str)
{
   FT_Library lib = NULL;
   FT_Face face = NULL;
   int i;

   if (FT_Init_FreeType(&lib)) {
      printf("failed to initialize freetype\n");
      goto fail;
   }

   if (FT_New_Face(lib, font, 0, &face)) {
      printf("failed to load %s\n", glyph_string);
      goto fail;
   }

   if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
      printf("failed to select an unicode charmap\n");
      goto fail;
   }

   if (FT_Set_Pixel_Sizes(face, size, size))
      printf("failed to set pixel sizes\n");

   for (i = 0; str[i]; i++) {
      VGfloat origin[2], escapement[2];
      VGHandle handle;

      /*
       * if a character appears more than once, it will be loaded and converted
       * again...
       */
      if (FT_Load_Char(face, str[i], FT_LOAD_DEFAULT)) {
         printf("failed to load glyph '%c'\n", str[i]);
         goto fail;
      }

      escapement[0] = (VGfloat) face->glyph->advance.x / 64.0f;
      escapement[1] = (VGfloat) face->glyph->advance.y / 64.0f;

      switch (face->glyph->format) {
      case FT_GLYPH_FORMAT_OUTLINE:
         handle = convert_outline_glyph(face->glyph);
         origin[0] = 0.0f;
         origin[1] = 0.0f;
         glyph_string_add_path((VGPath) handle, origin, escapement);
         break;
      case FT_GLYPH_FORMAT_BITMAP:
         handle = convert_bitmap_glyph(face->glyph);
         origin[0] = (VGfloat) (-face->glyph->bitmap_left);
         origin[1] = (VGfloat)
            (face->glyph->bitmap.rows - face->glyph->bitmap_top);
         glyph_string_add_image((VGImage) handle, origin, escapement);
         break;
      default:
         printf("unsupported format for glyph '%c'\n", str[i]);
         break;
      }
      if (handle == VG_INVALID_HANDLE)
         printf("failed to add glyph '%c'\n", str[i]);
   }

fail:
   if (face)
      FT_Done_Face(face);
   if (lib)
      FT_Done_FreeType(lib);
}

static void
display(void)
{
   vgClear(0, 0, width, height);
   glyph_string_draw(10.0, 10.0);
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
   VGPaint paint;
   float clear_color[4] = { 0.9, 0.9, 0.9, 1.0 };

   vgSetfv(VG_CLEAR_COLOR, 4, clear_color);
   vgSeti(VG_IMAGE_MODE, VG_DRAW_IMAGE_STENCIL);

   paint = vgCreatePaint();
   vgSetParameteri(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
   vgSetColor(paint, 0x660000ff);
   vgSetPaint(paint, VG_FILL_PATH);

#ifdef OPENVG_VERSION_1_1
   {
      const char *ver = (const char *) vgGetString(VG_VERSION);

      if (!strcmp(ver, "1.1"))
         glyph_string_font = vgCreateFont(0);
   }
#endif

   if (glyph_string_font != VG_INVALID_HANDLE)
      printf("using OpenVG text support\n");
}

int
main(int argc, char *argv[])
{
   const char *font, *str;

   if (argc < 2) {
      printf("Usage: %s <path-to-font> [<string>]\n", argv[0]);
      return 1;
   }

   font = argv[1];
   str = (argc > 2) ? argv[2] : "Hello World";

   eglutInitWindowSize(480, 80);
   eglutInitAPIMask(EGLUT_OPENVG_BIT);
   eglutInit(argc, argv);

   eglutCreateWindow("Text Example");

   eglutReshapeFunc(reshape);
   eglutDisplayFunc(display);

   init();
   glyph_string_init(font, 64, str);

   eglutMainLoop();

   return 0;
}
