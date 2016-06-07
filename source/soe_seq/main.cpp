/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

/**
   This program is a naive implementation of the
   sieve of Eratosthenes (SOE).  It consists of two stages:
   - Stage 1: sieving out the "small" primes, i.e. from 0 to some
     smaller bound 'nbits'.
   - Stage 2: using the primes/sieve table from stage 1, sieve the 
     numbers starting from a given base.
   Here of course, nbits*nbits > base should be true.
 */

#include "../utils/utils.h"

int main()
{
	std::cout << "Simple Sieve of Eratosthenese\n\n";

	std::cout << "lower bound " << LOWER_BOUND << std::endl;
	std::cout << "upper bound " << UPPER_BOUND << std::endl;
	
	word_t number_of_words = (UPPER_BOUND - LOWER_BOUND) / (2 * sizeof(word_t) * CHAR_BIT);
	word_t number_of_bits = number_of_words * sizeof(word_t) * CHAR_BIT;
	word_t* chunk = new word_t[number_of_words];
	for(size_t j=0; j<number_of_words; ++j) chunk[j]=0;
	
	std::cout << "number of words " << number_of_words << std::endl;
	std::cout << "number of bits " << number_of_bits << std::endl;

	word_t last_number = sqrt(UPPER_BOUND);
	size_t log_upper_bound = log2(last_number + 1);
	// 2^(log_upper_bound) - 1 is the last number in the sieve table, i.e. last_number
	
	std::cout << "last_number " << last_number << std::endl;
	std::cout << "log_upper_bound " << log_upper_bound << std::endl;
	
	size_t n = log_upper_bound < 7 ? 1 : P2I(1<<(log_upper_bound-LOG_WORD_SIZE));
	size_t nbits = last_number / 2 + 1;
	
	/**
		Allocate `st`.
	*/
	word_t* st = new word_t[n];
	for(size_t j=0; j<n; ++j) st[j]=0;
	
	std::cout << "n " << n << std::endl;
	std::cout << "nbits " << nbits << "\n\n";
	
	prime_t chunk_base = LOWER_BOUND / 2;
	soe_init(nbits, st);

	soe_chunk(nbits, st, number_of_bits, chunk, chunk_base);
	
	std::cout << "The found primes in the given interval:\n";
	print_primes_chunk(number_of_bits, chunk, chunk_base);

	delete[] chunk;
	delete[] st;

	std::cout << "--- the end ---\n";
	
	return 0;
}