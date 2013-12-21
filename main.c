#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "bogo.h"
#include "list.h"

int main() {
    if (!setlocale(LC_CTYPE, "")) {
      fprintf(stderr, "Can't set the specified locale! "
              "Check LANG, LC_CTYPE, LC_ALL.\n");
      return 1;
    }

    struct TransT items[] = {
        {TRANS_APPEND, L"a", 0, 0, 0, 0},
        {TRANS_MARK, L"a", MARK_HAT, {} , 1, 0}
    };

    struct List *seq = new(struct List);
    listFromArray(seq, items, sizeof(struct TransT), 2);

    ((struct TransT *) listIndex(seq, 1)->item)->targets[0] = (struct TransT *) listIndex(seq, 0)->item;
//    seq[1].targets[0] = &seq[0]; // The second "a" targets the first "a"

    bgStr result;
    flatten(result, seq);
    printf("%ls\n", result);


    bgStr chr = L"u";
    addToneToChar(chr, TONE_ACUTE);

    printf("%ls\n", chr);
    return 0;
}
