#ifndef MONEY_TO_STRING_H
#define MONEY_TO_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1000


typedef char* ( *MoneyProviderFunction )( long long );

char *money_to_string(long long value);

#endif // MONEY_TO_STRING_H