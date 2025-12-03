#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CACHE_T {
	unsigned long num;
	unsigned long blinks;
	size_t count;
} CACHE_T;

CACHE_T *cache = NULL;
size_t cache_size = 0;

size_t count(unsigned long, unsigned long);

int main(int argc, char **argv) {
	if (argc != 3) { fprintf(stderr, "Usage: %s <filepath> <blinks>\n", *argv); return -1; }

	char *line_ptr = NULL;
	size_t n = 0;
	unsigned long blinks = strtoul(argv[2], NULL, 10);
	FILE *in_file = fopen(argv[1], "r");

	getline(&line_ptr, &n, in_file);

	fclose(in_file);

	unsigned long *stones = NULL;
	size_t num_stones = 0;
	char *tok_ptr = strtok(line_ptr, " ");

	do {
		stones = realloc(stones, ++num_stones * sizeof(*stones));
		stones[num_stones - 1] = strtoul(tok_ptr, NULL, 10);
	} while ( (tok_ptr = strtok(NULL, " ")) );

	free(line_ptr);

	size_t total = 0;

	for (size_t i = 0; i < num_stones; i++) total += count(stones[i], blinks);

	free(stones);
	free(cache);

	printf("Num Stones = %lu\n", total);

	return 0;
}

int compare(const void *a, const void *b) { 
	CACHE_T cache_a = *(CACHE_T *)a, cache_b = *(CACHE_T *)b;
	
	if (cache_a.num < cache_b.num) return -1;
	if (cache_a.num > cache_b.num) return 1;
	if (cache_a.blinks < cache_b.blinks) return -1;
	if (cache_a.blinks > cache_b.blinks) return 1;

	return 0;
}

CACHE_T search_cache, *cache_hit;

size_t count(unsigned long num, unsigned long blinks) {
	if (! blinks) return 1;
	
	search_cache.num = num;
	search_cache.blinks = blinks;
	cache_hit = bsearch(&search_cache, cache, cache_size, sizeof(*cache), &compare);
	
	if (cache_hit) return cache_hit->count;

	size_t result;

	if (! num) result = count(1, blinks - 1);
	else {
		char buff[32];
		char left[32];
		sprintf(buff, "%lu\0", num);
		size_t len = strlen(buff), k;

		if (len % 2 == 0) {
			for (k = 0; k < len / 2; k++) left[k] = buff[k];

			left[k] = '\0';

			result = count(strtoul(left, NULL, 10), blinks - 1) + count(strtoul(buff + k, NULL, 10), blinks - 1);
		}
		else result = count(num * 2024, blinks - 1);
	}

	cache = realloc(cache, (cache_size + 1) * sizeof(*cache));
	cache[cache_size].num = num;
	cache[cache_size].blinks = blinks;
	cache[cache_size].count = result;
	cache_size++;
	
	qsort(cache, cache_size, sizeof(*cache), &compare);

	return result;
}
