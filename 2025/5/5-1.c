#include <stdio.h>
#include <stdlib.h>

typedef struct RANGE_T {
	unsigned long lower;
	unsigned long upper;
} RANGE_T;

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr;
	size_t n = 0, num_range = 0;
	RANGE_T *ranges = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) > 1) {
		ranges = realloc(ranges, sizeof(*ranges) * ++num_range);
		ranges[num_range - 1].lower = strtoul(line_ptr, &end_ptr, 10);
		ranges[num_range - 1].upper = strtoul(end_ptr + 1, NULL, 10);
	}

	unsigned long sum = 0, check;

	while (getline(&line_ptr, &n, in_file) > 1) {
		check = strtoul(line_ptr, NULL, 10);

		for (unsigned i = 0; i < num_range; i++) if (check >= ranges[i].lower && check <= ranges[i].upper) { sum++; break; }
	}

	fclose(in_file);
	free(line_ptr);
	free(ranges);

	printf("Sum = %lu\n", sum);

	return 0;
}
