#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BOX 256

typedef struct {
	char *label;
	unsigned foc_len;
} LENSE_T;

typedef struct {
	unsigned length;
	LENSE_T *lenses;
} BOX_T;

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, infile);
	
	fclose(infile);

	char *str_ptr = line, *seq_ptr = NULL, dash, *label = NULL, replace;
	unsigned i, seq_sum, sum = 0, focal;

	BOX_T *boxes = calloc(NUM_BOX, sizeof(BOX_T));

	while (str_ptr) {
		seq_ptr = strsep(&str_ptr, ",");
		seq_sum = 0;

		dash = 0;

		for (i = 0; seq_ptr[i]; i++) {
			switch (seq_ptr[i]) {
				case '\n': continue;
				case '-': dash = 1;
				case '=':
					label = malloc((i + 1) * sizeof(char));
					memcpy(label, seq_ptr, (i + 1) * sizeof(char));
					label[i] = '\0';
					focal = strtoul(&seq_ptr[i + 1], NULL, 10);
					
					goto exit_loop;
				default:
					break;
			}

			seq_sum += seq_ptr[i];
			seq_sum *= 17;
			seq_sum %= 256;	
		}

	exit_loop:

		if (dash) {
			for (i = 0; i < boxes[seq_sum].length; i++) {
				if (strcmp(label, boxes[seq_sum].lenses[i].label)) continue;

				free(boxes[seq_sum].lenses[i].label);

				memmove(&boxes[seq_sum].lenses[i], &boxes[seq_sum].lenses[i + 1], (boxes[seq_sum].length - i - 1) * sizeof(LENSE_T));
				boxes[seq_sum].length--;
				break;
			}
		}
		else {
			replace = 0;

			for (i = 0; i < boxes[seq_sum].length; i++) {
				if (strcmp(label, boxes[seq_sum].lenses[i].label)) continue;

				boxes[seq_sum].lenses[i].foc_len = focal;
				replace = 1;

				break;
			}

			if (! replace) {
				boxes[seq_sum].lenses = realloc(boxes[seq_sum].lenses, ++boxes[seq_sum].length * sizeof(LENSE_T));
				boxes[seq_sum].lenses[boxes[seq_sum].length - 1].label = strdup(label);
				boxes[seq_sum].lenses[boxes[seq_sum].length - 1].foc_len = focal;
			}
		}

		free(label);
	}

	free(line);

	unsigned power;

	for (i = 0; i < NUM_BOX; i++) {
		for (unsigned j = 0; j < boxes[i].length; j++) {
			power = (i + 1) * (j + 1) * boxes[i].lenses[j].foc_len;
			printf(" - %s: %u * %u * %u = %u\n", boxes[i].lenses[j].label, i + 1, j + 1, boxes[i].lenses[j].foc_len, power);
			sum += power;
			free(boxes[i].lenses[j].label);
		}

		free(boxes[i].lenses);
	}

	free(boxes);

	printf("Sum: %u\n", sum);

	return 0;
}
