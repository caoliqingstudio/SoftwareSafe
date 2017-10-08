#pragma once
#include "stdafx.h"
#include <string>

#define TEXT_LENGTH 10000
#define min(x,y) (((x)>(y))?(y):(x))

#define WORDSTRUC(c) ((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_')
#define NUMBSTRUC(c) (c>='0'&&c<='9')

typedef struct funchild {
	char name[50];
	struct funchild *next;
	struct function *child;
}FUN_C, *FUN_C_t;
typedef struct  function {
	char name[50];
	bool state;
	int num;
	FUN_C_t child;
	struct function *next;
}FUN, *FUN_t;

class  TEXT
{
public:
	 TEXT();
    ~TEXT();
	double similar(char *filename1,char *filename2);
    double similars(char *file, char *filename);
private:
	int edit(const char *str1, const char* str2);
};

class CFG {
public:
	CFG();
	~CFG();
	FUN_t hash1, hash2;
	double nextsimilar(char *filename2, int file2type);
    double similars(char *file,int type, char *filename);
	// filetype   1: C  2 JAVA  3 C++ 4 python
	double similar(char *filename1,int file1type, char *filename2,int file2type);
protected:
	FILE *aimfile1, *aimfile2;
	char thefilename[100];
	bool treeCreate(FILE *file,FUN_t &hash);
	inline int NextText(int j, char *str) {
		while (str[j] == ' '||str[j] == '\t')j++;
		return j;
	}
	bool keyword(char *x);
	char key[32][10];
	int func(char *str, FUN_t thislocal);
	FUN_t insertFunc(char *name,FUN_t &hash);
	double treecmp(FUN_t hash1, FUN_t hash2);
    int treeread(FUN_t next, int &x, int &y, int&num, int level,FUN_t hash,FILE *file,bool fileState);
	bool treeCreatePY(FILE *file, FUN_t &hash);
	bool treeCreatePHP(FILE *file, FUN_t &hash);
    bool fileCFG_1,fileCFG_2;
};



