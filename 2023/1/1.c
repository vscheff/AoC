#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char * line_ptr = NULL;
	size_t n = 0, i = 0, sum = 0;
	ssize_t num_char = 0;

	while ( (num_char = getline(&line_ptr, &n, infile)) != -1 ) {
		for (i = 0; line_ptr[i] < '0' || line_ptr[i] > '9'; i++);
		
		sum += (line_ptr[i] - '0') * 10;

		for (i = num_char - 1; line_ptr[i] < '0' || line_ptr[i] > '9'; i--);

		sum += line_ptr[i] - '0';
	}

	free(line_ptr);

	printf("Sum = %u\n", sum);

	return 0;
}
