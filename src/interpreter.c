//
// This file is a part of bogoengine project.
//
// Copyright (C) 2013 Trung Ngo <ndtrung4419 AT gmail DOT com>
//
// This file is distributed under the GPL v3 license. See COPYING at the
// project's top level for more details.
//


#include <stdio.h>
#include <time.h>
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
        input = readline("bogo > ");

        // Control-D, should quit.
        if (input == NULL) {
            printf("\n");
            break;
        }

        if (input[0] != '\0') {
            clock_t startTime = clock();

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

            add_history(input);

            clock_t endTime = clock();
            printf("%.3f ms\n", (endTime - startTime) * 1000.0 / CLOCKS_PER_SEC);
        }

        free(input);
    }

    return 0;
}
