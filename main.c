#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "tone.h"

int main(int argc, char const *argv[])
{
    char buff[32];
    wctomb(buff, add_tone_to_char(L'a', ACUTE));

    printf("%s\n", buff);
    return 0;
}
