#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
Program flow:
1. Take user input for number of flashcards
2. Start timer
3. Present flash cards to the user one by one and wait for their response
4. 

*/

void PrintCard( int a, int b )
{
    char midline[13];
    if( a > 9 && b > 9 ) {
        strcpy(midline, "|  %dx%d  |\n");
    } else if( a > 9 ) {
        strcpy(midline, "|  %dx%d   |\n");
    } else if( b > 9 ) {
        strcpy(midline, "|   %dx%d  |\n");
    } else {
        strcpy(midline, "|   %dx%d   |\n");
    }

    printf("+---------+\n");
    printf("|What is: |\n");
    printf(midline, a, b);
    printf("|         |\n");
    printf("+---------+\n");
}

void SwapElements( int &a, int &b )
{
    int temp = b;
    b = a;
    a = temp;
}

void ShuffleQuestions( int questions[] )
{
    for( int i = 0; i < 144; ++i ) {
        int swap_index = rand() % 144;
        SwapElements( questions[i], questions[swap_index] );
    }
}

int main( void )
{
    srand(time(NULL));
    int questions[144];
    for( int i = 0; i < 144; ++i ){
        questions[i] = i;
    }
    ShuffleQuestions(questions);

    int num_questions;
    printf("How many questions would you like to answer?\n");
    while(1){
        scanf("%d", &num_questions);
        if( num_questions > 0 && num_questions <= 144 ) {
            break;
        }
        printf("Please enter a number between 1 and 144!\n");
    }

    int correct_count = 0;
    int start_time = time(NULL);
    for( int i = 0; i < num_questions; ++i ) {
        int a = questions[i] % 12 + 1;
        int b = (questions[i] - a) / 12 + 1;
        int correct_answer = a * b;
        int guess;
        PrintCard(a, b);
        scanf("%d", &guess);
        if( guess == correct_answer ) {
            printf("CORRECT!\n");
            ++correct_count;
        } else {
            printf("WRONG! The correct answer is %d x %d = %d\n", a, b, correct_answer);
        }
    }
    int end_time = time(NULL);
    double percent = ((double)correct_count / (double)num_questions) * 100;
    int time_elapsed = (int)difftime(end_time, start_time);
    printf("You answered %d/%d questions right (%2.2lf%%) in %d seconds", correct_count, num_questions, percent, time_elapsed);
    return 0;
}