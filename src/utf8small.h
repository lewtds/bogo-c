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

#ifndef __UTF8SMALL_H__
#define __UTF8SMALL_H__

#include <inttypes.h>
#include <string.h>

#define BG_MAX_BUFFER 4096
#define strlen_ strlen

typedef uint8_t   bgbyte;
typedef uint8_t   bgstr[BG_MAX_BUFFER];
typedef bgstr     bgchar;
typedef int32_t   bglen_t;
typedef char      bgbool;

/* Useful when debugging */
void printBits (const char *str, bglen_t byteNum);

void printStrBits    (const char *str); /* No bgstr version, printing bits
                                           mean we don't care about multibyte */
void printStr        (const char *str);
void printBgstr      (const bgstr str); /* Alias of printStr, human-readable
                                           forms should be exactly the same in
                                           all cases */

/* Ha ha. Actually no kidding at all. Small but useful when debugging */
void printStrNewline    (const char *str);
void printBgstrNewLine  (const bgstr str);

bglen_t bgcharCountBytes (const bgchar ch); /* Number of bytes of a UTF8
                                               char */
bglen_t bgstrCountBytes  (const bgstr str); /* Number of bytes of a UTF8
                                               string */

bglen_t bgcharLen          (const bgchar ch);
bglen_t bgstrLen           (const bgstr str);
bglen_t bgcharCountBytesAt (const bgstr str, bglen_t position);
bglen_t bgstrGetCharLenAt  (const bgstr str, bglen_t position);
void bgstrGetCharAt        (const bgstr str, bgstr target, bglen_t position);

/* Convert n-th UTF8 char to its corresponding n-th byte */
bglen_t bgNthBgcharToNthByte (const bgstr str, bglen_t position);

void bgstrDup        (const bgstr source, bgstr target);
void strToBgstr      (const char *source, bgstr target);
void bgstrToStr      (const bgstr source, char *target);
void bgstrAssign     (bgstr target, bgstr source);

void bgstrCopy    (bgstr source, bgstr target, bglen_t from, bglen_t count);
void bgstrSubStr  (bgstr source, bgstr target, bglen_t from, bglen_t count);

void bgstrInsertCharAt  (bgstr source,
                         bgstr target,
                         bgstr ch,
                         bglen_t position);
void bgstrRemoveCharAt  (bgstr source, bgstr target, bglen_t position);
void bgstrAppend        (bgstr source, bgstr target, bgstr str);
void bgstrPrepend       (bgstr source, bgstr target, bgstr str);

void bgstrInsertStrAt  (bgstr source,
                        bgstr target,
                        bgstr str,
                        bglen_t position);

void bgstrFirstChar     (bgstr source, bgstr target);
void bgstrLastChar      (bgstr source, bgstr target);

void bgstrToLower    (bgstr source, bgstr target);
void bgstrToUpper    (bgstr source, bgstr target);

int bgstrCmp         (bgstr str1, bgstr str2);
int bgstrCmpI        (bgstr str1, bgstr str2);

bgbool bgStartsWith  (bgstr str, bgstr pattern);
bgbool bgEndsWith    (bgstr str, bgstr pattern);

#endif
