#pragma once

#define FUNC 15

#define UCHAR 2
#define CHAR 3
#define USINT 4
#define SINT 5
#define UINT 6
#define INT 7
#define ULINT 8
#define LINT 9
#define ULLINT 10
#define LLINT 11
#define FLOAT 22
#define DOUBLE 24

#define OTHER 13

#define HEAP 1
#define STACK 2
#define IN_OPEA 3
#define IN_SYM 4
#define IN_LEN 5

#define WORD(c) ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_')
#define NUMB(c) (c>='0'&&c<='9')


typedef struct token{
	char name[50];
	char rank;
	int num;
	struct token *next;
	int type;
	bool point;
	int pointtype;
}TK,*TK_t;
typedef struct thebug {
	char name[50];//function name
	int line;//ÐÐÊý
	int bugtype;
	char rank;
	struct thebug * next;
}BG,*BG_t;

class BUG
{
public:
	BUG();
	~BUG();
	BG bugs;
	TK tokens;
	BG_t find(char * filename);
private:
	int line;
	void textAn(char *text,int length);
	struct keyword
	{
		char name[20];
		int token;
		char argv[50];
	}key[20];
	TK_t temp_tokens;
	BG_t temp_bugs;
	char opera[23][10];
	bool strcmp_typekey(char *text, int &length, int &type);
	void lineNum(char* const text, int line, int length);
	bool instertBu(char*name, int line, int bugType,int rank);
	void lineAn(char*text, int line,int length);
	void lineFindFun(char*text, int line, int length);
};

