#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[3];
	char left[3];
	char right[3];
} NODE_T;

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

	NODE_T cur_node = nodes[0], next;
	unsigned ins_num = 0;
	uint64_t count = 0;

	while (strncmp(cur_node.name, "ZZZ", 3)) {
		if (! instructions[ins_num]) ins_num = 0;

		if (instructions[ins_num] == 'L') memcpy(next.name, cur_node.left, 3);
		else memcpy(next.name, cur_node.right, 3);

		cur_node = *(NODE_T *)bsearch(&next, nodes, num_nodes, sizeof(NODE_T), &compar);
		count++;
		ins_num++;
	}

	free(instructions);
	free(nodes);

	printf("Steps = %lu\n", count);

	return 0;
}
