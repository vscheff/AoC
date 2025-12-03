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
	long sum = 0;
	char num[128];
	int len;
	short flag;

	for (long i = lower; i <= upper; i++) {
		len = sprintf(num, "%ld", i);

		for (int j = 1; j <= len / 2; j++) {
			if (len % j) continue;

			flag = 0;

			for (int k = 1; k < len / j; k += 1) {
				if (strncmp(num, num + k * j, j)) {
					flag = 1;
					break;
				}
			}

			if (! flag) { sum += i; break; }
		}
	}

	return sum;
}
