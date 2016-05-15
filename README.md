# Efficient Cryptanalysis of Homophonic Substitution Ciphers

1. To compile:
make HomophonicCipher

2. To remove object files and built binary:
make clean

3. To run:
./HomophonicCipher <digram_statistics_file> <ciphertext_file> [<final_key_output_file>]

Example:
./HomophonicCipher zodiac408_solution.txt cipher340.txt pkey_340.txt
