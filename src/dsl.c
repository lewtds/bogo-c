/*

  This file is a part of bogoengine project.

  Copyright (C) 2013 Duong H. Nguyen <cmpitgATgmaildotcom>
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


#include "bogo.h"
#include "dsl.h"
#include "utf8small.h"

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
void parseRuleFromString(struct RuleT *rule,
                const bgstr str)
{
    bgstr tmp;

    stripSpaces(tmp, str); /* Safe, not that stripSpaces(str, str) */

    /* By default, the transformation is appending */
    bgstrAssign(rule->key, "");
    bgstrAssign(rule->effectOn, "");
    rule->type = TRANS_APPEND;

    /* Ignore comments and blank strings */
    if (bgStartsWith(tmp, "#") || strIsEmpty(tmp)) {
        return;
    }

    *rule->key = tmp[0];
    *rule->effectOn = tmp[2];

    /* Last part: transformation type */
    bgstr lastChar;
    bgstrLastChar(lastChar, tmp);
    parseTransType(&(rule->type), &(rule->transMethod), lastChar);
}

void parseTransType(enum TransEnum *transType, union TransTypeUnion *transMethod, const bgstr str)
{
    if (bgstrEqual(str, "'")) {
        *transType = TRANS_TONE;
        transMethod->tone = TONE_ACUTE;

    } else if (bgstrEqual(str, "`")) {
        *transType = TRANS_TONE;
        transMethod->tone = TONE_GRAVE;

    } else if (bgstrEqual(str, "?")) {
        *transType = TRANS_TONE;
        transMethod->tone = TONE_HOOK;

    } else if (bgstrEqual(str, "~")) {
        *transType = TRANS_TONE;
        transMethod->tone = TONE_TILDE;

    } else if (bgstrEqual(str, ".")) {
        *transType = TRANS_TONE;
        transMethod->tone = TONE_DOT;

    } else if (bgstrEqual(str, "^")) {
        *transType = TRANS_MARK;
        transMethod->mark = MARK_HAT;

    } else if (bgstrEqual(str, "(")) {
        *transType = TRANS_MARK;
        transMethod->mark = MARK_BREVE;

    } else if (bgstrEqual(str, "+")) {
        *transType = TRANS_MARK;
        transMethod->mark = MARK_HORN;

    } else if (bgstrEqual(str, "-")) {
        *transType = TRANS_MARK;
        transMethod->mark = MARK_DASH;
    }
}

