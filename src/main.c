#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "string.h"
#include "bogo.h"
#include "list.h"
#include "dsl.h"

int main() {
    if (!setlocale(LC_CTYPE, "")) {
      fprintf(stderr, "Can't set the specified locale! "
              "Check LANG, LC_CTYPE, LC_ALL.\n");
      return 1;
    }

    struct List *rules = new(struct RuleT);
    struct RuleT aHatRule;

    bgstr ruleTemplates[] = {
        "a a a^",
        "e e e^",
        "o f o`"
    };

    for (int i = 0; i < 2; ++i) {
        struct RuleT *rule = new(struct RuleT);
        parseRuleFromString(rule, ruleTemplates[i]);
        listAppend(rules, rule);
    }

    bgstr input = "aa";
    bgstr output;

    processString(rules, output, input);

    printf("%s\n", output);

    return 0;
}
