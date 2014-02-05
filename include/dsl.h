//
// This file is a part of bogoengine project.
//
// Copyright (C) 2013 Duong H. Nguyen <cmpitgATgmaildotcom>
// Copyright (C) 2013 Trung Ngo <ndtrung4419 AT gmail DOT com>
//
// This file is distributed under the GPL v3 license. See COPYING at the
// project's top level for more details.
//


#ifndef DSL_H
#define DSL_H

#include "common.h"
#include "bogo.h"

void parseRuleFromString (struct Rule *rule, const bgstr str);
void parseTransType      (enum TransformationType *transType, union ToneMarkUnion *transMethod, const bgstr str);

#endif // DSL_H
