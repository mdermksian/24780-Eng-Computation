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

char *TextBase::GetPointer()
{
	return str;
}
const char *TextBase::GetPointer() const
{
	return str;
}

TextBase::TextBase()
{
	str=nullptr;
}
TextBase::~TextBase()
{
	clear();
}
void TextBase::clear(void)
{
	if(nullptr!=str)
	{
		delete [] str;
		str=nullptr;
	}
}
void TextBase::resize(long long int newLen)
{
	char *newStr=new char [newLen];
	if(nullptr!=str)
	{
		for(int i=0; i<newLen && 0!=str[i]; ++i)
		{
			newStr[i]=str[i];
		}
		delete [] str;
	}
	str=newStr;
}


class TextString : public TextBase
{
public:
	void LoopHole(void) const;

	TextString();
	TextString(const char incoming[]);
	TextString(const TextString &incoming);
	TextString &operator=(const TextString &incoming);
	~TextString();

	const char *fgets(FILE *fp);

	void set(const char str[]);
	void push_back(char c);
	void pop_back(void);
	const char *c_str(void) const;
	long long int size(void) const;
};

void TextString::LoopHole(void) const
{
//	auto s=GetPointer();
//	s[0]=100;
}

TextString::TextString()
{
}
TextString::TextString(const char incoming[])
{
	set(incoming);
}
TextString::TextString(const TextString &incoming)
{
	set(incoming.c_str());
}
TextString &TextString::operator=(const TextString &incoming)
{
	set(incoming.c_str());
	return *this;
}

TextString::~TextString()
{
	clear();
}

const char *TextString::fgets(FILE *fp)
{
	clear();

	char buf[16];
	while(nullptr!=::fgets(buf,15,fp))
	{
		for(int i=0; 0!=buf[i]; ++i)
		{
			if(buf[i]=='\n')
			{
				return c_str();
			}
			this->push_back(buf[i]);
		}
	}
	if(0<size())
	{
		return GetPointer();
	}
	return nullptr;
}

void TextString::set(const char incoming[])
{
	if(incoming!=GetPointer())
	{
		clear();
		if(nullptr!=incoming)
		{
			int n=0;
			for(n=0; 0!=incoming[n]; ++n)
			{
			}

			resize(n+1);
			auto s=GetPointer();
			for(int i=0; i<n; ++i)
			{
				s[i]=incoming[i];
			}
			s[n]=0;
		}
	}
}

void TextString::push_back(char c)
{
	auto l=size();
	auto newL=l+1;
	auto ptr=c_str();

	resize(newL+1);
	auto s=GetPointer();
	s[l]=c;
	s[newL]=0;
}

void TextString::pop_back(void)
{
	auto l=size();
	if(0<l)
	{
		auto newL=l-1;
		resize(newL+1);
		auto s=GetPointer();
		s[newL]=0;
	}
}

const char *TextString::c_str(void) const
{
	if(nullptr!=GetPointer())
	{
		return GetPointer();
	}
	else
	{
		return "";
	}
}

long long int TextString::size(void) const
{
	auto ptr=c_str();
	long long int n=0;
	for(n=0; 0!=ptr[n]; ++n)
	{
	}
	return n;
}


int main(void)
{
	TextString s;

	s.push_back('A');
	s.push_back('B');
	s.push_back('C');
	printf("%s\n",s.c_str());

	s.pop_back();
	printf("%s\n",s.c_str());

	s.pop_back();
	printf("%s\n",s.c_str());


	TextString s2("incoming");
	TextString s3(s);
	TextString s4;

	s4=s;


	printf(">");
	TextString fName;
	fName.fgets(stdin);
	printf(" Entere: %s\n",fName.c_str());



	return 0;
}
