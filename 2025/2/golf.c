#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv){
    char *line_ptr = NULL;
    size_t n = 0;
    getline(&line_ptr, &n, fopen(argv[1], "r"));
    long lower, upper, sum1 = 0, sum2 = 0, master, flag1, flag2;
    char *end_ptr = NULL, *str_ptr = strtok(line_ptr, ","), num[128], front[128], back[128];
    do { for (long i = strtol(str_ptr, &end_ptr, 10); i <= strtol(end_ptr + 1, NULL, 10); i++) {
            argc = sprintf(num, "%ld", i);
            flag2 = 0;
            for (int j = 1; j <= argc / 2; j++) { if (argc % j) continue;
                flag1 = 0;
                for (int k = 1; k < argc / j; k += 1) { if (strncmp(num, num + k * j, j)) { flag1 = 1; break; } }
                if (! flag1) { if (argc % 2 == 0 && j == argc / 2) sum1 += i; if (! flag2) { sum2 += i; flag2 = 1; } } } }
    } while ( (str_ptr = strtok(NULL, ",")) );
    printf("%ld\n%ld\n", sum1, sum2); }