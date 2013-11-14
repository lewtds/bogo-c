#define _GNU_SOURCE

#include <wchar.h>
#include "mark.h"

wchar_t add_mark_to_char(wchar_t chr, Mark mark)
{
    // TODO Backup and restore the tone
    static wchar_t *mark_groups[] =
    {L"aâăaa", L"eêeee", L"oôoơo", L"uuuưu", L"ddddđ"};

    for (int i = 0; i < 5; i++) {
        if (wcschr(mark_groups[i], chr) != NULL) {
            return mark_groups[i][mark];
        }
    }

    return chr;
}

wchar_t *add_mark_to_string(wchar_t *str, Mark mark)
{
    /* Process from the tail. Try to add the mark to each character and see
       if it changes. If it does then consider that a successful match and
       return.
    */
    wchar_t *clone = wcsdup(str);
    for (int i = wcslen(clone) - 1; i > -1; i--) {
        wchar_t result = add_mark_to_char(clone[i], mark);
        if (result != clone[i]) {
            clone[i] = result;
            return clone;
        }
    }
}

wchar_t strip_mark_from_char(wchar_t chr)
{
    return add_mark_to_char(chr, MARK_NONE);
}
