#ifndef STRING_H
#define STRING_H

#include <wchar.h>
#include "common.h"

typedef wchar_t bgChar;
typedef wchar_t bgStr[MAXSTRLEN];

void strSubstr(bgStr dest, const bgStr src, int position, int len);
void stripSpaces(bgStr dest, const bgStr src);
void strAssign(bgStr dest, const bgStr src);
bool strStartsWith(const bgStr str, const bgStr pattern);
int strIndexOf(const bgStr str, const bgStr pattern, int startFrom);
size_t strLen(const bgStr);
bool strIsEmpty(const bgStr str);
bool charEqual(bgChar left, bgChar right);
bool strEqual(const bgStr left, const bgStr right);
void strGetLastChar(bgStr lastChar, const bgStr str);

#endif // STRING_H
