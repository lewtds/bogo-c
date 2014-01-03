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

#ifndef __UTF8SMALL_H__
#define __UTF8SMALL_H__

#include <inttypes.h>
#include <string.h>

#define BG_MAX_BUFFER 4096
#define strlen_ strlen

typedef uint8_t   bgbyte;
typedef int32_t   bglen_t;
typedef int       bgbool;

typedef char      bgstr[BG_MAX_BUFFER];
typedef bgstr     bgchar;

/* Helper */
/* Copy string using memcpy instead of strncpy and make sure it's
   NULL-terminated */
void strCopy (char *target,
              const char *source,
              bglen_t fromByte,
              bglen_t count);

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
void bgstrGetCharAt        (bgstr target, const bgstr str, bglen_t position);

/* Convert n-th UTF8 char to its corresponding n-th byte */
bglen_t bgNthBgcharToNthByte (const bgstr str, bglen_t position);

void bgstrDup        (bgstr target, const bgstr source);
void strToBgstr      (bgstr target, const char *source);
void bgstrToStr      (char *target, const bgstr source);
void bgstrAssign     (bgstr target, const bgstr source);

void bgstrCopy    (bgstr target, const bgstr source, bglen_t from, bglen_t count);
void bgstrSubStr  (bgstr target, const bgstr source, bglen_t from, bglen_t count);

void bgstrInsertStrAt  (bgstr target,
                        bgstr source,
                        bgstr str,
                        bglen_t position);
void bgstrInsertCharAt  (bgstr target,
                         bgstr source,
                         bgstr ch,
                         bglen_t position);
void bgstrRemoveCharAt  (bgstr target, bgstr source, bglen_t position);
void bgstrAppend        (bgstr target, bgstr source, bgstr str);
void bgstrPrepend       (bgstr target, bgstr source, bgstr str);

void bgstrFirstChar     (bgstr target, const bgstr source);
void bgstrLastChar      (bgstr target, const bgstr source);

void bgstrToLower    (bgstr target, const bgstr source);
void bgstrToUpper    (bgstr target, const bgstr source);

int bgstrCmp         (const bgstr str1, const bgstr str2);
int bgstrCmpI        (const bgstr str1, const bgstr str2);

bgbool bgStartsWith  (const bgstr str, const bgstr pattern);
bgbool bgEndsWith    (const bgstr str, const bgstr pattern);

void stripSpaces(bgstr dest, const bgstr src);
bgbool strIsEmpty(const bgstr str);
int bgstrIndexOf(const bgstr str, const bgstr pattern, int startFrom);

bgbool bgstrEqual(const bgstr left, const bgstr right);
#endif
