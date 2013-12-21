#ifndef COMMON_H
#define COMMON_H

#include <wchar.h>

#define MAXSTRLEN 4096
#define MAXTRANSLEN 20

#ifndef bool
#define bool char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef wchar_t bgChar;
typedef wchar_t bgStr[MAXSTRLEN];

#define new(type) malloc(sizeof(type))

#endif // COMMON_H
