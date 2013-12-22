#ifndef STRING_H
#define STRING_H

#include <wchar.h>
#include "common.h"

void bgstrSubstr(bgstr dest, const bgstr src, int position, int len);
void stripSpaces(bgstr dest, const bgstr src);
void bgstrAssign(bgstr dest, const bgstr src);
bool bgStartsWith(const bgstr str, const bgstr pattern);
int strIndexOf(const bgstr str, const bgstr pattern, int startFrom);
size_t bgstrLen(const bgstr);
bool strIsEmpty(const bgstr str);
bool charEqual(bgChar left, bgChar right);
bool bgstrCmp(const bgstr left, const bgstr right);
void bgstrLastChar(bgstr lastChar, const bgstr str);

void bgstrGetCharAt(bgstr output, const bgstr input, int position);

#endif // STRING_H
