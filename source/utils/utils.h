/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

#ifndef UTILS
#define UTILS
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <iostream>
#include <math.h>

#define LOG_WORD_SIZE 6 // 1 word = 2^(LOG_WORD_SIZE) i.e. 64 bit

#define LOWER_BOUND 536870912
#define UPPER_BOUND 1073741824 // both should be a power of 2

#define INDEX(i) ((i)>>(LOG_WORD_SIZE))
#define MASK(i) ((word_t)(1) << ((i)&(((word_t)(1)<<LOG_WORD_SIZE)-1)))
#define GET(p,i) (p[INDEX(i)]&MASK(i))
#define SET(p,i) (p[INDEX(i)]|=MASK(i))
#define RESET(p,i) (p[INDEX(i)]&=~MASK(i))
#define P2I(p) ((p)>>1) // (((p-2)>>1)) 
#define I2P(i) (((i)<<1)+1) // ((i)*2+3)

#define DEBUG false // true if we want to print debug info
 
typedef uint64_t prime_t;
typedef uint64_t word_t;

#if DEBUG
#include "debug.h"
#endif

/**
   Stage 1: soe_init() implements the sieving of "small" primes.  
   
   @param: `nbits` is the *effective* nbits, the number of candidates
   stored on `nbits` is `I2P(nbits)`.
   
   @param: `st` is a pointer to the sieve table, with `nbits` number
   of bits allocated i.e. `nbits/8` bytes.
 */
void soe_init(size_t nbits, word_t* st){
	prime_t p = 3; // first prime
	prime_t q = P2I(p); // index in st

	while(p * p < I2P(nbits)) // need to sieve only until sqrt(upper_bound)
	{
		while(GET(st,q)) q++; // search the next 0 in st, i.e. the next prime to sieve with

		p = I2P(q); // what is this next sieving prime
		prime_t i = P2I(p*p); // need to sieve only from p^2 because the smallers are already sieved

		while(i < nbits) // sieve until it is in st
		{
			SET(st,i); // mark as composite
			i += p; // step forward p
		}
		q++; // step forward 1, so the 2. while can find the next prime to sieve with
	}
}

/**
   negmodp2I() calculates the index of the candidate \f$-x \bmod p\f$,
   taking special care when \f$-x \bmod p\f$ is even.
*/
inline prime_t negmodp2I(prime_t x, prime_t p)
{
	prime_t q = x % p;
	q = q ? p-q : 0;
	return q % 2 ? P2I(q + p) : P2I(q);
}

/**
   Performs SOE on a chunk with primes read from the sieve table `st`
   of size `nbits` (effective number bits).  A chunk (at `chunk`) is a
   chunk of a sieve table of bits from `base` to `base+chunk_bits-1`
   bits.
   
   @param: `nbits` is the effective number of bits stored at `st`.
   
   @param: `st` is the pointer to the sieve table.
   
   @param: `chunk_bits` is the effective number of bits allocated at
   `chunk_bits`
   
   @param: `base` is the number of bits before the chunk (of course not
   allocated).
 */
void soe_chunk(size_t nbits, word_t* st,
			   size_t chunk_bits, word_t* chunk, 
			   prime_t base)
{
	for(size_t i=1; i<nbits; ++i) // start from 1 if 1 is in primes
	{
		if(! GET(st,i)) // if it's a prime, then we sieve
		{
			prime_t p = I2P(i); // the prime in dec
			prime_t q = I2P(base);  // the first number in the chunk
			
			q = negmodp2I(q, p); // calculate offset
			
			while(q < chunk_bits) // while we are in the chunk
			{
				SET(chunk, q); // mark as composite
				q += p; // next multiplier
			}
		}
	}
}

#endif