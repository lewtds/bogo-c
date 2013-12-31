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

/*
 * Helpers:
 *
 * void  strSubStr(output, theString, position, numberOfCharacters);
 * int   strIndexOf(theString, theSubString, startFrom);
 * void  strAssign(destString, inputString);
 * bool  strStartsWith(theString, theSubString);
 *
 * void  hashGetValuerUnion(outputAsUnion, hashTable, key);
 * void  hashGetValueStr(outputAsString, hashTable, key);
 *
 */

/*
 * Constants:
 *
 * STRING_TO_TRANS = {
 *     "'": TONE_ACUTE,
 *     "`": TONE_GRAVE,
 *     "?": TONE_HOOK,
 *     "~": TONE_TILDE,
 *     ".": TONE_DOT,
 *     "_": TONE_NONE,
 *     "+": MARK_HORN = 0,
 *     "(": MARK_BREVE,
 *     "^": MARK_HAT,
 *     "-": MARK_DASH
 * }
 *
 */



#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "list.h"
#include "bogo.h"
#include "utf8small.h"


const bgstr VOWELS = "àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";

// Note that some text editors/IDEs doesn't understand designated initialization yet
// and will flag this as syntax error.

const struct Rule APPEND_RULE = {
    .key = "",
    .effectOn = "",
    .type = TRANS_APPEND,
    .toneMarkDetail = 0
};


/*
Apply all transformations in a list of transformations into a result string.

input_list = [{
    type: TRANS_APPEND,
    key: "a"
},
{
    type: TRANS_TONE,
    key: "s",
    effect: TONE_ACUTE,
    targets: [input_list[0]]
}]

output = "á"
*/
void flatten(bgstr output,
             struct List *transList)
{
    int output_index = 0;

    // Holds output characters to be joined into "output"
    struct List *outputArray = listNew();

    struct ListItem *iterator = transList->first;
    while (iterator != NULL) {

        struct Transformation *trans = (struct Transformation *) iterator->item;

        bgstrheap toBeAppended;
        bgstrheap toBeChanged;

        switch (trans->rule.type) {
        case TRANS_APPEND:
            toBeAppended = malloc(MAXSTRLEN);

            bgstrAssign(toBeAppended, trans->rule.key);
            listAppend(outputArray, toBeAppended);

            trans->dest_index = output_index;
            output_index++;  // Only TRANS_APPEND creates a new char
            break;
        case TRANS_TONE:
            toBeChanged = listIndex(outputArray, trans->target->dest_index)->item;
            addToneToChar(toBeChanged,
                             trans->rule.toneMarkDetail.tone);
            break;
        case TRANS_MARK:
            toBeChanged = listIndex(outputArray, trans->target->dest_index)->item;
            addMarkToChar(toBeChanged,
                             trans->rule.toneMarkDetail.mark);
            break;
        }

        iterator = iterator->next;
    }


    // Join outputArray into "output"
    output_index = 0;
    iterator = outputArray->first;
    while(iterator != NULL) {
        bgstrAssign(output + output_index, iterator->item);
        output_index += strlen(iterator->item);

        iterator = iterator->next;
    }
}

void addToneToChar(bgstr chr, enum Tone tone)
{
    int index = bgstrIndexOf(VOWELS, chr, 0);

    if (index != -1) {
        int current_tone = index % 6;
        int offset = tone - current_tone;

        bgstrSubStr(chr, VOWELS, index + offset, 1);
    }
}

void addMarkToChar(bgstr chr, enum Mark mark)
{
    // TODO Backup and restore the tone
    static bgstr mark_groups[] =
    {"aâăaa", "eêeee", "oôoơo", "uuuưu", "ddddđ"};

    for (int i = 0; i < 5; i++) {
        if (bgstrIndexOf(mark_groups[i], chr, 0) != -1) {
            bgstrSubStr(chr, mark_groups[i], mark, 1);
            break;
        }
    }
}

void findMarkTarget(struct List *transList, struct Transformation *trans, struct Rule *rule) {
    struct ListItem *iter = transList->last;
    while (iter != NULL) {
        ITERITEM(iter, struct Transformation, currentTrans);
        if (bgstrEqual(currentTrans->rule.key, rule->effectOn)) {
            trans->target = currentTrans;
            break;
        }
        iter = iter->prev;
    }

    if (trans->target != NULL) {
        trans->rule = *rule;
    }
}


void processChar(struct List *rules, struct List *transList, bgstr chr) {
    struct List *applicable_rules = listNew();

    // Build a list of applicable rules whose key matches chr
    struct ListItem *iter = rules->first;
    while (iter != NULL) {
        struct Rule *rule = (struct Rule *) iter->item;
        if (bgstrEqual(rule->key, chr)) {
            listAppend(applicable_rules, rule);
        }
        iter = iter->next;
    }

    struct Transformation *newTrans = new(struct Transformation);

    // A transformation is by default an appending one
    newTrans->rule = APPEND_RULE;
    bgstrAssign(newTrans->rule.key, chr);

    if (applicable_rules->length != 0) {
        struct ListItem *ruleIter = rules->first;
        while (ruleIter != NULL) {
            struct Rule *rule = (struct Rule *) ruleIter->item;

            if (rule->type == TRANS_MARK) {
                findMarkTarget(transList, newTrans, rule);
            } else {
                // Must be tonal then
//                findToneTarget(transList, newTrans, rule);
            }

            ruleIter = ruleIter->next;
        }
    }

    listAppend(transList, newTrans);
    listFree(applicable_rules);
}


void processString(struct List *rules, bgstr output, const bgstr input) {
    struct List *transList = listNew();

    for (int i = 0; i < bgstrLen(input); ++i) {
        bgstr chr;
        bgstrGetCharAt(chr, input, i);
        processChar(rules, transList, chr);
    }

    flatten(output, transList);

    listFree(transList);
}

