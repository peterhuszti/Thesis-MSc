/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

#ifndef SIEVER
#define SIEVER

#include <stdint.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <thread>

#define LOG_WORD_SIZE 6 // 1 word = 2^(LOG_WORD_SIZE) i.e. 64 bit

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
	word_t lower_bound;
	word_t upper_bound;
	word_t last_number; // last number in the sieving table
	size_t log_upper_bound; // 2^(log_upper_bound) - 1 is the last number in the sieve table, i.e. last_number
	size_t size_of_st; // size of st in words
	size_t nbits; // effective number bits
	word_t number_of_chunks; // actual number of chunks
	word_t chunk_bits; // number of bits in each chunk
	word_t chunk_size; // size of chunks in words
	prime_t chunk_base; // first number in the searched interval
	int number_of_threads;
	int chunk_per_thread;
	int plus_one_sieve; // how many thread have to sieve one more chunk than the others (< number_of_threads)
	
	word_t* st; // sieving table
	word_t** chunks;
	std::vector<std::thread> threads;
	
	struct Params {int chunk_per_thread, index, first_chunk_to_sieve; prime_t offset;};
	
public:	

	Siever(word_t max_number_of_chunks, word_t low, word_t up, int num_thread): 
		lower_bound(low)
		, upper_bound(up)
		, number_of_threads(num_thread)
	{
		/**
			Initial calculations.
		*/
		last_number = sqrt(upper_bound);
		log_upper_bound = log2(last_number + 1);
		
		size_of_st = log_upper_bound < 7 ? 1 : P2I(1<<(log_upper_bound-LOG_WORD_SIZE));
		nbits = last_number / 2 + 1;
		
		word_t chunk_temp = upper_bound - lower_bound;
		number_of_chunks = max_number_of_chunks == 1 ? 1 : (chunk_temp / (1 << LOG_WORD_SIZE)) / 2;
		while(number_of_chunks > max_number_of_chunks)
		{
			number_of_chunks /= 2;
		}
		
		chunk_temp /= number_of_chunks;
		chunk_bits = chunk_temp <= 64 ? 64 : chunk_temp >> 1;
		chunk_size = chunk_bits / (sizeof(word_t) * CHAR_BIT);
		
		chunk_base = lower_bound / 2;
		
		/**
			Calculate how many chunks have to be sieved by each thread
		*/
		chunk_per_thread = number_of_chunks / number_of_threads;
		plus_one_sieve = number_of_chunks - chunk_per_thread*number_of_threads;	
		
		/**
			Allocate `st`.
		*/
		st = new word_t[size_of_st];
		for(size_t j=0; j<size_of_st; ++j) st[j]=0;
		
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
	
	/**
		Clean-up.
	*/
	~Siever()
	{
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
	   of size `nbits`.  A chunk is a chunk of a sieve table of 
	   bits from `base` to `base+chunk_bits-1` bits.
	 */
	void soe_chunks()
	{
		std::vector<Params> params(number_of_threads);

		for(size_t j=0; j<number_of_threads; ++j)
		{
			params[j].chunk_per_thread = j<plus_one_sieve ? chunk_per_thread+1 : chunk_per_thread;
			if(j == 0) 
			{
				params[j].offset = chunk_base;
				params[j].first_chunk_to_sieve = 0;
			}
			else 
			{
				params[j].offset = chunk_base + j*chunk_bits*params[j-1].chunk_per_thread;
				params[j].first_chunk_to_sieve = params[j-1].first_chunk_to_sieve + params[j-1].chunk_per_thread;
			}
			params[j].index = j;
		}
	
		for(size_t j=0; j<number_of_threads; ++j)
		{
			int chunk_per_thread_temp = j < plus_one_sieve ? chunk_per_thread+1 : chunk_per_thread;
			threads.push_back( std::thread( [this, j, &params] {
				for(int k=0; k<(params[j].chunk_per_thread); ++k)
				{
					for(size_t i=1; i<this->nbits; ++i) // start from 1 if 1 is in primes
					{
						if(! GET(this->st,i)) // if it's a prime, then we sieve
						{
							prime_t p = I2P(i); // the prime in dec
							prime_t q = I2P(params[j].offset);  // the first number in the chunk
							
							q = Siever::negmodp2I(q, p); // calculate offset in the actual chunk
							
							while(q < this->chunk_bits) // while we are in the chunk
							{
								SET(this->chunks[params[j].first_chunk_to_sieve+k], q); // mark as composite
								q += p; // next multiplier
							}
						}
					}
					(params[j].offset) += this->chunk_bits;		
				}
			}));			
		}
		
		for(size_t j=0; j<number_of_threads; ++j)
		{
			threads[j].join();
		}
	}
	
	/**
		Print/debug functions.
	*/
	friend class Printer;
	
private:

	/**
	   negmodp2I() calculates the index of the candidate \f$-x \bmod p\f$,
	   taking special care when \f$-x \bmod p\f$ is even.
	*/
	inline static prime_t negmodp2I(prime_t x, prime_t p)
	{
		prime_t q = x % p;
		q = q ? p-q : 0;
		return q % 2 ? P2I(q + p) : P2I(q);
	}

};

#endif











