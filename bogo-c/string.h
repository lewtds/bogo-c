#ifndef STRING_H
#define STRING_H

#include <wchar.h>
#include "common.h"

void strSubstr(bgstr dest, const bgstr src, int position, int len);
void stripSpaces(bgstr dest, const bgstr src);
void strAssign(bgstr dest, const bgstr src);
bool strStartsWith(const bgstr str, const bgstr pattern);
int strIndexOf(const bgstr str, const bgstr pattern, int startFrom);
size_t strLen(const bgstr);
bool strIsEmpty(const bgstr str);
bool charEqual(bgChar left, bgChar right);
bool strEqual(const bgstr left, const bgstr right);
void strGetLastChar(bgstr lastChar, const bgstr str);

void strIndex(bgstr output, const bgstr input, int position);

#endif // STRING_H
