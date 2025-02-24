#include "money_to_string.h"

#ifdef _WIN32
__declspec(dllexport)
#endif

// String arrays for number conversion
const char *units[] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
                        "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
                        "seventeen", "eighteen", "nineteen" };

const char *tens[] = { "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

const char *multipliers[] = { "", "thousand", "million", "billion", "trillion" };

// Function to concatenate two strings with a size limit
void concat( char *string1, const char *string2, size_t size ) {
    size_t ix = strlen( string1 );
    size_t jx = 0;

    while ( string2[jx] != '\0' && ix < size - 1 ) {
        string1[ix++] = string2[jx++];
    }
    string1[ix] = '\0';
}

// Function to convert a number < 1000 into words
void hundred_num_to_string( int value, char *buffer, size_t size ) {
    buffer[0] = '\0';

    if ( value >= 100 ) {
        concat( buffer, units[value / 100], size) ;
        concat( buffer, " hundred", size );
        value %= 100;
        if ( value > 0 )
            concat( buffer, " and ", size );
    }

    if ( value >= 20 ) {
        concat( buffer, tens[value / 10], size );
        value %= 10;
        if ( value > 0)
            concat( buffer, "-", size );
    }

    if ( value > 0 ) {
        concat( buffer, units[value], size );
    }
}

// Function to convert a number ≥ 1000 into words
void num_to_string( long long value, char *buffer, size_t size ) {
    buffer[0] = '\0';

    if ( value == 0 ) {
        concat( buffer, "zero", size );
        return;
    }

    int mult = 0;
    char temp_num[MAX_BUFFER_SIZE];

    while ( value > 0 ) {
        if ( value % 1000 != 0 ) {
            hundred_num_to_string( value % 1000, temp_num, sizeof(temp_num) );

            if ( multipliers[mult][0] != '\0' ) {
                concat( temp_num, " ", sizeof(temp_num) );
                concat( temp_num, multipliers[mult], sizeof(temp_num) );
            }

            concat( temp_num, " ", sizeof(temp_num) );
            concat( temp_num, buffer, sizeof(temp_num) );

            memcpy( buffer, temp_num, size );
            buffer[size - 1] = '\0';
        }
        value /= 1000;
        mult++;
    }
}

// Main money conversion function
char *money_to_string( long long value ) {
    long long dollars = value / 100;
    int cents = value % 100;

    static char buffer[MAX_BUFFER_SIZE];
    buffer[0] = '\0';

    char temp_num[MAX_BUFFER_SIZE];

    if ( dollars > 0 ) {
        num_to_string( dollars, temp_num, sizeof(temp_num) );
        concat( buffer, temp_num, sizeof(buffer) );
        concat( buffer, "dollar", sizeof(buffer) );
        if ( dollars > 1 )
            concat( buffer, "s ", sizeof(buffer) );
    }

    if ( cents > 0 ) {
        if ( dollars > 0 ) {
            concat( buffer, "and ", sizeof(buffer) );
        }
        num_to_string( cents, temp_num, sizeof(temp_num) );
        concat( buffer, temp_num, sizeof(buffer) );
        concat( buffer, "cent", sizeof(buffer) );
        if ( cents > 1 )
            concat( buffer, "s", sizeof(buffer) );
    }

    return buffer;
}