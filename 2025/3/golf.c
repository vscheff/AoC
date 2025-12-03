#include <stdio.h>
#include <stdlib.h>
short FLAG;
size_t get_largest(char *line, ssize_t len, unsigned pos, ssize_t digit) {
	FLAG = 0;
	if (digit == 0) return 0;
	char target = '9';
loop:   for (unsigned i = pos; line[i] >= '0' && i <= len - digit; i++) { if (line[i] == target) {
			size_t result = get_largest(line, len, i + 1, digit - 1);
			if (FLAG == 0) {
				size_t value = target - '0';
				for (unsigned j = 1; j < digit; j++) value *= 10;
				return value + result; } } }
	if (target > '0') { target--; goto loop; }
	FLAG = 1;
	return 0; }
int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0, sum1 = 0, sum2 = 0;;
	ssize_t len, digit;
	FILE *in_file = fopen(argv[1], "r");
	while ( (len = getline(&line_ptr, &n, in_file)) != -1) { if (line_ptr[len - 1] == '\n') len--;
		sum1 += get_largest(line_ptr, len, 0, 2);
		sum2 += get_largest(line_ptr, len, 0, 12); }
	printf("%lu\n%lu\n", sum1, sum2);
	return 0; }
