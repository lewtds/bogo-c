//
// This file is a part of bogoengine project.
//
// Copyright (C) 2012-2013 Duong H. Nguyen <cmpitgATgmaildotcom>
// Copyright (C) 2013 Trung Ngo <ndtrung4419 AT gmail DOT com>
//
// This file is distributed under the GPL v3 license. See COPYING at the
// project's top level for more details.
//


#include <stdio.h>
#include <ctype.h>
#include "common.h"
#include "utf8small.h"

/* ----------------------------------------------------------------------- */
/* Helpers */

int getBit (int num,
            int n) {
    return (n >> num) & 1;
}

void printBits (const char *str,
                bglen_t byteNum) {
    bglen_t i, j;

    for (i = 0; i < byteNum; i++) {
        for (j = 7; j >= 0; j--) {
            printf ("%i", getBit (j, str[i]));
        }
        printf (" ");
    }
    printf ("\n");
}

void printStrBits (const char *str) {
    printBits(str, strlen_ (str));
}

void printStr (const char *str) {
    fprintf (stdout, "%s", str);
}

void printBgstr (const bgstr str) {
    printStr (str);
}

void printStrNewline (const char *str) {
    printStr (str);
    fprintf (stdout, "\n");
}

void printBgstrNewline (const bgstr str) {
    printStrNewline (str);
}

/* ----------------------------------------------------------------------- */
/* Main */

void bgstrFirstChar (bgstr target,
                     const bgstr source) {
    bgstrGetCharAt (target, source, 0);
}

void bgstrLastChar (bgstr target,
                    const bgstr source) {
    bgstrGetCharAt (target, source, bgstrLen (source) - 1);
}

void bgstrPrepend (bgstr target,
                   bgstr source,
                   bgstr str) {
    bgstrInsertStrAt (target, source, str, 0);
}

void bgstrAppend (bgstr target,
                  bgstr source,
                  bgstr str) {
    bgstrInsertStrAt (target, source, str, bgstrLen (source));
}

bglen_t bgcharCountBytesAt (const bgstr str,
                            bglen_t position) {
    bgchar ch;
    bgstrGetCharAt (ch, str, position);
    return bgcharCountBytes (ch);
}

bglen_t bgNthBgcharToNthByte (const bgstr str,
                              bglen_t position) {
    bglen_t i;
    bglen_t nth;
    bgchar tmpChar;

    /* Guarding */
    if (position < 0 || bgstrLen (str) <= position) {
        return 0;
    }

    nth = 0;
    for (i = 0; i < position; i++) {
        nth += bgcharCountBytesAt (str, i);
    }

    return nth;
}

bglen_t bgcharCountBytes (const bgchar ch) {
    bgchar tmpChar;

    bgstrGetCharAt (tmpChar, ch, 0);
    return bgstrCountBytes (tmpChar);
}

bglen_t bgstrCountBytes (const bgstr str) {
    return strlen_ (str);
}

void bgstrInsertStrAt  (bgstr target,
                        bgstr source,
                        bgstr ch,
                        bglen_t position) {
    bglen_t length = bgstrLen (source);
    bglen_t newLength = 0;
    bgstr substr1, substr2;

    bgstr result;               /* Safe copying, in case source == target */
    bgstrAssign (result, "");   /* Make sure it's a completely empty string */

    /* Guarding */
    if (position < 0 || length < position) {
        bgstrDup (result, target);
        return;
    }

    bgstrSubStr (substr1, source, 0, position);
    bgstrSubStr (substr2, source, position, -1);

    strcat (result, substr1);
    strcat (result, ch);
    strcat (result, substr2);
    newLength = bgstrCountBytes (source) + bgstrCountBytes (ch);
    result[newLength + 1] = 0;      /* Make sure it's NULL-terminated */

    bgstrAssign (result, target);
}

void bgstrInsertCharAt  (bgstr target,
                         bgstr source,
                         bgstr ch,
                         bglen_t position) {
    bgstrInsertStrAt (source, target, ch, position);
}

void bgstrAssign (bgstr target,
                  const bgstr source) {
    bgstrDup (target, source);
}

bglen_t bgstrGetCharLenAt (const bgstr str,
                           bglen_t position) {
    bgchar ch;
    bgstrGetCharAt (ch, str, position);
    return bgcharLen (ch);
}

void bgstrSubStr (bgstr target,
                  const bgstr source,
                  bglen_t from,
                  bglen_t count) {
     bgstrCopy (target, source, from, count);
}

void strCopy (char *target,
              const char *source,
              bglen_t fromByte,
              bglen_t count) {
    memcpy (target, source + fromByte, count);
    target[count] = 0;          /* Make sure the string is properly ended */
}

void bgstrCopy (bgstr target,
                const bgstr source,
                bglen_t from,
                bglen_t count) {
    bglen_t length = bgstrLen (source);
    bglen_t lastCharPosition;
    bglen_t startFrom;
    bglen_t nBytes;

    bgstr result;

    /* By default, copy all character from `from` to the end */
    if (count == -1 || length <= from + count) {
        count = length - from;
    }

    /* Guarding */
    if (from < 0 || length <= from || count == 0) {
        bgstrAssign (target, "");
        return;
    }

    lastCharPosition = from + count - 1;

    startFrom = bgNthBgcharToNthByte (source, from);
    nBytes    = bgNthBgcharToNthByte (source, from + count - 1)
        + bgstrGetCharLenAt (source, lastCharPosition) - startFrom;

    strCopy (result, source, startFrom, nBytes);
    bgstrAssign (target, result); /* Guard in case source == target */
}

void bgstrDup (bgstr target,
               const bgstr source) {
    memset (target, 0, sizeof (target));

    /* Empty string */
    if (strcmp ("", source) == 0) {
        return;
    }

    strCopy (target, source, 0, strlen_ (source));
}

void strToBgstr (bgstr target,
                 const char *source) {
    bgstrDup (target, source);
}

void bgstrToStr (char *target,
                 const bgstr source) {
    bgstrDup (target, source);
}

bglen_t bgcharLen (const bgchar ch) {
    bglen_t mask = 0xfc;
    bglen_t maskShift = 4;

    bgbyte  c = ch[0];
    bglen_t res;

    for (res = 6; res > 1; res--) {
        if ((c & mask) == mask) {
            break;
        }
        mask &= ~maskShift;
        maskShift <<= 1;
    }

    return res;
}

bglen_t bgstrLen (const bgstr str) {
    bglen_t res = 0;
    bglen_t pos = 0;
    bglen_t byteNum = strlen (str);

    while (pos < byteNum) {
        res++;
        pos += bgcharLen (str + pos);
    }

    return res;
}

void bgstrGetCharAt (bgstr target,
                     const bgstr source,
                     bglen_t pos) {
    /* Out-of-range guard */
    if (pos < 0 || bgstrLen (source) <= pos) {
        target[0] = 0;
        return;
    }

    bglen_t currentUnicodePos = 0;
    bglen_t currentBytePos    = 0;

    while (currentUnicodePos < pos) {
        currentBytePos += bgcharLen (source + currentBytePos);
        currentUnicodePos++;
    }

    bglen_t charLength = bgcharLen (source + currentBytePos);

    memcpy (target, source + currentBytePos, charLength);
    target[charLength] = 0;
}

int bgstrCmp (const bgstr str1, const bgstr str2) {
    return strcmp(str1, str2);
}


bgbool bgStartsWith(const bgstr str, const bgstr pattern)
{
    int i = 0;
    while(pattern[i] != '\0') {
        if (str[i] != pattern[i]) {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}


bgbool strIsEmpty(const bgstr str)
{
    return bgstrLen(str) == 0;
}


void stripSpaces(bgstr dest, const bgstr src)
{
    bgstrAssign(dest, src);
}


int bgstrIndexOf(const bgstr str, const bgstr pattern, int startFrom)
{
    int currentUnicodePos = 0;
    int currentBytePos    = 0;
    while(str[currentBytePos] != '\0') {
        if (bgStartsWith(str + currentBytePos, pattern)) {
            return currentUnicodePos;
        }
        currentBytePos += bgcharLen(str + currentBytePos);
        currentUnicodePos++;
    }
    return -1;
}


bgbool bgstrEqual(const bgstr left, const bgstr right)
{
    return strcmp(left, right) == 0;
}


void bgcharLower(bgchar target, bgchar source)
{
    // FIXME: Only works with ASCII for now
    if (strlen(source) == 1) {
        target[0] = tolower(source[0]);
        target[1] = '\0';
    }
}


void bgcharUpper(bgchar target, bgchar source)
{
    // FIXME: Only works with ASCII for now
    if (strlen(source) == 1) {
        target[0] = toupper(source[0]);
        target[1] = '\0';
    }
}
