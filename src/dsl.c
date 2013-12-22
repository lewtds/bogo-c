#include "string.h"
#include "bogo.h"
#include "dsl.h"

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

/*

Convert a transformation from string to SingleTransT.

E.g.

  strToTrans(_, "a a a^")      _ => { key: "a", effectOn: "a", transType: MARK_BREVE }
  strToTrans(_, "o f o`")      _ => { key: "f", effectOn: "o", transType: TONE_GRAVE }
  strToTrans(_, "# A comment") _ => { key: "",  effectOn: "",  transType: APPEND     }
  strToTrans(_, "   ")         _ => { key: "",  effectOn: "",  transType: APPEND     }

*/
void strToTrans(struct RuleT *rule,
                const bgstr str)
{
    bgstr tmp;

    stripSpaces(tmp, str); /* Safe, not that stripSpaces(str, str) */

    /* By default, the transformation is appending */
    bgstrAssign(rule->key, L"");
    bgstrAssign(rule->effectOn, L"");
    rule->type = TRANS_APPEND;

    /* Ignore comments and blank strings */
    if (bgStartsWith(tmp, L"#") || strIsEmpty(tmp)) {
        return;
    }

    *rule->key = tmp[0];
    *rule->effectOn = tmp[2];

    /* Last part: transformation type */
    bgstr lastChar;
    bgstrLastChar(lastChar, tmp);
    strToTransType(&(rule->transMethod), lastChar);
}

void strToTransType(union TransTypeUnion *transType, const bgstr str)
{
    if (bgstrCmp(str, L"'")) {
        (*transType).tone = TONE_ACUTE;
    } else if (bgstrCmp(str, L"`")) {
        (*transType).tone = TONE_GRAVE;
    } else if (bgstrCmp(str, L"?")) {
        (*transType).tone = TONE_HOOK;
    } else if (bgstrCmp(str, L"~")) {
        (*transType).tone = TONE_TILDE;
    } else if (bgstrCmp(str, L".")) {
        (*transType).tone = TONE_DOT;
    } else if (bgstrCmp(str, L"^")) {
        (*transType).mark = MARK_HAT;
    } else if (bgstrCmp(str, L"(")) {
        (*transType).mark = MARK_BREVE;
    } else if (bgstrCmp(str, L"+")) {
        (*transType).mark = MARK_HORN;
    } else if (bgstrCmp(str, L"-")) {
        (*transType).mark = MARK_DASH;
    }
}

