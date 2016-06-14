/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

/**
   This program is a cache optimized implementation of the
   sieve of Eratosthenes (SOE). It consists of two stages:
   - Stage 1: sieving out the "small" primes, i.e. from 0 to some
     smaller bound 'nbits'.
   - Stage 2: using the primes/sieve table from stage 1, sieve chunks
     starting from a given base.
   Here of course, nbits*nbits > base should be true.
 */

#include "../utils/Siever.h"
 
#define MAX_NUMBER_OF_CHUNKS 128

int main()
{	
	Siever siever(MAX_NUMBER_OF_CHUNKS);

	siever.soe_init();
	
	siever.soe_chunks();
	
	#if DEBUG
		siever.print_debug_info();
		siever.print_sieving_table();
		siever.print_primes_found();
		siever.print_number_of_found_primes();
	#endif
	
	return 0;
}