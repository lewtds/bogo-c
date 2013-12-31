/*

  This file is a part of bogoengine project.

  Copyright (C) 2012-2013 Duong H. Nguyen <cmpitgATgmaildotcom>

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
#include <malloc.h>
#include "unittest.h"

#include "bogo.c"

int testFindMarkTarget(void) {
    initTestCase ("Find mark target");

    struct List *transList = listNew();

    struct RuleT dDashRule = {
        "d",        // Key
        "d",        // Effect on
        TRANS_MARK, // Type
        MARK_DASH   // Transformation form
    };

    struct TransT firstD;
    bgstrAssign(firstD.rule.key, "d");
    firstD.rule.type = TRANS_APPEND;

    struct TransT secondD;
    bgstrAssign(secondD.rule.key, "d");
    secondD.rule.type = TRANS_APPEND;


    findMarkTarget(transList, &firstD, &dDashRule);
    listAppend(transList, &firstD);
    findMarkTarget(transList, &secondD, &dDashRule);

    assertInt(TRANS_APPEND, firstD.rule.type);
    assertInt(TRANS_MARK, secondD.rule.type);
    assertTrue(&firstD == secondD.target);

    // ----------------------------- //

    struct TransT firstN;
    bgstrAssign(firstD.rule.key, "d");
    firstD.rule.type = TRANS_APPEND;

    struct TransT secondN = firstN;

    listFree(transList);
    transList = listNew();

    listAppend(transList, &firstN);
    listAppend(transList, &secondN);
    findMarkTarget(transList, &firstD, &dDashRule);
    listAppend(transList, &firstD);
    findMarkTarget(transList, &secondD, &dDashRule);

    assertTrue(&firstD == secondD.target);

    return finishTestCase ();
}


struct List *buildRules() {
    struct List *rules = listNew();

    bgstr ruleTemplates[] = {
        "a a a^",
        "e e e^",
        "o f o`"
    };

    for (int i = 0; i < 2; ++i) {
        struct RuleT *rule = new(struct RuleT);
        parseRuleFromString(rule, ruleTemplates[i]);
        listAppend(rules, rule);
    }

    return rules;
}

int testTest(void) {
    initTestCase("Blah");

    struct List *rules = buildRules();

    bgstr output;

    processString(rules, output, "aa");
    assertStr("â", output);

    processString(rules, output, "naa");
    assertStr("nâ", output);

    processString(rules, output, "naane");
    assertStr("nâne", output);

    return finishTestCase();
}

int main (int argc, char *argv[]) {

    addTest (testFindMarkTarget);
    addTest (testTest);

    runAllTests ();

    return 0;
}
