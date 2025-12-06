#include <stdio.h>
#include <stdlib.h>

typedef struct RANGE_T {
	unsigned long lower;
	unsigned long upper;
} RANGE_T;

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr;
	size_t n = 0;
	int num_range = 0;
	RANGE_T *ranges = NULL;
	short found;
	unsigned long lower, upper, sum = 0;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) > 1) {
		lower = strtoul(line_ptr, &end_ptr, 10);
		upper = strtoul(end_ptr + 1, NULL, 10);

		found = 0;

		for (int i = 0; i < num_range; i++) {
			if (lower >= ranges[i].lower && lower <= ranges[i].upper){
				lower = ranges[i].upper + 1;
				found = 1;
			}
			if (upper >= ranges[i].lower && upper <= ranges[i].upper) {
				upper = ranges[i].lower - 1;
				found = 1;
			}
			
			if (upper < lower) break;
			
			if (found) {
				found = 0;
				i = -1;
			}
		}

		printf("Lower: %lu\tUpper: %lu\t", lower, upper);
		if (upper >= lower) {
			sum += upper - lower + 1;
			ranges = realloc(ranges, sizeof(*ranges) * ++num_range);
			ranges[num_range - 1].lower = lower;
			ranges[num_range - 1].upper = upper;
		}
		printf("Sum: %lu\n\n", sum);
	}

	fclose(in_file);
	free(line_ptr);
	free(ranges);

	printf("Sum = %lu\n", sum);

	return 0;
}
