#include <stdio.h>

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

    const char *inputGameString(FILE *fp);
    void CalculateScore(void);

    const char *getGameString(void);
    const int getScore(void);
    void setScore(int score);
};

BowlScore::BowlScore(){
}
BowlScore::~BowlScore(){
    clear();
}

const char *BowlScore::inputGameString(FILE *fp){
    clear();
    printf("Enter Frames> ");
    char buf[22];
    while (nullptr!=fgets(buf,21,fp))
    {
        for(int i = 0; 0!=buf[i]; ++i){
            if(buf[i] == '\n'){
                return c_str();
            }
            push_back(buf[i]);
        }
        if(0<size()){
            return GetPointer();
        }
        return nullptr;
    }
}

const char *BowlScore::getGameString(void){
    return c_str();
}

int main(void) {
    BowlScore bs;

    bs.inputGameString(stdin);

    printf("%s", bs.getGameString());
}