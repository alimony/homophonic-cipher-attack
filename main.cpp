//
// main.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus.magnuson@gmail.com>
//

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "homophonic_cipher_matrix.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    char *buffer_d;
    char *buffer_e;
    int distinct[TOTAL_ASCII_COUNT];
    int cipher_symbols_count = 0;

    if (argc < 2) {
        std::cout << "USAGE: ./HomophonicCipher <ciphertext_file> [<digram_statistics_file>] [<final_key_output_file>]\n";
        exit(0);
    }

    text_matrix e_matrix(E_LETTER_COUNT);

    // A second optional argument can be passed to base the digram frequencies
    // comparison on the passed file contents rather than on English.
    if (argc >= 3) {
        buffer_e = get_file_to_buffer(argv[2]);
        if (buffer_e == NULL) {
            std::cout << "\nbuffer_e is NULL\n";
            return 1;
        }
        e_matrix.update(buffer_e);
    } else {
        // Just use standard English digram frequencies for comparison.
        e_matrix.update_hardcoded();
    }

    // The first argument passed is a text file containing the ciphertext.
    buffer_d = get_file_to_buffer(argv[1]);
    if (buffer_d == NULL) {
        std::cout << "\nbuffer_d is NULL\n";
        return 1;
    }

    cipher_symbols_count = get_distinct_symbols(buffer_d, distinct);
    homophonic_cipher_matrix d_matrix(cipher_symbols_count);
    d_matrix.update(buffer_d, distinct);
    d_matrix.solve_cipher(e_matrix, buffer_d);

    // A third optional argument can be passed to save the resulting key to a
    // file before exiting.
    if (argc == 4) {
        d_matrix.copy_final_key_file(argv[3]);
    }

    printf("\n");

    delete [] buffer_e;
    delete [] buffer_d;

    return 0;
}
