#define _GNU_SOURCE

#include <wchar.h>
#include "tone.h"

// See notes in tone.h

// Interesting observations:
// - The position of a vowel in the vowel list tells
//   its tone (by taking the remainder after dividing the
//   position by 6)
// - The position can also tell the vowel's family (aăâeêioôơuư)

const wchar_t *VOWELS = L"àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";

static void find_rightmost_vowel_group(wchar_t *str,
                                int *out_start_index,
                                int *out_len);

static int is_vowel(wchar_t chr)
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

/*
This function only supports the traditional tone positions (hòa vs. hoà).

Process from the end of the string:

- If an ê or an ơ is found then put the tone there (tuyển)
- If 3 continuous vowels are found then put on the
  second vowel (khuỷu)
- If 2 continuous vowels are found with no adjacent consonant
  then put on the first vowel (tòa)
- If 2 continuous vowels are found with 1 adjacent consonant
  then put on the second vowel (toàn)
- If 1 vowel is found then put the tone on it (án)
*/
wchar_t *add_tone_to_string(wchar_t *str, Tone tone)
{
    wchar_t *clone = wcsdup(str);

    int start, vlen;
    find_rightmost_vowel_group(clone, &start, &vlen);

    if (start != -1) {

        wchar_t *target = NULL;

        // Find ê, ơ
        wchar_t *e_cir_pos = NULL;
        wchar_t *o_horn_pos = NULL;
        wchar_t *pointer = clone;

        while(*pointer != L'\0') {
            wchar_t naked = strip_tone_from_char(*pointer);
            if (naked == L'ê') {
                e_cir_pos = pointer;
            }
            if (naked == L'ơ') {
                o_horn_pos = pointer;
            }
            pointer++;
        }

        if (e_cir_pos != NULL || o_horn_pos != NULL) {
            target = e_cir_pos > o_horn_pos ? e_cir_pos : o_horn_pos;
        } else {
            switch (vlen) {
            case 3:
                target = clone + start + 1;
                break;
            case 2:
                if (str[start + 2] == L'\0') {
                    target = clone + start;
                } else {
                    target = clone + start + 1;
                }
                break;
            case 1:
                target = clone + start;
                break;
            }
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
