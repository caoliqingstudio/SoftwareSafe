#include "stdafx.h"
#include "BUG.h"

using namespace std;

BUG::BUG()
{
	strcpy(key[0].name, "strcpy"); key[0].token = 1;
	strcpy(key[1].name, "strncpy"); key[1].token = 1;
	strcpy(key[2].name, "memcpy"); key[2].token = 1;
	strcpy(key[3].name, "memncpy"); key[3].token = 1;
	strcpy(key[4].name, "strcat"); key[4].token = 1;
	strcpy(key[5].name, "strncat"); key[5].token = 1;
	strcpy(key[6].name, "sprintf"); key[6].token = -1;
	strcpy(key[7].name, "vsprintf"); key[7].token = -1;
	strcpy(key[8].name, "gets"); key[8].token = 1;
	strcpy(key[9].name, "getchar"); key[9].token = 0;
	strcpy(key[10].name, "fgetc"); key[10].token = 0;
	strcpy(key[11].name, "getc"); key[11].token = 0;
	strcpy(key[12].name, "read"); key[12].token = 2;
	strcpy(key[13].name, "sscanf"); key[13].token = -1;
	strcpy(key[14].name, "fscanf"); key[14].token = -1;
	strcpy(key[15].name, "vfscanf"); key[15].token = -1;
	strcpy(key[16].name, "vscanf"); key[16].token = -1;
	strcpy(key[17].name, "vsscanf"); key[17].token = -1;
	strcpy(key[18].name, "scanf"); key[18].token = -1;
	strcpy(key[19].name, "printf"); key[19].token = -1;
	char theoper[24][8] = { "++","--","%","<=",">=",">","<","==","!=","^","|","+=","-=","*=","/=","&=","^=","|=", "+", "-", "*", "&", "/"};
	for (int i = 0; i < 23; i++)
	{
		strcpy(opera[i], theoper[i]);
	}
	tokens.next=NULL;
	bugs.next = NULL;
	line = 1;
	temp_bugs = &bugs;
	temp_tokens = &tokens;
	bugs.next = NULL;
	tokens.next = NULL;
}


BUG::~BUG()
{
}

BG_t BUG::find(char * filename)
{
	FILE *file;
	file = fopen(filename, "r");
	if (!file) {
		cout << filename << " fail to open" << endl;
		return NULL;
	}
	char *text;
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	rewind(file);
	text = (char *)malloc(length+6);
    char c = fgetc(file);
    length = 0;
    while (!feof(file)) {
        text[length++] = c;
        c = fgetc(file);
    }
	text[0] = '\0';
	text[length + 1] = '\n';
	text[length + 2] = '\0';
	text[length + 3] = 'a';
	text[length + 4] = '\n';
	text[length + 5] = '\0';
	textAn(text+1,length);
	free(text);
	return &bugs;
}

void BUG::textAn(char * text,int length)
{
	int i = 0;
	while (i < length) {
		char *text_this = (&text[i])+1;
		int j = i;
		while (text[i] != '\n'&&i < length) i++;
		text[i] = '\0';
		//printf("%s\n", text_this);
		lineAn(text_this, line, i - j);
		lineFindFun(text_this, line, i - j);
		lineNum(text_this, line, i - j);
		line++;
	}
}

bool BUG::strcmp_typekey(char * text, int &length, int &type)
{
	int i = 0;
	while (text[i] == ' ' || text[i] == '\t') i++;
	char word[100];
	while (text[i] != '\n'&&text[i] != ' '&&text[i] != '\t') {
		word[i] = text[i];
		++i;
	}
	word[i] = 0;
	bool typesym = false;
	if (!strcmp(word, "unsigned")) {
		typesym = true;
		int j = 0;
		while (text[i] == ' ' || text[i] == '\\t') i++;
		while (text[i] != '\n'&&text[i] != ' '&&text[i] != '\t'&&text[i] != '*') {
			word[j] = text[i];
			++i; ++j;
		}
		word[j] = 0;
	}
	else if (!strcmp(word, "signed")) {
		int j = 0;
		while (text[i] == ' ' || text[i] == '\t') i++;
		while (text[i] != '\n'&&text[i] != ' '&&text[i] != '\t'&&text[i] != '*') {
			word[j] = text[i];
			++i; ++j;
		}
		word[j] = 0;
	}
	
	if(!strcmp(word, "char")) {
		type = typesym ? UCHAR : CHAR;
	}
	else if (!strcmp(word, "int")) {
		type = typesym ? UINT : INT;
	}
	else if (!strcmp(word, "long")) {
		int j = 0,k=i;
		while (text[k] == ' ' || text[k] == '\t') k++;
		while (text[k] != '\n'&&text[k] != ' '&&text[k] != '\t'&&text[k] != '*') {
			word[j] = text[k];
			++k; ++j;
		}
		word[j] = 0;
		if (!strcmp(word, "long")) {
			type = typesym ? ULLINT : LLINT;
			i = k;
			int j = 0;
			while (text[k] == ' ' || text[k] == '\t') k++;
			while (text[k] != '\n'&&text[k] != ' '&&text[k] != '\t'&&text[k] != '*') {
				word[j] = text[k];
				++k; ++j;
			}
			word[j] = 0;
		}
		else {
			type = typesym ? ULINT : LINT;
		}
		if (!strcmp(word, "int")) {
			i = k;
		}
	}
	else if (!strcmp(word, "short")) {
		int j = 0, k = i;
		while (text[k] == ' ' || text[k] == '\t') k++;
		while (text[k] != '\n'&&text[k] != ' '&&text[k] != '\t'&&text[k] != '*') {
			word[j] = text[k];
			++k; ++j;
		}
		word[j] = 0;
		if (!strcmp(word, "int")) {
			i = k;
		}
		type = typesym ? USINT : SINT;
	}
	else {
		return false;
	}
	while (text[i] == ' ' || text[i] == '\t') i++;
	if (text[i] == '*') {
		i++;
		type = -1 * type;
	}
	length = i;
	return true;
}

void BUG::lineNum(char * const text, int line, int length)
{
	char *thistext = text;
	for (int i = 0; i < 23; i++) {
		thistext = text;
		while ((thistext = strstr(thistext, opera[i])) && thistext<(text + length)) {
			thistext[0] = ' ';
			thistext[strlen(opera[i]) - 1] = ' ';
			char name1[50], name2[50];
			char *namestart = thistext - 1;
			while (*namestart == ' ' || *namestart == '\t') namestart--;
			while (WORD(*namestart) || NUMB(*namestart)) namestart--;
			namestart++;
			int k = 0;
			while (WORD(*namestart) || NUMB(*namestart)) {
				name1[k++] = *namestart;
				namestart++;
			}
			name1[k] = '\0';
			namestart = thistext + 1;
			while (*namestart == '\t'||*namestart == ' ') namestart++;
			bool statePoint = false;
			/*if (*namestart == '-') {
				while (!WORD(*namestart) && (*namestart != '\0') && (*namestart != '*')) namestart++;
				}
				if (*namestart == '*') {
				namestart++;
				while (*namestart != '\t'&&*namestart != ' ') namestart++;
				statePoint = true;
				}//Î´¿¼ÂÇ*/

			int length = 0, type = 0;
			if (*namestart == '(') {
				namestart++;
				if (strcmp_typekey(namestart, length, type)) {
					namestart += length;
					while (*namestart != ')') namestart++;
				}
				while (*namestart != '\t'&&*namestart != ' ') namestart++;
			}
			k = 0;
			while (WORD(*namestart) || NUMB(*namestart)) {
				name2[k++] = *namestart;
				namestart++;
			}
			name2[k] = '\0';
			TK_t temp1 = &tokens;
			TK_t temp2 = &tokens;
			if (temp1) {
				temp1 = temp1->next;
				while (temp1) {
					if (!strcmp(temp1->name, name1)) {
						temp1->pointtype = HEAP;
						break;
					}
					temp1 = temp1->next;
				}
			}
			if (temp2) {
				temp2 = temp2->next;
				while (temp2) {
					if (!strcmp(temp2->name, name2)) {
						temp2->pointtype = HEAP;
						break;
					}
					temp2 = temp2->next;
				}
			}
			if (temp1&&temp2) {
				int x = ((temp1->type) / 2) - ((temp2->type) / 2);
				x = 0 - x;
				char c;
				if (x < 2) c = 'C';
				else if (x < 3) c = 'B';
				else c = 'A';
				if (((temp1->type) / 2) < ((temp2->type) / 2)&&opera[i][1]=='=') {
					instertBu(name1, line, IN_LEN,c);
				}
				if (((temp1->type) % 2) != ((temp2->type) % 2)) {
					instertBu(name1, line, IN_SYM,c);
				}
				instertBu(name1, line, IN_OPEA,c);
			}
			else if(!temp1&&!temp2){
				cout << name1 << " " << name2 << " fail to find" << endl;
			}

			thistext++;
		}
	}
	thistext = text;
	while ((thistext = strstr(thistext, "=")) && thistext<(text + length)) {
		if (*(thistext + 1) != '=') {
			*thistext = ' ';
			char name1[50], name2[50];
			char *namestart = thistext - 1;
			while (*namestart == ' ' || *namestart == '\t') namestart--;
			while (WORD(*namestart) || NUMB(*namestart)) namestart--;
			namestart++;
			int k = 0;
			while (WORD(*namestart) || NUMB(*namestart)) {
				name1[k++] = *namestart;
				namestart++;
			}
			name1[k] = '\0';
			namestart = thistext + 1;
			while (*namestart == '\t'||*namestart == ' ') namestart++;
			bool statePoint = false;
			/*if (*namestart == '-') {
			while (!WORD(*namestart) && (*namestart != '\0') && (*namestart != '*')) namestart++;
			}
			if (*namestart == '*') {
			namestart++;
			while (*namestart != '\t'&&*namestart != ' ') namestart++;
			statePoint = true;
			}//Î´¿¼ÂÇ*/

			int length = 0, type = 0;
			if (*namestart == '(') {
				namestart++;
				if (strcmp_typekey(namestart, length, type)) {
					namestart += length;
					while (*namestart != ')') namestart++;
				}
				while (*namestart != '\t'&&*namestart != ' ') namestart++;
			}
			k = 0;
			while (WORD(*namestart) || NUMB(*namestart)) {
				name2[k++] = *namestart;
				namestart++;
			}
			name2[k] = '\0';
			TK_t temp1 = &tokens;
			TK_t temp2 = &tokens;
			if (temp1) {
				temp1 = temp1->next;
				while (temp1) {
					if (!strcmp(temp1->name, name1)) {
						temp1->pointtype = HEAP;
						break;
					}
					temp1 = temp1->next;
				}
			}
			if (temp2) {
				temp2 = temp2->next;
				while (temp2) {
					if (!strcmp(temp2->name, name2)) {
						temp2->pointtype = HEAP;
						break;
					}
					temp2 = temp2->next;
				}
			}
			if (temp1&&temp2) {
				if (((temp1->type) / 2) < ((temp2->type) / 2)) {
					int x = ((temp1->type) / 2) - ((temp2->type) / 2);
					x = 0 - x;
					char c;
					if (x < 2) c = 'C';
					else if (x < 3) c = 'B';
					else c = 'A';
					instertBu(name1, line, IN_LEN,c);
				}
				if (((temp1->type) % 2) != ((temp2->type) % 2)) {
					int x = ((temp1->type) / 2) - ((temp2->type) / 2);
					x = 0 - x;
					char c;
					if (x < 2) c = 'C';
					else if (x < 3) c = 'B';
					else c = 'A';
					instertBu(name1, line, IN_SYM,c);
				}
			}
			else {
				cout << name1 << " " << name2 << " fail to find" << endl;
			}
		}
		thistext++;
	}
}

bool BUG::instertBu(char * name, int line, int bugType, int rank)
{
	temp_bugs->next = (BG_t)malloc(sizeof(BG));
	temp_bugs = temp_bugs->next;
	if (temp_bugs) {
		temp_bugs->next = NULL;
		strcpy(temp_bugs->name, name);
		temp_bugs->line = line;
		temp_bugs->rank = rank;
		temp_bugs->bugtype = bugType;
		return true;
	}
	else {
		cout << "ÄÚ´æ·ÖÅäÊ§°Ü" << endl;
	}
	return false;
}

void BUG::lineAn(char * text, int line,int length)
{
	int i = 0;
	while (i < length) {
		if (text[i] == '/') {
			if (text[++i] == '/') {
				while (text[i] != '\n'&&text[i] != '\0') {
					text[i] = ' ';
					i++;
				}
			}
			else if (text[++i] == '*') {
				i++;
				while (text[i]!='*'&&text[i+1] != '/')
				{
					if (text[i] != '\n'&&text[i] != '\0') text[i] = ' ';
					i++;
				}
			}
		}
		if (text[i] == '\"') {
			int j = i + 1;
			while (j<length) {
				if (text[j] == '\"'&&text[j - 1] != '\\') {
					while (i <= j) text[i++] = ' ';
				}
				j++;
			}
		}
		i++;
	}
	i = 0;
	bool  funcstate = false;
	while (i < length) {
		int textlength = 0, type = 0;
		while (text[i] == ' ' || text[i] == '\t') i++;
		bool nextname = false;
	first_this_bug:		
		if (strcmp_typekey(&text[i], textlength, type)) {
			for (int j = i; j  < i+textlength; j ++)
			{
				text[j] = ' ';
			}
			i += textlength;
			while (text[i] == ' ' || text[i] == '\t') i++;
			int thisj = i;
			while (WORD(text[thisj]) || NUMB(text[thisj])) thisj++;
			while (text[thisj] == ' ' || text[thisj] == '\t') thisj++;
			if (text[thisj] == '(') {
				i++;
				funcstate = true;
			}
			else {
				while (text[i] != ';'&&text[i] != '\0'&&text[i] != ')'&&i<length) {
 					temp_tokens->next = (TK_t)malloc(sizeof(TK));
					temp_tokens = temp_tokens->next;
					temp_tokens->point = type < 0 ? true : false;
					temp_tokens->pointtype = STACK;
					temp_tokens->rank = 'B';
					temp_tokens->type = type < 0 ? 0 - type : type;
					temp_tokens->next = NULL;
					int j = 0;
					while (text[i] == ' ' || text[i] == '\t') i++;
					if (text[i] == '*'&&nextname) {
						i++;
						text[i] = ' ';
						type = type < 0 ? type : -type;
						while (text[i] == ' ' || text[i] == '\t') i++;
					}
					else if(nextname){
						type = type > 0 ? type : -type;
					}
					while (WORD(text[i]) || NUMB(text[i])) {
						temp_tokens->name[j] = text[i];
						j++; i++;
					}
					temp_tokens->name[j] = '\0';

					while (text[i] == ' ' || text[i] == '\t') i++;
					if (text[i] == '[') {
						i++;
						temp_tokens->point = true;
						char number[20];
						int j = 0;
						while (text[i] != ']') {
							if (text[i] != ' ' || text[i] != '\t') number[j++] = text[i];
							i++;
						}
						number[j] = '\0';
						if (j == 0) {
							temp_tokens->num = -1;
						}
						else {
							temp_tokens->num = atoi(number);
						}
						i++;
					}
					while (text[i] == ' ' || text[i] == '\t') i++;
					if (text[i] == ',') {
						i++;
						nextname = true;
						if (funcstate) goto first_this_bug;
						continue;
					}
					else if (text[i] == '=') {
						while (text[i] != ';' && text[i] == ','&&text[i] != '{') i++;
						if (text[i] == ';') {
							i++;
							break;
						}
						else if (text[i] == ',') {
							i++;
							continue;
						}
						else if(text[i]=='{'){
							int state = 1;
							while (state) {
								if (text[i] == '{') {
									state++;
									i++;
								}
								if (text[i] == '}') {
									state--;
								}
								i++;
							}
							i++;
						}
						else {
							i++;
						}
					}
					else {
						i++;
					}
				}
				if (text[i] == ')')funcstate = false;
			}
		}
		else {
			i++;
		}
	}
}

void BUG::lineFindFun(char * text, int line, int length)
{
	char * start;
	while ((start = strstr(text, "malloc"))||(start = strstr(text, "HeapAlloc"))) {
		cout << line << "  ¶Ñ" << endl;
		start[0] = ' '; start[1] = ' '; start[2] = ' '; start[3] = ' '; start[4] = ' '; start[5] = ' ';
		while (*start != '='&&*start != '\n'&&*start != ';') start--;
		//start--;
		if (*start == '=') {
			*start = ' ';
			start--;
			while (*start == ' ' || *start == '\t') start--;
			char name[50];
			char *namestart = start - 1;
			while (WORD(*namestart) || NUMB(*namestart)) namestart--;
			namestart++;
			int k = 0;
			while (namestart <= start) {
				name[k++] = *namestart;
				namestart++;
			}
			name[k] = '\0';
			TK_t temp = &tokens;
			if (temp) {
				temp = temp->next;
				while (temp) {
					if (!strcmp(temp->name, name)) {
						temp->pointtype = HEAP;
						break;
					}
					temp = temp->next;
				}
			}
		}
	}
	start = text;
	while(start = strstr(start, "strlen")) {
		int i = 0;
		while (start[i] != '('&&start[i] != '\0') i++;
		if (start[i] == '(') {
			i++;
			while (start[i] == ' ' || start[i] == '\t') i++;
			char name[50];
			int j = 0;
			while (WORD(start[i]) || NUMB(start[i])) {
				name[j] = start[i];
				j++; i++;
			}
			name[j] = '\0';
			TK_t temp;
			temp = &tokens;
			if (temp) {
				temp = temp->next;
				while (temp) {
					if (!strcmp(temp->name, name)) {
						temp->rank = 'C';
						break;
					}
					temp = temp->next;
				}
			}
			start++;
		}
	}
	for (int i = 0; i < 20; i++)//18ÎªÉèÖÃµÄÊýÁ¿ Ãô¸Ð²Ù×÷
	{
		char * starttext = text;
		while((starttext=strstr(starttext, key[i].name))&&!WORD(*(starttext-1))) {
			for (unsigned int i = 0; i < strlen(key[i].name)-1; i++)
			{
				starttext[i] = ' ';
			}
			char * thistext = starttext;
			starttext+= strlen(key[i].name);
			char name[50];
			if (key[i].token==-1) {
				thistext += strlen(key[i].name);
				while (*thistext != '(') thistext++;
				while (*thistext != ','&&*thistext!=')') thistext++;
				if (*thistext == ')') {
					continue;
				}
				else {
					thistext++;
				}
				//while (*thistext != '"') thistext++;
				//thistext++;
				//while ((*thistext != '"'||*(thistext-1) != '\\')&&*thistext!='\0') thistext++;
				//thistext++;
				while (*thistext != ')') {
					while (*thistext == '\t'||*thistext == ' ') thistext++;
					int k = 0;
					//thistext++;
					while (WORD(*thistext) || NUMB(*thistext)) {
						name[k++] = *thistext;
						thistext++;
					}
					name[k] = '\0';
					TK_t temp = &tokens;
					if (temp) {
						temp = temp->next;
						while (temp) {
							if (!strcmp(temp->name, name)) {
								break;
							}
							temp = temp->next;
						}
					}
					if (temp) {
						temp_bugs->next = (BG_t)malloc(sizeof(BG));
						temp_bugs = temp_bugs->next;
						temp_bugs->line = line;
						temp_bugs->next = NULL;
						temp_bugs->bugtype = temp->pointtype;
						if (temp->point) temp_bugs->rank = temp->rank;
						temp_bugs->rank = temp->rank;
						strcpy(temp_bugs->name, key[i].name);
					}
					while (*thistext != ')'&&*thistext != ',') thistext++;
					if (*thistext == ')')break;
				}
			}
			else {
				if (key[i].token == 0) {
					while (*thistext != '\0'&&*thistext != '=') thistext--;
					if (*thistext == '=') {
						while (*thistext == ' ' || *thistext == '\t') thistext--;
						char *namestart = thistext;
						while (WORD(*namestart) || NUMB(*namestart)) namestart--;
						namestart++;
						int k = 0;
						while (namestart < start) {
							name[k++] = *namestart;
							namestart++;
						}
						name[k] = '\0';
					}
				}
				else if (key[i].token == 1) {
					thistext += strlen(key[i].name);
					while ((*thistext) == '\t' || (*thistext) == ' ') thistext++;
					while ((*thistext) != '(') thistext++;
					thistext++;
					while ((*thistext) == '\t' || (*thistext) == ' ') thistext++;
					int k = 0;
					while (WORD(*thistext) || NUMB(*thistext)) {
						name[k++] = *thistext;
						thistext++;
					}
					name[k] = '\0';
				}
				else if (key[i].token == 2) {
					thistext += strlen(key[i].name);
					while ((*thistext) != ',') thistext++;
					while ((*thistext) != '\t' && (*thistext) != ' ') thistext++;
					int k = 0;
					thistext++;
					while (WORD(*thistext) || NUMB(*thistext)) {
						name[k++] = *thistext;
					}
					name[k] = '\0';
				}
				TK_t temp = &tokens;
				if (temp) {
					temp = temp->next;
					while (temp) {
						if (!strcmp(temp->name, name)) {
							break;
						}
						temp = temp->next;
					}
				}
				if (temp) {
					temp_bugs->next = (BG_t)malloc(sizeof(BG));
					temp_bugs = temp_bugs->next;
					temp_bugs->line = line;
					temp_bugs->next = NULL;
					temp_bugs->bugtype = temp->pointtype;
					temp_bugs->rank = temp->rank;
					strcpy(temp_bugs->name, key[i].name);
				}
			}
		}
	}
}
