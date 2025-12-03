#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int64_t get_next(int64_t *, unsigned);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, *str_ptr, *num_ptr;
	size_t n = 0;
	int64_t *sequence, sum = 0;
	unsigned seq_len;

	while (getline(&line, &n, infile) > 1) {
		str_ptr = line;
		sequence = NULL;
		seq_len = 0;
		
		while (str_ptr) {
			num_ptr = strsep(&str_ptr, " ");
			sequence = realloc(sequence, ++seq_len * sizeof(*sequence));
			sequence[seq_len - 1] = strtol(num_ptr, NULL, 10);
		}
		
		sum += get_next(sequence, seq_len);

		free(sequence);
	}

	fclose(infile);
	free(line);

	printf("Sum = %ld\n", sum);
	
	return 0;
}

int64_t get_next(int64_t *sequence, unsigned seq_len) {
	int64_t *new_seq = malloc((seq_len - 1) * sizeof(int64_t));
	unsigned all_zero = 1;

	for (unsigned i = 0; i < seq_len - 1; i++) {
		if ( (new_seq[i] = sequence[i + 1] - sequence[i]) ) all_zero = 0;
	}

	int64_t result = 0;

	if (! all_zero) result = get_next(new_seq, seq_len - 1);

	free(new_seq);
	
	result += sequence[seq_len - 1];

	return result;
}
