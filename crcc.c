#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h> /* for strlen */

#define CRC4_MASK "10011"
#define XOR ^

int main (int argc, char *argv[]) {

    if (argc < 2) {
        printf("Too few arguments.\n");
        return -1;
    }

    int input_len = strlen(argv[1]);
    int crcmask_len = strlen(CRC4_MASK);
    int j = 0;

    /* input check */
    if (input_len < crcmask_len) {
        printf("Input shorter than CRC mask.\n");
        return -2;
    }

    /* j is the position of the first 1 in the string */
    while (j <= input_len - crcmask_len) {
        if (argv[1][j] == '1') {
            for (int i = 0; i < crcmask_len; i++) {
                argv[1][j] = argv[1][j] XOR CRC4_MASK[i] ? '1' : '0';
                j++;
            }
            j = 0;
        } else {
            j++;
        }
    }

    while (j < input_len) {
        putchar(argv[1][j++]);
    }
    putchar('\n');
    
    return 0;
}