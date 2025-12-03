#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long check_range(long, long);

int main(int argc, char **argv){
	FILE *in_file = fopen(argv[1], "r");
	char *line_ptr = NULL;
	size_t n = 0;
	getline(&line_ptr, &n, in_file);

	fclose(in_file);

	long lower, upper, sum = 0;
	char *end_ptr = NULL, *str_ptr = strtok(line_ptr, ",");

	do {
		lower = strtol(str_ptr, &end_ptr, 10);
		upper = strtol(end_ptr + 1, NULL, 10);
		sum += check_range(lower, upper);
	} while ( (str_ptr = strtok(NULL, ",")) );

	free(line_ptr);

	printf("Sum = %ld\n", sum);

	return 0;
}

long check_range(long lower, long upper) {
	long sum = 0, first, last;
	char num[128], front[128], back[128];
	int len;

	for (long i = lower; i <= upper; i++) {
		len = sprintf(num, "%ld", i);

		if (len % 2) continue;

		memcpy(front, num, len / 2);
		front[len / 2] = 0;
		memcpy(back, num + len / 2, len / 2 + 1);

		first = strtol(front, NULL, 10);
		last = strtol(back, NULL, 10);

		if (first == last) sum += i;
	}

	return sum;
}
