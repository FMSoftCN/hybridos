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
#define animation_begin(func_name, func_list) \
    Animation * func_name func_list ;

#define animation_end

#define group_begin(type)
#define group_end
#define add_anim(ani)

#define set_duration(n)
#define set_loop(n)
#define set_keeplive(n)
#define set_direction(n)
#define set_curve(n)
#define set_startvalue(n)
#define set_endtvalue(n)

#define foreach_view( ... )
#define end_foreach

#define move_view(view, target_pt)
#define move_view_begin(view, target_pt)
#define move_view_end

#define alpha_view_begin(view, target_alpha)

#define scale_view_begin(view, target_rect)

#define scale_view_end
#define scale_view(view, target_rect)

#define alpha_view_end
#define alpha_view(view, target_alpha)

#define RunAnimation(anim_name, args)
