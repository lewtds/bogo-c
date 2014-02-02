/*

  This file is a part of bogoengine project.

  Copyright (C) 2012-2013 Duong H. Nguyen <cmpitgATgmaildotcom>
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

/* See `Makefile` for more details on how this file is used */

#include <stdio.h>
#include <string.h>
#include "unittest.h"

#include "dsl.h"
#include "bogo.h"

int testParseTransType(void) {
    initTestCase("Parse transformation type and method");

    union ToneMarkUnion method;
    enum TransformationType type;
    parseTransType(&type, &method, "^");

    assertInt(TRANS_MARK, type);
    assertInt(MARK_HAT, method.mark);

    parseTransType(&type, &method, "'");

    assertInt(TRANS_TONE, type);
    assertInt(TONE_ACUTE, method.tone);

    return finishTestCase();
}

int testSimpleHatRules(void) {
    initTestCase ("Parse simple hat rules");

    struct Rule rule;
    parseRuleFromString(&rule, "a a a^");

    assertInt(TRANS_MARK, rule.type);
    assertStr("a", rule.key);
    assertStr("a", rule.effectOn);
    assertInt(MARK_HAT, rule.toneMarkDetail.mark);

    parseRuleFromString(&rule, "a w a(");

    assertInt(TRANS_MARK, rule.type);
    assertStr("w", rule.key);
    assertStr("a", rule.effectOn);
    assertInt(MARK_BREVE, rule.toneMarkDetail.mark);

    return finishTestCase ();
}

int main (int argc, char *argv[]) {

    addTest (testSimpleHatRules);
    addTest (testParseTransType);

    runAllTests ();

    return 0;
}
