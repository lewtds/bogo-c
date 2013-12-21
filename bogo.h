#ifndef BOGO_H
#define BOGO_H

#include "list.h"
#include "common.h"

enum TransEnum {
    TRANS_TONE = 0,
    TRANS_MARK,
    TRANS_APPEND
};

enum ToneEnum {
    TONE_GRAVE,
    TONE_ACUTE,
    TONE_HOOK,
    TONE_TILDE,
    TONE_DOT,
    TONE_NONE
};

enum MarkEnum {
    MARK_NONE,
    MARK_HAT,
    MARK_BREVE,
    MARK_HORN,
    MARK_DASH
};

union TransTypeUnion {
    enum ToneEnum   tone;   /* Larger data structure goes first */
    enum MarkEnum   mark;
};

struct TransT {
    enum TransEnum         type;        /* Can be TRANS_TONE or TRANS_MARK    */
    bgStr                  key;         /* "a"                                */
    union TransTypeUnion   effect;      /* MARK_HAT, TONE_ACUTE...            */
    struct TransT          *target;
    int                    dest_index;  /* For TRANS_APPEND, a pointer to the */
                                        /* char in the flattened string made  */
                                        /* by this TransT                     */
};

struct RuleT {
    bgStr key;
    bgStr effectOn;
    enum TransEnum type;
    union TransTypeUnion transMethod;
};

void flatten(bgStr output, struct List *transList);
void addToneToChar(bgStr chr, enum ToneEnum tone);
void addMarkToChar(bgStr chr, enum MarkEnum mark);

void processString(struct List *rules, bgStr output, const bgStr input);

#endif // BOGO_H
