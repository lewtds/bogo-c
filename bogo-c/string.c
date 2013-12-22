#include "string.h"

bool charEqual(bgChar left, bgChar right) {
    return left == right;
}

bool strEqual(const bgstr left, const bgstr right) {
    int i = 0;
    while(!charEqual(left[i], L'\0')) {
        if (!charEqual(left[i], right[i])) {
            return FALSE;
        }
        i++;
    }
    return charEqual(right[i], L'\0');
}

/*
 * Strip leading and trailing spaces.
 * TODO Actually implement it
 */
void stripSpaces(bgstr dest, const bgstr src)
{
    int i = 0, k = 0;

    // Leading spaces
    while(!charEqual(src[i], L'\0') && charEqual(src[i], L' ')) {
        i++;
    }
    int start = i;

    i = strLen(src) - 1;
    while(i > -1 && charEqual(src[i], L' ')) {
        i--;
    }
    int end = i + 1;

    strSubstr(dest, src, start, end - start);
}

/*
 * Copy strings
 */
void strAssign(bgstr dest, const bgstr src)
{
    int i = 0;
    while(!charEqual(src[i], L'\0')) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = L'\0';
}

bool strStartsWith(const bgstr str, const bgstr pattern)
{
    int i = 0;
    while(!charEqual(pattern[i], L'\0')) {
        if (str[i] != pattern[i]) {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}

int strIndexOf(const bgstr str, const bgstr pattern, int startFrom)
{
    int i = 0;
    while(!charEqual(str[i], L'\0')) {
        if (strStartsWith(str + startFrom + i, pattern)) {
            return i + startFrom;
        }
        i++;
    }
    return -1;
}

size_t strLen(const bgstr str)
{
    int i = 0;
    while(!charEqual(str[i], L'\0')) {
        i++;
    }
    return i;
}

bool strIsEmpty(const bgstr str)
{
    return strLen(str) == 0;
}

void strSubstr(bgstr dest, const bgstr src, int position, int len) {
    int dest_index = 0;
    for (int i = position; dest_index < len; i++) {
        dest[dest_index] = src[i];
        dest_index++;
    }
    dest[dest_index] = L'\0';
}

void strGetLastChar(bgstr lastChar, const bgstr str) {
    int i = 0;
    while(!charEqual(str[i], L'\0')) {
        i++;
    }
    strAssign(lastChar, str + i - 1);
}

void strIndex(bgstr output, const bgstr input, int position) {
    strSubstr(output, input, position, 1);
}
