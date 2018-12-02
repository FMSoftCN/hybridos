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
#ifndef _SCALEVIEWANIMATION_H_
#define _SCALEVIEWANIMATION_H_

#include "viewanimation.h"

namespace hfcl {

class ScaleViewAnimation  : public ViewAnimation 
{
    public:
        ScaleViewAnimation(View *view, IntRect &rect)
			:ViewAnimation(view, MGEFF_RECT){
			//setStartValue((void *)&(view->getRect()));
			setEndValue((void *)&rect);		
		}
        virtual ~ScaleViewAnimation(){

		}

	protected:
		void setProperty(int id, void* value);
		void onStart();
};

} // namespace hfcl {
#endif
