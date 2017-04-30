/*
	author: Peter Huszti
 */

/**
   This program is an implementation of the
   sieve of Eratosthenes (SOE).  It consists of two stages:
   - Stage 1: sieving out the "small" primes, i.e. from 0 to some
     smaller bound 'nbits'.
   - Stage 2: using the primes/sieve table from stage 1, sieve the 
     numbers starting from a given base.
   Here of course, nbits*nbits > base should be true.
   The user can configure the working of the algorithm in the input file
   given as the parameter of the program.
 */

#include <fstream>
 
#include "utils/Siever.h"
#include "utils/Printer.h"
#include "utils/utils.h"

int main(int argc, char *argv[])
{
	start(argc, argv);
		
	return 0;
}