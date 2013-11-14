#include <stdlib.h>
#include "tone.h"

const wchar_t *VOWELS = L"àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";

bool is_vowel(wchar_t chr) {
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
void find_rightmost_vowel_group(wchar_t *str,
                                int *out_start_index,
                                int *out_end_index)
{
    int len = len(str);
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
    *out_end_index = end;
}
