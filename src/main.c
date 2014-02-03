#include <stdio.h>
#include <malloc.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "bogo.h"
#include "dsl.h"

struct RuleQueue *buildRules() {
    struct RuleQueue *rules = newRuleQueue();
    TAILQ_INIT(rules);

    // NOTE: The rules order matters. "u w u+" should come before "a w a(" so
    //       that "uaw" resolves to ưa and not uă.
    //
    // FIXME: Should this behavior be tolerated? Should we fix uă explicitly
    //        in postProcessString()?
    bgstr ruleTemplates[] = {
        "o w o+",
        "u w u+",
        "a a a^",
        "a w a(",
        "e e e^",
        "o o o^",
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
    char* input;
    bgstr chunk;
    bgstr output;

    while (1) {
        input = readline("> ");


        int k = 0;
        for (int i = 0;; ++i) {
            if (input[i] != ' ' && input[i] != '\0') {
                chunk[k++] = input[i];
            } else {
                chunk[k] = '\0';
                processString(rules, output, chunk);

                char sep = input[i] == '\0' ? '\n' : ' ';
                printf("%s%c", output, sep);

                if (input[i] == '\0') {
                    break;
                }
                k = 0;
            }
        }

        free(input);
    }

    return 0;
}
