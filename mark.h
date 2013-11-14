#ifndef MARK_H
#define MARK_H

#include <stdlib.h>

typedef enum {
    MARK_HAT,
    MARK_BREVE,
    MARK_HORN,
    MARK_DASH,
    MARK_NONE
} Mark;

wchar_t strip_mark_from_char(wchar_t chr);

#endif
