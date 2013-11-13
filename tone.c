#include <stdlib.h>
#include "tone.h"

const wchar_t *VOWELS = L"àáảãạaằắẳẵặăầấẩẫậâèéẻẽẹeềếểễệêìíỉĩịi" \
                        "òóỏõọoồốổỗộôờớởỡợơùúủũụuừứửữựưỳýỷỹỵy";

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
