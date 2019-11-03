#include <stdio.h>
#include <time.h>

#include "fssimplewindow.h"
#include "ysglfontdata.h"


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


class TextInput
{
public:
	TextString output;
	int Input(const char msg[]);
};

int TextInput::Input(const char msg[])
{
	output="";

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ENTER==key)
		{
			return key;
		}
		if(FSKEY_ESC==key)
		{
			output="";
			return key;
		}
		if(FSKEY_BS==key)
		{
			output.pop_back();
		}

		auto c=FsInkeyChar();
		if(' '<=c)
		{
			output.push_back(c);
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3ub(0,0,255);
		glRasterPos2i(40,40);
		YsGlDrawFontBitmap20x32(msg);

		TextString s2=output;
		if(0!=time(nullptr)%2)
		{
			s2.push_back('|');
		}
		else
		{
			s2.push_back('_');
		}

		glRasterPos2i(40,72);
		YsGlDrawFontBitmap20x32(s2.c_str());

		FsSwapBuffers();

		FsSleep(10);
	}
	return 0;
}



int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	TextInput input;
	if(FSKEY_ENTER==input.Input("Enter Text"))
	{
		printf("User entered: %s\n",input.output.c_str());
	}
	else
	{
		printf("User cancelled.\n");
	}

	return 0;
}
