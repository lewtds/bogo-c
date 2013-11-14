#define _GNU_SOURCE

#include <wchar.h>
#include "tone.h"

const wchar_t *VOWELS = L"àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";

void find_rightmost_vowel_group(wchar_t *str,
                                int *out_start_index,
                                int *out_len);

int is_vowel(wchar_t chr)
{
    return wcschr(VOWELS, chr) != NULL;
}

wchar_t add_tone_to_char(wchar_t chr, Tone tone)
{
    wchar_t *match = wcschr(VOWELS, chr);

    if (match != NULL) {
        int position = match - VOWELS;
        int current_tone = position % 6;
        int offset = tone - current_tone;

        return VOWELS[position + offset];
    } else {
        return chr;
    }
}

wchar_t strip_tone_from_char(wchar_t chr)
{
    return add_tone_to_char(chr, NONE);
}

wchar_t *strip_tone_from_string(const wchar_t *str)
{
    wchar_t *dest = wcsdup(str);

    wchar_t *index = dest;
    while (*index != 0) {
        *index = strip_tone_from_char(*index);
    }

    return dest;
}

wchar_t *add_tone_to_string(wchar_t *str, Tone tone)
{
    wchar_t *clone = wcsdup(str);

    int start, vlen;
    find_rightmost_vowel_group(clone, &start, &vlen);

    if (start != -1) {

        wchar_t *target = NULL;
        switch (vlen) {
        case 3:
            break;
        case 2:
            break;
        case 1:
            target = clone + start;
            break;
        }

        *target = add_tone_to_char(*target, tone);
    }

    return clone;
}

void find_rightmost_vowel_group(wchar_t *str,
                                int *out_start_index,
                                int *out_len)
{
    int len = wcslen(str);
    int start = -1;
    int end = -1;

    for (int pos = len - 1; pos > -1; pos--) {
        if (is_vowel(str[pos])) {
            if (end == -1) {
                end = pos;
            }
        } else {
            if (end != -1) {
                start = pos + 1;
                break;
            }
        }
    }

    // The string consists only of vowels
    if (start == -1 && end != -1) {
        start = 0;
    }

    *out_start_index = start;
    *out_len = end - start + 1;
}
