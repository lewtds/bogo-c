#define _GNU_SOURCE

#include <wchar.h>
#include "mark.h"

wchar_t add_mark_to_char(wchar_t chr, Mark mark)
{

}

wchar_t strip_mark_from_char(wchar_t chr)
{
    // TODO Backup and restore the tone
    static wchar_t *mark_groups[] = {L"aăâ", L"eê", L"oôơ", L"uư", L"dđ"};
    for (int i = 0; i < 5; i++) {
        if (wcschr(mark_groups[i], chr) != NULL) {
            return mark_groups[i][0];
        }
    }

    return chr;
}
