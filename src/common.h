/*

  This file is a part of bogoengine project.

  Copyright (C) 2013 Trung Ngo <ndtrung4419 AT gmail DOT com>

  bogoengine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  bogoengine is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef COMMON_H
#define COMMON_H

#include <wchar.h>
#include "utf8small.h"

#define MAXSTRLEN 4096
#define MAXTRANSLEN 20

#ifndef bool
#define bool char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//typedef wchar_t bgChar;
//typedef wchar_t bgstr[MAXSTRLEN];

#define new(type) malloc(sizeof(type))
#define ITERITEM(iter, type, name) type* name = (type*) iter->item

#endif // COMMON_H
