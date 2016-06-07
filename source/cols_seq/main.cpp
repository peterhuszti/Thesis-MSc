/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

/**
   This program is a simple implementation of the
   sieve of Eratosthenes (SOE).  It consists of two stages:
   - Stage 1: sieving out the "small" primes, i.e. from 0 to some
     smaller bound 'nbits'.
   - Stage 2: using the primes/sieve table from stage 1, sieve chunks
     starting from a given base.
   Here of course, nbits*nbits > base should be true.
 */

#include "../utils/utils.h"
 
#define MAX_NUMBER_OF_CHUNKS 32

int main()
{
	std::cout << "Sieve of Eratosthenese\n\n";

	std::cout << "lower bound " << LOWER_BOUND << std::endl;
	std::cout << "upper bound " << UPPER_BOUND << std::endl;
	
	size_t chunk_temp = UPPER_BOUND - LOWER_BOUND;
	size_t number_of_chunks = (chunk_temp / (1 << LOG_WORD_SIZE)) / 2;
	while(number_of_chunks > MAX_NUMBER_OF_CHUNKS)
	{
		number_of_chunks /= 2;
	}
	
	std::cout << "number of chunks " << number_of_chunks << std::endl;
	
	chunk_temp /= number_of_chunks;
	size_t chunk_bits =  chunk_temp <= 64 ? 64 : chunk_temp >> 1;
	size_t chunk_size = chunk_bits / (sizeof(word_t) * CHAR_BIT);
	
	/**
		Initialization of chunks 
	*/
	std::cout << "chunk_size " << chunk_size << std::endl;
	std::cout << "chunk_bits " << chunk_bits << std::endl;
	
	word_t** chunks = new word_t*[number_of_chunks];
	for(size_t i=0; i<number_of_chunks; ++i)
	{
		chunks[i] = new word_t[chunk_size];
		for(size_t j=0; j<chunk_size; ++j) chunks[i][j]=0;
	}
	
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

	std::cout << "The found primes in the given interval:\n";
	for(size_t i=0; i<number_of_chunks; ++i) 
	{
		soe_chunk(nbits, st, chunk_bits, chunks[i], chunk_base);
		print_primes_chunk(chunk_bits, chunks[i], chunk_base);
		chunk_base += chunk_bits;
	}

	for(size_t i=0; i<number_of_chunks; i++) delete[] chunks[i];
	delete[] chunks;
	delete[] st;

	std::cout << "--- the end ---\n";
	
	return 0;
}