//
// utils.h
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#ifndef ATTACK_UTILS_H__
#define ATTACK_UTILS_H__

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#define BUFFER_SIZE 100
#define E_LETTER_COUNT 26
#define TOTAL_ASCII_COUNT 256
#define MAX_RANDOM_TRIALS 40

char * get_file_to_buffer(char *filename);

int get_distinct_symbols(char *buffer, int distinct[256]);

#endif  // ATTACK_UTILS_H__
