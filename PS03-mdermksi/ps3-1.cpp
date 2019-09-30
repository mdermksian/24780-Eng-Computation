#include <stdio.h>

int main( void )
{
    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    printf("What month and day are you interested in?\n");
    int m, d;
    scanf("%d %d", &m, &d);

    if( m > 12 || m < 1 ) {
        printf("Please enter a valid month");
        return 0;
    }

    if( d > days_in_month[m - 1] || d < 1 ) {
        printf("Please enter a valid day");
        return 0;
    }

    int num_days = 0;
    for( int i = 0; i < m - 1; ++i ) {
        num_days += days_in_month[i];
    }

    num_days += d;
    printf("It is day #%d of the year.", num_days);

    return 0;
}