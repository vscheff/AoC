#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compar(const void *a, const void *b) { return *(unsigned long *)a - *(unsigned long *)b;  }

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	unsigned long total = 0, arr[3];
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		arr[0] = strtoul(strtok(line_ptr, "x"), NULL, 10);
		arr[1]  = strtoul(strtok(NULL, "x"), NULL, 10);
		arr[2] = strtoul(strtok(NULL, "x"), NULL, 10);

		qsort(arr, 3, sizeof(*arr), &compar);

		total += 2 * (arr[0] + arr[1]) + arr[0] * arr[1] * arr[2];
	}

	fclose(in_file);
	free(line_ptr);

	printf("Total = %lu\n", total);

	return 0;
}
