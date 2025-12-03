#include <stdio.h>
#include <stdlib.h>

int compar(const void *a, const void *b) { return *(long *)a - *(long *)b; }

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr = NULL;
	size_t n = 0, length = 0;
	long *left = NULL, *right = NULL;

	FILE *in_file = fopen(argv[1], "r");
	
	while ( getline(&line_ptr, &n, in_file) != -1 ) {
		left = realloc(left, ++length * sizeof(*left));
		right = realloc(right, length * sizeof(*right));
		
		left[length - 1] = strtol(line_ptr, &end_ptr, 10);
		right[length - 1] = strtol(end_ptr + 3, NULL, 10);
	}

	fclose(in_file);

	qsort(left, length, sizeof(*left), &compar);
	qsort(right, length, sizeof(*right), &compar);

	long sum_diff = 0;
	unsigned occur;

	for (size_t i = 0; i < length; i++) {
		occur = 0;

		for (size_t j = 0; j < length; j++) {
			if (left[i] == right[j]) occur++;
			else if (left[i] < right[j]) break;
		}

		sum_diff += left[i] * occur;
	}

	free(left);
	free(right);

	printf("Sum = %ld\n", sum_diff);

	return 0;
}
