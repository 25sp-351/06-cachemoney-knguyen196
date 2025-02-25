#include "money_to_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#define EXPORT __declspec( dllexport )
#else
#define EXPORT
#endif

typedef struct {
    long long key;
    char value[MAX_BUFFER_SIZE];
} MoneyCacheEntry;

#define CACHE_SIZE 100
static MoneyCacheEntry cache[CACHE_SIZE];
static int cache_count = 0;

static char *cached_money_to_string( long long value ) {
    for  ( int ix = 0; ix < cache_count; ix++ ) {
        if ( cache[ix].key == value ) {
            return cache[ix].value;
        }
    }

    char *result = money_to_string( value );

    if ( cache_count < CACHE_SIZE ) {
        cache[cache_count].key = value;
        memcpy(cache[cache_count].value, result, MAX_BUFFER_SIZE);
        cache[cache_count].value[MAX_BUFFER_SIZE - 1] = '\0';
        cache_count++;
    }
        return result;
}

EXPORT MoneyProviderFunction set_provider( MoneyProviderFunction downstream ) {
    ( void )downstream;
    return cached_money_to_string;
}

typedef struct {
    int requests;
    int hits;
    int misses;
} MoneyCacheStats;

static MoneyCacheStats stats = {0, 0, 0};

EXPORT void reset_statistics() {
    stats.requests = 0;
    stats.hits = 0;
    stats.misses = 0;
}

EXPORT MoneyCacheStats *statistics( void ) {
    MoneyCacheStats *s = malloc( sizeof(MoneyCacheStats) );
    *s = stats;
    return s;
}

EXPORT void cleanup( void ) {
    cache_count = 0;
}
