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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#define animation_begin(func_name, func_list) \
	Animation * func_name func_list { \
		Animation *ret_aim = NULL;  \
		GroupAnimation * __group = NULL; \
		__group = NULL;

#define foreach_view( ... )  do { \
	unsigned int i = 0; \
	View* __views[] = { __VA_ARGS__} ; \
	for(i = 0; i < sizeof(__views) / sizeof(View*); i ++) { \
		View * _theview = __views[i];

#define end_foreach   } } while(0);
	
#define animation_end \
		return ret_aim; }

#define group_begin(type) do{ \
	GroupAnimation* tmp_group = NGUX_NEW_EX(GroupAnimation, ((type), __group)); \
	GroupAnimation* __group = tmp_group; \
	Animation* __anim = (Animation*)__group;

#define group_end \
	ret_aim = __anim; } while(0);

#define add_anim(ani)  __group->add((ani))

#define set_duration(n)     __anim->setDuration((n))
#define set_loop(n)         __anim->setLoopCount((n))
#define set_keeplive(n)     __anim->setKeepLive((n))
#define set_direction(n)    __anim->setDirection((n))
#define set_curve(n)        __anim->setCurve((n))
#define set_startvalue(n)   __anim->setStartValue((n))
#define set_endtvalue(n)    __anim->setEndValue((n))

#define move_view(view, target_pt) ret_aim = (Animation*) NGUX_NEW_EX(MoveViewAnimation, ((view), (target_pt)))

#define move_view_begin(view, target_pt) do{ \
	Animation* __anim = (Animation*) NGUX_NEW_EX(MoveViewAnimation, ((view), (target_pt))); 

#define move_view_end \
	if(__group) __group->add(__anim); \
	ret_aim = __anim; } while(0);


#define scale_view_begin(view, target_rect) do{ \
	Animation* __anim = (Animation*) NGUX_NEW_EX(ScaleViewAnimation, ((view), (target_rect))); 

#define alpha_view_begin(view, target_alpha) do{ \
	Animation* __anim = (Animation*) NGUX_NEW_EX(AlphaViewAnimation, ((view), (target_alpha)));

#define scale_view_end \
	if(__group) __group->add(__anim); \
	ret_aim = __anim; } while(0);

#define scale_view(view, target_rect) ret_aim = (Animation*) NGUX_NEW_EX(MoveViewAnimation, ((view), (target_rect)))

#define alpha_view_end \
	if(__group) __group->add(__anim); \
	ret_aim = __anim; } while(0);

#define alpha_view(view, target_alpha) ret_aim = (Animation*) NGUX_NEW_EX(AlphaViewAnimation, ((view), (target_alpha)))


#define RunAnimation(anim_name, args) \
do{ Animation *ani = anim_name args; if(ani) ani->start(ture); } while(0)
