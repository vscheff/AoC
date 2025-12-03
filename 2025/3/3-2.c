#include <stdio.h>
#include <stdlib.h>

size_t get_largest(char *, ssize_t, unsigned, ssize_t);

int main(int argc, char **argv) {
	char *line_ptr = NULL, target1, target2;
	size_t n = 0, sum = 0;
	ssize_t len;
	FILE *in_file = fopen(argv[1], "r");
	
	while ( (len = getline(&line_ptr, &n, in_file)) != -1) {
		if (line_ptr[len - 1] == '\n') len--;

		sum += get_largest(line_ptr, len, 0, 12);
	}

	fclose(in_file);
	free(line_ptr);

	printf("Sum = %lu\n", sum);

	return 0;
}

short FLAG;

size_t get_largest(char *line, ssize_t len, unsigned pos, ssize_t digit) {
	FLAG = 0;

	if (digit == 0) {
		return 0;
	}
	
	char target = '9';

loop:
	for (unsigned i = pos; line[i] >= '0' && i <= len - digit; i++) {
		if (line[i] == target) {
			size_t result = get_largest(line, len, i + 1, digit - 1);
			
			if (FLAG == 0) {
				size_t value = target - '0';
				
				for (unsigned j = 1; j < digit; j++) value *= 10;

				return value + result;
			}
		}
	}

	if (target > '0') {
		target--;
		goto loop;
	}

	FLAG = 1;

	return 0;
}
