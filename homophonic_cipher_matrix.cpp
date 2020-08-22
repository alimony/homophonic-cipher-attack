//
// homophonic_cipher_matrix.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#include "homophonic_cipher_matrix.h"

#include <cstdlib>
#include <iostream>
#include <random>
#include <cstring>

extern int freq_distribution_final[130][27];

homophonic_cipher_matrix::homophonic_cipher_matrix(int count):text_matrix(count) {
    int i, j;

    letter_count = count;

    putative_key = new int[letter_count];
    if (putative_key == NULL) {
        std::cout << "new failed for putative_key in homophonic_cipher_matrix";
        return;
    }

    matrix = new int*[letter_count];
    if (matrix) {
        i = 0;
        while (i < letter_count) {
            matrix[i] = new int[letter_count];
            if (matrix[i] == NULL) {
                std::cout << "new failed for matrix[" << i << "] in homophonic_cipher_matrix";
                // Delete all allocated memory
                delete [] putative_key;
                for (j = 0; j < i; j++) {
                    delete [] matrix[j];
                }
                delete [] matrix;
                return;
            }
            i++;
        }
    } else {
        std::cout << "new failed for matrix in homophonic_cipher_matrix";
        delete [] putative_key;
        return;
    }

    for (i = 0; i < letter_count; i++) {
        character_frequency[i].frequency = 0;
        character_frequency[i].character = -1;
        putative_key[i] = -1;
        for (j = 0; j < letter_count; j++) {
            matrix[i][j] = 0;
        }
    }

    for (i = 0; i < 256; i++) {
        letter_mapping[i] = -1;
    }
}

homophonic_cipher_matrix::~homophonic_cipher_matrix() {
    delete [] putative_key;

    for (int i = 0; i < letter_count; i++) {
        delete [] matrix[i];
    }

    delete [] matrix;
}

void homophonic_cipher_matrix::display() {
    int i, j;
    int temp;
    char ch;

    std::cout << "\n\nletter_count : " << letter_count << "\n";
    std::cout << "\n\nDisplaying Cipher Symbols Frequency\n";

    std::cout << "\nLetter Mapping:\n";
    for (i = 0; i < 256; i++) {
        if (letter_mapping[i] != -1) {
            ch = i;
            std::cout << i << ":" << ch << ":" << letter_mapping[i] << "|";
        }
    }

    std::cout << "\n\nCharacter Frequency:\n";
    for (i = 0; i < letter_count; i++) {
        ch = character_frequency[i].character;
        std::cout << ch << ":" << character_frequency[i].frequency << "|";
    }

    std::cout << "\n\nDisplaying Cipher Digram Frequency:\n    ";
    for (i = 0; i < letter_count; i++) {
        if (i < 10) {
            std::cout << "0";
        }
        std::cout << i << " ";
    }

    std::cout << "\n    ";

    temp = (letter_count * 3);
    for (i = 0; i < temp; i++) {
        std::cout << "-";
    }

    for (i = 0; i < letter_count; i++) {
        if (i < 10) {
            std::cout << "\n0" << i << " |";
        } else {
            std::cout << "\n" << i << " |";
        }
        for (j = 0; j < letter_count; j++) {
            if (matrix[i][j] < 10) {
                std::cout << "0";
            }
            std::cout << matrix[i][j] << " ";
        }
    }

    std::cout << "\n\nPutative Key:\n";
    for (i = 0; i < letter_count; i++) {
        ch = 'a' + putative_key[i];
        char ch1 = character_frequency[i].character;
        std::cout << i << ":" << ch1 << ":" << putative_key[i] << ":" << ch << "|";
    }
    std::cout << "\n";
}

void homophonic_cipher_matrix::display_putative_key() {
    display_key(putative_key);
}

void homophonic_cipher_matrix::display_key(int key[]) {
    std::cout << "\nKey\n";
    for (int i = 0; i < letter_count; i++) {
        char ch = 'a' + key[i];
        char ch1 = character_frequency[i].character;
        std::cout << i << ":" << ch1 << ":" << key[i] << ":" << ch << "|";
    }
    std::cout << "\n";
}

void homophonic_cipher_matrix::update(char text_buffer[], int distinct[256]) {
    int i = 0;
    int j = 0;
    int prev = -1;

    text_len = strlen(text_buffer);
    double text_length = static_cast<double>(text_len);

    /* For alphabets only
     * 1. distinct array is already filled with the counts of all 256 ASCII characters
     *    present in the text_buffer. For example "ABA" will end up as
     *    distinct[65] = 2, distinct[66] = 1, & distinct[68] = 0
     * 2. Using distinct[] array, letter_mappingi[256] & character_frequency[letter_count] are filled
     *    as follows:
     *    a. letter_mapping[i] - letter_mapping is of size 256 to contain all
     *    mappings from ASCCI characters to numeric indices in
     *    character_frequency. 'i' is the ASCII character
     *    b. character_frequency[j] - character_frequency is of size
     *    letter_frequency. i.e. number of distinct symbols in cipher text.
     *    example: text_buffer="ABA"
     *    letter_mapping[65] = 0 & letter_mapping[66] = 1
     *    character_frequency[0].character = 65 &
     *    character_frequency[0].frequency = 2
     *    character_frequency[1].character = 66 &
     *    character_frequency[1].frequency = 1
     */
    for (i = 0, j = 0; i < TOTAL_ASCII_COUNT && j < letter_count; i++) {
        if (distinct[i]) {
            if (((i >= 65) && (i <= 90)) || ((i >= 97) && (i <= 122))) {
                letter_mapping[i] = j;
                character_frequency[j].frequency = (distinct[i] / text_length) * 10000;
                character_frequency[j].character = i;
                j++;
            }
        }
    }

    // For Non alphabet
    for (i = 0; i < TOTAL_ASCII_COUNT && j < letter_count; i++) {
        if (distinct[i]) {
            if ((i < 65) || ((i > 90) && (i < 97)) || (i > 122)) {
                letter_mapping[i] = j;
                character_frequency[j].frequency = (distinct[i] / text_length) * 10000;
                character_frequency[j].character = i;
                j++;
            }
        }
    }

    sort_character_frequency(0, letter_count);

    // Update the letter mapping after sorting character frequency.
    for (i = 0; i < letter_count; i++) {
        letter_mapping[character_frequency[i].character] = i;
    }

    // Update the Digram matrix
    for (i = 0; i < text_len; i++) {
        /* copy the ASCII character value of text_buffer[i] into j. j will form
         * as an index into letter_mapping to get the relevant index in
         * character_frequency[]
         */
        j = text_buffer[i];
        int index = letter_mapping[j];
        if ((prev >= 0) && (prev < letter_count) && (index >= 0) && (index < letter_count)) {
            // If prev and index both contain values between 0 to letter_count,
            // then update the matrix[][] value
            matrix[prev][index]++;
        }
        prev = index;
    }
}

void homophonic_cipher_matrix::solve_cipher(const text_matrix &e_matrix, char text_buffer[]) {
    int freq_distribution_index = 0;
    int freq_distribution[27];
    int score;
    int i;
    int score_least;
    int *curr_best_key = NULL;

    if (text_buffer == NULL) {
        return;
    }

    curr_best_key = new int[letter_count];

    if (curr_best_key == NULL) {
        std::cout << "new failed for curr_best_key in homophonic_cipher_matrix";
        return;
    }

    if (letter_count >= E_LETTER_COUNT && letter_count < 130) {
        freq_distribution_index = letter_count - E_LETTER_COUNT;
    }

    if (letter_count > 0) {
        for (i = 0; i < 27; i++) {
            freq_distribution[i] = freq_distribution_final[freq_distribution_index][i];
        }
    }

    std::cout << "Initial frequency distribution: ";
    for (i = 0; i < 27; i++) {
        std::cout << freq_distribution[i] << " ";
    }
    std::cout << "\n";

    score_least = random_initial_key(e_matrix, freq_distribution, text_buffer);

    for (i = 0; i < letter_count; i++) {
        curr_best_key[i] = putative_key[i];
    }

    int a, b, j, k;
    int score_flag = 0;

    // Modify the putative keys in nested for loops
    a = 1;
    b = 1;
    i = 0;
    while (b < 2) {
        int less_score_flag = 0;
        for (a = 0; (a + b) < E_LETTER_COUNT; a++) {
            score_flag = 0;
            i = a;
            j = a + b;

            // Try i-- and j++
            if (freq_distribution[i] > 0) {
                freq_distribution[i]--;
                freq_distribution[j]++;

                score = random_initial_key(e_matrix, freq_distribution, text_buffer);

                std::cout << "Current outer hill climb score: " << score;

                if (score < score_least) {
                    std::cout << " (new best)";

                    score_least = score;

                    for (k = 0; k < letter_count; k++) {
                        curr_best_key[k] = putative_key[k];
                    }

                    less_score_flag = 1;
                    score_flag = 1;
                } else {
                    for (k = 0; k < letter_count; k++) {
                        putative_key[k] = curr_best_key[k];
                    }
                    freq_distribution[i]++;
                    freq_distribution[j]--;
                }

                std::cout << "\n";
            }

            if ((score_flag == 0) && (freq_distribution[j] > 0)) {
                // Try i++ and j--
                freq_distribution[i]++;
                freq_distribution[j]--;

                score = random_initial_key(e_matrix, freq_distribution, text_buffer);

                std::cout << "Current outer hill climb score: " << score;

                if (score < score_least) {
                    std::cout << " (new best)";

                    score_least = score;

                    for (k = 0; k < letter_count; k++) {
                        curr_best_key[k] = putative_key[k];
                    }

                    less_score_flag = 1;
                } else {
                    for (k = 0; k < letter_count; k++) {
                        putative_key[k] = curr_best_key[k];
                    }
                    freq_distribution[i]--;
                    freq_distribution[j]++;
                }

                std::cout << "\n";
            }
        }

        if (less_score_flag == 1) {
            a = 0;
            b = 1;
            i = a;
            j = a + b;
        } else {
            b = 2;
        }
    }

    std::cout << "Finished outer hill climb, results:\n";

    std::cout << "Final score: " << score_least << "\n";

    std::cout << "Final frequency distribution:";
    for (i = 0; i < 27; i++) {
        std::cout << " " << freq_distribution[i];
    }

    std::cout << "\nFinal key:\n     Cipher symbol: ";
    for (i = 0; i < letter_count; i++) {
        char ch = character_frequency[i].character;
        std::cout << ch;
    }
    std::cout << "\n  Plaintext letter: ";
    for (i = 0; i < letter_count; i++) {
        char ch = 'a' + putative_key[i];
        std::cout << ch;
    }
    std::cout << "\n";

    std::cout << "Decrypted text:\n";
    print_text_using_current_key(text_buffer);

    delete [] curr_best_key;
}

int homophonic_cipher_matrix::random_initial_key(const text_matrix &e_matrix, int freq_distribution[27], char text_buffer[]) {
    int *cipher_alphabet = NULL;
    int i;
    int j;
    int score_least = 0;
    int *curr_putative_key = NULL;

    if (text_buffer == NULL) {
        return 100000;
    }

    cipher_alphabet = new int[letter_count];

    if (cipher_alphabet == NULL) {
        std::cout << "new failed for cipher_alphabet in homophonic_cipher_matrix";
        return 100000;
    }

    for (i = 0; i < letter_count; i++) {
        cipher_alphabet[i] = i;
    }

    std::cout << "Creating " << MAX_RANDOM_TRIALS << " random keys using frequency distribution: ";
    for (i = 0; i < 27; i++) {
        std::cout << freq_distribution[i] << " ";
    }
    std::cout << "\n";

    create_initial_key(e_matrix, freq_distribution);
    score_least = inner_hill_climb(e_matrix, putative_key);

    std::cout << "Initial putative key score: " << score_least << "\n";

    curr_putative_key = new int[letter_count];

    if (curr_putative_key == NULL) {
        std::cout << "new failed for curr_putative_key in homophonic_cipher_matrix";
        delete [] cipher_alphabet;
        return 100000;
    }

    int k, no_repetitions = 0;
    int temp_char_index = 0;
    for (j = 0; j < MAX_RANDOM_TRIALS; j++) {
        random_permutation(cipher_alphabet);

        for (i = 0, k = 0; k < E_LETTER_COUNT; k++) {
            temp_char_index = e_matrix.character_frequency[k].character;
            no_repetitions = freq_distribution[k];
            while (no_repetitions > 0 && i < letter_count) {
                curr_putative_key[cipher_alphabet[i]] = temp_char_index;
                i++;
                no_repetitions--;
            }
        }

        int score = inner_hill_climb(e_matrix, curr_putative_key);

        std::cout << "Current inner hill climb score: " << score;

        if (score < score_least) {
            std::cout << " (new best)";

            // copy curr_putative_key to putative_key
            for (i = 0; i < letter_count; i++) {
                putative_key[i] = curr_putative_key[i];
            }
            score_least = score;
        }

        std::cout << "\n";
    }

    std::cout << "Best score from inner hill climb: " << score_least << "\n";

    delete [] curr_putative_key;
    delete [] cipher_alphabet;

    return score_least;
}

// TODO(AD): Try writing a better function to accomodate all valid english
// frequencies and d_matrix frequencies.
void homophonic_cipher_matrix::random_permutation(int *cipher_alphabet) {
    int modulus = letter_count - 2;

    std::random_device rd;
    std::mt19937 gen(rd());

    if (cipher_alphabet == NULL) {
        return;
    }
    for (int i = letter_count - 1; i > 0 && modulus > 0; i--, modulus--) {
        std::uniform_int_distribution<> dis(0, modulus);
        int j = dis(gen);
        int temp = cipher_alphabet[j];
        cipher_alphabet[j] = cipher_alphabet[i];
        cipher_alphabet[i] = temp;
    }
}

void homophonic_cipher_matrix::create_initial_key(const text_matrix &e_matrix, int freq_distribution[27]) {
    int i = 0;
    int j = 0;
    int k = 0;
    int m = 0;
    int temp_freq = 0;
    int no_repetitions = 0;
    int no_symbols = 0;
    char ch;

    if (letter_count > E_LETTER_COUNT) {
        no_symbols = freq_distribution[26];
        j = no_symbols;
        k = 0;
    }

    while ((i < letter_count) && (j < E_LETTER_COUNT)) {
        if (no_symbols > 0 && k <= 26) {
            if (freq_distribution[k] < 1) {
                k++;
            }
            no_repetitions = freq_distribution[k];
            temp_freq = e_matrix.character_frequency[k].frequency / no_repetitions;

            if (character_frequency[i].frequency < temp_freq) {
                for (m = 0; ((m < no_repetitions) && (i < letter_count)); m++, i++) {
                    ch = 'a' + e_matrix.character_frequency[k].character;
                    if (ch >= 'a' && ch <= 'z') {
                        putative_key[i] = ch - 'a';
                    }
                }
                no_symbols--;
                k++;
                continue;
            }
        }
        ch = 'a' + e_matrix.character_frequency[j].character;
        if (ch >= 'a' && ch <= 'z') {
            putative_key[i] = ch - 'a';
        }
        i++;
        j++;  // to be removed later
    }

    while ((i < letter_count) && (no_symbols > 0)) {
        no_repetitions = freq_distribution[k];
        for (m = 0; ((m < no_repetitions) && (i < letter_count)); m++, i++) {
            ch = 'a' + e_matrix.character_frequency[k].character;
            if (ch >= 'a' && ch <= 'z') {
                putative_key[i] = ch - 'a';
            }
        }
        no_symbols--;
        k++;
    }
}

void homophonic_cipher_matrix::print_text_using_current_key(char text_buffer[]) {
    size_t length = strlen(text_buffer);

    for (int i = 0; i < length; i++) {
        int letter_index = static_cast<int>(text_buffer[i]);
        int index = letter_mapping[letter_index];
        char ch = 'a' + putative_key[index];
        std::cout << ch;
    }
}

void homophonic_cipher_matrix::apply_putative_key(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT], int *curr_putative_key) {
    int i, j;
    double text_length = static_cast<double>(text_len);

    if (!curr_putative_key) {
        return;
    }

    /*
     * Apply the putative_key to the matrix[letter_count][letter_count]
     * to construct matrix_d[26][26]
     * matrix_d contains the actual counts of the digrams from ciphertext
     */
    for (i = 0; i < letter_count; i++) {
        for (j = 0; j < letter_count; j++) {
            matrix_d[curr_putative_key[i]][curr_putative_key[j]]+= matrix[i][j];
        }
    }

    /*
     * Once we have the matrix_d updated with the counts,
     * compute the percentages of digram frequencies
     */
    for (i = 0; i < E_LETTER_COUNT; i++) {
        for (j = 0; j < E_LETTER_COUNT; j++) {
            matrix_d[i][j] = (matrix_d[i][j] / (text_length - 1)) * 10000;
        }
    }
}

void homophonic_cipher_matrix::modify_putative_key(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT], int temp_putative_key[], int swap_i, int swap_j) {
    if (temp_putative_key == NULL) {
        return;
    }

    if (swap_i >= letter_count || swap_j >= letter_count || temp_putative_key[swap_i] == temp_putative_key[swap_j]) {
        return;
    }

    // Swap the elements in temp_putative_key
    int temp = temp_putative_key[swap_i];
    temp_putative_key[swap_i] = temp_putative_key[swap_j];
    temp_putative_key[swap_j] = temp;

    int key_i = temp_putative_key[swap_i];
    int key_j = temp_putative_key[swap_j];

    /* Reset the matrix_d rows and columns corresponding to temp_putative_key[i]
     * and temp_putative_key[j]
     */
    for (int k = 0; k < E_LETTER_COUNT; k++) {
        matrix_d[key_i][k] = 0;
        matrix_d[key_j][k] = 0;

        matrix_d[k][key_i] = 0;
        matrix_d[k][key_j] = 0;
    }

    /*
     * Recalculate those elements of matrix_d[26][26] which are affected by
     * swapping of swap_i and swap_j
     */
    for (int i = 0; i < letter_count; i++) {
        int temp_i = temp_putative_key[i];
        int should_calc = temp_i == key_i || temp_i == key_j;
        for (int j = 0; j < letter_count; j++) {
            if (should_calc || temp_putative_key[j] == key_i || temp_putative_key[j] == key_j) {
                matrix_d[temp_i][temp_putative_key[j]] += matrix[i][j];
            }
        }
    }

    /*
     * Once we have updated the matrix_d with the counts,
     * compute the percentages of digram frequencies
     */
    double text_length = static_cast<double>(text_len);
    for (int k = 0; k < E_LETTER_COUNT; k++) {
        if (k != key_i) {
            matrix_d[k][key_i] = (matrix_d[k][key_i] / (text_length - 1)) * 10000;
            matrix_d[key_i][k] = (matrix_d[key_i][k] / (text_length - 1)) * 10000;
        } else {
            matrix_d[key_i][k] = (matrix_d[key_i][k] / (text_length - 1)) * 10000;
        }

        if (k != key_j) {
            if (k != key_i) {
                matrix_d[k][key_j] = (matrix_d[k][key_j] / (text_length - 1)) * 10000;
                matrix_d[key_j][k] = (matrix_d[key_j][k] / (text_length - 1)) * 10000;
            }
        } else {
            matrix_d[key_j][k] = (matrix_d[key_j][k] / (text_length - 1)) * 10000;
        }
    }
}

int homophonic_cipher_matrix::inner_hill_climb(const text_matrix &e_matrix, int *curr_putative_key) {
    int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT];
    int i, j, score;
    int score_least = 0;

    if (curr_putative_key == NULL) {
        std::cout << "curr_putative_key is NULL";
        return -1;
    }

    // Initialize the D-Matrix[26][26]
    for (i = 0; i < E_LETTER_COUNT; i++) {
        for (j = 0; j < E_LETTER_COUNT; j++) {
            matrix_d[i][j] = 0;
        }
    }

    // Update the matrix_d with the putative key
    apply_putative_key(matrix_d, curr_putative_key);

    // Compute the score by comparing E-Matrix and D-Matrix
    score = e_matrix.compute_score(matrix_d);

    /* Main Hill Climbing algorithm
     * modify the putative kye a bit, and see if the score improves
     */
    if (score > 1000) {
        /* Have a temporary putative key which will be used for modification */
        int *temp_putative_key = new int[letter_count];
        if (temp_putative_key == NULL) {
            std::cout << "new failed for temp_putative_key in homophonic_cipher_matrix";
            return -1;
        }

        for (i = 0; i < letter_count; i++) {
            temp_putative_key[i] = curr_putative_key[i];
        }

        // save the least score till now into score_least
        score_least = score;

        // Modify the putative keys in nested for loops
        int a = 1;
        int b = 1;
        i = 0;
        j = 0;
        int previous_matrix_d[E_LETTER_COUNT][E_LETTER_COUNT];
        int matrix_did_change = 1;

        for (b = 1; b < letter_count; b++) {
            for (a = 0; (a + b) < letter_count; a++) {
                if (matrix_did_change) {
                    // Make a copy of the d-matrix for faster undo of swapping.
                    for (int k = 0; k < E_LETTER_COUNT; k++) {
                        for (int l = 0; l < E_LETTER_COUNT; l++) {
                            previous_matrix_d[k][l] = matrix_d[k][l];
                        }
                    }
                }

                i = a;
                j = a + b;
                modify_putative_key(matrix_d, temp_putative_key, i, j);
                score = e_matrix.compute_score(matrix_d);

                if (score < score_least) {
                    for (int ii = 0; ii < letter_count; ii++) {
                        curr_putative_key[ii] = temp_putative_key[ii];
                    }
                    score_least = score;
                    a = 0;
                    b = 1;
                    i = a;
                    j = a + b;
                    matrix_did_change = 1;
                } else {
                    // Undo the swapping.
                    if (i >= letter_count || j >= letter_count || temp_putative_key[i] == temp_putative_key[j]) {
                        continue;
                    }

                    int temp = temp_putative_key[j];
                    temp_putative_key[j] = temp_putative_key[i];
                    temp_putative_key[i] = temp;

                    for (int k = 0; k < E_LETTER_COUNT; k++) {
                        for (int l = 0; l < E_LETTER_COUNT; l++) {
                            matrix_d[k][l] = previous_matrix_d[k][l];
                        }
                    }

                    matrix_did_change = 0;
                }
            }
        }

        delete [] temp_putative_key;  // delete temp putative_key
    }

    return score_least;
}

void homophonic_cipher_matrix::copy_key_from_file(char *file_name) {
    int i, j;
    char ch;
    char *buffer = NULL;

    if (file_name == NULL) {
        return;
    }

    buffer = get_file_to_buffer(file_name);
    if (buffer == NULL) {
        return;
    }

    size_t file_size = strlen(buffer);

    for (i = 0, j = 0; (i < file_size) && (j < letter_count); i++) {
        if (buffer[i] == ':') {
            i++;
            if (i < file_size) {
                ch = buffer[i];
                if (ch >= 'a' && ch <= 'z') {
                    putative_key[j] = ch - 'a';
                    j++;
                }
            }
        }
    }

    delete [] buffer;
}

void homophonic_cipher_matrix::copy_final_key_file(char *file_name) {
    int i;
    FILE *fp;
    int buf_index = 0;
    char *buffer = NULL;

    if (file_name == NULL) {
        return;
    }

    int file_size;
    file_size = letter_count * 5;
    buffer = new char[file_size];
    if (!buffer) {
        std::cout << "buffer new for cipher text file failed";
        return;
    }

    if ((fp = fopen(file_name, "w")) == NULL) {
        std::cout << "Cannot open file.\n";
        delete [] buffer;
        exit(1);
    }

    for (i = 0; (i < letter_count) && (buf_index < (file_size - 5)); i++) {
        char ch = 'a' + putative_key[i];
        char ch1 = character_frequency[i].character;

        buffer[buf_index++] = ch1;
        buffer[buf_index++] = ':';
        buffer[buf_index++] = ch;
        buffer[buf_index++] = '|';
    }
    buffer[buf_index] = '\0';

    fprintf(fp, "%s", buffer);

    delete [] buffer;

    fclose(fp);
}

void homophonic_cipher_matrix::display_matrix(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT]) {
    std::cout << "\n\nDisplaying Digram Frequency of ciphertext after applying putative key:";
    std::cout << "\n    a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  -";
    std::cout << "\n   ---------------------------------------------------------------------------------";
    for (int i = 0; i < E_LETTER_COUNT; i++) {
        char ch = 'a' + i;
        std::cout << "\n" << ch << " |";
        for (int j = 0; j < E_LETTER_COUNT; j++) {
            if (matrix_d[i][j] < 10) {
                std::cout << "0";
            }
            std::cout << matrix_d[i][j] << " ";
        }
    }
    std::cout << "\n";
}
