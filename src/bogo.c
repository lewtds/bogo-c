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


/* Conventions:
 * - Structural types:
 *   - Suffix `struct` with `T`
 *   - Suffix `enum` with `Enum`
 *   - Suffix `union` with `Union`
 *
 *   Why not? We have huge vocabulary!
 *
 * - Other `typedef` shouldn't have any type suffixes
 *
 * - Shorten all "transformation" and *transform* to *trans*
 *
 * - All characters should be treated as strings
 *
 * - TransT actually denotes a state/instruction of the transformation process
 *
 * - All control structures should use code block
 *
 * - Functions with outputs should be written in forms:
 *
 *   void func1 (outputVar, inputVar);
 *   void func2 (outputVar, inputVar, arg1, arg2, arg3, ...);
 *   void func3 (outputVar1, outputVar2, inputVar1, inputVar2, arg1, arg2, arg3, ...);
 *
 *   Except when the function returns a `bgChar` or a numeric type, all other
 *   functions should follow that convention.
 *
 * - All functions should have clear documentation along with examples of all
 *   possible use cases.  Examples are not need only when the use of the function
 *   is *extremely* obvious from the context.
 */



#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "bogo.h"
#include "utf8small.h"


const bgstr VOWELS = "àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";


#define TAILQ_FREE(iter, list, field) do {  \
    TAILQ_FOREACH(iter, list, field) {      \
        free(iter);                         \
    }                                       \
    free(list);                             \
} while(0)

// Note that some text editors/IDEs doesn't understand designated initialization yet
// and will flag this as syntax error.

const struct Rule APPEND_RULE = {
    .key = "",
    .effectOn = "",
    .type = TRANS_APPEND,
    .toneMarkDetail = 0
};


struct Rule* newRule(void)
{
    struct Rule* tmp = malloc(sizeof(struct Rule));
    return tmp;
}

struct Transformation* newTransformation(void)
{
    struct Transformation* tmp = malloc(sizeof(struct Transformation));
    tmp->target = NULL;
    return tmp;
}

struct TransformationQueue* newTransformationQueue(void)
{
    struct TransformationQueue* tmp = \
            malloc(sizeof(struct TransformationQueue));
    TAILQ_INIT(tmp);
    return tmp;
}

struct RuleQueue* newRuleQueue(void)
{
    struct RuleQueue* tmp = malloc(sizeof(struct RuleQueue));
    TAILQ_INIT(tmp);
    return tmp;
}


/*
 * Apply all transformations in a list of transformations into a result string.
 *
 * bgstr buffer;
 *
 * flatten(output    = buffer,
 *         transList = [{ key:a, type:APPEND },
 *                      { key:s, type:TONE, tone:HAT, target:&transList[0] }])
 *
 * => buffer = "á"
 */
void flatten(bgstr output,
             struct TransformationQueue *transList)
{
    int output_index = 0;

    // Holds output characters to be joined into "output"
    // FIXME: Possible stack buffer overflow bug
    bgchar outputArray[32];

    struct Transformation *trans;
    TAILQ_FOREACH(trans, transList, queuePtrs) {

        switch (trans->rule.type) {
        case TRANS_APPEND:
            bgstrAssign(outputArray[output_index], trans->rule.key);

            trans->dest_index = output_index;
            output_index++;  // Only TRANS_APPEND creates a new char
            break;
        case TRANS_TONE:
            addToneToChar(outputArray[trans->target->dest_index],
                          trans->rule.toneMarkDetail.tone);
            break;
        case TRANS_MARK:
            addMarkToChar(outputArray[trans->target->dest_index],
                          trans->rule.toneMarkDetail.mark);
            break;
        }
    }


    // Join outputArray into "output"
    int outputLen = output_index;
    output_index = 0;
    for (int i = 0; i < outputLen; ++i) {

        bgstrAssign(output + output_index, outputArray[i]);
        output_index += strlen(outputArray[i]);
    }
}


/*
 * addToneToChar("a", TONE_ACUTE) => "á"
 */
void addToneToChar(bgstr chr, enum Tone tone)
{
    int index = bgstrIndexOf(VOWELS, chr, 0);

    if (index != -1) {
        int current_tone = index % 6;
        int offset = tone - current_tone;

        bgstrSubStr(chr, VOWELS, index + offset, 1);
    }
}


/*
 * getToneFromChar("a") => TONE_NONE
 * getToneFromChar("b") => TONE_NONE
 * getToneFromChar("á") => TONE_ACUTE
 */
enum Tone getToneFromChar(bgchar chr)
{
    int index = bgstrIndexOf(VOWELS, chr, 0);
    if (index != -1) {
        int current_tone = index % 6;
        return current_tone;
    } else {
        return TONE_NONE;
    }
}

/*
 * addMarkToChar("a", MARK_HAT) => "â"
 */
void addMarkToChar(bgstr chr, enum Mark mark)
{
    static bgstr mark_groups[] =
    {"aâăaa", "eêeee", "oôoơo", "uuuưu", "ddddđ"};

    // Backup and clear the tone
    enum Tone tone = getToneFromChar(chr);
    addToneToChar(chr, TONE_NONE);

    for (int i = 0; i < 5; i++) {
        if (bgstrIndexOf(mark_groups[i], chr, 0) != -1) {
            bgstrSubStr(chr, mark_groups[i], mark, 1);
            break;
        }
    }

    // Restore the tone
    addToneToChar(chr, tone);
}


/*
 * // aos -> áo
 * findToneTarget(prevTrans = [{key:a, type:APPEND}, {key:o, type:APPEND}],
 *                trans     = {target:NULL, rule:NULL},
 *                rule      = {key:s, type:TONE, tone:TONE_ACUTE})
 *
 * => trans = {
 *      target: {key:a, type:APPEND},
 *      rule  : {key:s, type:TONE, tone:TONE_ACUTE}
 *    }
 */
bool findToneTarget(struct TransformationQueue *prevTransformations,
                    struct Transformation *trans,
                    struct Rule *rule)
{
    int vcount = 0;

    bool isVowel(bgchar chr)
    {
        // Note that transformations' key can only be ASCII chars.
        return bgstrEqual(chr, "a") ||
               bgstrEqual(chr, "e") ||
               bgstrEqual(chr, "i") ||
               bgstrEqual(chr, "o") ||
               bgstrEqual(chr, "u") ||
               bgstrEqual(chr, "y");
    }

    /*
     * Find the right-most chunk of at most 3 continuous vowels in
     * prevTransformations. The results are in reverse.
     *
     * e.g. findRightmostVowels("athuong") -> "ou"
     */
    struct Transformation **findRightmostVowels()
    {
        struct Transformation** vowels = malloc(sizeof(void*) * 3);

        struct Transformation* pointer;
        TAILQ_FOREACH_REVERSE(pointer,
                              prevTransformations,
                              TransformationQueue,
                              queuePtrs) {
            if (pointer->rule.type == TRANS_APPEND &&
                isVowel(pointer->rule.key)) {
                vowels[vcount] = pointer;
                vcount++;
            }

            if (vcount == 3) {
                break;
            }
        }

        return vowels;
    }

    /*
     * Check if a vowel is followed by a TRANS_APPEND that contains
     * a consonant.
     *
     * Linked list: {key:a, type:APPEND} -> {key:n, type:APPEND}
     * hasConsonantBehind({key:a, type:APPEND}) => TRUE
     *
     * Linked list: {key:a, type:APPEND} -> NULL
     * hasConsonantBehind({key:a, type:APPEND}) => FALSE
     */
    bool hasConsonantBehind(struct Transformation *vowel)
    {
        bool found = FALSE;
        struct Transformation *next = vowel;

        while (next = TAILQ_NEXT(next, queuePtrs)) {
            if (next->rule.type == TRANS_APPEND && !isVowel(next->rule.key)) {
                found = TRUE;
                break;
            }
        }

        return found;
    }

    // Special cases

    bool isUo(struct Transformation **vowels)
    {
        return bgstrEqual(vowels[0]->rule.key, "o") &&
               bgstrEqual(vowels[1]->rule.key, "u");
    }

    bool isUye(struct Transformation **vowels)
    {
        return bgstrEqual(vowels[0]->rule.key, "e") &&
               bgstrEqual(vowels[1]->rule.key, "y") &&
               bgstrEqual(vowels[2]->rule.key, "u");
    }

    struct Transformation **rightmostVowels = findRightmostVowels();

    switch (vcount) {
    case 1:
        // cá
        trans->target = rightmostVowels[0];
        break;
    case 2:
        if (hasConsonantBehind(rightmostVowels[0]) ||
            isUo(rightmostVowels)) {
            // nước, thuở
            trans->target = rightmostVowels[0];
        } else {
            // cáo
            trans->target = rightmostVowels[1];
        }
        break;
    case 3:
        if (isUye(rightmostVowels)) {
            // chuyển
            trans->target = rightmostVowels[0];
        } else {
            // khuỷu
            trans->target = rightmostVowels[1];
        }
        break;
    default:
        trans->target = NULL;
        break;
    }

    if (trans->target != NULL) {
        trans->rule = *rule;
        return TRUE;
    } else {
        return FALSE;
    }
}


/*
 * // ana -> ân
 * findMarkTarget(prevTrans = [{key:a, type:APPEND}, {key:n, type:APPEND}],
 *                trans     = {target:NULL, rule:NULL},
 *                rule      = {key:a, type:MARK, mark:HAT})
 *
 * => trans = {
 *      target: {key:a, type:APPEND},
 *      rule  : {key:a, type:MARK, mark:HAT}
 *    }
 */
bool findMarkTarget(struct TransformationQueue *prevTransformations,
                   struct Transformation *trans,
                   struct Rule *rule)
{
    struct Transformation *currentTrans;

    TAILQ_FOREACH_REVERSE(currentTrans,
                          prevTransformations,
                          TransformationQueue,
                          queuePtrs) {
        if (bgstrEqual(currentTrans->rule.key, rule->effectOn)) {
            trans->target = currentTrans;
            break;
        }
    }

    if (trans->target != NULL) {
        trans->rule = *rule;
        return TRUE;
    } else {
        return FALSE;
    }
}


/*
 * // aa -> â
 * processChar(rules     = [{key:a, effectOn:a, type:MARK, mark:HAT}],
 *             prevTrans = [{key:a, type:APPEND}],
 *             chr       = "a");
 *
 * => prevTrans = [
 *      {key:a, type:APPEND},
 *      {key:a, type:MARK, mark:HAT, target:&prevTrans[0]}
 *    ]
 */
void processChar(struct RuleQueue *rules,
                 struct TransformationQueue *prevTransformations,
                 bgstr chr)
{
    struct RuleQueue *applicable_rules = newRuleQueue();

    // Build a list of applicable rules whose key matches chr
    struct Rule *rule;
    TAILQ_FOREACH(rule, rules, queuePtrs) {
        if (bgstrEqual(rule->key, chr)) {
            struct Rule *ruleClone = newRule();
            *ruleClone = *rule;
            TAILQ_INSERT_TAIL(applicable_rules, ruleClone, queuePtrs);
        }
    }

    struct Transformation *newTrans = newTransformation();

    // A transformation is by default an appending one
    newTrans->rule = APPEND_RULE;
    bgstrAssign(newTrans->rule.key, chr);

    if (!TAILQ_EMPTY(applicable_rules)) {
        struct Rule *rule;
        int found = 0;
        TAILQ_FOREACH(rule, applicable_rules, queuePtrs) {

            if (rule->type == TRANS_MARK) {
                found = findMarkTarget(prevTransformations, newTrans, rule);
            } else if (rule->type == TRANS_TONE) {
                found = findToneTarget(prevTransformations, newTrans, rule);
            }

            if (found) {
                break;
            }
        }
    }

    TAILQ_INSERT_TAIL(prevTransformations, newTrans, queuePtrs);

    // Free up applicable_rules
    struct Rule *iter;
    TAILQ_FREE(iter, applicable_rules, queuePtrs);
}


/*
 * bgstr buffer;
 * processString(rules  = [{key:f, type:TONE, tone:GRAVE}],
 *               output = buffer,
 *               input  = "meof")
 *
 * => buffer = mèo
 */
void processString(struct RuleQueue *rules, bgstr output, const bgstr input)
{

    struct TransformationQueue *transList = newTransformationQueue();

    for (int i = 0; i < bgstrLen(input); ++i) {
        bgstr chr;
        bgstrGetCharAt(chr, input, i);
        processChar(rules, transList, chr);
    }

    flatten(output, transList);

    struct Transformation *iter;
    TAILQ_FREE(iter, transList, queuePtrs);
}

