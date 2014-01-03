/*

  This file is a part of bogoengine project.

  Copyright (C) 2013 Duong H. Nguyen <cmpitgATgmaildotcom>
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


#ifndef DSL_H
#define DSL_H

#include "common.h"
#include "bogo.h"

void parseRuleFromString (struct Rule *rule, const bgstr str);
void parseTransType      (enum TransformationType *transType, union ToneMarkUnion *transMethod, const bgstr str);

#endif // DSL_H
