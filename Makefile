CC = g++
OBJS = utils.o homophonic_cipher_matrix.o text_matrix.o
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -O3
LFLAGS = -Wall $(DEBUG)

HomophonicCipher : main.o $(OBJS)
	$(CC) $(LFLAGS) main.o $(OBJS) -o HomophonicCipher

EnglishMatrixGeneration : generate_e_matrix.o $(OBJS)
	$(CC) $(LFLAGS) generate_e_matrix.o $(OBJS) -o EnglishMatrixGenerationr

main.o : utils.h homophonic_cipher_matrix.h homophonic_cipher_matrix.o text_matrix.o main.cpp
	$(CC) $(CFLAGS) main.cpp

generate_e_matrix.o : utils.h homophonic_cipher_matrix.h homophonic_cipher_matrix.o text_matrix.o generate_e_matrix.cpp
	$(CC) $(CFLAGS) generate_e_matrix.cpp

utils.o : utils.h utils.cpp
	$(CC) $(CFLAGS) utils.cpp

homophonic_cipher_matrix.o : homophonic_cipher_matrix.h e_hardcoded_values.h text_matrix.o homophonic_cipher_matrix.cpp
	$(CC) $(CFLAGS) homophonic_cipher_matrix.cpp

text_matrix.o : text_matrix.h utils.h e_hardcoded_values.h text_matrix.cpp
	$(CC) $(CFLAGS) text_matrix.cpp

clean:
	rm -f *.o HomophonicCipher
