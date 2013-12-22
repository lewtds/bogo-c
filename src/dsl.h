#ifndef DSL_H
#define DSL_H

#include "common.h"

void strToTrans(struct RuleT *rule, const bgstr str);
void strToTransType(union TransTypeUnion *transType, const bgstr str);

#endif // DSL_H
