#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "StringProcess.h"

#define BASE_256 256
#define BASE_65536 65536

struct bigint {
	unsigned char* data;
	int digit_c;
	bool sign; //Positive = true; Negative = false

	bigint& operator=(bigint a) {
		if (digit_c >= 0 && data != NULL) free(data);
		digit_c = a.digit_c;
		sign = a.sign;
		data = a.data;
		return *this;
	}
};

bigint init();

void dispose(bigint& a);

bigint duplicate(bigint a);

bigint itoBigInt(int a);

bigint operator&(bigint a, bigint b);

bigint operator|(bigint a, bigint b);

bigint operator~(bigint a);

bigint operator^(bigint a, bigint b);

bigint twoComplement(bigint a, bool sign);

bigint shiftOneleft(bigint a);

bigint shiftleft(bigint a, int count);

bigint shiftOneright(bigint a);

bigint shiftright(bigint a, int count);

bool operator < (bigint a, bigint b);

bool operator > (bigint a, bigint b);

bool operator == (bigint a, bigint b);

bool operator != (bigint a, bigint b);

bigint operator + (bigint a, bigint b);

bigint operator - (bigint a, bigint b);

bigint operator * (bigint a, bigint b);

bigint operator / (bigint a, bigint b);

bigint operator % (bigint a, bigint b);

bigint abs(bigint a);

bigint min(bigint a, bigint b);

bigint max(bigint a, bigint b);

bigint pow(bigint a, bigint b);

bigint BigInt(const char* origin, const int n, const int base);

unsigned char* to_decimal(bigint a, int& count);

unsigned char* to_base64(bigint a, int& count);

unsigned char* to_base32(bigint a, int& count);

unsigned char* to_base58(bigint a, int& count);

bigint randBigInt(bigint max);

bool isPrime(bigint n, int k);

int digits(bigint a);

char* to_string(bigint a, int base);

void print(bigint a, int base);