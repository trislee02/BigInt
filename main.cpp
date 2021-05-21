#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "StringProcess.h"
#include "BigInt.h"
#include "StringProcess.h"

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("Error: Missing arguments\n");
		return 1;
	}
	char* input_name = argv[1];
	char* output_name = argv[2];

	srand(time(0));
	char* origin;
	char** lines;
	int text_size, lines_count;
	readfile(input_name, origin, text_size);
	splitstr(origin, lines, lines_count, '\n');
	bigint a = init(), b = init(), ans = init();
	FILE* outfile = fopen(output_name, "w");
	if (outfile == NULL) {
		printf("Error: Unable to write output!\n");
		return 1;
	}
	for (int i = 0; i < lines_count; i++) {
		clock_t begin = clock();
		printf("Status: Processing... - ");
		int myargv_count;
		char** myargv;
		splitstr(lines[i], myargv, myargv_count, ' ');
		int base = atoi(myargv[0]);
		if (myargv_count == 4) {
			if (strcmp(myargv[2], "+") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a + b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "-") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a - b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "*") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a * b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "%") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a % b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "/") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a / b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], ">>") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = shiftright(a, b);
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "<<") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = shiftright(a, b);
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[1], "min") == 0) {
				a = BigInt(myargv[2], base);
				b = BigInt(myargv[3], base);
				ans = min(a, b);
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[1], "max") == 0) {
				a = BigInt(myargv[2], base);
				b = BigInt(myargv[3], base);
				ans = max(a, b);
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[1], "pow") == 0) {
				a = BigInt(myargv[2], base);
				b = BigInt(myargv[3], base);
				ans = pow(a, b);
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "&") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a & b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "|") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a | b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[2], "^") == 0) {
				a = BigInt(myargv[1], base);
				b = BigInt(myargv[3], base);
				ans = a ^ b;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
		}

		else if (myargv_count == 3) {
			if (strcmp(myargv[0], "2") == 0) {
				if (strcmp(myargv[1], "10") == 0) {
					a = BigInt(myargv[2], base);
					fprintf(outfile, "%s\n", to_string(a, 10));
				}
			}
			else if (strcmp(myargv[0], "10") == 0) {
				if (strcmp(myargv[1], "2") == 0) {
					a = BigInt(myargv[2], base);
					fprintf(outfile, "%s\n", to_string(a, 2));
				}
			}
			if (strcmp(myargv[1], "~") == 0) {
				a = BigInt(myargv[2], base);
				ans = ~a;
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[1], "abs") == 0) {
				a = BigInt(myargv[2], base);
				ans = abs(a);
				fprintf(outfile, "%s\n", to_string(ans, base));
			}
			else if (strcmp(myargv[1], "to_base32") == 0) {
				a = BigInt(myargv[2], base);
				fprintf(outfile, "%s\n", to_string(a, 32));
			}
			else if (strcmp(myargv[1], "to_base58") == 0) {
				a = BigInt(myargv[2], base);
				fprintf(outfile, "%s\n", to_string(a, 58));
			}
			else if (strcmp(myargv[1], "to_base64") == 0) {
				a = BigInt(myargv[2], base);
				fprintf(outfile, "%s\n", to_string(a, 64));
			}
			else if (strcmp(myargv[1], "digits") == 0) {
				a = BigInt(myargv[2], base);
				fprintf(outfile, "%d\n", digits(a));
			}
			else if (strcmp(myargv[1], "is_prime") == 0) {
				a = BigInt(myargv[2], base);
				if (is_prime(a))
					fprintf(outfile, "True\n");
				else
					fprintf(outfile, "False\n");
			}
		}
		printf("Time spent: %lf\n", ((double)(clock() - begin) / CLOCKS_PER_SEC));
	}
	dispose(a);
	dispose(b);
	dispose(ans);
	delete[] origin;
	delete[] lines;
	fclose(outfile);
	printf("Status: Succeed.\n");
	return 0;
}
