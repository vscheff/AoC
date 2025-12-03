#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *in_file = fopen(argv[1], "r");
	char *line_ptr = NULL;
	size_t n = 0;
	int dial = 50, password = 0;

	while ((getline(&line_ptr, &n, in_file)) != -1) {
		int rotation = atoi(line_ptr + 1);

		while (rotation > 99) {
			rotation -= 100;
			password++;
		}

		if (*line_ptr == 'L') {
			if (rotation > dial) {
				if (dial) password++;
				dial = 100 - (rotation - dial);
			}
			else {
				dial -= rotation;
				if (! dial) password++;
			}
		}
		else {
			dial += rotation;
			
			if (dial > 99) {
				dial -= 100;
				password++;
			}
		}
	}

	fclose(in_file);
	free(line_ptr);

	printf("Password: %d\n", password);

	return 0;
}
