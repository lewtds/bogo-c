#ifndef TONE_H
#define TONE_H

typedef enum {
    TONE_GRAVE, // huyền
    TONE_ACUTE, // sắc
    TONE_HOOK,  // hỏi
    TONE_TILDE, // ngã
    TONE_DOT,   // nặng
    TONE_NONE   // ngang
} Tone;

wchar_t add_tone_to_char(wchar_t chr, Tone tone);

wchar_t *add_tone_to_string(wchar_t *str, Tone tone);

wchar_t strip_tone_from_char(wchar_t chr);

wchar_t *strip_tone_from_string(const wchar_t *str);
#endif
