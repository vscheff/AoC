#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[3];
	char left[3];
	char right[3];
} NODE_T;

uint64_t lcm(uint64_t, unsigned);

int compar(const void *a, const void *b) { return strncmp((*(NODE_T *)a).name, (*(NODE_T *)b).name, 3); }

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, *instructions = NULL;
	size_t n = 0;

	ssize_t num_char = getline(&instructions, &n, infile);
	instructions[num_char - 1] = '\0';
	
	n = 0;
	getline(&line, &n, infile);

	NODE_T *nodes = NULL;
	unsigned num_nodes = 0;

	while (getline(&line, &n, infile) > 1) {
		nodes = realloc(nodes, (num_nodes + 1) * sizeof(*nodes));
		memcpy(nodes[num_nodes].name, line, 3);
		memcpy(nodes[num_nodes].left, &line[7], 3);
		memcpy(nodes[num_nodes].right, &line[12], 3);
		num_nodes++;
	}

	fclose(infile);
	free(line);

	qsort(nodes, num_nodes, sizeof(NODE_T), &compar);

	NODE_T **paths = NULL;
	unsigned num_paths = 0;

	for (unsigned i = 0; i < num_nodes; i++) {
		if (nodes[i].name[2] != 'A') continue;
		
		paths = realloc(paths, ++num_paths * sizeof(*paths));
		paths[num_paths - 1] = &nodes[i];
	}

	unsigned ins_num, count;
	NODE_T next;
	unsigned *counts = malloc(num_paths * sizeof(unsigned));

	for (unsigned i = 0; i < num_paths; i++) {
		ins_num = 0;
		count = 0;
		while (paths[i]->name[2] != 'Z') {
			if (! instructions[ins_num]) ins_num = 0;
			if (instructions[ins_num] == 'L') memcpy(next.name, paths[i]->left, 3);
			else memcpy(next.name, paths[i]->right, 3);

			paths[i] = bsearch(&next, nodes, num_nodes, sizeof(*nodes), &compar);
			count++;
			ins_num++;
		}

		counts[i] = count;
	}
	
	uint64_t least_cm = counts[0];

	for (unsigned i = 1; i < num_paths; i++) {
		least_cm = lcm(least_cm, counts[i]);
	}

	free(instructions);
	free(nodes);
	free(paths);
	free(counts);

	printf("Steps = %lu\n", least_cm);

	return 0;
}

uint64_t gcd(uint64_t a, uint64_t b) {
	if (! b) return a;

	return gcd(b, a % b);
}

uint64_t lcm(uint64_t a, unsigned b) {
	return (a / gcd(a, b)) * b;
}
