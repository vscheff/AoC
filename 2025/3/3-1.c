#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL, target1, target2;
	size_t n = 0, sum = 0;
	FILE *in_file = fopen(argv[1], "r");
	
	while (getline(&line_ptr, &n, in_file) != -1) {
		target1 = '9';
loop1:
		for (unsigned i = 0; line_ptr[i] >= '0'; i++) {
			if (line_ptr[i] != target1) continue;
			
			target2 = '9';
loop2:
			for (unsigned j = i + 1; line_ptr[j] >= '0'; j++) {
				if (line_ptr[j] != target2) continue;
			
				printf("%c%c\n", line_ptr[i], line_ptr[j]);	
				sum += (line_ptr[i] - '0') * 10 + line_ptr[j] - '0';
				
				goto endloop;
			}

			if (target2 > '0') {
				target2--;
				goto loop2;
			}
		}

		target1--;
		goto loop1;
endloop:
	}

	fclose(in_file);
	free(line_ptr);

	printf("Sum = %lu\n", sum);

	return 0;
}
