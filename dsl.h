#ifndef DSL_H
#define DSL_H

#include "common.h"

void strToTrans(struct RuleT *rule, const bgStr str);
void strToTransType(union TransTypeUnion *transType, const bgStr str);

#endif // DSL_H
