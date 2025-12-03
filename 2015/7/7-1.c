#include <search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WIRE_T {
	char label[3];
	uint16_t power;
	unsigned powered;
	char *logic;
} WIRE_T;

typedef enum LOGIC_E { And, Or, Lshift, Rshift, Not, None } LOGIC_E;

int wire_compare(const void *a, const void *b) { return strcmp((*(WIRE_T *)a).label, (char *)b); }
int wire_sort(const void *a, const void *b) { return strcmp((*(WIRE_T *)a).label, (*(WIRE_T *)b).label); }

int main(int argc, char **argv) {
	char *line_ptr = NULL, *index;
	size_t n = 0, num_wires = 0, num_char;
	WIRE_T *wires = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		line_ptr[num_char - 1] = '\0';
		
		wires = realloc(wires, (num_wires + 1) * sizeof(*wires));
		wires[num_wires].powered = 0;
		wires[num_wires].logic = line_ptr;

		for (index = line_ptr + num_char - 2; *(index - 1) != ' '; index--) ;

		strcpy(wires[num_wires].label, index);
		num_wires++;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	unsigned all_powered = 0;
	char buff[32], *label1, *label2;
	LOGIC_E logic_type;
	uint16_t power1, power2;
	WIRE_T *wire1, *wire2;

	while (! all_powered) {
		all_powered = 1;

		for (size_t i = 0; i < num_wires; i++) {
			if (wires[i].powered) continue;

			all_powered = 0;

			strcpy(buff, wires[i].logic);

			if (! strncmp(buff, "NOT", 3)) logic_type = Not;
			else if (strstr(buff, "AND")) logic_type = And;
			else if (strstr(buff, "OR")) logic_type = Or;
			else if (strstr(buff, "LSHIFT")) logic_type = Lshift;
			else if (strstr(buff, "RSHIFT")) logic_type = Rshift;
			else logic_type = None;

			if (logic_type == None) {
				if (*buff >= '0' && *buff <= '9') power1 = (uint16_t)strtoul(buff, NULL, 10);
				else {
					wire1 = lfind(strtok(buff, " "), wires, &num_wires, sizeof(*wires), &wire_compare);
					
					if (! wire1->powered) continue;

					power1 = wire1->power;
				}

				wires[i].power = power1;
				wires[i].powered = 1;
			}
			else if (logic_type == Not) {
				wire1 = lfind(strtok(buff + 4, " "), wires, &num_wires, sizeof(*wires), &wire_compare);
				
				if (! wire1->powered) continue;

				wires[i].power = ~ wire1->power;
				wires[i].powered = 1;
			}
			else {
				label1 = strtok(buff, " ");
				
				if (*label1 >= '0' && *label1 <= '9') power1 = strtoul(label1, NULL, 10);
				else {
					wire1 = lfind(label1, wires, &num_wires, sizeof(*wires), &wire_compare);

					if (! wire1->powered) continue;

					power1 = wire1->power;
				}

				strtok(NULL, " ");
				label2 = strtok(NULL, " ");

				if (*label2 >= '0' && *label2 <= '9') power2 = strtoul(label2, NULL, 10);
				else {
					wire2 = lfind(label2, wires, &num_wires, sizeof(*wires), &wire_compare);

					if (! wire2->powered) continue;

					power2 = wire2->power;
				}

				switch (logic_type)
				{
				case And: 
					wires[i].power = power1 & power2;
					break;
				case Or:
					wires[i].power = power1 | power2;
					break;
				case Lshift:
					wires[i].power = power1 << power2;
					break;
				case Rshift:
					wires[i].power = power1 >> power2;
					break;
				}

				wires[i].powered = 1;
			}
		}
	}

	qsort(wires, num_wires, sizeof(*wires), &wire_sort);

	for (size_t i = 0; i < num_wires; i++) 	printf("%s: %u\n", wires[i].label, wires[i].power);

	free(wires);

	return 0;
}
