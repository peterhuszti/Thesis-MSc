/*
	author: Peter Huszti
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
 
#include <fstream>
 
#include "../utils/Siever.h"
#include "../utils/Printer.h"
#include "../utils/utils.h"

#define MAX_NUMBER_OF_CHUNKS 128

int main(int argc, char *argv[])
{
	start(argc, argv, MAX_NUMBER_OF_CHUNKS);
		
	return 0;
}