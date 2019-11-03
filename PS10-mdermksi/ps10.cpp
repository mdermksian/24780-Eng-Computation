#include <stdio.h>

class TextBase
{
private:
    char *str;
protected:
    char *GetPointer();
    const char *GetPointer() const;
    void resize(long long int newLen);
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

class BowlScore : public TextBase {
private:
    int score;
public:
    BowlScore();
    ~BowlScore();

    const char *getScore(FILE *fp);

    void set(const char str[]);
    void push_back(char c);
    void pop_back(void);
    const char *c_str(void) const;
    long long int size(void) const;
};

BowlScore::BowlScore(){
}
BowlScore::~BowlScore(){
    clear();
}

const char *BowlScore::getScore(FILE *fp){
    clear();

    char buf[22];
    while (nullptr!=::fgets(buf,21,fp))
    {
        
    }
    
}

int main(void) {

}