#include <stdio.h>
#include <stdlib.h> /* for malloc */
#include <string.h> /* for strlen */

#define CRC4_MASK "10011"
#define XOR ^

int increment_binary (char* currentBinaryNum, int bitLength);

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Too few arguments. Usage:\ncrcc_stat (amount of bits) [f]\n\nAdding the 'f' flag will output to a file instead of stdout.\n");
        return -1;
    }

    _Bool printToFile = 0;
    for (int i = argc - 1; i > 0; i--) {
        if (*argv[i] == 'f') {
            printToFile = 1;
        }
    }

    int bitLength = atoi(argv[1]);
    /* plus 4 for the CRC states */
    bitLength += 4;
    int crcmask_len = strlen(CRC4_MASK);

    /* malloc yet one more byte for \0 marker */
    char *currentBinaryNum = malloc(bitLength * sizeof(char) + 1);
    char *checksumCalcBuffer = malloc(bitLength * sizeof(char) + 1);
    for (int i = 0; i < bitLength; i++) {
        currentBinaryNum[i] = '0';
        checksumCalcBuffer[i] = '0';
    }
    currentBinaryNum[bitLength] = '\0';
    checksumCalcBuffer[bitLength] = '\0';
    printf("Allocated %d Bytes of memory.\n", bitLength * 2 + 2);

    FILE *outfile = fopen("out.txt", "w");
    if (outfile == NULL) {
        printf("Error opening file!\n");
        return -2;
    }

    while (increment_binary(currentBinaryNum, bitLength - 5) == 1) {
        strcpy(checksumCalcBuffer, currentBinaryNum);
        int j = 0;

        /* input check */
        if (bitLength < crcmask_len) {
            printf("Input: %s is shorter than CRC mask.\n", currentBinaryNum);
        } else {
            if (printToFile == 1) {
                fprintf(outfile, "{'Input':%s,", currentBinaryNum);
            } else {
                printf("{'Input':%s,", currentBinaryNum);
            }
            /* j is the position of the first 1 in the string */
            while (j <= bitLength - crcmask_len) {
                if (checksumCalcBuffer[j] == '1') {

                    for (int i = 0; i < crcmask_len; i++) {
                        checksumCalcBuffer[j] = checksumCalcBuffer[j] XOR CRC4_MASK[i] ? '1' : '0';
                        j++;
                    }
                    j = 0;
                } else {
                    j++;
                }
            }

            if (printToFile == 1) {
                fprintf(outfile, "'Checksum':'");
                while (j < bitLength) {
                    fprintf(outfile, "%c", checksumCalcBuffer[j++]);
                }
                fprintf(outfile, "'}\n");
            } else {
                printf("'Checksum':'");
                while (j < bitLength) {
                    putchar(checksumCalcBuffer[j++]);
                }
                printf("'}\n");
            }
        }
    }

    free(currentBinaryNum);
    free(checksumCalcBuffer);
    return 0;
}

int increment_binary (char* currentBinaryNum, int bitLength) {
    /* find position of last 0 in char array */
    int lastZero = 0;
    for (int i = bitLength; i >= 0; i--) {
        if (currentBinaryNum[i] == '0') {
            lastZero = i;
            break;
        }
    }

    if (lastZero == 0 && currentBinaryNum[0] == '1') {
        printf("Done!\n");
        return -1;
    }

    currentBinaryNum[lastZero] = '1';

    for (int i = lastZero + 1; i <= bitLength; i++) {
        currentBinaryNum[i] = '0';
    }

    return 1;
}