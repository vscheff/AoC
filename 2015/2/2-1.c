#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	unsigned long l, w, h, s1, s2, s3, extra, total = 0;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		l = strtoul(strtok(line_ptr, "x"), NULL, 10);
		w = strtoul(strtok(NULL, "x"), NULL, 10);
		h = strtoul(strtok(NULL, "x"), NULL, 10);

		s1 = l * w;
		s2 = l * h;
		s3 = w * h;

		if (s1 <= s2 && s1 <= s3) extra = s1;
		else if (s2 <= s1 && s2 <= s3) extra = s2;
		else extra = s3;

		total += 2 * (s1 + s2 + s3) + extra;
	}

	fclose(in_file);
	free(line_ptr);

	printf("Total = %lu\n", total);

	return 0;
}
