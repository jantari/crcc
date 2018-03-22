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

    FILE *outputStream = stdout;
    if (argc > 2 && *argv[2] == 'f') {
        outputStream = fopen("out.txt", "w");
        if (outputStream == NULL) {
            printf("Error opening output file!\n");
            return -2;
        }
    }

    int j = 0;

    while (increment_binary(currentBinaryNum, bitLength - 5) == 1) {
        strcpy(checksumCalcBuffer, currentBinaryNum);
        j = 0;

        /* input check */
        if (bitLength < crcmask_len) {
            printf("Input: %s is shorter than CRC mask.\n", currentBinaryNum);
        } else {
            fprintf(outputStream, "{'Input':%s,", currentBinaryNum);

            /* j is the position of the first 1 in the string */
            while (j <= bitLength - crcmask_len) {
                if (checksumCalcBuffer[j] == '1') {
                    for (int i = 0; i < crcmask_len; i++) {
                        checksumCalcBuffer[j] = checksumCalcBuffer[j] XOR CRC4_MASK[i] ? '1' : '0';
                        j++;
                    }
                    /* Reset j to the position before what was currently changed so we don't have to traverse the whole string to find the next '1' */
                    j = j - crcmask_len;
                } else {
                    j++;
                }
            }

            fprintf(outputStream, "'Checksum':'");
            while (j < bitLength) {
                fprintf(outputStream, "%c", checksumCalcBuffer[j++]);
            }
            fprintf(outputStream, "'}\n");
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