#include "BigInt.h"

bigint init() {
	bigint ans;
	ans.data = NULL;
	ans.digit_c = -1;
	ans.sign = true;
	return ans;
}

void dispose(bigint& a) {
	if (a.digit_c >= 0 && a.data != NULL) 
		free(a.data);
	a.digit_c = -1;
	a.data = NULL;
}

bigint duplicate(bigint a) {
	bigint res;
	res.digit_c = a.digit_c;
	res.sign = a.sign;
	if (res.digit_c > 0) {
		res.data = new unsigned char[res.digit_c];
		memcpy(res.data, a.data, res.digit_c);
	}
	else res.data = NULL;
	return res;
}

bigint itoBigInt(int a) {
	unsigned char* ptr = (unsigned char*)&a;
	bigint ans = init();
	if (a < 0) {
		ans.sign = false;
		a *= -1;
	}
	else ans.sign = true;	
	unsigned char temp[8];
	int index = 0;
	temp[0] = 0;
	while (a > 0) {
		temp[index] = a % 256;
		a /= 256;
		index++;
	}
	ans.digit_c = index == 0 ? 1 : index;
	ans.data = new unsigned char[ans.digit_c];
	for (int i = 0; i < ans.digit_c; i++) ans.data[i] = temp[i];
	return ans;
}



bigint operator&(bigint a, bigint b) {
	int min = a.digit_c < b.digit_c ? a.digit_c : b.digit_c;
	int max = a.digit_c + b.digit_c - min;
	bigint* ptr = a.digit_c > b.digit_c ? &a : &b;
	bigint ans = init();
	ans.data = new unsigned char[max];
	for (int i = 0; i < min; i++) {
		ans.data[i] = a.data[i] & b.data[i];
	}
	for (int i = min; i < max; i++) {
		ans.data[i] = (ptr->data[i]) & 0;
	}
	ans.digit_c = max;
	return ans;
}

bigint operator|(bigint a, bigint b) {
	int min = a.digit_c < b.digit_c ? a.digit_c : b.digit_c;
	int max = a.digit_c + b.digit_c - min;
	bigint* ptr = a.digit_c > b.digit_c ? &a : &b;
	bigint ans = init();
	ans.data = new unsigned char[max];
	for (int i = 0; i < min; i++) {
		ans.data[i] = a.data[i] | b.data[i];
	}
	for (int i = min; i < max; i++) {
		ans.data[i] = ptr->data[i] | 0;
	}
	ans.digit_c = max;
	return ans;
}

bigint operator~(bigint a) {
	bigint ans = init();
	ans.data = new unsigned char[a.digit_c];
	for (int i = 0; i < a.digit_c; i++) {
		ans.data[i] = ~a.data[i];
	}
	ans.digit_c = a.digit_c;
	return ans;
}

bigint operator^(bigint a, bigint b) {
	int min = a.digit_c < b.digit_c ? a.digit_c : b.digit_c;
	int max = a.digit_c + b.digit_c - min;
	bigint* ptr = a.digit_c > b.digit_c ? &a : &b;
	bigint ans = init();
	ans.data = new unsigned char[max];
	for (int i = 0; i < min; i++) {
		ans.data[i] = a.data[i] ^ b.data[i];
	}
	for (int i = min; i < max; i++) {
		ans.data[i] = (ptr->data[i]) ^ 0;
	}
	ans.digit_c = max;
	return ans;
}

bigint twoComplement(bigint a, bool sign) {
	bigint ans = init();
	ans = duplicate(a);
	if (sign) return ans;
	ans = ~ans;
	int i = 0;
	while (((int) ans.data[i]) + 1 > 255 && i < ans.digit_c) {
		ans.data[i]++;
		i++;
	}
	
	if (i < ans.digit_c) ans.data[i]++;
	else {
		for (int i = 0; i < ans.digit_c; i++) ans.data[i] = 0;
	}
	ans.sign = sign;
	return ans;
}

bigint shiftOneleft(bigint a) {
	int newDigit = a.digit_c;
	bigint ans = init();
	ans.digit_c = a.digit_c;
	ans.sign = a.sign;
	ans.data = new unsigned char[a.digit_c];
	memcpy(ans.data, a.data, a.digit_c);

	if (ans.data[ans.digit_c - 1] >= 128) {
		ans.data = (unsigned char*) realloc(ans.data, ans.digit_c + 1);
		ans.data[ans.digit_c] = 1;
		newDigit++;
	}
	
	bool inc = false;
	for (int i = 0; i < ans.digit_c; i++) {
		int bonus = inc ? 1 : 0;
		if (ans.data[i] >= 128) inc = true;
		else inc = false;
		ans.data[i] = ans.data[i] * 2 + bonus;
	}
	ans.digit_c = newDigit;
	return ans;
}

bigint shiftleft(bigint a, int count) {
	bigint ans = init();
	if (count >= 8) {
		ans.digit_c = count / 8 + a.digit_c;
		ans.data = (unsigned char*)calloc(ans.digit_c, 1);
		memcpy(&ans.data[ans.digit_c - a.digit_c], a.data, a.digit_c);
		count = count - (count / 8) * 8;
	}
	else 
		ans = duplicate(a);
	if (count > 0) ans = shiftOneleft(ans);
	for (int i = 0; i < count - 1; i++) 
		ans = shiftOneleft(ans);
	return ans;
}

bigint shiftOneright(bigint a) {
	int newDigit = a.digit_c;
	bigint ans = init();
	ans.digit_c = a.digit_c;
	ans.sign = a.sign;
	ans.data = new unsigned char[a.digit_c];
	memcpy(ans.data, a.data, a.digit_c);

	bool inc = false;
	if (ans.digit_c > 1 && ans.data[ans.digit_c - 1] == 1) {
		ans.data = (unsigned char*)realloc(ans.data, ans.digit_c - 1);
		newDigit--;
		inc = true;
	}
	
	for (int i = newDigit - 1; i >= 0; i--) {
		int bonus = inc ? 128 : 0;
		if (ans.data[i] % 2 == 1) inc = true;
		else inc = false;
		ans.data[i] = ans.data[i] / 2 + bonus;
	}
	ans.digit_c = newDigit;
	return ans;
}


bigint shiftright(bigint a, int count) {
	bigint ans = init();
	if (count > 0) ans = shiftOneright(a);
	for (int i = 0; i < count - 1; i++) ans = shiftOneright(ans);
	return ans;
}

bool operator < (bigint a, bigint b) {
	if (a.sign && !b.sign) return false;
	if (!a.sign && b.sign) return true;
	if (a.sign) {
		int max = a.digit_c > b.digit_c ? a.digit_c : b.digit_c;
		for (int i = max - 1; i >= 0; i--) {
			if (a.digit_c <= i && b.data[i] != 0) return true;
			if (b.digit_c <= i && a.data[i] != 0) return false;
			if (i < a.digit_c && i < b.digit_c && b.data[i] != a.data[i]) return a.data[i] < b.data[i];
		}
	}
	return false;
}

bool operator > (bigint a, bigint b) {
	return b < a;
}

bool operator == (bigint a, bigint b) {
	return (!(a < b) && !(b < a));
}

bool operator != (bigint a, bigint b) {
	return !(!(a < b) && !(b < a));
}

bool isNegative(bigint a) {
	return !a.sign;
}

bool isPositive(bigint a) {
	return a.sign;
}

bool isZero(bigint a) {
	a.sign = true;
	bigint zero = itoBigInt(0);
	bool ans = a == zero;
	dispose(zero);
	return ans;
}

bigint operator+(bigint a, bigint b) {
	if (a.sign ^ b.sign) {
		if (a.sign) {
			b.sign = true;
			return a - b;
		}
		a.sign = true;
		return b - a;
	}
	int min = a.digit_c < b.digit_c ? a.digit_c : b.digit_c;
	int max = a.digit_c + b.digit_c - min;
	bigint* ptr = a.digit_c > b.digit_c ? &a : &b;
	bigint ans = init();
	ans.data = new unsigned char[max];
	int carry = 0, i;
	for (i = 0; i < min; i++) {
		ans.data[i] = a.data[i] + b.data[i] + carry;
		carry = a.data[i] + b.data[i] + carry;
		if (carry > 255) carry = 1;
		else carry = 0;
	}
	while (carry > 0 && i < max) {
		ans.data[i] = (ptr->data[i]) + carry;
		carry = (ptr->data[i]) + carry;
		if (carry > 255) carry = 1;
		else carry = 0;
		i++;
	}
	if (i < max) {
		for (; i < max; i++) ans.data[i] = ptr->data[i];
	}
	else if (carry != 0 && i == max) {
		max++;
		ans.data = (unsigned char*)realloc(ans.data, max);
		ans.data[max - 1] = carry;
	}
	ans.digit_c = max;
	ans.sign = a.sign;
	return ans;
}

bigint operator-(bigint a, bigint b) {
	if (a.sign ^ b.sign) {
		if (b.sign) {
			b.sign = false;
			return a + b;
		}
		b.sign = true;
		return a + b;
	}
	bigint ans = init();
	if (a < b) {
		ans = b - a;
		ans.sign = false;
		return ans;
	}
	int digit = 0;
	int min = a.digit_c < b.digit_c ? a.digit_c : b.digit_c;
	int max = a.digit_c + b.digit_c - min;
	bigint* ptr = a.digit_c > b.digit_c ? &a : &b;
	ans.data = new unsigned char[max];
	int borrow = 0, i;
	for (i = 0; i < min; i++) {
		ans.data[i] = a.data[i] - b.data[i] - borrow;
		if (ans.data[i] != 0) digit = i;
		borrow = a.data[i] - b.data[i] - borrow;
		if (borrow < 0) borrow = 1;
		else borrow = 0;
	}
	while (borrow > 0 && i < max) {
		ans.data[i] = (ptr->data[i]) - borrow;
		if (ans.data[i] != 0) digit = i;
		borrow = (ptr->data[i]) - borrow;
		if (borrow < 0) borrow = 1;
		else borrow = 0;
		i++;
	}
	if (i < max) {
		for (; i < max; i++) {
			ans.data[i] = ptr->data[i];
			if (ans.data[i] != 0) digit = i;
		}
	}
	ans.digit_c = digit + 1;
	ans.data = (unsigned char*) realloc(ans.data, ans.digit_c);
	return ans;
}

bigint operator*(bigint a, bigint b) {
	bigint ans = init();
	ans.digit_c = b.digit_c + a.digit_c - 1;
	ans.data = (unsigned char*)calloc(ans.digit_c, 1);
	int carry = 0;
	int i = 0, j = 0;
	for (i = 0; i < a.digit_c; i++) {
		for (j = 0; j < b.digit_c; j++) {
			int temp = ans.data[i + j] + a.data[i] * b.data[j] + carry;
			ans.data[i + j] = temp;
			carry = temp / 256;
			int h = ans.data[i + j];
			int haha = 1;
		}
		if (carry > 0) {
			if (i + j >= ans.digit_c) {
				ans.digit_c++;
				ans.data = (unsigned char*)realloc(ans.data, ans.digit_c);
				ans.data[i + j] = 0;
			}
			ans.data[i + j] += carry;
			carry = 0;
			int x = ans.data[i + j];
			int haha = 1;
		}
	}
	ans.sign = !(a.sign ^ b.sign);
	return ans;
}

bigint simple_divide(bigint a, bigint b) {	
	bigint res, _a, _b, one;
	_a = duplicate(a);
	_b = duplicate(b);
	one = itoBigInt(1);
	res = itoBigInt(0);
	
	int count = 0;
	for (int i = _a.digit_c - 1; i >= 0; i--) 
		for (int c = 7; c >= 0; c--)
		{
			int k = _a.data[i] >> c;
			if (k & 1) {
				i = -1;
				c = -1;
			}
			else count++;
		}
	
	int count2 = 0;
	for (int i = _b.digit_c - 1; i >= 0; i--) 
		for (int c = 7; c >= 0; c--)
		{
			int k = _b.data[i] >> c;
			if (k & 1) {
				i = -1;
				c = -1;
			}
			else count2++;
	}
	int bit = _a.digit_c * 8 - count - (_b.digit_c * 8 - count2);
	
	_b = shiftleft(_b, bit);
	while (bit >= 0) {
		if (!(_a < _b)) {
			res.data[0] = res.data[0] + 1;
			_a = _a - _b;
		}
		if (bit > 0) {
			res = shiftleft(res, 1);
			_b = shiftright(_b, 1);
		}
		
		bit--;
	}
	dispose(_b);
	dispose(_a);
	dispose(one);
	return res;
}

bigint advanced_divide(bigint a, bigint b) {
	bigint res, _a, _b, t_a = init();
	res = itoBigInt(0);
	if (a < b) return res;

	_a = duplicate(a);
	_b = duplicate(b);	
	int count = 0;
	for (int i = _a.digit_c - 1; i >= 0; i--)
		for (int c = 7; c >= 0; c--)
		{
			int k = _a.data[i] >> c;
			if (k & 1) {
				i = -1;
				c = -1;
			}
			else count++;
		}

	int count2 = 0;
	for (int i = _b.digit_c - 1; i >= 0; i--)
		for (int c = 7; c >= 0; c--)
		{
			int k = _b.data[i] >> c;
			if (k & 1) {
				i = -1;
				c = -1;
			}
			else count2++;
		}
	int bit = _a.digit_c * 8 - count - (_b.digit_c * 8 - count2);
	if (count2 < count) 
		_a = shiftleft(_a, count - count2);
	else if (count2 > count) _a = shiftleft(_a, 8 - count2 + count);
	res.digit_c = bit / 8 + 1;
	res.data = (unsigned char*) calloc(res.digit_c, 1);
	t_a.data = new unsigned char[b.digit_c];
	t_a.digit_c = b.digit_c;
	memcpy(t_a.data, &_a.data[_a.digit_c - _b.digit_c], t_a.digit_c);
	t_a.sign = true;
	int rad = 7, stt = _a.digit_c - _b.digit_c;
	stt -= stt > 0 ? 1 : 0;
	for (; bit >= 0; bit--) {
		if (!(t_a < _b)) {
			t_a = t_a - _b;
			res.data[bit / 8] += (int)pow(2, bit % 8);
		}
		t_a = shiftleft(t_a, 1);
		if (stt >= 0 && _a.data[stt] >= pow(2, rad)) {
			_a.data[stt] -= (int) pow(2, rad);
			t_a.data[0] += 1;
		}
		if (rad == 0) {
			rad = 8;
			stt--;
		}
		rad--;		
	}
	dispose(_a);
	dispose(_b);
	dispose(t_a);
	return res;
}

bigint operator / (bigint a, bigint b) {
	bigint ans;
	ans = advanced_divide(a, b);
	ans.sign = !(a.sign ^ b.sign);
	return ans;
}

bigint operator % (bigint a, bigint b) {
	bigint ans;
	ans = a / b;
	ans = ans * b;
	ans = a - ans;
	ans.sign = !(a.sign ^ b.sign);
	return ans;
}

bigint abs(bigint a) {
	bigint ans;
	ans = duplicate(a);
	ans.sign = true;
	return ans;
}

bigint min(bigint a, bigint b) {
	return a < b ? duplicate(a) : duplicate(b);
}

bigint max(bigint a, bigint b) {
	return a > b ? duplicate(a) : duplicate(b);
}

bigint pow(bigint a, bigint b)
{
	bigint zero, one, two, ans = init();
	zero = itoBigInt(0);
	one = itoBigInt(1);
	two = itoBigInt(2);
	ans = duplicate(zero);
	bigint halfb;
	halfb = b / two;
	
	if (isZero(b)) {
		dispose(zero);
		dispose(two);
		dispose(ans);
		dispose(halfb);
		return one;
	}
	
	bigint powerhalf;
	powerhalf = pow(a, halfb);
	
	bigint temp;
	temp = halfb * two;
	temp = b - temp;

	if (isZero(temp)) {
		ans = powerhalf * powerhalf;
	}
	else
	{
		ans = a * powerhalf;
		ans = ans * powerhalf;
	}
	
	dispose(temp);
	dispose(powerhalf);
	dispose(halfb);
	dispose(zero);
	dispose(one);
	dispose(two);
	return ans;
}

bigint BigInt(const char* origin, const int n, const int base) {
	bigint ans;
	switch (base) {
	case 10: {
		bigint radix, ten, one;
		ans = itoBigInt(0);
		radix = itoBigInt(0);
		ten = itoBigInt(10);
		one = itoBigInt(1);
		int i = 0;
		bool sign = true;
		if (origin[i] == '-') {
			sign = false;
			i++;
		}
		for (; i < n; i++) {
			bigint temp = init();
			temp = itoBigInt(origin[i] - '0');
			ans = ans * ten;
			ans = ans + temp;
			dispose(temp);
		}
		dispose(radix);
		dispose(ten);
		dispose(one);
		ans.sign = sign;
		break;
	}
	case 2: {
		int bit = 0;
		ans.digit_c = n / 8 + 1;
		ans.data = (unsigned char*)calloc(ans.digit_c, 1);
		ans.sign = true;
		for (int i = n - 1; i >= 0; i--) {
			if (origin[i] - '0')
				ans.data[bit / 8] += pow(2, bit % 8);
			bit++;
		}
		break;
	}
	}
	return ans;
}

unsigned char* to_decimal(bigint a, int& count) {
	bigint zero, ten, t_a;
	zero = itoBigInt(0);
	char str[] = "10000000000";
	ten = BigInt(str, strlen(str), 10);
	t_a = duplicate(a);
	unsigned char pans[1000];
	unsigned char* ans = NULL;
	pans[0] = 0;
	count = 0;
	int index = 0;
	t_a = abs(t_a);
	while (t_a > zero) {
		bigint temp = init();
		clock_t begin = clock();
		bigint t = init();
		t = t_a / ten;
		temp = t * ten;
		temp = t_a - temp;
		t_a = duplicate(t);
		dispose(t);
		unsigned long long int a = 0;
		for (int i = temp.digit_c - 1; i >= 0; i--) a = a * 256 + (unsigned long long int) temp.data[i];		
		if (t_a > zero) {
			int digit = 10;
			while (digit > 0) {
				pans[index] = a % 10;
				a /= 10;
				index++;
				if (index == 1000) {
					ans = (unsigned char*)realloc(ans, count + index);
					for (int i = 0; i < 1000; i++) {
						ans[count] = pans[i];
						count++;
					}
					index = 0;
				}
				digit--;
			}
		}
		else {
			while (a > 0) {
				pans[index] = a % 10;
				a /= 10;
				index++;
				if (index == 1000) {
					ans = (unsigned char*)realloc(ans, count + index);
					for (int i = 0; i < 1000; i++) {
						ans[count] = pans[i];
						count++;
					}
					index = 0;
				}
			}
		}
		dispose(temp);

	}
	if (count == 0 && index == 0) {
		index = 1;
	}
	
	ans = (unsigned char*)realloc(ans, count + index);
	
	for (int i = 0; i < index; i++) {
		ans[count] = pans[i];
		count++;
	}

	dispose(zero);
	dispose(ten);
	dispose(t_a);
	return ans;
}

unsigned char* to_base64(bigint a, int& count) {
	int padding = a.digit_c % 3;
	unsigned char *ans;
	count = a.digit_c;
	if (padding != 0) {
		padding = 3 - padding;
		count += padding;
	}
	unsigned char* _adata = (unsigned char*)calloc(count, 1);
	memcpy(&_adata[padding == 0 ? 0 : padding], a.data, a.digit_c);
	count = (count * 8) / 6;
	ans = (unsigned char*)calloc(count, 1);
	memset(ans, -1, padding);
	int bit = padding * 6;
	while (bit < count * 6) {
		if (_adata[bit / 8] & ((unsigned char)pow(2, bit % 8))) 
			ans[bit / 6] += pow(2, bit % 6);
		bit++;
	}
	free(_adata);
	return ans;
}

unsigned char* to_base32(bigint a, int& count) {
	int padding = a.digit_c % 5;
	unsigned char* ans;
	count = a.digit_c;
	if (padding != 0) {
		padding = 5 - padding;
		count += padding;
	}
	unsigned char* _adata = (unsigned char*)calloc(count, 1);
	memcpy(&_adata[padding == 0 ? 0 : padding], a.data, a.digit_c);
	count = (count * 8) / 5;
	ans = (unsigned char*)calloc(count, 1);
	memset(ans, -1, (padding * 8) / 5);
	int bit = padding * 5;
	while (bit < count * 5) {
		if (_adata[bit / 8] & ((unsigned char)pow(2, bit % 8)))
			ans[bit / 5] += pow(2, bit % 5);
		bit++;
	}
	free(_adata);
	return ans;
}

unsigned char* to_base58(bigint a, int& count) {
	bigint zero, ten, t_a;
	zero = itoBigInt(0);
	ten = itoBigInt(58);
	t_a = duplicate(a);
	unsigned char pans[1000];
	unsigned char* ans = NULL;
	pans[0] = 0;
	count = 0;
	int index = 0;
	t_a = abs(t_a);
	while (t_a > zero) {
		bigint temp = init();
		clock_t begin = clock();
		bigint t = init();
		t = t_a / ten;
		temp = t * ten;
		temp = t_a - temp;

		pans[index] = temp.data[0];
		index++;
		if (index == 1000) {
			ans = (unsigned char*)realloc(ans, count + index);
			for (int i = 0; i < 1000; i++) {
				ans[count] = pans[i];
				count++;
			}
			index = 0;
		}

		t_a = duplicate(t);
		dispose(t);
		dispose(temp);

	}
	if (count == 0 && index == 0) {
		index = 1;
	}
	if (a < zero)
		ans = (unsigned char*)realloc(ans, count + index + 1);
	else
		ans = (unsigned char*)realloc(ans, count + index);

	for (int i = 0; i < index; i++) {
		ans[count] = pans[i];
		count++;
	}
	
	dispose(zero);
	dispose(ten);
	dispose(t_a);
	return ans;
}

int randint(int lower, int upper) {
	if (upper <= lower) return upper;
	return (rand() % (upper - lower + 1)) + lower;
}

bigint randBigInt(bigint max) {
	bigint ans;
	ans.data = new unsigned char[max.digit_c];
	ans.digit_c = max.digit_c;
	bool isZero = true;
	bool isSmaller = false;
	if (max.digit_c == 1) 
		ans.data[0] = randint(2, max.data[0]);
	else {
		for (int i = max.digit_c - 1; i >= 0; i--) {
			if (!isSmaller) {
				ans.data[i] = randint(0, max.data[i]);
				isSmaller = ans.data[i] < max.data[i];
			}
			else {
				ans.data[i] = randint(0, 255);
			}
			isZero = (ans.data[i] == 0) && isZero;
			if (isZero && (i == 0)) {
				ans.data[i] = randint(2, 255);
				isZero = false;
			}
			else if (isZero) ans.digit_c--;
		}
	}
	if (ans.digit_c < max.digit_c) ans.data = (unsigned char*)realloc(ans.data, ans.digit_c);
	ans.sign = true;
	return ans;
}

/*Return: (x^y) % p*/
bigint miillerPow(bigint x, bigint y, bigint p) {
	bigint res, _x, _y, _p, zero;

	res = itoBigInt(1);
	zero = itoBigInt(0);
	_x = duplicate(x);
	_y = duplicate(y);
	_p = duplicate(p);

	_x = _x % _p;

	while (_y > zero) {
		if (_y.data[0] & 1) {
			res = res * _x;
			res = res % _p;
		}
		_y = shiftright(_y, 1);
		_x = _x * _x;
		_x = _x % _p;
	}
	dispose(_x);
	dispose(_y);
	dispose(_p);
	dispose(zero);
	return res;
}

bool miillerTest(bigint d, bigint n) {
	bigint _a = init(), _d, _x = init(), four, two, one, temp = init();
	four = itoBigInt(4);
	two = itoBigInt(2);
	one = itoBigInt(1);
	_d = duplicate(d);
	temp = n - two;
	_a = randBigInt(temp);

	_x = miillerPow(_a, d, n);
	temp = n - one;
	if (_x == one || _x == temp) {
		dispose(_a);
		dispose(_d);
		dispose(_x);
		dispose(one);
		dispose(four);
		dispose(two);
		dispose(temp);
		return true;
	}
	while (_d != temp) {
		_x = _x * _x;
		_x = _x % n;
		_d = shiftleft(_d, 1);

		if (_x == one) {
			dispose(_a);
			dispose(_d);
			dispose(_x);
			dispose(one);
			dispose(four);
			dispose(two);
			dispose(temp);
			return false;
		}
		if (_x == temp) {
			dispose(_a);
			dispose(_d);
			dispose(_x);
			dispose(one);
			dispose(four);
			dispose(two);
			dispose(temp);
			return true;
		}
	}
	dispose(_a);
	dispose(_d);
	dispose(_x);
	dispose(one);
	dispose(four);
	dispose(two);
	dispose(temp);
	return false;
}

bool isPrime(bigint n, int k) {
	bigint one, four, three;
	one = itoBigInt(1);
	four = itoBigInt(4);
	three = itoBigInt(3);
	if (n < one || n == one || n == four) {
		dispose(one);
		dispose(four);
		dispose(three);
		return false;
	}
	if (n < three || n == three) {
		dispose(one);
		dispose(four);
		dispose(three);
		return true;
	}
	dispose(four);
	dispose(three);
	bigint d, two;
	two = itoBigInt(2);
	d = n - one;
	while (!(d.data[0] & 1)) {
		d = shiftright(d, 1);
	}

	for (int i = 0; i < k; i++) {
		if (!miillerTest(d, n)) {
			dispose(d);
			dispose(two);
			dispose(one);
			return false;
		}
	}
	dispose(d);
	dispose(two);
	dispose(one);
	return true;
}

int digits(bigint a) {
	int count;
	unsigned char* ad = to_decimal(a, count);
	free(ad);
	return count;
}

char* to_string(bigint a, int base) {
	char* ans = NULL;
	switch (base) {
	case 64:
	{
		int count;
		char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		unsigned char* ad = to_base64(a, count);
		ans = new char[count + 1];
		for (int i = count - 1; i >= 0; i--) {
			if (ad[i] == 255) ans[count - 1 - i] = '=';
			else ans[count - 1 - i] = alphabet[ad[i]];
		}
		ans[count] = '\0';
		free(ad);
		break;
	}
	case 32:
	{
		int count;
		char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		unsigned char* ad = to_base32(a, count);
		ans = new char[count + 1];
		for (int i = count - 1; i >= 0; i--) {
			if (ad[i] == 255) ans[count - 1 - i] = '=';
			else ans[count - 1 - i] = alphabet[ad[i]];
		}
		ans[count] = '\0';
		free(ad);
		break;
	}
	case 58:
	{
		int count;
		unsigned char* ad = to_base58(a, count);
		ans = new char[count + 1];
		char alphabet[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
		for (int i = count - 1; i >= 0; i--) {
			ans[count - 1 - i] = alphabet[ad[i]];
		}
		ans[count] = '\0';
		free(ad);
		break;
	}
	case 10:
	{
		int count, sign = 0;
		unsigned char* ad = to_decimal(a, count);
		bigint zero = itoBigInt(0);
		if (!a.sign && (!isZero(a))) {
			sign = 1;
			ans = new char[count + 2];
			ans[0] = '-';
		}
		else 
			ans = new char[count + 1];
		
		
		for (int i = count - 1; i >= 0; i--) {
			ans[count - 1 - i + sign] = '0' + ad[i];
		}
		ans[count + sign] = '\0';
		free(ad);
		break;
	}
	case 2:
	{
		bigint _a = duplicate(a);
		_a = twoComplement(_a, _a.sign);
		int index = 0;
		bool hadFirst = false;
		int count = 0;
		for (int i = _a.digit_c - 1; i >= 0; i--) {
			for (int c = 7; c >= 0; c--)
			{
				int k = _a.data[i] >> c;
				if (_a.sign) {
					if (!hadFirst) {
						if (k & 1) {
							hadFirst = true;
							ans = new char[_a.digit_c * 9 + 1 - count];
							ans[index] = '1';
							index++;
						}
						else count++;
					}
					else {
						if (k & 1)
							ans[index] = '1';
						else
							ans[index] = '0';
						index++;
					}
				}
				else {
					if (!hadFirst) {
						if (k % 2 == 0) {
							hadFirst = true;
							ans = new char[_a.digit_c * 9 + 2 - count];
							ans[index] = '1';
							ans[index + 1] = '0';
							index += 2;
						}
						else count++;
					}
					else {
						if (k & 1)
							ans[index] = '1';
						else
							ans[index] = '0';
						index++;
					}
				}
			}
		}

		if (!hadFirst) {
			ans = new char[2];
			ans[0] = '0';
			ans[1] = '\0';
		}
		else ans[index] = '\0';
		break;
	}
	}
	return ans;
}

void print(bigint a, int base) {
	switch (base) {
	case 256:
		{
			for (int i = 0; i < a.digit_c; i++) {
				printf("%d ", a.data[i]);
			}
			break;
		}
	case 2:
		{
		for (int i = a.digit_c - 1; i >= 0; i--) {
			for (int c = 7; c >= 0; c--)
			{
				int k = a.data[i] >> c;
				if (k & 1)
					printf("1");
				else
					printf("0");
			}
		}
		}
	}	
}