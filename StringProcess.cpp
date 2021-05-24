#include "StringProcess.h"

void readfile(const char* filename, char*& str, int& cnt)
{
	FILE* f = fopen(filename, "r");
	if (!f) {
		printf(FILE_NOT_FOUND);
		return;
	}
	fseek(f, 0, SEEK_END);
	cnt = ftell(f);
	char c;
	fseek(f, 0, SEEK_SET);
	str = (char*)calloc(cnt + 1, 1);
	if (str == NULL) {
		printf(INSUFFICIENT_MEMORY);
		str = NULL;
		cnt = 0;
		return;
	}
	int index = 0;
	while (!feof(f)) {
		fscanf(f, "%c", &c);
		str[index] = c;
		index++;
	}
	str[index - 1] = '\0';
	fclose(f);
}

int countsub(char* str, char splitchr) {
	int dem = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == splitchr) dem++;
	}
	return dem + 1;
}

void splitstr(char* _str, char**& s, int& cnt2, char splitchr) {
	char* str = new char[strlen(_str) + 1];
	memcpy(str, _str, strlen(_str) + 1);

	int sstr_cnt = countsub(str, splitchr);
	s = new char* [sstr_cnt];
	if (s == NULL) {
		printf(INSUFFICIENT_MEMORY);
		return;
	}
	int j = 0;
	bool isSubStr = false;
	cnt2 = 0;
	for (int i = 0; i < strlen(_str); i++) {
		if (str[i] != splitchr && str[i] != '\0') {
			if (!isSubStr) {
				isSubStr = true;
				s[cnt2] = &str[i];
			}
		}
		else {
			str[i] = '\0';
			cnt2++;
			isSubStr = false;
		}
	}
	cnt2++;
}