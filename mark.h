#ifndef MARK_H
#define MARK_H

#include <stdlib.h>

typedef enum {
    MARK_NONE,
    MARK_HAT,
    MARK_BREVE,
    MARK_HORN,
    MARK_DASH
} Mark;

wchar_t strip_mark_from_char(wchar_t chr);
wchar_t add_mark_to_char(wchar_t chr, Mark mark);
wchar_t *add_mark_to_string(wchar_t *str, Mark mark);

#endif
