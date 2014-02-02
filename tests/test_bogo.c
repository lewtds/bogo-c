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
#include "dsl.h"

int testFindMarkTarget(void) {
    initTestCase ("Find mark target");

    struct TransformationQueue *transList = newTransformationQueue();
    TAILQ_INIT(transList);

    struct Rule dDashRule = {
        "d",        // Key
        "d",        // Effect on
        TRANS_MARK, // Type
        MARK_DASH   // Mark detail
    };

    struct Transformation firstD;
    bgstrAssign(firstD.rule.key, "d");
    firstD.rule.type = TRANS_APPEND;

    struct Transformation secondD;
    bgstrAssign(secondD.rule.key, "d");
    secondD.rule.type = TRANS_APPEND;


    findMarkTarget(transList, &firstD, &dDashRule);

    TAILQ_INSERT_TAIL(transList, &firstD, queuePtrs);

    findMarkTarget(transList, &secondD, &dDashRule);

    assertInt(TRANS_APPEND, firstD.rule.type);
    assertInt(TRANS_MARK, secondD.rule.type);
    assertTrue(&firstD == secondD.target);

    // ----------------------------- //

    struct Transformation firstN;
    bgstrAssign(firstD.rule.key, "d");
    firstD.rule.type = TRANS_APPEND;

    struct Transformation secondN = firstN;

//    listFree(transList);
    TAILQ_INIT(transList);

    TAILQ_INSERT_TAIL(transList, &firstN, queuePtrs);
    TAILQ_INSERT_TAIL(transList, &secondN, queuePtrs);

    findMarkTarget(transList, &firstD, &dDashRule);

    TAILQ_INSERT_TAIL(transList, &firstD, queuePtrs);

    findMarkTarget(transList, &secondD, &dDashRule);

    assertTrue(&firstD == secondD.target);

    return finishTestCase ();
}


int testFindToneTarget1(void) {
    initTestCase ("Find tone target with two open-ended vowels");

    struct TransformationQueue *transList = newTransformationQueue();

    struct Transformation a;
    bgstrAssign(a.rule.key, "a");
    a.rule.type = TRANS_APPEND;

    struct Transformation o;
    bgstrAssign(o.rule.key, "o");
    o.rule.type = TRANS_APPEND;

    TAILQ_INSERT_TAIL(transList, &a, queuePtrs);
    TAILQ_INSERT_TAIL(transList, &o, queuePtrs);

    struct Rule acute;
    acute.type = TRANS_TONE;
    acute.toneMarkDetail.tone = TONE_ACUTE;

    struct Transformation newTrans;

    findToneTarget(transList, &newTrans, &acute);

    assertPtr(&a, newTrans.target);

    return finishTestCase();
}

int testFindToneTarget2(void) {
    initTestCase ("Find tone target with one open-ended vowel");

    struct TransformationQueue *transList = newTransformationQueue();

    struct Transformation a;
    bgstrAssign(a.rule.key, "a");
    a.rule.type = TRANS_APPEND;

    TAILQ_INSERT_TAIL(transList, &a, queuePtrs);

    struct Rule acute;
    acute.type = TRANS_TONE;
    acute.toneMarkDetail.tone = TONE_ACUTE;

    struct Transformation newTrans;

    findToneTarget(transList, &newTrans, &acute);

    assertPtr(&a, newTrans.target);

    return finishTestCase();
}

int testFindToneTarget3(void) {
    initTestCase ("Find tone target with two close-ended vowels");

    struct TransformationQueue *transList = newTransformationQueue();

    struct Transformation u;
    bgstrAssign(u.rule.key, "u");
    u.rule.type = TRANS_APPEND;

    struct Transformation o;
    bgstrAssign(o.rule.key, "o");
    o.rule.type = TRANS_APPEND;

    struct Transformation c;
    bgstrAssign(c.rule.key, "c");
    c.rule.type = TRANS_APPEND;

    TAILQ_INSERT_TAIL(transList, &u, queuePtrs);
    TAILQ_INSERT_TAIL(transList, &o, queuePtrs);
    TAILQ_INSERT_TAIL(transList, &c, queuePtrs);

    struct Rule acute;
    acute.type = TRANS_TONE;
    acute.toneMarkDetail.tone = TONE_ACUTE;

    struct Transformation newTrans;

    findToneTarget(transList, &newTrans, &acute);

    assertPtr(&o, newTrans.target);

    return finishTestCase();
}


struct RuleQueue *buildRules() {
    struct RuleQueue *rules = newRuleQueue();
    TAILQ_INIT(rules);

    bgstr telexRules[] = {
        "a a a^",
        "a w a(",
        "e e e^",
        "o o o^",
        "o w o+",
        "u w u+",
        "_ f `"
    };

    int len = sizeof(telexRules) / sizeof(bgstr);

    for (int i = 0; i < len; ++i) {
        struct Rule *rule = newRule();
        parseRuleFromString(rule, telexRules[i]);
        TAILQ_INSERT_TAIL(rules, rule, queuePtrs);
    }

    return rules;
}

int testProcessString(void) {
    initTestCase("Test whole string processing");

    struct RuleQueue *rules = buildRules();

    bgstr output;

    processString(rules, output, "ae");
    assertStr("ae", output);

    processString(rules, output, "aa");
    assertStr("â", output);

    processString(rules, output, "naa");
    assertStr("nâ", output);

    processString(rules, output, "naane");
    assertStr("nâne", output);

    processString(rules, output, "naanee");
    assertStr("nânê", output);

    processString(rules, output, "aw");
    assertStr("ă", output);

    return finishTestCase();
}

int main (int argc, char *argv[]) {

    addTest(testFindMarkTarget);
    addTest(testFindToneTarget1);
    addTest(testFindToneTarget2);
    addTest(testFindToneTarget3);
    addTest(testProcessString);

    runAllTests ();

    return 0;
}
