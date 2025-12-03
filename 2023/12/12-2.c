#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t solve(char *, int *);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, **map = NULL, *num_ptr = NULL, *str_ptr = NULL;
	size_t n = 0;
	unsigned map_height = 0;
	int **damaged = NULL;
	
	while (getline(&line, &n, infile) > 1) {
		map = realloc(map, (map_height + 1) * sizeof(*map));
		damaged = realloc(damaged, (map_height + 1) * sizeof(*damaged));
		num_ptr = line;
		line = strsep(&num_ptr, " ");
		map[map_height] = malloc((num_ptr - line) * 5 * sizeof(char));

		for (unsigned i = 0; i < 5; i++) {
			str_ptr = strcpy(map[map_height] + (num_ptr - line) * i, line);
			unsigned j;

			if (i < 4) {
				for (j = 0; str_ptr[j]; j++);
				str_ptr[j] = '?';
			}
		}

		str_ptr = num_ptr;

		unsigned num_damaged = 0;
		damaged[map_height] = NULL;

		while (str_ptr) {
			num_ptr = strsep(&str_ptr, ",");
			damaged[map_height] = realloc(damaged[map_height], ++num_damaged * sizeof(**damaged));
			damaged[map_height][num_damaged - 1] = atoi(num_ptr);
		}

		damaged[map_height] = realloc(damaged[map_height], (num_damaged * 5 + 1) * sizeof(**damaged));

		for (unsigned i = 1; i < 5; i++) {
			for (unsigned j = 0; j < num_damaged; j++)
				damaged[map_height][j + i * num_damaged] = damaged[map_height][j];
		}

		damaged[map_height][num_damaged * 5] = -1;

		map_height++;
	}

	free(line);
	fclose(infile);

	uint64_t sum = 0, result;

	for (unsigned i = 0; i < map_height; i++) {
		printf("Line %u of %u: %s\n", i + 1, map_height, map[i]);
		result = solve(map[i], damaged[i]);
		printf("\tResult = %lu\n", result);
		sum += result;
		free(map[i]);
		free(damaged[i]);
	}

	free(damaged);
	free(map);

	printf("Sum = %lu\n", sum);
	
	return 0;
}

typedef struct {
	uint64_t i;
	uint64_t j;
	uint64_t k;
} TUPLE_T;

TUPLE_T *keys = NULL;
uint64_t *values = NULL;
unsigned size = 0;

int get_index(TUPLE_T key) {
	for (unsigned i = 0; i < size; i++) {
		if (key.i == keys[i].i && key.j == keys[i].j) return i;
	}

	return -1;
}

void insert(TUPLE_T key, uint64_t value) {
	unsigned index = get_index(key);

	if (index != -1) { values[index] += value; return; }

	keys = realloc(keys, ++size * sizeof(*keys));
	values = realloc(values, size * sizeof(*values));
	memcpy(&keys[size - 1], &key, sizeof(key));
	values[size - 1] = value;
}

unsigned get(TUPLE_T key) {
	int index = get_index(key);

	if (index == -1) return -1;

	return values[index];
}

void clear() {
	free(keys);
	free(values);
	keys = NULL;
	values = NULL;
	size = 0;
}

uint64_t solve(char *line, int *damaged) {
	TUPLE_T tup = {0, 0, 0};
	insert(tup, 1);
	TUPLE_T *next = NULL;
	unsigned next_size = 0;
	uint64_t perm_count, group_id, group_amount;

	for (unsigned i = 0; line[i]; i++) {
		for (unsigned j = 0; j < size; j++) {
			perm_count = values[j];
			group_id = keys[j].i;
			group_amount = keys[j].j;

			if (line[i] != '#') {
				if (! group_amount) {
					tup.i = group_id;
					tup.j = group_amount;
					tup.k = perm_count;
					next = realloc(next, ++next_size * sizeof(*next));
					memcpy(&next[next_size - 1], &tup, sizeof(tup));
				}
				else if (group_amount == damaged[group_id]) {
					tup.i = group_id + 1;
					tup.j = 0;
					tup.k = perm_count;
					next = realloc(next, ++next_size * sizeof(*next));
					memcpy(&next[next_size - 1], &tup, sizeof(tup));
				}
			}
			if (line[i] != '.') {
				if (damaged[group_id] != -1 && group_amount < damaged[group_id]) {
					tup.i = group_id;
					tup.j = group_amount + 1;
					tup.k = perm_count;
					next = realloc(next, ++next_size * sizeof(*next));
					memcpy(&next[next_size - 1], &tup, sizeof(tup));
				}
			}
		}

		clear();

		for (unsigned i = 0; i < next_size; i++) {
			tup = (TUPLE_T){next[i].i, next[i].j};
			insert(tup, next[i].k);
		}

		free(next);
		next = NULL;
		next_size = 0;
	}

	unsigned dmg_len = 0;
	for ( ; damaged[dmg_len] != -1; dmg_len++);

	uint64_t sum = 0;

	for (unsigned i = 0; i < size; i++) {
		if (keys[i].i == dmg_len || (keys[i].i == dmg_len - 1 && keys[i].j == damaged[keys[i].i])) sum += values[i];
	}

	clear();

	return sum;
}

