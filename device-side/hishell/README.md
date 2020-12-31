# hiShell

hiShell is the app running enviornment (the shell) for HybridOS Device Side.

- [Introduction](#introduction)
- [How to Build](#how-to-build)
- [Current Status](#current-status)
- [TODO List](#todo-list)
- [Copying](#copying)
   + [Commercial License](#commercial-license)
   + [Special Statement](#special-statement)

## Introduction

hiShell is the app running enviornment (the shell) for HybridOS Device Side.

The hiShell contains the following modules:

1. The server of MiniGUI. This process acts as the window manager of MiniGUI.
    It is developed in C/C++, and includes a customized window compositor of MiniGUI,
    which provides the main window switching function and animation effects when switching
    among windows.
1. Live wallpaper program. This program showes a live wallpaper.
1. The app agent program (`appagent`). This program is responsible for creating and maintaining
   the system main windows, for examples, Status Bar and Docker Bar, launches and manages
   the life-cycle of a HybridOS app.
1. `hiwebkit-run`: This program runs a HybridOS app on hiWebKit.
1. `hiacejs-run`: This program runs a light-weight HybridOS app on hiACEJS.

## Current Status

- Dec. 2020: First release (version 0.9).

## TODO List

(TBD)

## Copying

Copyright (C) 2020 FMSoft <https://www.fmsoft.cn>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

### Commercial License

If you cannot accept GPLv3, you need to be licensed from FMSoft.

For more information about the commercial license, please refer to
<https://hybridos.fmsoft.cn/blog/hybridos-licensing-policy>.

### Special Statement

The above open source or free software license(s) does
not apply to any entity in the Exception List published by
Beijing FMSoft Technologies Co., Ltd.

If you are or the entity you represent is listed in the Exception List,
the above open source or free software license does not apply to you
or the entity you represent. Regardless of the purpose, you should not
use the software in any way whatsoever, including but not limited to
downloading, viewing, copying, distributing, compiling, and running.
If you have already downloaded it, you MUST destroy all of its copies.

The Exception List is published by FMSoft and may be updated
from time to time. For more information, please see
<https://www.fmsoft.cn/exception-list>.

