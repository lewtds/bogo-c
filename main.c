#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "tone.h"

int main(int argc, char const *argv[])
{
    if (!setlocale(LC_CTYPE, "")) {
      fprintf(stderr, "Can't set the specified locale! "
              "Check LANG, LC_CTYPE, LC_ALL.\n");
      return 1;
    }

    wchar_t *input = L"con";
    wchar_t *output = add_tone_to_string(input, GRAVE);


    printf("%ls\n", output);
    return 0;
}
