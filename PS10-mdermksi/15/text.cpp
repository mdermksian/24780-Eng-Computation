#include <stdio.h>

class TextString
{
protected:
	char *str;

public:
	TextString();
	TextString(const char incoming[]);
	TextString(const TextString &incoming);
	TextString &operator=(const TextString &incoming);
	~TextString();
	void clear(void);

	const char *fgets(FILE *fp);

	void set(const char str[]);
	void push_back(char c);
	void pop_back(void);
	const char *c_str(void) const;
	long long int size(void) const;
};

TextString::TextString()
{
	str=nullptr;
}
TextString::TextString(const char incoming[])
{
	str=nullptr;
	set(incoming);
}
TextString::TextString(const TextString &incoming)
{
	str=nullptr;
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
void TextString::clear(void)
{
	if(nullptr!=str)
	{
		delete [] str;
	}
	str=nullptr;
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
		return str;
	}
	return nullptr;
}

void TextString::set(const char incoming[])
{
	if(incoming!=str)
	{
		clear();
		if(nullptr!=incoming)
		{
			int n=0;
			for(n=0; 0!=incoming[n]; ++n)
			{
			}

			auto *newStr=new char [n+1];
			for(int i=0; i<n; ++i)
			{
				newStr[i]=incoming[i];
			}
			newStr[n]=0;

			str=newStr;
		}
	}
}

void TextString::push_back(char c)
{
	auto l=size();
	auto newL=l+1;
	auto ptr=c_str();

	auto newStr=new char [newL+1];
	for(int i=0; i<l; ++i)
	{
		newStr[i]=ptr[i];
	}
	newStr[l]=c;
	newStr[newL]=0;

	clear();

	str=newStr;
}

void TextString::pop_back(void)
{
	auto l=size();
	if(0<l)
	{
		auto newL=l-1;
		auto newStr=new char [newL+1];

		for(int i=0; i<newL; ++i)
		{
			newStr[i]=str[i];
		}
		newStr[newL]=0;

		clear();

		str=newStr;
	}
}

const char *TextString::c_str(void) const
{
	if(nullptr!=str)
	{
		return str;
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
