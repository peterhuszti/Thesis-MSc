/*
	author: Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

/**
   @file: simple_soe.c This program is a simple implementation of the
   sieve of Eratosthenes (SOE).  It consists of two stages:
   - Stage 1: sieving out the "small" primes, i.e. from 0 to some
     smaller bound 'nbits'.
   - Stage 2: using the primes/sieve table from stage 1, sieve chunks
     starting from a given base.
   Here of course, nbits*nbits > base should be true.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <iostream>
#include <math.h>

typedef uint64_t prime_t;
typedef uint64_t word_t;
#define LOG_WORD_SIZE 6 // 1 word = 2^(LOG_WORD_SIZE) i.e. 64 bit

#define LOWER_BOUND 1024
#define UPPER_BOUND 2048 // both should be a power of 2
#define MAX_NUMBER_OF_CHUNKS 32

#define INDEX(i) ((i)>>(LOG_WORD_SIZE))
#define MASK(i) ((word_t)(1) << ((i)&(((word_t)(1)<<LOG_WORD_SIZE)-1)))
#define GET(p,i) (p[INDEX(i)]&MASK(i))
#define SET(p,i) (p[INDEX(i)]|=MASK(i))
#define RESET(p,i) (p[INDEX(i)]&=~MASK(i))
#define P2I(p) ((p)>>1) // (((p-2)>>1)) 
#define I2P(i) (((i)<<1)+1) // ((i)*2+3)


/**
   Print/debug functions.
*/
void print_table(size_t n, word_t *p)
{
	for(size_t i=n; i>0; --i)
		printf("%08lx ",p[i-1]);
	std::cout << std::endl;
}

void print_primes(size_t nbits, word_t *st)
{
	for(size_t i=0; i<nbits; i++) 
		if(! GET(st,i)) std::cout << I2P(i) << ", ";
	std::cout << std::endl;
}

void print_primes_chunk(size_t nbits, word_t *st, size_t base)
{
	for(size_t i=0; i<nbits; i++)
		if(! GET( st, i )) std::cout << I2P(i + base) << ", ";
	std::cout << std::endl;
}


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
			prime_t q = I2P( base );  // the first number in the chunk
			
			assert(p<q);
			q = negmodp2I( q, p ); // calculate offset
			
			while(q < chunk_bits) // while we are in the chunk
			{
				SET(chunk, q); // mark as composite
				q += p; // next multiplier
			}
		}
	}
}

int main()
{
	std::cout << "Simple Sieve of Eratosthenese\n\n";

	std::cout << "lower bound " << LOWER_BOUND << std::endl;
	std::cout << "upper bound " << UPPER_BOUND << std::endl;
	
	size_t chunk_temp = UPPER_BOUND - LOWER_BOUND;
	size_t number_of_chunks = (chunk_temp / (1 << LOG_WORD_SIZE)) / 2;
	while(number_of_chunks > MAX_NUMBER_OF_CHUNKS)
	{
		number_of_chunks /= 2;
	}
	
	std::cout << "number of chunks " << number_of_chunks << std::endl;
	
	word_t** chunks = (word_t**) malloc(sizeof(word_t*)*number_of_chunks);
	word_t* chunk;
	
	/**
	 Initialization of chunks 
	*/
	chunk_temp /= number_of_chunks;
	size_t chunk_bits =  chunk_temp <= 64 ? 64 : chunk_temp >> 1;
	size_t chunk_size = chunk_bits / (sizeof(word_t) * CHAR_BIT);

	std::cout << "chunk_size " << chunk_size << std::endl;
	std::cout << "chunk_bits " << chunk_bits << std::endl;
	
	for(size_t i=0; i<number_of_chunks; ++i) 
	{
		chunks[i] = (word_t*) calloc(sizeof(word_t), chunk_size);
	}
	
	/**
		Allocate `st`.
	*/
	word_t last_number = sqrt(UPPER_BOUND);
	size_t log_upper_bound = log2(last_number + 1);
	// 2^(log_upper_bound) - 1 is the last number in the sieve table, i.e. last_number
	
	std::cout << "last_number " << last_number << std::endl;
	std::cout << "log_upper_bound " << log_upper_bound << std::endl;
	
	size_t n = log_upper_bound < 7 ? 1 : P2I(1<<(log_upper_bound-LOG_WORD_SIZE));
	size_t nbits = last_number / 2 + 1;
	word_t *st = (word_t*) calloc(sizeof(*st), n);
	
	std::cout << "n " << n << std::endl;
	std::cout << "nbits " << nbits << "\n\n";
	
	prime_t chunk_base = LOWER_BOUND / 2;
	soe_init(nbits, st);

	std::cout << "The found primes in the given interval:\n";
	for(size_t i=0; i<number_of_chunks; ++i) 
	{
		soe_chunk( nbits, st, chunk_bits, chunks[i], chunk_base);
		print_primes_chunk( chunk_bits, chunks[i], chunk_base);
		chunk_base += chunk_bits;
	}

	for(size_t i=0; i<number_of_chunks; i++) free(chunks[i]);
	free(chunks);
	free(st);

	std::cout << "--- the end ---\n";
	
	return 0;
}