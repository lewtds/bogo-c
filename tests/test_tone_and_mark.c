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
#include "unittest.h"

#include "bogo.h"

int testAddToneToChar(void) {
    initTestCase ("Add tone to char");

    bgstr target = "a";
    addToneToChar(target, TONE_ACUTE);
    assertStr("á", target);

    // Consonants should not be touched
    bgstrAssign(target, "n");
    addToneToChar(target, TONE_ACUTE);
    assertStr("n", target);

    // Should work with complex vowel
    bgstrAssign(target, "ư");
    addToneToChar(target, TONE_ACUTE);
    assertStr("ứ", target);

    // Should also change tone
    bgstrAssign(target, "ứ");
    addToneToChar(target, TONE_GRAVE);
    assertStr("ừ", target);

    return finishTestCase ();
}

int testAddMarkToChar(void) {
    initTestCase("Add mark to char");

    bgstr target = "a";
    addMarkToChar(target, MARK_HAT);
    assertStr("â", target);

    // Char should stay the same if non-compatible mark is applied
    bgstrAssign(target, "a");
    addMarkToChar(target, MARK_DASH);
    assertStr("a", target);

    // Should change/remove mark
    bgstrAssign(target, "â");
    addMarkToChar(target, MARK_NONE);
    assertStr("a", target);

    return finishTestCase();
}

int main (int argc, char *argv[]) {

    addTest (testAddToneToChar);
    addTest (testAddMarkToChar);

    runAllTests ();

    return 0;
}
