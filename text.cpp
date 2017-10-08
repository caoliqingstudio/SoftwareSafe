#include "stdafx.h"
#include "text.h"

using namespace std;
TEXT::TEXT()
{
}

TEXT::~TEXT()
{
}

double TEXT::similar(char * filename1, char * filename2)
{
    FILE *file1, *file2;
    char text1[TEXT_LENGTH], text2[TEXT_LENGTH];
    file1 = fopen(filename1, "r");
    if (!file1) {
        cout << filename1 << " fail to open" << endl;
        return -1;
    }
    file2 = fopen(filename2, "r");
    if (!file2) {
        cout << filename2 << " fail to open" << endl;
        return  -1;
    }
    char c;
    c = fgetc(file1);
    int length1 = 0;
    int length2 = 0;
    while (!feof(file1)) {
        if (c != '\n'&&c != ' '&&c != '\t') {
            text1[length1++] = c;
        }
        c = fgetc(file1);
    }
    text1[length1++] = '\0';
    c = fgetc(file2);
    while (!feof(file2)) {
        if (c != '\n'&&c != ' '&&c != '\t') {
            text2[length2++] = c;
        }
        c = fgetc(file2);
    }
    text2[length2++] = '\0';
    int result = edit(text1, text2);
    int x = (length1 > length2) ? length1 : length2;
    fclose(file1);
    fclose(file2);
    return (1.0-((double)result) / ((double)x));
}

double TEXT::similars(char * file, char * filename)
{
    double result = 0;
//    filename = (char *)malloc(100);
    for (int i = 0; i < 100; i++)
    {
        char filename1[100] = "test_softsafe//";
        char filename2[10];
        char *name = itoa(i, filename2, 10);
        strcat(filename1, filename2);
        strcat(filename1, ".c");
        double thisresult=similar(file, filename1);
        if (result < thisresult) {
            result = thisresult;
            strcpy(filename, filename1);
        }
    }
    return result;
}

int TEXT::edit(const char *str1, const char* str2) {
    int min_distance = 0;
    if (str1 == NULL || str2 == NULL)
        return 0;
    int len1 = strlen(str1) + 1;
    int len2 = strlen(str2) + 1;
    int **distance = (int **)malloc(sizeof(int *) * len1);
    for (int i = 0; i < len1; i++) {
        distance[i] = (int *)malloc(sizeof(int) * len2);
    }
    for (int i = 0; i< len1; i++) {
        distance[i][0] = i;
    }

    for (int i = 0; i < len2; i++) {
        distance[0][i] = i;
    }
    for (int i = 1; i < len1; i++) {
        for (int j = 1; j < len2; j++) {
            int tmp = min(distance[i - 1][j] + 1, distance[i][j - 1] + 1);
            int d;
            if (str1[i - 1] == str2[j - 1]) {
                d = 0;
            }
            else {
                d = 2;
            }
            distance[i][j] = min(tmp, distance[i - 1][j - 1] + d);
        }
    }
    min_distance = distance[len1 - 1][len2 - 1];
    for (int i = 0; i < len1; i++) {
        free(distance[i]);
    }
    free(distance);
    return min_distance;
}

CFG::CFG()
{
    char x[32][10] = { "auto", "break", "case", "char", "const", "continue",
        "double", "default", "do", "else", "enum", "extern", "float", "for", "goto",
        "int", "if", "long", "register", "return", "struct", "switch", "short", "signed",
        "sizeof", "static", "typedef", "union", "unsigned", "void", "volatile", "while" };
    for (int i = 0; i < 32; ++i) {
        strcpy(key[i], x[i]);
    }
    fileCFG_1=true;
    fileCFG_2=true;
    hash1 = NULL;
    hash2 = NULL;
}

CFG::~CFG()
{
    FUN_t tempf = hash2;
    while (hash2) {
        FUN_C_t tempc = hash2->child;
        while (tempc) {
            FUN_C_t tempc2 = tempc->next;
            free(tempc);
            tempc = tempc2;
        }
        tempf = hash2->next;
        free(hash2);
        hash2 = tempf;
    }
    tempf = hash1;
    while (hash1) {
        FUN_C_t tempc = hash1->child;
        while (tempc) {
            FUN_C_t tempc1 = tempc->next;
            free(tempc);
            tempc = tempc1;
        }
        tempf = hash1->next;
        free(hash1);
        hash1 = tempf;
    }
}

double CFG::nextsimilar(char * filename2, int file2type)
{
    FILE *file2;
    FUN_t hashnext=NULL;
    file2 = fopen(filename2, "r");
    if (!file2) {
        cout << filename2 << " fail to open" << endl;
        return  -1;
    }
    switch (file2type)
    {
    case 1:
    case 2:
    case 3:
        treeCreate(file2, hashnext); break;
    case 4:
        treeCreatePY(file2, hashnext); break;
    case 5:treeCreatePHP(file2, hashnext); break;
    default:
        break;
    }
    fclose(file2);
    char aimFileName1[100] = "test_softsafe_CFG\\", aimFileName2[100] = "test_softsafe_CFG\\";
    int k = 0;
    for (unsigned int i = 0; i < strlen(thefilename); i++)
    {
        if (thefilename[i] == '\\' || thefilename[i] == '/') {
            k = i;
        }
    }
    if (k > 0)k++;
    strcat(aimFileName1, thefilename + k);
    aimfile1 = fopen(aimFileName1, "w");
    k = 0;
    for (unsigned int i = 0; i < strlen(filename2); i++)
    {
        if (filename2[i] == '\\' || filename2[i] == '/') {
            k = i;
        }
    }
    if (k > 0)k++;
    strcat(aimFileName2, filename2 + k);
    aimfile2 = fopen(aimFileName2, "w");
//    if (!aimfile1 && !aimfile2) {
//        cout << aimFileName1 << " " << aimFileName2 << " fail to open" << endl;
//        return  -1;
//    }
    if(!aimfile1){
        fileCFG_1=false;
    }
    if(!aimfile2){
        fileCFG_2=false;
    }
    double result = treecmp(hash1, hashnext);
    FUN_t tempf = hashnext;
    while (hashnext) {
        FUN_C_t tempc = hashnext->child;
        while (tempc) {
            FUN_C_t tempc2 = tempc->next;
            free(tempc);
            tempc = tempc2;
        }
        tempf = hashnext->next;
        free(hashnext);
        hashnext = tempf;
    }
    return	result;
}

double CFG::similars(char * file, int type, char * filename)
{
    double result = 0;
//    filename = (char *)malloc(100);
    for (int i = 0; i < 100; i++)
    {
        char filename1[100] = "test_softsafe//";
        char filename2[10];
        char *name = itoa(i, filename2, 10);
        strcat(filename1, filename2);
        strcat(filename1, ".c");
        double thisresult;
        if (i == 0) {
            thisresult = similar(file, type, filename1, 1);
        }
        else {
            thisresult = nextsimilar(filename1, 1);
        }
        if (result < thisresult) {
            result = thisresult;
            strcpy(filename, filename1);
        }
    }
    return result;
}

double CFG::similar(char * filename1,int file1type, char * filename2,int file2type)
{
    FILE *file1, *file2;
    //FUN_t hash1 = NULL;
    strcpy(thefilename, filename1);
    file1 = fopen(filename1, "r");
    if (!file1) {
        cout << filename1 << " fail to open" << endl;
        return -1;
    }
    file2 = fopen(filename2, "r");
    if (!file2) {
        cout << filename2 << " fail to open" << endl;
        return  -1;
    }
    char aimFileName1[100]="test_softsafe_CFG\\", aimFileName2[100] = "test_softsafe_CFG\\";
    int k = 0;
    for (unsigned int i = 0;i < strlen(filename1); i++)
    {
        if (filename1[i] == '\\' || filename1[i] == '/') {
            k = i;
        }
    }
    if (k > 0)k++;
    strcat(aimFileName1, filename1+k);
    aimfile1 = fopen(aimFileName1, "w");
    k = 0;
    for (unsigned int i = 0; i < strlen(filename2); i++)
    {
        if (filename2[i] == '\\' || filename2[i] == '/') {
            k = i;
        }
    }
    if (k > 0)k++;
    strcat(aimFileName2, filename2 + k);
    aimfile2 = fopen(aimFileName2, "w");
//    if (!aimfile1&&!aimfile2) {
//        cout << aimFileName1 <<" "<<aimFileName2 <<" fail to open" << endl;
//        return  -1;
//    }
    if(!aimfile1){
        fileCFG_1=false;
    }
    if(!aimfile2){
        fileCFG_2=false;
    }
    switch (file1type)
    {
    case 1:
    case 2:
    case 3:
        treeCreate(file1, hash1); break;
    case 4:
        treeCreatePY(file1, hash1); break;
    case 5:treeCreatePHP(file2, hash2); break;
    default:
        break;
    }
    switch (file2type)
    {
    case 1:
    case 2:
    case 3:
        treeCreate(file2, hash2); break;
    case 4:
        treeCreatePY(file2, hash2); break;
    case 5:treeCreatePHP(file2, hash2); break;
    default:
        break;
    }
    fclose(file1);
    fclose(file2);
    double result = treecmp(hash1, hash2);
    return	result;
}

bool CFG::treeCreate(FILE * file,FUN_t &hash)
{
    char str[10000];
    int i = 0;
    char lun[100];
    int lun_i = 1;
    lun[0] = '\n';
    while (!feof(file)) {
        lun[(lun_i)%100] = fgetc(file);
        if (lun[(lun_i) % 100] == '\"' || lun[(lun_i) % 100] == '\'') {
            if (lun[(lun_i-1) % 100] != '\\') {
                char c = fgetc(file);
                char b = lun[(lun_i) % 100];
                while ((c == '\'' || lun[(lun_i) % 100] == '\"') && b == '\\') {
                    b = c;
                    c = fgetc(file);
                }
            }
        }//去 字符串字面
        if (lun[(lun_i) % 100] == '{') {
            int j = lun_i - 1;
            while (lun[(j) % 100] == ' ' || lun[(j) % 100] == '\t'||lun[(j) % 100] == '\n') j--;
            if (lun[(j) % 100] == ')') {
                    while (lun[(j) % 100] != '(') j--;
                    j--;
                    while (lun[(j) % 100] == ' ' || lun[(j) % 100] == '\t') j--;
                    lun[(j + 1) % 100] = '\0';
                    while ((WORDSTRUC(lun[j%100]) || NUMBSTRUC(lun[j%100]))) j--;
                    char name[50];
                    //strcpy(name, &lun[(j + 1) % 100]);//获得函数名字

                    int textnum = 0;//copy name
                    for (int sti = j+1; lun[sti % 100] != '\0'; sti++,textnum++) { name[textnum] = lun[(sti) % 100]; }
                    name[textnum] = '\0';

                    FUN_t thisLocal = insertFunc(name,hash);
                    int x = 1;
                    int i = 0;
                    while (x > 0) {
                        str[i] = fgetc(file);
                        if (str[i] == '\"' || str[i] == '\'') {
                            if (str[i - 1] != '\\') {
                                char c = fgetc(file);
                                char b = str[i];
                                while ((c == '\'' || str[i] == '\"') && b == '\\') {
                                    b = c;
                                    c = fgetc(file);
                                }
                            }
                        }//去 字符串字面
                        if (str[i] == '{') x++;
                        if (str[i] == '}') x--;
                        i++;
                    }
                    str[i - 1] = '\0';
                    func(str,thisLocal);
                }
            }
        lun_i++;
    }
    return true;
}

bool CFG::keyword(char * x)
{
    for (int i = 0; i < 32; i++)
    {
        if (!strcmp(x, key[i])) return true;
    }
    return false;
}

int CFG::func(char * x, FUN_t thislocal)
{
    int i = 0;
    int length = strlen(x);
    while (i<length) {
        if (x[i] == '(') {
            int xi = i-1;
            while (xi >= 0 && (x[xi] == ' ' || x[xi] == '\t')) xi--;
            char token[50];
            x[xi + 1] = '\0';
            while (xi >= 0 && (WORDSTRUC(x[xi]) || NUMBSTRUC(x[xi]))) xi--;
            strcpy(token, &x[xi + 1]);
            if (!keyword(token)) {
                FUN_C_t child;
                if (thislocal->child) {
                    child = thislocal->child;
                    while (child->next) child = child->next;
                    child->next = (FUN_C_t)malloc(sizeof(FUN_C));
                    child = child->next;
                }
                else {
                    thislocal->child = (FUN_C_t)malloc(sizeof(FUN_C));
                    child = thislocal->child;
                }
                (thislocal->num)++;
                strcpy(child->name, token);
                child->next = NULL;
                child->child = NULL;
            }
        }
        i++;
    }
    return 0;
}

/*
int CFG::func(char * x, FUN_t thislocal)
{
    int i(0);
    int xi = 0;
    int length = strlen(x);
    char str[200];
    while (xi<length) {
        str[i] = x[xi];
        if (str[i] == '\n') {
            char token[50][50];
            int j = 0;
            int k = 0;
            int m = 0;
            j = NextText(j, str);
            while (j < i) {
                if (WORDSTRUC(str[j]) || NUMBSTRUC(str[j])) {
                    token[k][m++] = str[j];
                    while (j<i && (WORDSTRUC(str[j]) || NUMBSTRUC(str[j]))) {
                        token[k][m++] = str[j];
                    }
                    token[k][m] = '\0';
                    k++; m = 0;
                }
                else if (str[j] == ' ' || str[j] == '\t') {
                    ;
                }
                else {
                    token[k][m++] = str[j];
                    token[k][m] = '/0';
                    k++; m = 0;
                }
                j++;
            }
            for (int z = 0; z < k; z++)
            {
                bool thisstate = false;
                if (!strcmp(token[z], "(")) {
                    int zc = z;
                    if (zc > 0 && WORDSTRUC(token[zc - 1][0])) {
                        if (zc < 2 || keyword(token[zc - 2])) {
                            while (zc < k) {
                                if (!strcmp(token[zc], ")")) {
                                    if (!strcmp(token[zc + 1], ";")) {
                                        thisstate = false;
                                    }
                                    else if (!strcmp(token[zc + 1], "{"))
                                    {

                                    }
                                    break;
                                }
                                zc++;
                            }
                        }
                    }
                }
            }
            i = -1;
        }
        i++;
        xi++;
    }
    return 0;
}
*/
FUN_t CFG::insertFunc(char * name,FUN_t &hash)
{
    FUN_t x = hash;
    if (x) {
        while (x->next)
        {
            x = x->next;
        }
        x->next = (FUN_t)malloc(sizeof(FUN));
        x = x->next;
    }
    else {
        hash = (FUN_t)malloc(sizeof(FUN));
        x = hash;
    }
    strcpy(x->name, name);
    x->child = NULL;
    x->next = NULL;
    x->state = false;
    x->num = 0;
    return x;
}

double CFG::treecmp(FUN_t hash1, FUN_t hash2)
{
    double score1 = 0;
    double score2 = 0;
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    FUN_t temp = hash1;
    while (temp)
    {
        if (!strcmp(temp->name, "main"))break;
        temp = temp->next;
    }
    int num1 = 0; int num2 = 0;
    treeread(temp, x1, y1, num1, 0,hash1,aimfile1,fileCFG_1);
    temp = hash2;
    while (temp)
    {
        if (!strcmp(temp->name, "main"))break;
        temp = temp->next;
    }
    treeread(temp, x2, y2, num2, 0,hash2,aimfile2,fileCFG_2);
    score1 = (((double)x1) / ((double)num1));
    score2 = (((double)x2) / ((double)num2));
    double xdiff = (score1 - score2)*(score1 - score2);
    double score1y, score2y;
    score1y = (((double)y1) / ((double)num1));
    score2y = (((double)y2) / ((double)num2));
    double ydiff = (score1y - score2y)*(score1y - score2y);
    double result = xdiff/((score1>score2?score1:score2)*(score1>score2 ? score1 : score2))
        + ydiff / ((score1y>score2y ? score1y : score2y)*(score1y>score2y ? score1y : score2y));
    double endresult = 1 - sqrt(result);
    fclose(aimfile1);
    fclose(aimfile2);
    return endresult<0?0:endresult;
}

int CFG::treeread(FUN_t next, int & x, int & y, int & num, int level,FUN_t hash,FILE *file,bool fileState)
{
    x += (next->num);
    y += level;
    num++;
    if(fileState) fprintf(file, "%d", level);
    //cout << level;
    for (int  i = 0; i < level; i++)
    {
        if(fileState) fputc('\t', file);
        //cout << '\t';
    }
    if(fileState) fprintf(file, "%s\n", next->name);
    //cout << next->name<<"\n";//print
    FUN_C_t child = next->child;
    if (child) {
        while (child)
        {
            FUN_t temp;
            bool state=true;
            temp = hash;
            while (temp) {
                if (!strcmp(temp->name, child->name)) {
                    treeread(temp, x, y, num, level + 1, hash,file,fileState);
                    state = false;
                    break;
                }
                temp = temp->next;
            }
            if (state) {
                if(fileState) fprintf(file, "%d", level + 1);
                //cout << level + 1;
                for (int i = 0; i < level + 1; i++)
                {
                    if(fileState) fputc('\t', file);
                    //cout << '\t';
                }
                if(fileState) fprintf(file, "%s\n", child->name);
                //cout << child->name << "\n";//print
                x += (1);
                num++;
                y += (level + 1);
            }
            child = child->next;
        }
    }
    return 0;
}

bool CFG::treeCreatePY(FILE * file, FUN_t & hash)
{
    char str[10000];
    int temp_tree;
    int i = 0;
    char lun[10000];
    int lun_i = 1;
    lun[0] = '\n';
    while (!feof(file)) {
        lun[lun_i] = fgetc(file);
        if (lun[lun_i] == '\"' || lun[lun_i] == '\'') {
            if (lun[lun_i - 1] != '\\') {
                char c = fgetc(file);
                char b = lun[lun_i];
                while ((c == '\'' || lun[lun_i] == '\"') && b == '\\') {
                    b = c;
                    c = fgetc(file);
                }
            }
        }//去 字符串字面
        if (lun[lun_i] == ':') {
            int j = lun_i - 1;
            while (lun[j] == ' ' || lun[j] == '\t' || lun[j] == '\n') j--;
            if (lun[(j)] == ')') {
                while (lun[(j)] != '(') j--;
                j--;
                while (lun[(j) ] == ' ' || lun[(j)] == '\t') j--;
                int nulltest = j + 1;//用来记录存放函数名字的放0的位置//lun[(j + 1) % 100] = '\0';
                while ((WORDSTRUC(lun[j]) || NUMBSTRUC(lun[j]))) j--;
                int starttest = j + 1;//用来存放函数名的初始位置
                while (lun[(j)] == ' ' || lun[j] == '\t') j--;
                if (lun[(j)] == 'f'&&lun[(j - 1)] == 'e'&&lun[(j - 2)] == 'd') {//是函数体
                    temp_tree = j - 2;
                    char name[50];
                    lun[(nulltest)] = '\12';
                    //获得函数名字
                    int textnum = 0;//copy name
                    for (int sti = starttest; lun[sti] != '\12'; sti++, textnum++) { name[textnum] = lun[(sti)]; }
                    name[textnum] = '\0';

                    FUN_t thisLocal = insertFunc(name, hash);
                    int i = 0;
                    while ((str[i] = fgetc(file)) != '\n');
                    while ((str[i] = fgetc(file)) == ' ' || str[i] == '\t') i++;
                    int stateNULL = i;
                    i++;
                    while (1) {
                        while ((str[i] = fgetc(file)) != '\n') i++;
                        int start = i;
                        while (!feof(file)&&(str[i] = fgetc(file)) == ' ' || str[i] == '\t') i++;
                        if ((i - start) < stateNULL) break;
                        if (feof(file)) break;
                    }
                    str[i - 1] = '\0';
                    lun[temp_tree] = str[i];
                    func(str, thisLocal);
                    lun_i = temp_tree;
                }
            }
        }
        lun_i++;
    }
    lun[lun_i] = '\0';
    FUN_t treeLocal = insertFunc((char *)"main", hash);
    func(lun, treeLocal);
    return true;
}

bool CFG::treeCreatePHP(FILE * file, FUN_t & hash)
{
    char str[10000];
    int temp_tree;
    int i = 0;
    char lun[10000];
    int lun_i = 2;
    lun[0] = '\n';
    lun[1] = '\n';
    lun[2] = fgetc(file);
    lun[3] = fgetc(file);
    lun[4] = fgetc(file);
    lun[5] = fgetc(file);
    lun[6] = fgetc(file);
    int j = 6;
    while (!(lun[j - 4] == '<'&&lun[j - 3] == '?'&&lun[j - 2] == 'p'
        &&lun[j - 1] == 'h'&&lun[j] == 'p')&&!feof(file)) lun[j++] = fgetc(file);
    while (!feof(file)&&!(lun[lun_i-2]=='?'&&lun[lun_i-1] == '>')) {
        lun[lun_i] = fgetc(file);
        if (lun[lun_i] == '\"' || lun[lun_i] == '\'') {
            if (lun[lun_i - 1] != '\\') {
                char c = fgetc(file);
                char b = lun[lun_i];
                while ((c == '\'' || lun[lun_i] == '\"') && b == '\\') {
                    b = c;
                    c = fgetc(file);
                }
            }
        }//去 字符串字面
        if (lun[lun_i] == ':') {
            int j = lun_i - 1;
            while (lun[j] == ' ' || lun[j] == '\t' || lun[j] == '\n') j--;
            if (lun[(j)] == ')') {
                while (lun[(j)] != '(') j--;
                j--;
                while (lun[(j)] == ' ' || lun[(j)] == '\t') j--;
                int nulltest = j + 1;//用来记录存放函数名字的放0的位置//lun[(j + 1) % 100] = '\0';
                while ((WORDSTRUC(lun[j]) || NUMBSTRUC(lun[j]))) j--;
                int starttest = j + 1;//用来存放函数名的初始位置
                while (lun[(j)] == ' ' || lun[j] == '\t') j--;
                if (lun[(j)] == 'f'&&lun[(j - 1)] == 'e'&&lun[(j - 2)] == 'd') {//是函数体
                    temp_tree = j - 2;
                    char name[50];
                    lun[(nulltest)] = '\12';
                    //获得函数名字
                    int textnum = 0;//copy name
                    for (int sti = starttest; lun[sti] != '\12'; sti++, textnum++) { name[textnum] = lun[(sti)]; }
                    name[textnum] = '\0';

                    FUN_t thisLocal = insertFunc(name, hash);
                    int i = 0;
                    while ((str[i] = fgetc(file)) != '\n');
                    while ((str[i] = fgetc(file)) == ' ' || str[i] == '\t') i++;
                    int stateNULL = i;
                    i++;
                    while (1) {
                        while ((str[i] = fgetc(file)) != '\n') i++;
                        int start = i;
                        while (!feof(file) && (str[i] = fgetc(file)) == ' ' || str[i] == '\t') i++;
                        if ((i - start) < stateNULL) break;
                        if (feof(file)) break;
                    }
                    str[i - 1] = '\0';
                    lun[temp_tree] = str[i];
                    func(str, thisLocal);
                    lun_i = temp_tree;
                }
            }
        }
        lun_i++;
    }
    lun[lun_i] = '\0';
    FUN_t treeLocal = insertFunc((char *)"main", hash);
    func(lun, treeLocal);
    return true;
}


