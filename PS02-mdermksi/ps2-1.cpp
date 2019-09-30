#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void PrintBoard(char board[])
{
    for( int r = 0; r < 3; ++r )
    {
        for( int c = 0; c < 3; ++c )
        {
            printf("%c", board[r*3 + c]);
            if( c != 2) {
                printf("|");
            }
        }
        printf("\n");
        if( r != 2 ) {
            printf("-+-+-\n");
        }
    }
}

char CheckIfPlayerWinning(char board[], char s)
{
    if( (board[0] == s && board[1] == s && board[2] == s) ||
        (board[3] == s && board[4] == s && board[5] == s) ||
        (board[6] == s && board[7] == s && board[8] == s) ||
        (board[0] == s && board[3] == s && board[6] == s) ||
        (board[1] == s && board[4] == s && board[7] == s) ||
        (board[2] == s && board[5] == s && board[8] == s) ||
        (board[0] == s && board[4] == s && board[8] == s) ||
        (board[2] == s && board[4] == s && board[6] == s) )
    {
        return s;
    }

    return 0;
}

char CheckWinner(char board[])
{
    if( 0 != CheckIfPlayerWinning(board, 'X') ) {
        return 'X';
    } else if ( 0 != CheckIfPlayerWinning(board, 'O' ) ) {
        return 'O';
    }

    return 0;
}

bool BoardFilled(char board[])
{
    for( int i = 0; i < 9; ++i ) {
        if( ' ' == board[i] ) {
            return false;
        }
    }

    return true;
}

int MakeRandomPlay( char board[] )
{
    while(true) {
        int play = rand() % 10;
        if( board[play] == ' ' ) {
            return play;
        }
    }
}

int main(void) {

    srand(time(NULL));
    char board[9];
    for( int i = 0; i < 9; ++i ) {
        board[i] = ' ';
    }

    char turn = 'O';
    while(1){
        PrintBoard(board);

        int x,y;
        if( turn == 'O' ) {
            int guess = MakeRandomPlay(board);
            x = guess % 3;
            y = ( guess - x ) / 3;
        } else {
            printf("%c's turn\n", turn);
            printf("Enter X and Y: ");
            scanf("%d%d", &x, &y);
        }

        if(0<=x && x<3 && 0<=y && y<3){
            if(' ' == board[y*3 + x]){
                board[y*3 + x] = turn;
                if('O'==turn){
                    turn = 'X';
                } else {
                    turn = 'O';
                }
            } else {
                printf("Location is already taken.\n");
            }
        }

        auto w = CheckWinner(board);
        if( 0!=w ) {
            PrintBoard(board);
            printf( "%c wins!\n", w );
            break;
        }

        if( true == BoardFilled(board) ) {
            PrintBoard(board);
            printf("Draw.\n");
            break;
        }
    }

    return 0;
}