#ifndef TONE_H
#define TONE_H

/*
Naming Convention
-----------------

There are two classes of special markings used in Vietnamese.
We use the word 'tone' to refer to marks on vowels that change
their pitch. There are 6 such tones in Vietnamese:

* hanging, marked by a grave (à)
* sharp, marked by an acute mark (á)
* asking, marked by a hook (ả)
* tumbling, marked by a tilde (ã)
* heavy, marked by an under dot (ạ), and finally
* level or no tone

The other kind of marks are ones that create new letters from
existing ones. We call these simply 'marks'.

- circumflex, or hat (^) used in â, ê, ô
- breve, used in ă
- horn, used in ư, ơ
- dash, used in đ

*/

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
