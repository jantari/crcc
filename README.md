# crcc
Calculate CRC Checksums of binary input

`crcc` takes a binary number as input and calculates its CRC4 checksum.

`crcc_stat` takes a decimal number X as input and will calculate the CRC4 checksums of ALL binary numbers up to the highest possible with X digits. Outputs JSON. Specifying `f` will redirect the output to a file.

Example:

`crcc_stat 18 f`