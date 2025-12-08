#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned digits_remaining(ssize_t, char**, ssize_t);

int main(int argc, char **argv) {
	char *line_ptr = NULL, **input = NULL;
	size_t n = 0;
	ssize_t len, rows = 0;
	FILE *in_file = fopen(argv[1], "r");

	while ( (len = getline(&line_ptr, &n, in_file)) > 0) {
		if (*line_ptr == '*' || *line_ptr == '+') break;
		
		input = realloc(input, sizeof(*input) * ++rows);
		input[rows - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	unsigned long digit, value, dig_val, mult, sum = 0;
	ssize_t offset;

	for (ssize_t i = 0; i < len - 1; i++) {
		if (line_ptr[i] != '*' && line_ptr[i] != '+') continue;

		offset = 0;

		while (digits_remaining(rows, input, i + offset)) {
			digit = 0;
			value = 0;
			
			for (ssize_t j = rows - 1; j >= 0; j--) {
				if (isspace(input[j][i + offset])) continue;

				dig_val = input[j][i + offset] - '0';

				for (unsigned long k = 0; k < digit; k++) dig_val *= 10;

				value += dig_val;
				digit++;
			}

			if (line_ptr[i] == '+') sum += value;
			else {
				if (! offset) mult = value;
				else mult *= value;
			}

			offset++;
		}

		if (line_ptr[i] == '*') sum += mult;
	}

	free(line_ptr);

	for (size_t i = 0; i < rows; i++) free(input[i]);

	free(input);

	printf("Sum = %lu\n", sum);

	return 0;
}

unsigned digits_remaining(ssize_t rows, char **input, ssize_t pos) {
	for (size_t i = 0; i < rows; i++) if (! isspace(input[i][pos])) return 1;

	return 0;
}
