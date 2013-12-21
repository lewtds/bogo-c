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
 * void  strSubstr(output, theString, position, numberOfCharacters);
 * int   strIndexOf(theString, theSubstring, startFrom);
 * void  strAssign(destString, inputString);
 * bool  strStartsWith(theString, theSubstring);
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

#include "string.h"
#include "list.h"
#include "bogo.h"


const bgStr VOWELS = L"àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";



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
void flatten(bgStr output,
             struct List *transList)
{
    int output_index = 0;

    struct ListItem *iterator = transList->first;
    while (iterator != NULL) {

        struct TransT trans = *((struct TransT *) iterator->item);

        switch (trans.type) {
        case TRANS_APPEND:
            strAssign(output + output_index, trans.key);
            trans.dest_index = output_index;
            output_index++;  // Only TRANS_APPEND creates a new char
            break;
        case TRANS_TONE:
            addToneToChar(output + trans.target->dest_index,
                             trans.effect.tone);
            break;
        case TRANS_MARK:
            addMarkToChar(output + trans.target->dest_index,
                             trans.effect.mark);
            break;
        }

        iterator = iterator->next;
    }
}

void addToneToChar(bgStr chr, enum ToneEnum tone)
{
    int index = strIndexOf(VOWELS, chr, 0);

    if (index != -1) {
        int current_tone = index % 6;
        int offset = tone - current_tone;

        strSubstr(chr, VOWELS, index + offset, 1);
    }
}

void addMarkToChar(bgStr chr, enum MarkEnum mark)
{
    // TODO Backup and restore the tone
    static bgStr mark_groups[] =
    {L"aâăaa", L"eêeee", L"oôoơo", L"uuuưu", L"ddddđ"};

    for (int i = 0; i < 5; i++) {
        if (strIndexOf(mark_groups[i], chr, 0) != -1) {
            strSubstr(chr, mark_groups[i], mark, 1);
            break;
        }
    }
}

void findMarkTarget(struct List *transList, struct TransT *trans, struct RuleT *rule) {
    struct ListItem *iter = transList->last;
    while (iter != NULL) {
        ITERITEM(iter, struct TransT, currentTrans);
        if (strEqual(currentTrans->key, rule->effectOn)) {
            trans->target = currentTrans;
            break;
        }
        iter = iter->prev;
    }

    if (trans->target == NULL) {
        trans->type = TRANS_APPEND;
    } else {
        trans->type = TRANS_MARK;
        trans->effect = rule->transMethod;
    }
}


void processChar(struct List *rules, struct List *transList, bgStr chr) {
    struct List *applicable_rules = new(struct List);

    // Build a list of applicable rules whose key matches chr
    struct ListItem *iter = rules->first;
    while (iter != NULL) {
        struct RuleT *rule = (struct RuleT *) iter->item;
        if (strEqual(rule->key, chr)) {
            listAppend(applicable_rules, rule);
        }
        iter = iter->next;
    }

    struct TransT *newTrans = new(struct TransT);
    strAssign(newTrans->key, chr);

    if (applicable_rules->length == 0) {
        newTrans->type = TRANS_APPEND;
    } else {
        struct ListItem *ruleIter = rules->first;
        while (ruleIter != NULL) {
            struct RuleT *rule = (struct RuleT *) ruleIter->item;

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
}


void processString(struct List *rules, bgStr output, const bgStr input) {
    struct List *transList = new(struct List);

    for (int i = 0; i < strLen(input); ++i) {
        bgStr chr;
        strIndex(chr, input, i);
        processChar(rules, transList, chr);
    }

    flatten(output, transList);
}

