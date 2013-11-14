#ifndef TONE_H
#define TONE_H

extern const wchar_t *VOWELS;

typedef enum {
    GRAVE, // huyền
    ACUTE, // sắc
    HOOK,  // hỏi
    TILDE, // ngã
    DOT,   // nặng
    NONE   // ngang
} Tone;

wchar_t add_tone_to_char(wchar_t chr, Tone tone);

wchar_t *add_tone_to_string(wchar_t *str, Tone tone);

wchar_t strip_tone_from_char(wchar_t chr);

wchar_t *strip_tone_from_string(const wchar_t *str);
#endif
