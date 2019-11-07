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
** animals.c:
**  Funtions to render animals.
**
** Copyright (C) 2019 FMSoft (http://www.fmsoft.cn).
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

#include "hicairo.h"

typedef struct command {
  char type;
  float x0, y0;
  float x1, y1;
  float x2, y2;
} command_t;

#include "tiger.inc"
#include "lion.inc"

#define ARRAY_LENGTH(__array) ((sizeof (__array) / sizeof (__array[0])))

static struct animal_list {
    const char*         name;
    const command_t*    cmds;
    size_t              size;
} animal_list[] = {
    { "tiger", tiger_commands, ARRAY_LENGTH (tiger_commands) },
    { "lion",  lion_commands, ARRAY_LENGTH (lion_commands) },
};

static int
draw_animal (cairo_t *cr, int idx, int width, int height)
{
    const command_t* cmds;
    size_t size;
    size_t i;

    if (idx >= ARRAY_LENGTH(animal_list) || idx < 0)
        idx = 0;

    cmds = animal_list[idx].cmds;
    size = animal_list[idx].size;

    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba (cr, 0.1, 0.2, 0.3, 1.0);
    cairo_paint (cr);
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

    cairo_translate (cr, width/2, height/2);
    cairo_scale (cr, .85, .85);

    for (i = 0; i < size; i++) {
        const struct command *cmd = cmds + i;
        switch (cmd->type) {
        case 'm':
            cairo_move_to (cr, cmd->x0, cmd->y0);
            break;
        case 'l':
            cairo_line_to (cr, cmd->x0, cmd->y0);
            break;
        case 'c':
            cairo_curve_to (cr,
                    cmd->x0, cmd->y0,
                    cmd->x1, cmd->y1,
                    cmd->x2, cmd->y2);
            break;
        case 'f':
            cairo_set_source_rgba (cr,
                           cmd->x0, cmd->y0, cmd->x1, cmd->y1);
            cairo_fill (cr);
            break;
        }
    }

    return 0;
}

int draw_tiger(cairo_t *cr, int width, int height)
{
    return draw_animal (cr, 0, width, height);
}

int draw_lion(cairo_t *cr, int width, int height)
{
    return draw_animal (cr, 1, width, height);
}

