#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "tone.h"

int main(int argc, char const *argv[])
{
    if (!setlocale(LC_CTYPE, "")) {
        fprintf(stderr, "Can't set the specified locale! "
                "Check LANG, LC_CTYPE, LC_ALL.\n");
        return 1;
    }

    printf("%lc\n", add_tone_to_char(L'a', ACUTE));
    return 0;
}
