#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, infile);
	
	fclose(infile);

	char *str_ptr = line, *seq_ptr = NULL;
	unsigned i, seq_sum, sum = 0;

	while (str_ptr) {
		seq_ptr = strsep(&str_ptr, ",");
		seq_sum = 0;

		for (i = 0; seq_ptr[i]; i++) {
			if (seq_ptr[i] == '\n') continue;

			seq_sum += seq_ptr[i];
			seq_sum *= 17;
			seq_sum %= 256;	
		}

		sum += seq_sum;
	}

	free(line);

	printf("Sum: %u\n", sum);

	return 0;
}
