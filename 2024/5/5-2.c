#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct UPDATE_T {
	unsigned len;
	unsigned *arr;
} UPDATE_T;

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");
	unsigned long **rules = NULL, num_rules = 0, num_updates = 0;
	UPDATE_T **updates = NULL;

	while (getline(&line_ptr, &n, in_file) != -1) {
		if (*line_ptr == '\n') break;

		rules = realloc(rules, (num_rules + 1) * sizeof(*rules));
		rules[num_rules] = malloc(2 * sizeof(**rules));
		rules[num_rules][0] = strtoul(line_ptr, &end_ptr, 10);
		rules[num_rules][1] = strtoul(end_ptr + 1, NULL, 10);
		num_rules++;
	}

	while (getline(&line_ptr, &n, in_file) != -1) {
		updates = realloc(updates, (num_updates + 1) * sizeof(*updates));
		updates[num_updates] = malloc(sizeof(**updates));
		updates[num_updates]->len = 1;
		updates[num_updates]->arr = malloc(sizeof(*(updates[0]->arr)));
		updates[num_updates]->arr[0] = strtoul(strtok(line_ptr, ","), NULL, 10);

		while ( (end_ptr = strtok(NULL, ",")) ) {
			updates[num_updates]->arr = realloc(updates[num_updates]->arr, ++(updates[num_updates]->len) * sizeof(*(updates[0]->arr)));
			updates[num_updates]->arr[updates[num_updates]->len - 1] = strtoul(end_ptr, NULL, 10);
		}

		num_updates++;
	}

	fclose(in_file);
	free(line_ptr);

	unsigned rule_safe, total = 0, temp;

	for (unsigned i = 0; i < num_updates; i++) {
		rule_safe = 1;

		for (unsigned j = 0; j < updates[i]->len - 1; j++) {
			for (unsigned k = 0; k < num_rules; k++) {
				if (rules[k][1] != updates[i]->arr[j]) continue;

				for (unsigned l = j + 1; l < updates[i]->len; l++) {
					if(rules[k][0] != updates[i]->arr[l]) continue; 
					
					rule_safe = 0;
					temp = updates[i]->arr[l];
					updates[i]->arr[l] = updates[i]->arr[j];
					updates[i]->arr[j] = temp;
					k = (unsigned)-1;
					break;
				}
			}
		}

		if (rule_safe) {
			free(updates[i]->arr);
			free(updates[i]);
			continue;
		}

		total += updates[i]->arr[updates[i]->len / 2];
		free(updates[i]->arr);
		free(updates[i]);
	}

	free(updates);

	for (unsigned i = 0; i < num_rules; i++) free(rules[i]);

	free(rules);

	printf("Total = %u\n", total);

	return 0;
}
