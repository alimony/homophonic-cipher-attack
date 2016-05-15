//
// text_matrix.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus.magnuson@gmail.com>
//

#include "text_matrix.h"

#include <math.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "e_hardcoded_values.h"

text_matrix::text_matrix(int letter_count) {
    int i, j;

    character_frequency = new char_frequency[letter_count];
    if (character_frequency == NULL) {
        std::cout << "New failed for char_frequency in text_matrix";
        return;
    }

    text_len = 0;
    for (i = 0; i < E_LETTER_COUNT; i++) {
        letter_frequency[i] = 0;
        for (j = 0; j < E_LETTER_COUNT; j++) {
            matrix[i][j] = 0;
        }
    }
}

text_matrix::~text_matrix() {
    delete [] character_frequency;
}

void text_matrix::display() {
    int i, j;
    char ch;

    std::cout << "\nDisplaying English Letter Frequency:\n";
    for (i = 0; i < (E_LETTER_COUNT - 1); i++) {
        ch = 'a' + i;
        std::cout << ch << ":" << letter_frequency[i] << "| ";
    }

    std::cout << "\nText length:" << text_len << "\n";
    std::cout << "\nEnglish Frequency:\n";
    for (i = 0; i < E_LETTER_COUNT; i++) {
        ch = 'a' + character_frequency[i].character;
        std::cout << ch << ":" << character_frequency[i].frequency << "|";
    }

    std::cout << "\n\nDisplaying English Letter Digram Frequency:";
    std::cout << "\n    a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z   -";
    std::cout << "\n   -----------------------------------------------------------------------------------------------------------";
    for (i = 0; i < E_LETTER_COUNT; i++) {
        if (i != 26) {
            ch = 'a' + i;
            std::cout << "\n" << ch << " |";
        } else {
            std::cout << "\n- |";
        }
        for (j = 0; j < E_LETTER_COUNT; j++) {
            if (matrix[i][j] < 10) {
                std::cout << "00";
            } else if (matrix[i][j] < 100) {
                std::cout << "0";
            }
            std::cout << matrix[i][j] << " ";
        }
    }
}

void text_matrix::update_hardcoded() {
    for (int i = 0; i < E_LETTER_COUNT; i++) {
        character_frequency[i].character = char_freq_char[i];
        character_frequency[i].frequency = char_freq_freq[i];
        letter_frequency[char_freq_char[i] - 'a'] = char_freq_freq[i];
        for (int j = 0; j < E_LETTER_COUNT; j++) {
            matrix[i][j] = e_matrix_hardcoded[i][j];
        }
    }
    update_digram_frequencies();
}

void text_matrix::compute_freq_distribution(int letter_count, int freq_distribution_final[27]) {
    int i, j;

    std::cout << "\ncompute_freq_distribution letter_count:" << letter_count << std::endl;
    for (j = 0; j < E_LETTER_COUNT; j++) {
        freq_distribution_final[j] = 1;
    }

    int temp_freq = 0;
    int max_index = 0;
    double temp = 0;
    for (i = 0; i < letter_count; i++) {
        int max_freq = 0;
        for (j = 0; j < E_LETTER_COUNT; j++) {
            temp = freq_distribution_final[j] + 1;
            temp_freq = character_frequency[j].frequency / temp;
            if (temp_freq > max_freq) {
                max_freq = temp_freq;
                max_index = j;
            }
        }
        freq_distribution_final[max_index]++;
    }

    int count_symbols = 0;
    std::cout << "\n\nDisplaying Letter Frequency Distribution\n     ";
    for (j = 0; j < E_LETTER_COUNT; j++) {
        char ch = 'a' + character_frequency[j].character;
        std::cout << ch << ", ";
        if (freq_distribution_final[j] > 1) {
            count_symbols++;
        }
    }

    freq_distribution_final[j] = count_symbols;
    std::cout << "\n   -----------------------------------------------------------------------------------------------------------";
    std::cout << "\n{   ";
    count_symbols = 0;
    for (j = 0; j <= E_LETTER_COUNT; j++) {
        std::cout << freq_distribution_final[j] << ", ";
    }
    std::cout << "}";
    std::cout << "\n";
}

void text_matrix::update(char text_buffer[]) {
    int i = 0;
    int index = -1;
    int prev = -1;

    text_len = strlen(text_buffer);
    while (i < text_len) {
        if ((text_buffer[i] >= 'A') && (text_buffer[i] <= 'Z')) {
            index = text_buffer[i] - 'A';
        } else if ((text_buffer[i] >= 'a') && (text_buffer[i] <= 'z')) {
            index = text_buffer[i] - 'a';
        } else {
            index = -1;
            prev = -1;
        }

        if (index != -1) {
            letter_frequency[index]++;
            if (prev != -1) {
                matrix[prev][index]++;
            }
            prev = index;
        }
        i++;
    }

    int total_digrams = 0;
    int j;
    for (i = 0; i < E_LETTER_COUNT; i++) {
        for (j = 0; j < E_LETTER_COUNT; j++) {
            total_digrams += matrix[i][j];
        }
    }

    std::cout << "Total digrams in frequency source: " << total_digrams << "\n";

    // save the percentage frequencies in the matrix
    double text_length = static_cast<double>(total_digrams);

    for (i = 0; i < E_LETTER_COUNT; i++) {
        letter_frequency[i] = (letter_frequency[i] / text_length) * 10000;
        for (j = 0; j < E_LETTER_COUNT; j++) {
            matrix[i][j] = (matrix[i][j] / text_length) * 10000;
        }
    }

    update_digram_frequencies();
}

void text_matrix::update_digram_frequencies() {
    std::cout << "Updating character frequencies\n";

    for (int i= 0; i < E_LETTER_COUNT; i++) {
        character_frequency[i].character = i;
        character_frequency[i].frequency = letter_frequency[i];
    }

    sort_character_frequency(0, E_LETTER_COUNT);
}

void text_matrix::sort_character_frequency(int begin, int end) {
    int i;
    int size = end - begin;
    if (size < 2) {
        return;
    }
    int middle = begin + (size / 2);

    sort_character_frequency(begin, middle);
    sort_character_frequency(middle, end);

    struct char_frequency sorted[size];
    int left_index, right_index, sorted_index;
    left_index = begin;
    right_index = middle;
    sorted_index = 0;

    while ((left_index < middle) && (right_index < end)) {
        if (character_frequency[left_index].frequency < character_frequency[right_index].frequency) {
            sorted[sorted_index].frequency = character_frequency[right_index].frequency;
            sorted[sorted_index].character = character_frequency[right_index].character;
            right_index++;
        } else {
            sorted[sorted_index].frequency = character_frequency[left_index].frequency;
            sorted[sorted_index].character = character_frequency[left_index].character;
            left_index++;
        }
        sorted_index++;
    }

    while (left_index < middle) {
        sorted[sorted_index].frequency = character_frequency[left_index].frequency;
        sorted[sorted_index].character = character_frequency[left_index].character;
        left_index++;
        sorted_index++;
    }

    while (right_index < end) {
        sorted[sorted_index].frequency = character_frequency[right_index].frequency;
        sorted[sorted_index].character = character_frequency[right_index].character;
        right_index++;
        sorted_index++;
    }

    for (i = begin, sorted_index = 0; i < end; i++, sorted_index++) {
        character_frequency[i].frequency = sorted[sorted_index].frequency;
        character_frequency[i].character = sorted[sorted_index].character;
    }
}

int text_matrix::compute_score(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT]) const {
    int score = 0;
    int squared_score = 0;
    for (int i = 0; i < E_LETTER_COUNT; i++) {
        for (int j = 0; j < E_LETTER_COUNT; j++) {
            if (matrix[i][j] > matrix_d[i][j]) {
                squared_score = matrix[i][j] - matrix_d[i][j];
            } else {
                squared_score = matrix_d[i][j] - matrix[i][j];
            }
            score = score + squared_score;
        }
    }
    return score;
}
