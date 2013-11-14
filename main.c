#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>

#include "tone.h"
#include "mark.h"

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

    printf("%ls\n", add_tone_to_string(L"meo", GRAVE));
    printf("%ls\n", add_tone_to_string(L"hoan", GRAVE));
    printf("%ls\n", add_tone_to_string(L"khuyu", HOOK));
    printf("%ls\n", add_tone_to_string(L"huyên", HOOK));
    printf("%ls\n", add_tone_to_string(L"hương", HOOK));

    printf("%lc\n", strip_mark_from_char(L'ư'));
    printf("%lc\n", add_mark_to_char(L'u', MARK_HORN));

    printf("%ls\n", add_mark_to_string(L"tuong", MARK_HORN));
    printf("%ls\n", add_mark_to_string(L"tuơng", MARK_HORN));

    return 0;
}
