/*
	author: Peter Huszti
 */

/**
   This program is a parallel implementation of the
   sieve of Eratosthenes (SOE).  It consists of two stages:
   - Stage 1: sieving out the "small" primes, i.e. from 0 to some
     smaller bound 'nbits'.
   - Stage 2: using the primes/sieve table from stage 1, sieve the 
     numbers starting from a given base.
   Here of course, nbits*nbits > base should be true.
 */

#include <fstream>
 
#include "../utils/Siever.h"
#include "../utils/Printer.h"
#include "../utils/utils.h"

#define MAX_NUMBER_OF_CHUNKS 128
#define NUMBER_OF_THREADS 32 // should be < number chunks

int main()
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;
	
	auto bounds = read_config();
		
	lower_bound = bounds.first;
	upper_bound = bounds.second;
	
	Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, NUMBER_OF_THREADS);
	
	#if DEBUG
		Printer printer(&siever);
	
		printer.print_debug_info();
	#endif
	
	siever.soe_init();
	siever.soe_chunks();
	
	#if DEBUG
		printer.print_sieving_table();
		printer.print_primes_found();
		printer.print_number_of_found_primes();
	#endif
		
	return 0;
}