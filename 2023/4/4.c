#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN_COUNT 10
#define OUR_COUNT 25

int compar(const void *a, const void *b) { return *(long*)a - *(long*)b; }

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	long *win_nums = calloc(WIN_COUNT, sizeof(long));
	long *our_nums = calloc(OUR_COUNT, sizeof(long));

	char *line = NULL, *win_num = NULL, *our_num = NULL, *num_ptr = NULL;
	size_t n = 0;
	unsigned sum = 0;

	while (getline(&line, &n, infile) != -1) {
		our_num = line;
		strsep(&our_num, ":");
		while (*our_num == ' ') our_num++;		

		win_num = strsep(&our_num, "|");
		while (*our_num == ' ') our_num++;		
		
		unsigned count = 0;
		while (win_num) {
			num_ptr = strsep(&win_num, " ");
			if (win_num) while (*win_num == ' ') win_num++;
			win_nums[count] = atoi(num_ptr);
			count++;
		}

		count = 0;
		while (our_num) {
			num_ptr = strsep(&our_num, " ");
			if (our_num) while (*our_num == ' ') our_num++;
			our_nums[count] = atoi(num_ptr);
			count++;
		}

		unsigned score = 0;
		size_t nmemb = WIN_COUNT;
		for (unsigned i = 0; i < OUR_COUNT; i++) {
			if (lfind(&our_nums[i], win_nums, &nmemb, sizeof(long), &compar)) {
				if (score) score <<= 1;
				else score = 1;
			}
		}

		printf("Score = %u\n", score);

		sum += score;
	}

	fclose(infile);
	//free(line);
	//free(win_nums);
	//free(our_nums);

	printf("Sum = %u\n", sum);

	return 0;
}
