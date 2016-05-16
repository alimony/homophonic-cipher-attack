# Homophonic Cipher Attack

This is an implementation of the paper [“Efficient Cryptanalysis of Homophonic
Substitution Ciphers”](http://www.cs.sjsu.edu/~stamp/RUA/homophonic.pdf) by
Amrapali Dhavare, Richard M. Low and Mark Stamp. The code was originally written
by Amrapali Dhavare in 2011 and later improved by Markus Amalthea Magnuson in
2016.

Included in the file `texts/zodiac_340_ciphertext.txt` is the ciphertext of the
world’s probably most infamous homophonic substitution cipher, the [unsolved 340-character cipher by the Zodiac killer](http://zodiackillerciphers.com/wiki/index.php?title=Unsolved_340-character_cipher),
which is discussed in the latter part of the original paper. Zodiac sent many
letters, some of which [have been confirmed authentic and others not](https://en.wikisource.org/wiki/Zodiac_Killer_letters).
The file `texts/known_zodiac_letters_combined.txt` contains a concatenation of
all confirmed letters and can be used as digram frequency comparison instead
of the standard English.

## Building
Run `make` to build or `make clean` to remove compiled files.

## Usage
```shell
./HomophonicCipher <ciphertext_file> [<digram_statistics_file>] [<final_key_output_file>]
```
* The first argument is required and should be a text file containing the
ciphertext.
* The second argument is optional and is a text file from which to
calculate the digram frequencies that should be compared against when scoring
potential solutions. If no such file is passed, the comparison will be against
English digram frequencies.
* The third argument is optional and is the name of a
text file to which the final resulting key should be written when done. The
result will always be printed to screen as well.

**Example usage:**
```shell
# Compare against English digram frequencies:
./HomophonicCipher texts/zodiac_340_ciphertext.txt

# Compare against confirmed Zodiac letter frequencies:
./HomophonicCipher texts/zodiac_340_ciphertext.txt texts/known_zodiac_letters_combined.txt
```
