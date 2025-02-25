#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "money_to_string.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#define TEST_COUNT 1000
#define MAX_TEST_NUMBER 500

typedef MoneyProviderFunction (*SetMoneyProvider_fptr)(MoneyProviderFunction);

int rand_between(int min, int max);

int main() {
  
#ifdef _WIN32
    HMODULE handle = LoadLibrary("money_cache.dll");
#else
    void *handle = dlopen("./money_cache.so", RTLD_NOW);
#endif

    if (!handle) {
#ifdef _WIN32
        fprintf(stderr, "Error loading money_cache.dll (code %lu)\n", GetLastError());
#else
        fprintf(stderr, "Error loading money_cache.so: %s\n", dlerror());
#endif
        return 1;
    }


#ifdef _WIN32
    SetMoneyProvider_fptr set_provider = (SetMoneyProvider_fptr)(void *)GetProcAddress(handle, "set_provider");
#else
    SetMoneyProvider_fptr set_provider = (SetMoneyProvider_fptr)dlsym(handle, "set_provider");
#endif

    if (!set_provider) {
        fprintf(stderr, "Error: could not load set_provider function\n");
        return 1;
    }


    MoneyProviderFunction get_me_a_value = set_provider(money_to_string);


    for ( int test_number = 0; test_number < TEST_COUNT; test_number++ ) {
        int randomnumber = rand_between(1, MAX_TEST_NUMBER);

        printf( "\n\nBeginning test %2d: %d Number value\n", test_number, randomnumber );

        char *word_representation = get_me_a_value(randomnumber);

        printf( "Done with test %2d: %d = %s\n", test_number, randomnumber, word_representation );
    }


#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
    return 0;
}

int rand_between(int min, int max) {
    int dollars = min + rand() % (max - min + 1);
    int cents = rand() % 100;
    int result = dollars * 100 + cents;
    return result;
}