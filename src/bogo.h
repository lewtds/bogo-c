#ifndef BOGO_H
#define BOGO_H
/*

  This file is a part of bogoengine project.

  Copyright (C) 2013 Duong H. Nguyen <cmpitgATgmaildotcom>
  Copyright (C) 2013 Trung Ngo <ndtrung4419 AT gmail DOT com>

  bogoengine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  bogoengine is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <sys/queue.h>

#include "common.h"

enum TransformationType {
    TRANS_APPEND,
    TRANS_TONE,
    TRANS_MARK,
};

enum Tone {
    TONE_GRAVE,
    TONE_ACUTE,
    TONE_HOOK,
    TONE_TILDE,
    TONE_DOT,
    TONE_NONE
};

enum Mark {
    MARK_NONE,
    MARK_HAT,
    MARK_BREVE,
    MARK_HORN,
    MARK_DASH
};

// lewtds@github:
//   I would really prefer it if we can put tone and mark
//   directly inside Rule
union ToneMarkUnion {
    enum Tone   tone;   /* Larger data structure goes first */
    enum Mark   mark;
};

struct Rule {
    bgstr key;
    bgstr effectOn;
    enum TransformationType type;
    union ToneMarkUnion toneMarkDetail;

    TAILQ_ENTRY(Rule) queuePtrs;
};

struct Transformation {
    struct Rule           rule;
    struct Transformation *target;
    int                   dest_index;  /* For TRANS_APPEND, a pointer to the */
                                       /* char in the flattened string made  */
                                       /* by this TransT                     */

    TAILQ_ENTRY(Transformation) queuePtrs;
};

// Collection types

TAILQ_HEAD(RuleQueue, Rule);
TAILQ_HEAD(TransformationQueue, Transformation);


// Functions

void addToneToChar (bgstr chr, enum Tone tone);
void addMarkToChar (bgstr chr, enum Mark mark);

void flatten       (bgstr output, struct TransformationQueue *transList);
void processString (struct RuleQueue *rules, bgstr output, const bgstr input);

#endif // BOGO_H
