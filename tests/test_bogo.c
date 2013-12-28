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

    return finishTestCase ();
}

int main (int argc, char *argv[]) {

    addTest (testAddToneToChar);

    runAllTests ();

    return 0;
}
