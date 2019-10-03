#include <stdio.h>
#include <ctype.h>

void RemoveNewLine( char s[] ){
    for( int i=0; 0 !=s[i]; ++i ) {
        if('\n' == s[i] ){
            s[i] = 0;
            break;
        }
    }
}

int MakeLowerCaseAndGetLength( char s[] ){
    int length = 0;
    for( int i=0; 0 != s[i]; ++i ) {
        s[i] = tolower(s[i]);
        ++length;
    }
    return length;
}

int main( void )
{
    char fName[256];
    char word[256];
    printf("Enter File Name> ");
    fgets(fName, 255, stdin);
    printf("Enter keyword> ");
    fgets(word, 255, stdin);
    RemoveNewLine(fName);
    RemoveNewLine(word);

    int word_count = 0;
    int line_count = 1;
    int word_pos = 0;
    int word_length = MakeLowerCaseAndGetLength(word);

    FILE *fp = fopen(fName, "r");
    if(nullptr != fp) {
        char line[256];
        while(nullptr != fgets(line, 255, fp)) {
            for(int i = 0; line[i] != 0; ++i) {
                char current_letter = word[word_pos];
                if(line[i] == current_letter || line[i] == toupper(current_letter)){
                    //printf("%c = %c\n", line[i], current_letter);
                    ++word_pos;
                } else {
                    word_pos = 0;

                }
                if(word_pos == word_length){
                    ++word_count;
                    printf("%d: %s", line_count, line);
                    word_pos = 0;
                }
            }
            ++line_count;
        }
        
    } else {
        printf("Cannot open file.\n");
        return 1;
    }

    printf("Appeared %d times.", word_count);

    return 0;
}