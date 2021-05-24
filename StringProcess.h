#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Constant.h"

void readfile(const char* filename, char*& str, int& cnt);

int countsub(char* str, char splitchr);

void splitstr(char* _str, char**& s, int& cnt2, char splitchr);