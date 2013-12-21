#include "string.h"
#include "bogo.h"
#include "dsl.h"

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

