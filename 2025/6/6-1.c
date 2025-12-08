#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr;
	size_t n = 0, rows = 0, cols = 0;
	unsigned long **input = NULL, value;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) > 0) {
		if (*line_ptr == '*' || *line_ptr == '+') break;
		
		input = realloc(input, sizeof(*input) * (rows + 1));
		input[rows] = NULL;
		cols = 0;
		value = strtoul(line_ptr, &end_ptr, 10);

		for (;;) {
			input[rows] = realloc(input[rows], sizeof(**input) * ++cols);
			input[rows][cols - 1] = value;

			if (*end_ptr == '\n') break;

			value = strtoul(end_ptr + 1, &end_ptr, 10);
		}

		rows++;
	}

	fclose(in_file);

	unsigned long sum = 0, mult;
	char *operator = line_ptr;

	for (size_t i = 0; i < cols; i++) {
		if (*operator == '+') for (size_t j = 0; j < rows; j++) 
			sum += input[j][i];
		else {
			mult = 1;

			for (size_t j = 0; j < rows; j++) mult *= input[j][i];

			sum += mult;
		}
		
		do operator++; while (*operator == ' ');
	}

	free(line_ptr);

	for (size_t i = 0; i < rows; i++) free(input[i]);

	free(input);

	printf("Sum = %lu\n", sum);

	return 0;
}
