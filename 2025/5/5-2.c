#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct RANGE_T {
	unsigned long lower;
	unsigned long upper;
} RANGE_T;

int compar(const void *a, const void *b) { 
	unsigned long a_low = ((RANGE_T *)a)->lower, b_low = ((RANGE_T *)b)->lower;

	if (a_low < b_low) return -1;
	if (a_low > b_low) return 1;

	return 0;	
}

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

	fclose(in_file);
	free(line_ptr);

	qsort(ranges, num_range, sizeof(*ranges), &compar);

	for (size_t i = 0; i < num_range - 1; ) {
		if (ranges[i].upper >= ranges[i + 1].lower) {
			if (ranges[i].upper < ranges[i + 1].upper) ranges[i].upper = ranges[i + 1].upper;
			if (i < num_range - 2) 
				memmove(ranges + i + 1, ranges + i + 2, sizeof(*ranges) * (num_range - i - 2));
			num_range--;
		}
		else i++;
	}

	unsigned long sum = 0;

	for (size_t i = 0; i < num_range; i++) sum += ranges[i].upper - ranges[i].lower + 1;

	free(ranges);

	printf("Sum = %lu\n", sum);

	return 0;
}
