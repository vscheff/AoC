#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
	FILE *in_file = fopen(argv[1], "r"); char *line_ptr = NULL; size_t n = 0; int dial = 50, pw1 = 0, pw2 = 0;
	while ((getline(&line_ptr, &n, in_file)) != -1) {
		for (int i = 0; i < atoi(line_ptr+1); i++) { if (*line_ptr == 'L') dial = (dial-1+100)%100; else dial = (dial+1)%100; if (! dial) pw2++; }
		if (! dial) pw1++;}
	printf("Part 1: %d\nPart 2: %d\n", pw1, pw2); }
