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
    bgstr chunk;
    bgstr output;

    while (1) {
        printf("> ");
        fgets(input, sizeof(bgstr), stdin);

        int k = 0;
        for (int i = 0; i < sizeof(bgstr) && input[i]; ++i) {
            if (input[i] != ' ' && input[i] != '\n') {
                chunk[k++] = input[i];
            } else {
                chunk[k] = '\0';
                processString(rules, output, chunk);

                char sep = input[i] == '\n' ? '\n' : ' ';
                printf("%s%c", output, sep);

                k = 0;
            }
        }
    }

    return 0;
}
