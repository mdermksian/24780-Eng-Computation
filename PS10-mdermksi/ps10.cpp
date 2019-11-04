#include <stdio.h>
#include <stdlib.h>

class TextBase
{
private:
    char *str;
protected:
    char *GetPointer();
    const char *GetPointer() const;
    const char *c_str() const;
    void resize(long long int newLen);
    long long int size(void) const;
    void push_back(char c);
public:
    TextBase();
    ~TextBase();
    void clear(void);
};

char *TextBase::GetPointer(){
    return str;
}

const char *TextBase::GetPointer() const {
    return str;
}

const char *TextBase::c_str() const {
    if(nullptr!=GetPointer())
	{
		return GetPointer();
	}
	else
	{
		return "";
	}
}

TextBase::TextBase(){
    str = nullptr;
}

TextBase::~TextBase(){
    clear();
}

void TextBase::clear(void) {
    if(nullptr != str){
        delete [] str;
        str = nullptr;
    }
}

void TextBase::resize(long long int newLen) {
    char *newStr = new char [newLen];
    if(nullptr!=str){
        for(int i = 0; i<newLen && 0!=str[i]; ++i){
            newStr[i] = str[i];
        }
        delete [] str;
    }
    str = newStr;
}

long long int TextBase::size(void) const{
    long long int l = 0;
    auto *ptr = c_str();
    for(l=0; ptr[l]!=0; ++l){}
    return l;
}

void TextBase::push_back(char c){
    long long int l = size();
    long long int newL = l+1;
    resize(newL+1);
    auto s=GetPointer();
    s[l] = c;
    s[newL] = 0;
}

class BowlScore : public TextBase {
private:
    int score;
public:
    BowlScore();
    ~BowlScore();

    bool inputGameString(FILE *fp);
    bool isLegalChar(char c);
    int scoreFromFrame(int pinsKnocked[21], int frameNo);
    void calculateScore(void);

    const char *getGameString(void);
    const int getScore(void);
    void setScore(int num);
};

BowlScore::BowlScore(){
    score = 0;
}
BowlScore::~BowlScore(){
    clear();
}

bool BowlScore::isLegalChar(char c){
    char legalChars[] = {' ','-','0','1','2','3','4','5','6','7','8','9','X','/'};
    for(char legal : legalChars){
        if(c == legal){
            return true;
        }
    }
    return false;
}

bool BowlScore::inputGameString(FILE *fp){
    clear();
    printf("Enter Frames> ");
    char buf[23];
    fgets(buf, 22, fp);

    for(int i = 0; 0!=buf[i]; ++i){
        push_back(buf[i]);
        if(i == 20 && buf[i] != '\n'){
            return true;
        }
        if(buf[i] == '\n'){
            printf("Error: Frame string is too short\n");
            clear();
            return false;
        }
        if(!isLegalChar(buf[i])){
            printf("Error: Invalid character in string\n");
            clear();
            return false;
        }
    }
    return false;
}

int BowlScore::scoreFromFrame(int pinsKnocked[21], int frameNo){
    int score;
    if(frameNo < 9) {
        int firstPins = pinsKnocked[frameNo*2];
        int secondPins = pinsKnocked[frameNo*2 + 1];
        int pinsInFrame = firstPins + secondPins;
        if(pinsInFrame < 10) {
            score = pinsInFrame;
        } else if (firstPins == 10){
            if(pinsKnocked[frameNo*2 + 2] == 10 && frameNo < 8){
                score = pinsInFrame + pinsKnocked[frameNo*2 + 2] + pinsKnocked[frameNo*2 + 4];
            } else {
                score = pinsInFrame + pinsKnocked[frameNo*2 + 2] + pinsKnocked[frameNo*2 + 3];
            }
        } else {
            score = pinsInFrame + pinsKnocked[frameNo*2 + 2];
        }
    } else {
        int firstPins = pinsKnocked[frameNo*2];
        int secondPins = pinsKnocked[frameNo*2+1];
        int thirdPins = pinsKnocked[frameNo*2+2];
        score = firstPins + secondPins + thirdPins;
    }
    return score;
}

void BowlScore::calculateScore(void){
    auto gameString = getGameString();
    int pinsKnocked[21];
    for(int i = 0; i <= 21; ++i){
        switch (gameString[i]){
            case '0':
            case '-':
            case ' ':
                pinsKnocked[i] = 0;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                pinsKnocked[i] = gameString[i]-'0';
                break;
            case '/':
                pinsKnocked[i] = 10-pinsKnocked[i-1];
                break;
            case 'X':
                pinsKnocked[i] = 10;
                break;
            default:
                break;
        }
    }
    int curScore = 0;
    for(int i = 0; i<10; ++i){
        curScore += scoreFromFrame(pinsKnocked, i);
    }
    setScore(curScore);
}

const char *BowlScore::getGameString(void){
    return c_str();
}

const int BowlScore::getScore(void){
    return score;
}

void BowlScore::setScore(int num){
    score = num;
}

int main(void) {
    BowlScore bs;

    if(bs.inputGameString(stdin)){
        bs.calculateScore();
        printf("The score is:\n%d\n", bs.getScore());
    }
    return 0;
}