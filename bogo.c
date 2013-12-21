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

/*
 * DSL:
 *
 * # Dau mu
 * a a a^
 * o o o^
 * e e e^
 * o w o+
 * u w u+
 * uo w u+o+
 * a w a(
 * d d -
 *
 * # Dau thanh
 * ## Huyen
 * a f a`
 * o f o`
 * u f u`
 * e f e`
 *
 * ## Sac
 * a s a'
 * o s o'
 * u s u'
 * e s e'
 *
 * ## Hoi
 * a r a?
 * o r o?
 * u r u?
 * e r e?
 *
 * ## Nga
 * a x a~
 * o x o~
 * u x u~
 * e x e~
 *
 * ## Nang
 * a j a.
 * o j o.
 * u j u.
 * e j e.
 *
 * ## Xoa thanh
 * a z a_
 * o z o_
 * u z u_
 * e z e_
 */

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "string.h"
#include "bogo.h"


const bgStr VOWELS = L"àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";


/*

Convert a transformation from string to SingleTransT.

E.g.

  strToTrans(_, "a a a^")      _ => { key: "a", effectOn: "a", transType: MARK_BREVE }
  strToTrans(_, "o f o`")      _ => { key: "f", effectOn: "o", transType: TONE_GRAVE }
  strToTrans(_, "# A comment") _ => { key: "",  effectOn: "",  transType: APPEND     }
  strToTrans(_, "   ")         _ => { key: "",  effectOn: "",  transType: APPEND     }

*/
void strToTrans(struct RuleT *rule,
                const bgStr str)
{
    bgStr tmp;

    stripSpaces(tmp, str); /* Safe, not that stripSpaces(str, str) */

    /* By default, the transformation is appending */
    strAssign(rule->key, L"");
    strAssign(rule->effectOn, L"");
    rule->transType.append = TRANS_APPEND;

    /* Ignore comments and blank strings */
    if (strStartsWith(tmp, L"#") || strIsEmpty(tmp)) {
        return;
    }

    *rule->key = tmp[0];
    *rule->effectOn = tmp[2];

    /* Last part: transformation type */
    bgStr lastChar;
    strGetLastChar(lastChar, tmp);
    strToTransType(&(rule->transType), lastChar);
}


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
             const struct TransT *transList,
             size_t transListLen)
{

    int output_index = 0;
    for (int i = 0; i < transListLen; i++) {

        struct TransT trans = transList[i];

        switch (transList[i].type) {
        case TRANS_APPEND:
            strAssign(output + output_index, trans.key);
            trans.dest_index = output_index;
            output_index++;  // Only TRANS_APPEND creates a new char
            break;
        case TRANS_TONE:
            for (int k = 0; k < trans.targetsLen; k++) {
                addToneToChar(output + trans.targets[k]->dest_index,
                                 trans.effect.tone);
            }
            break;
        case TRANS_MARK:
            for (int k = 0; k < trans.targetsLen; k++) {
                addMarkToChar(output + trans.targets[k]->dest_index,
                                 trans.effect.mark);
            }
            break;
        }
    }
}


void strToTransType(union TransTypeUnion *transType, const bgStr str)
{
    if (strEqual(str, L"'")) {
        (*transType).tone = TONE_ACUTE;
    } else if (strEqual(str, L"`")) {
        (*transType).tone = TONE_GRAVE;
    } else if (strEqual(str, L"?")) {
        (*transType).tone = TONE_HOOK;
    } else if (strEqual(str, L"~")) {
        (*transType).tone = TONE_TILDE;
    } else if (strEqual(str, L".")) {
        (*transType).tone = TONE_DOT;
    } else if (strEqual(str, L"^")) {
        (*transType).mark = MARK_HAT;
    } else if (strEqual(str, L"(")) {
        (*transType).mark = MARK_BREVE;
    } else if (strEqual(str, L"+")) {
        (*transType).mark = MARK_HORN;
    } else if (strEqual(str, L"-")) {
        (*transType).mark = MARK_DASH;
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


