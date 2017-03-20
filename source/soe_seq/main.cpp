/*
	author: Peter Huszti
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
#include "../utils/utils.h"

#define MAX_NUMBER_OF_CHUNKS 1
#define NUMBER_OF_THREADS 1 // should be < number chunks

int main(int argc, char *argv[])
{
	start(argc, argv, MAX_NUMBER_OF_CHUNKS, NUMBER_OF_THREADS);
		
	return 0;
}