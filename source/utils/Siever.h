/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

#ifndef SIEVER
#define SIEVER

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <iostream>
#include <math.h>

#define LOG_WORD_SIZE 6 // 1 word = 2^(LOG_WORD_SIZE) i.e. 64 bit

#define LOWER_BOUND 536870912
#define UPPER_BOUND 1073741824 // both should be a power of 2
// #define LOWER_BOUND 1024
// #define UPPER_BOUND 2048 // both should be a power of 2

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

class Siever
{

private:

	/**
		Fields
	*/
	word_t last_number;
	size_t log_upper_bound; // 2^(log_upper_bound) - 1 is the last number in the sieve table, i.e. last_number
	size_t n;
	size_t nbits; // effective number bits
	size_t number_of_chunks;
	word_t chunk_bits;
	word_t chunk_size;
	prime_t chunk_base;
	
	word_t* st;
	word_t** chunks;
	
public:	

	Siever(size_t max_number_of_chunks)
	{
		/**
			Initial calculations.
		*/
		last_number = sqrt(UPPER_BOUND);
		log_upper_bound = log2(last_number + 1);
		
		n = log_upper_bound < 7 ? 1 : P2I(1<<(log_upper_bound-LOG_WORD_SIZE));
		nbits = last_number / 2 + 1;
		
		size_t chunk_temp = UPPER_BOUND - LOWER_BOUND;
		number_of_chunks = max_number_of_chunks == 1 ? 1 : (chunk_temp / (1 << LOG_WORD_SIZE)) / 2;
		while(number_of_chunks > max_number_of_chunks)
		{
			number_of_chunks /= 2;
		}
		
		chunk_temp /= number_of_chunks;
		chunk_bits = chunk_temp <= 64 ? 64 : chunk_temp >> 1;
		chunk_size = chunk_bits / (sizeof(word_t) * CHAR_BIT);
		
		chunk_base = LOWER_BOUND / 2;
		
		/**
			Allocate `st`.
		*/
		st = new word_t[n];
		for(size_t j=0; j<n; ++j) st[j]=0;
		
		/**
			Initialization of chunks 
		*/	
		chunks = new word_t*[number_of_chunks];
		for(size_t i=0; i<number_of_chunks; ++i)
		{
			chunks[i] = new word_t[chunk_size];
			for(size_t j=0; j<chunk_size; ++j) chunks[i][j]=0;
		}
	}
	
	~Siever()
	{
		/**
			Clean-up.
		*/
		for(size_t i=0; i<number_of_chunks; i++) delete[] chunks[i];
		delete[] chunks;
		delete[] st;
	}

	/**
	   Implements the sieving of "small" primes.  
	*/
	void soe_init()
	{
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
	   Performs SOE on the chunks with primes read from the sieve table `st`
	   of size `nbits` (effective number bits).  A chunk (at `chunk`) is a
	   chunk of a sieve table of bits from `base` to `base+chunk_bits-1`
	   bits.
	 */
	void soe_chunks()
	{
		prime_t chunk_base_temp = chunk_base;
	
		for(size_t j=0; j<number_of_chunks; ++j)
		{
			for(size_t i=1; i<nbits; ++i) // start from 1 if 1 is in primes
			{
				if(! GET(st,i)) // if it's a prime, then we sieve
				{
					prime_t p = I2P(i); // the prime in dec
					prime_t q = I2P(chunk_base_temp);  // the first number in the chunk
					
					q = negmodp2I(q, p); // calculate offset
					
					while(q < chunk_bits) // while we are in the chunk
					{
						SET(chunks[j], q); // mark as composite
						q += p; // next multiplier
					}
				}
			}
			
			chunk_base_temp += chunk_bits;
		}
	}
	
	/**
		Print/debug functions.
	*/
	void print_debug_info()
	{
		std::cout << "Sieve of Eratosthenese\n\n";
		
		std::cout << "lower bound " << LOWER_BOUND << std::endl;
		std::cout << "upper bound " << UPPER_BOUND << std::endl;
		
		std::cout << "number of chunks " << number_of_chunks << std::endl;
		
		std::cout << "chunk_size " << chunk_size << std::endl;
		std::cout << "chunk_bits " << chunk_bits << std::endl;
		
		std::cout << "last_number " << last_number << std::endl;
		std::cout << "log_upper_bound " << log_upper_bound << std::endl;
			
		std::cout << "n " << n << std::endl;
		std::cout << "nbits " << nbits << "\n\n";
	}
	
	void print_primes_found()
	{
		std::cout << "The found primes in the given interval:\n";
		
		prime_t chunk_base_temp = chunk_base;
		
		for(size_t j=0; j<number_of_chunks; ++j)
		{
			for(size_t i=0; i<chunk_bits; ++i)
				if(! GET( chunks[j], i )) 
				{
					std::cout << I2P(i + chunk_base_temp) << ", ";
				}
			std::cout << std::endl;
			
			chunk_base_temp += chunk_bits;
		}
		
		std::cout << std::endl;
	}
	
	void print_sieving_table()
	{
		std::cout << "The sieving table:\n";
		
		for(size_t i=0; i<nbits; i++) 
			if(! GET(st,i)) std::cout << I2P(i) << ", ";
		std::cout << "\n\n";
	}
	
	void print_number_of_found_primes()
	{
		std::cout << "The number of found primes: ";
	
		int number_of_found_primes = 0;
		prime_t chunk_base_temp = chunk_base;
		
		for(size_t j=0; j<number_of_chunks; ++j)
		{
			for(size_t i=0; i<chunk_bits; ++i)
				if(! GET( chunks[j], i )) 
				{
					number_of_found_primes++;
				}
			chunk_base_temp += chunk_bits;
		}
		
		std::cout << number_of_found_primes << std::endl;
	}
	
private:

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

};

#endif











