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

#include <fstream>
 
#include "../utils/Siever.h"
#include "../utils/Printer.h"

#define MAX_NUMBER_OF_CHUNKS 1

#define CONFIG "../utils/config.txt" //!!!!!!!!!!!!!!!!

int main()
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;
	
	/**
		Read config file
	*/
	std::ifstream read_config;
	read_config.open(CONFIG);	
	char line[64];
	read_config.getline(line, 64, ' ');
	read_config >> lower_bound;
	read_config.getline(line, 64, ' ');
	read_config >> upper_bound;
	read_config.close();
	
	Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound);
	
	siever.soe_init();
	siever.soe_chunks();

	#if DEBUG
		Printer printer(&siever);
	
		printer.print_debug_info();
		printer.print_sieving_table();
		printer.print_primes_found();
		printer.print_number_of_found_primes();
	#endif
		
	return 0;
}