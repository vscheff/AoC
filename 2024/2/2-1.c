#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL, *str_ptr = NULL;
	size_t n = 0;
	int curr_num, prev_num, increasing, safe;
	unsigned num_safe = 0;

	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		str_ptr = strtok(line_ptr, " ");
		prev_num = atoi(str_ptr);
		increasing = -1;
		safe = 1;

		while ( (str_ptr = strtok(NULL, " ")) ) {
			curr_num = atoi(str_ptr);

			if (curr_num > prev_num) {
				if (! increasing) { safe = 0; break; }

				if (increasing == -1) increasing = 1;

				if (curr_num - prev_num > 3) { safe = 0; break; }
			}
			else if (curr_num < prev_num) {
				if (increasing == 1) { safe = 0; break; }

				if (increasing == -1) increasing = 0;

				if (prev_num - curr_num > 3) { safe = 0; break; }
			}
			else { safe = 0; break; }
		
			prev_num = curr_num;
		}

		if (safe) num_safe++;
	}

	printf("Num Safe = %u\n", num_safe);

	return 0;
}
