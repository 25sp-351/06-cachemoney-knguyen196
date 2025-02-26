#include "money_to_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEMO_SIZE 10000

typedef struct {
    long long key;
    char value[MAX_BUFFER_SIZE];
} Memo;

static Memo memo[MEMO_SIZE];
static int memo_count = 0;

// Checks if a value is already memoized
static char *memoized( long long key ) {
    for ( int ix = 0; ix < memo_count; ix++ ) {
        if ( memo[ix].key == key ) {
            return memo[ix].value;
        }
    }
    return NULL;
}

// Stores a value in the memoization table
static void memo_store( long long key, char *value ) {
    if ( memo_count < MEMO_SIZE ) {
        memo[memo_count].key = key;
        memcpy( memo[memo_count].value, value, MAX_BUFFER_SIZE );
        memo[memo_count].value[MAX_BUFFER_SIZE - 1] = '\0';
        memo_count++;
    }
}

// Checks memoization before calling money_cache.c
char *memoized_money_to_string( long long value ) {
    char *memo_result = memoized( value );
    if ( memo_result ) {
        return memo_result;
    }
    
    char *result = money_to_string( value );

    
    memo_store( value, result );

    return result;
}