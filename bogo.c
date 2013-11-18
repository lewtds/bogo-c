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
 *     "^": MARK_HAT
 * }
 *
 */

#include <stdlib.h>
#include <wchar.h>


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

typedef wchar_t bgChar;
typedef wchar_t bgStr[MAXSTRLEN];

enum TransEnum {
    TRANS_TONE = 0,
    TRANS_MARK,
    TRANS_APPEND
};

enum ToneEnum {
    TONE_ACUTE = 0,
    TONE_GRAVE,
    TONE_HOOK,
    TONE_TILDE,
    TONE_DOT,
    TONE_NONE,
};

enum MarkEnum {
    MARK_HORN = 0,
    MARK_BREVE,
    MARK_HAT
};

union TransTypeUnion {
    enum ToneEnum   tone;   /* Larger data structure goes first */
    enum MarkEnum   mark;
};

struct TransT {
    enum TransEnum         type;        /* Can be TRANS_TONE or TRANS_MARK    */
    bgStr                  key;         /* "a"                                */
    union TransTypeUnion   effect;      /* MARK_HAT, ...                     */
    struct TransT          *targets[MAXTRANSLEN];
    size_t                 targetsLen;
    bgStr                  dest;        /* For TRANS_APPEND, a pointer to the */
                                        /* char in the flattened string made  */
                                        /* by this TransT                     */
};

struct RuleT {
    bgStr key;
    bgStr effectOn;
    union TransTypeUnion transType;
};

void flatten(bgStr output, const struct TransT *transList, size_t transListLen);
void add_tone_to_char(bgStr chr, enum ToneEnum tone);
void add_mark_to_char(bgStr chr, enum MarkEnum mark);
// void strSubstr(output, theString, position, numberOfCharacters);
void strToTrans(struct RuleT *rule, const bgStr str);

void stripSpaces(bgStr dest, const bgStr src);
void strAssign(bgStr dest, const bgStr src);
bool strStartsWith(const bgStr str, const bgStr pattern);

/*
* Get sub-string from a string
*/
// void strSubstr(output, theString, position, numberOfCharacters)
// {

// }

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
    /* looks confused and dangerous         */
    strAssign(str, tmp);

    /* By default, the transformation is appending */
    strAssign(trans.key, "");
    strAssign(trans.effectOn, "");
    trans.transType.append = TRANS_APPEND;

    /* Ignore comments and blank strings */
    if (strStartsWith(str, "#") || strIsEmpty(str)) {
        return;
    }

    /* Second part: effectOn */
    position = strIndexOf(str, " ", 0);
    strSubstr(rule.effectOn, str, 0, position + 1);

    /* First part: key */
    oldPosition = position;
    position = strIndexOf(str, " ", oldPosition + 1);
    strSubstr(rule.key, str, oldPosition, position - oldPosition + 1);

    /* Last part: transformation type */
    strLastChar(tmp, str);
    hashGetValueUnion(&rule.transType, STRING_TO_TRANS, tmp);
}


/*
# Dau mu
a a a^
o o o^
e e e^
o w o+
u w u+
uo w u+o+
a w a(
d d -

# Dau thanh
## Huyen
a f a`
o f o`
u f u`
e f e`

## Sac
a s a'
o s o'
u s u'
e s e'

## Hoi
a r a?
o r o?
u r u?
e r e?

## Nga
a x a~
o x o~
u x u~
e x e~

## Nang
a j a.
o j o.
u j u.
e j e.

## Xoa thanh
a z a_
o z o_
u z u_
e z e_
*/


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
            // output[output_index] = trans.key;
            output_index++;  // Only TRANS_APPEND creates a new char
            break;
        case TRANS_TONE:
            for (int k = 0; i < trans.targetsLen; i++) {
                add_tone_to_char(trans.targets[k]->dest,
                                 trans.effect.tone);
            }
            break;
        case TRANS_MARK:
            for (int k = 0; i < trans.targetsLen; i++) {
                add_mark_to_char(trans.targets[k]->dest,
                                 trans.effect.mark);
            }
            break;
        }
    }
}

void add_tone_to_char(bgStr chr, enum ToneEnum tone)
{

}

void add_mark_to_char(bgStr chr, enum MarkEnum mark)
{

}

bool charEqual(bgChar left, bgChar right) {
    return left == right;
}

void stripSpaces(bgStr dest, bgStr src)
{
    int i = 0, k = 0;

    while(!charEqual(src[i], L"\0")) {
        if (src[i] != L" ") {
            dest[k] = src[i];
        }
        i++;
        k++;
    }
}

void strAssign(bgStr dest, bgStr src)
{
    int i = 0;
    while(!charEqual(src[i], L"\0")) {
        dest[i] = src[i];
        i++;
    }
}

bool strStartsWith(bgStr str, bgStr pattern)
{
    int i = 0;
    while(!charEqual(pattern[i], L"\0")) {
        if (str[i] != pattern[i]) {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}
