#ifndef DSL_H
#define DSL_H

#include "common.h"

void strToTrans(struct RuleT *rule, const bgstr str);
void parseTransType(enum TransEnum *transType, union TransTypeUnion *transMethod, const bgstr str);

#endif // DSL_H
