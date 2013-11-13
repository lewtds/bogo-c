#ifndef TONE_H
#define TONE_H

extern const wchar_t *VOWELS;

typedef enum {
    GRAVE,
    ACUTE,
    HOOK,
    TILDE,
    DOT,
    NONE
} Tone;

wchar_t add_tone_to_char(wchar_t chr, Tone tone);

#endif
