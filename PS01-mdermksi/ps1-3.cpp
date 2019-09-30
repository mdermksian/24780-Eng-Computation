#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( void ) {
    int responseIndex;
    char responses[5][100] = {
        "Something terrible will happen to you today",
        "Today won't be very fun",
        "Today will be nothing special",
        "Today will be pretty good",
        "Today will be the best day ever"
    };

    srand( time( NULL ) );

    printf( "24-780 Engineering Computation Problem Set 1-3 - Virtual Fortune Cookie\n");
    printf( "Press the ENTER key to see your fortune for today >");
    getchar();
    responseIndex = rand() % 5;
    printf("%s\nPress the ENTER key to close the program >", responses[ responseIndex ] );
    getchar();

    return 0;
}