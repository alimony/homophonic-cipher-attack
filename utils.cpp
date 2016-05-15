//
// utils.cpp
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus.magnuson@gmail.com>
//

#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int get_distinct_symbols(char *buffer, int distinct[256]) {
    int i;
    int count = 0;

    size_t buff_size = strlen(buffer);

    for (i = 0; i < 256; i++) {
        distinct[i] = 0;
    }

    for (i = 0; i < buff_size; i++) {
        int index = buffer[i];
        distinct[index]++;
    }

    for (i = 0; i < 256; i++) {
        if (distinct[i]) {
            count++;
        }
    }

    return count;
}

char * get_file_to_buffer(char *filename) {
    char *buffer;
    std::streamoff begin, end, file_size;

    std::fstream file_stream;
    file_stream.open(filename, std::fstream::in | std::fstream::out);
    if (!file_stream) {
        std::cout << "Can not open the file:" << filename << "\n";
        return NULL;
    }

    begin = file_stream.tellg();
    file_stream.seekg(0, std::ios::end);
    end = file_stream.tellg();
    file_size = end - begin;
    file_stream.seekg(0, std::ios::beg);

    buffer = new char[file_size];
    if (!buffer) {
        std::cout << "buffer new for cipher text file failed";
        file_stream.close();
        return NULL;
    }

    file_stream.read(buffer, file_size);
    buffer[file_size - 1] = '\0';

    file_stream.close();

    return buffer;
}
