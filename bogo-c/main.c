#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "string.h"
#include "bogo.h"
#include "list.h"

int main() {
    if (!setlocale(LC_CTYPE, "")) {
      fprintf(stderr, "Can't set the specified locale! "
              "Check LANG, LC_CTYPE, LC_ALL.\n");
      return 1;
    }

    struct List *rules = new(struct RuleT);
    struct RuleT aHatRule;
    strAssign(aHatRule.effectOn, L"a");
    strAssign(aHatRule.key, L"a");
    aHatRule.type = TRANS_MARK;
    aHatRule.transMethod.mark = MARK_HAT;

    listAppend(rules, &aHatRule);

    bgStr input = L"aa";
    bgStr output;

    processString(rules, output, input);

    printf("%ls\n", output);

    return 0;
}
