#include <stdio.h>
#include "bogo.h"
#include "dsl.h"

struct RuleQueue *buildRules() {
    struct RuleQueue *rules = newRuleQueue();
    TAILQ_INIT(rules);

    bgstr ruleTemplates[] = {
        "a a a^",
        "a w a(",
        "e e e^",
        "o o o^",
        "o w o+",
        "u w u+",
        "d d d-",
        "_ f `",
        "_ r ?",
        "_ x ~",
        "_ j .",
        "_ s '",
    };

    int len = sizeof(ruleTemplates) / sizeof(bgstr);

    for (int i = 0; i < len; ++i) {
        struct Rule *rule = newRule();
        parseRuleFromString(rule, ruleTemplates[i]);
        TAILQ_INSERT_TAIL(rules, rule, queuePtrs);
    }

    return rules;
}

int main() {

    struct RuleQueue *rules = buildRules();
    bgstr input;
    bgstr output;

    while (1) {
        printf("> ");
        scanf("%s", input);
        processString(rules, output, input);
        printf("%s\n", output);
    }

    return 0;
}
