//
// homophonic_cipher_matrix.h
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus.magnuson@gmail.com>
//

#ifndef ATTACK_HOMOPHONIC_CIPHER_MATRIX_H__
#define ATTACK_HOMOPHONIC_CIPHER_MATRIX_H__

#include "text_matrix.h"

/* Class: homophonic_cipher_matrix
 * Description:
 *      1. Derived from text_matrix in protected mode
 *      2. Is used to represent a digram matrix for cipher text symbols
 *      3. The matrix size is derived at runtime from the number of total
 *         distinct symbols in ciphertext
 */
class homophonic_cipher_matrix: protected text_matrix {
    /* Variable: letter_count
     * Description: number of total distinct symbols in ciphertext
     */
    int letter_count;

    /* Variable: matrix
     * Description: Contains the digram frequencies of cipher text
     */
    int **matrix;

    /* Variable: putative_key
     * Description: Contains the putative solution of the mapping from
     *          ciphertext symbols to plaintext letters
     */
    int *putative_key;

    /* Variable: letter_mapping
     * Description: Contains the mapping from ascii character to ciphertext
     *          symbols
     */
    int letter_mapping[256];

 public:
    /* Function: homophonic_cipher_matrix
     * Description:
     *      1. Constructor
     *      2. Allocate memory to putative_key, 2D matrix
     *      3. Initializes putative_key, 2D matrix
     *      4. Initializes character_frequency, letter_mapping
     * Params:
     *      1. count: Number of distinct symbols int the input
     *                cipher text buffer
     * Return Value: None
     */
    explicit homophonic_cipher_matrix(int count);

    /* Function: ~homophonic_cipher_matrix
     * Description:
     *      1. Destructor
     *      2. Deletes the memory allocated to putative_key, 2D matrix
     * Params: None
     * Return Value: None
     */
    ~homophonic_cipher_matrix();

    /* Function: display
     * Description:
     *      1. Displays the contains of letter_mapping, character_frequency,
     *         matrix & putative_key
     * Params: None
     * Return Value: None
     */
    void display();

    /* Function: display_matrix
     * Description:
     *      1. Displays the contains of the matrix
     * Params: None
     * Return Value: None
     */
    void display_matrix(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT]);

    /* Function: display_putative_key
     * Description:
     *      1. Display the putative key
     * Params: None
     * Return Value: None
     */
    void display_putative_key();

    /* Function: display_key
     * Description:
     *      1. Display arbitrary key
     * Params: None
     * Return Value: None
     */
    void display_key(int key[]);

    /* Function: update
     * Description:
     *      1. Update the letter_mapping, character_frequency
     *      2. Update the matrix with digram frequencies from the cipher
     *      text buffer
     *      3. Sort the character_frequency
     * Params:
     *      1. text_buffer: Input cipher text buffer
     *      2. distinct: frequency of distinct symbols
     * Return Value: None
     */
    void update(char text_buffer[], int distinct[256]);

    /* Function: get_letter_count
     * Description:
     *      1. Return the total number of distinct symbols
     * Params: None
     * Return Value: letter_count
     */
    int get_letter_count() { return letter_count; }

    /* Function: create_initial_key
     * Description:
     *      1. Create the initial putative key from sorted
     *         character_frequency using standard english letter frequency
     * Params:
     *      1. e_matrix: pointer to object of E-matrix to get the letter
     *                   frequency
     * Return Value: None
     */
    void create_initial_key(const text_matrix &e_matrix, int freq_distribution[27]);

    /* Function: modify_putative_key
     * Description:
     *      1. Swap the elements of temp_putative_key at indices i and j
     *      2. Swap the rows and columns of matrix_d accordingly
     * Params:
     *      1. matrix_d: D matrix
     *      2. temp_putative_key: temporary putative key used for swapping
     *      3. swap_i: index of temp_putative_key to be swapped
     *      4. swap_j: index of temp_putative_key to be swapped
     * Return Value None
     */
    void modify_putative_key(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT], int temp_putative_key[], int swap_i, int swap_j);

    /* Function: apply_putative_key
     * Description:
     *      1. Populate the D matrix using using the putative key
     *      2. collapse the bigger D matrix to the size of E matrix
     * Params:
     *      1. matrix_d: D matrix
     *      2. curr_putative_key: Current Putative key
     * Return Value: None
     */
    void apply_putative_key(int matrix_d[E_LETTER_COUNT][E_LETTER_COUNT], int *curr_putative_key);

    /* Function: print_text_using_current_key
     * Description:
     *      1. Decrypt the cipher text using the putative key
     *      2. Display the decrypted text
     * Params:
     *      1. text_buffer: input cipher text
     * Return Value: None
     */
    void print_text_using_current_key(char text_buffer[]);

    /* Function: copy_final_key_file
     * Description:
     *      1. Write the final key to a file
     * Params:
     *      1. file_name: filename of the file to which the key should be
     *      written
     * Return Value: None
     */
    void copy_final_key_file(char *file_name);

    /* Function: copy_key_from_file
     * Description:
     *      1. Copy key from the file
     * Params:
     *      1. file_name: filename of the file to which the key should be
     *      written
     * Return Value: None
     */
    void copy_key_from_file(char *file_name);

    /* Function: inner_hill_climb
     * Description:
     *      1. Hill Climbing Algorithm
     *      2. Take the initial putative key
     *      3. Compute Score
     *      4. Modify the current key slightly
     *      5. Again Compute Score
     *      6. If score improves, keep the modification. If not, discard the
     *         modification
     *      7. Repeat steps 4-6 for certain number of iterations
     * Params:
     *      1. pointer to object of E-Matrix
     *      2. curr_putative_key: Current Putative key
     * Return Value: Returns the best score
     */
    int inner_hill_climb(const text_matrix &e_matrix, int *curr_putative_key);

    /* Function: random_initial_key
     * Description:
     *      1. Create multiple random initial keys
     *      2. Call inner_hill_climb on each of the random key
     * Params:
     *      1. pointer to object of E-Matrix
     * Return Value: return score
     */
    int random_initial_key(const text_matrix &e_matrix, int freq_distribution[27], char text_buffer[]);

    /* Function: random_permutation
     * Description:
     *      1. Create a random permutation of english_alphabet
     * Params:
     *      1. integer array english_alphabet
     * Return Value: None
     */
    void random_permutation(int english_alphabet[26]);

    /* Function: solve_cipher
     * Description:
     *      1. From initial letter frequencies generate new ones
     *      2. Call random_initial_key on each of the new frequencies
     * Params:
     *      1. pointer to object of E-Matrix
     * Return Value: None
     */
    void solve_cipher(const text_matrix &e_matrix, char text_buffer[]);
};

#endif  // ATTACK_HOMOPHONIC_CIPHER_MATRIX_H__
