//
// text_matrix.h
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#ifndef ATTACK_TEXT_MATRIX_H__
#define ATTACK_TEXT_MATRIX_H__

#include "utils.h"

/* Class: text_matrix
 * Description:
 *      1. Is used to represent a digram matrix for english letters.
 *      2. Therefore the matrix is defined of size 26 (english letters)
 */
class text_matrix {
    /* variable: matrix
     * Description: contains the digram frequencies of english text
     */
    int matrix[E_LETTER_COUNT][E_LETTER_COUNT];

    /* variable: letter_frequency
     * Description: contains individual letter frequency
     */
    int letter_frequency[E_LETTER_COUNT];

 public:
    /* variable: text_len
     * Description: The total length of input text (plain text or cipher text)
     */
    size_t text_len;

    /* variable: struct char_frequency
     * Description:
     *      1. Used to bind the letter frequency to the letter.
     *      2. This structure is helpful when the letters need to be sorted
     *         according to its frequencies
     */
    struct char_frequency {
        int character;
        int frequency;
    };

    /* variable: character_frequency
     * Description: pointer to struct char_frequency
     */
    struct char_frequency *character_frequency;

    /* variable: freq_distribution_final
     * Description: Contains the distribution mapping
     *
     */
    void compute_freq_distribution(int letter_count, int freq_distribution_final[27]);

    /* Function: text_matrix
     * Description:
     *      1. Constructor
     *      2. Allocate memory to character_frequency
     *      3. Initializes array elements of letter_frequency to 0
     *      4. Initializes array elements of matrix to 0
     *  Params:
     *      1. letter_count: Number of distinct symbols int the input plain/cipher text buffer
     *  Return Value: None */
    explicit text_matrix(int letter_count);

    /* Function: ~text_matrix
     * Description:
     *      1. Destructor
     *      2. Deletes the memory allocated to character_frequency
     *  Params: None
     *  Return Value: None
     */
    ~text_matrix();

    /* Function: display
     * Description:
     *      1. Displays the contains of matrix, letter_frequency, and
     *      character_frequency
     * Params: None
     * Return Value: None
     */
    void display();

    /* Function: update
     * Description:
     *      1. Update the letter_frequency with the letter frequencies from
     *         input text (plain/cipher) buffer
     *      2. Update the matrix with digram frequencies from the input text
     *         (plain/cipher) buffer
     * Params:  input text (plain/cipher) buffer
     * Return Value: None
     */
    void update(char text_buffer[]);

    /* Function: update_digram_frequencies
     * Description:
     *      1. Update the character_frequency with the letter and percentage
     *         frequency calculated form letter_frequency and text buffer length
     *      2. Sort the letters according the frequncy in descending order
     * Params: None
     * Return Value: None
     */
    void update_digram_frequencies();

    /* Function: update_hardcoded
     * Description:
     *      1. Update the character_frequency from hardcoded values from
     *         e_matrix.h
     *      2. Update the matrix with hardcoded digram frequencies from e_matrix.h
     * Params:  None
     * Return Value: None
     */
    void update_hardcoded();

    /* Function: sort_character_frequency
     * Description:
     *      1. Use merge sort to sort the character_frequency
     * Params:
     *      1. begin: begin index of the character_frequency
     *      2. end : end index of the character_frequency
     * Return Value: None
     */
    void sort_character_frequency(int begin, int end);

    /* Function: compute_score
     * Description:
     *      1. Initialize an integer score to 0
     *      2. Compare D-Matrix and E-matrix element by element
     *      3. Add the difference to score
     * Params:
     *      1. D-Matrix (The E-Matrix is already available as the class
     *         member variable)
     * Return Value: computed score
     */
    int compute_score(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT]) const;
};

#endif  // ATTACK_TEXT_MATRIX_H__
