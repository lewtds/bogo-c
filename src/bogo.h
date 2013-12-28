#ifndef BOGO_H
#define BOGO_H

#include "list.h"
#include "common.h"

enum TransEnum {
    TRANS_APPEND,
    TRANS_TONE,
    TRANS_MARK,
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
    const struct RuleT           *rule;
    struct TransT          *target;
    int                    dest_index;  /* For TRANS_APPEND, a pointer to the */
                                        /* char in the flattened string made  */
                                        /* by this TransT                     */
};

struct RuleT {
    bgstr key;
    bgstr effectOn;
    enum TransEnum type;
    union TransTypeUnion transMethod;
};

void flatten(bgstr output, struct List *transList);
void addToneToChar(bgstr chr, enum ToneEnum tone);
void addMarkToChar(bgstr chr, enum MarkEnum mark);

void processString(struct List *rules, bgstr output, const bgstr input);

#endif // BOGO_H
