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

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define DEBUG true // true if we want to print debug info
#define PRIMES true // true if we want to show the found primes
 
typedef uint64_t prime_t;
typedef uint64_t word_t;
typedef uint16_t offset_t;
typedef prime_t circle_t;
typedef prime_t bucket_t;

struct pair_t
{
	size_t prime_index;
	offset_t offset;
};

struct input
{
	word_t low;
	word_t up;
	int max_number_of_chunks;
	int number_of_threads;
	
	input(word_t l, word_t u, int ch, int th):
		low(l),
		up(u),
		max_number_of_chunks(ch),
		number_of_threads(th)
	{}
};

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
	int number_of_circles;
	int number_of_buckets;
	
	word_t* st; // sieving table
	pair_t** st_pairs;
	word_t** chunks;
	bucket_t** buckets;
	circle_t* circles;
	std::vector<std::thread> threads;
	
	struct Params_for_threads 
	{
		int chunk_per_thread;
		int first_chunk_to_sieve;
		prime_t starting_point;
	};
	
public:	
	
	Siever() = delete;
	Siever(input in)
	{
		lower_bound = in.low;
		upper_bound = in.up;
		number_of_threads = in.number_of_threads;
		
		/**
			Initial calculations.
		*/
		last_number = sqrt(upper_bound);
		log_upper_bound = log2(last_number + 1);
	
		size_of_st = log_upper_bound < 7 ? 1 : P2I(1<<(log_upper_bound-LOG_WORD_SIZE));
		nbits = last_number / 2 + 1;

		word_t chunk_temp = upper_bound - lower_bound;

		number_of_chunks = in.max_number_of_chunks == 1 ? 1 : (chunk_temp / (1 << LOG_WORD_SIZE)) / 2;
		while (number_of_chunks > in.max_number_of_chunks)
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
			Allocate st.
		*/
		st = new word_t[size_of_st];
		for (size_t j=0; j<=size_of_st; ++j) st[j]=0;
		
		/**
			Allocate st_pairs.
		*/
		st_pairs = new pair_t*[number_of_threads];
		for (size_t j=0; j<number_of_threads; ++j) 
		{
			st_pairs[j] = new pair_t[nbits];
			for (int i=0; i<nbits; ++i)
			{
				st_pairs[j][i].prime_index = i+1;
				st_pairs[j][i].offset = 0;
			}
		}
		
		/**
			Initialization of chunks 
		*/
		chunks = new word_t*[number_of_chunks];
		for (size_t i=0; i<=number_of_chunks; ++i)
		{
			chunks[i] = new word_t[chunk_size];
			for (size_t j=0; j<=chunk_size; ++j) chunks[i][j]=0;
		}

		number_of_circles = (nbits / chunk_bits) + 1;
		/**
			Allocate circles
			The circles reference the offset tables, but they won't be modified during the run, 
			so don't need to allocate for all threads.
		*/
		circles = new circle_t[number_of_circles];
		
		number_of_buckets = number_of_circles * (number_of_circles+1) / 2;
		/**
			Allocate buckets
			Every thread needs its own buckets, because it will reference the offset tables
		*/
		buckets = new bucket_t*[number_of_threads];
		for (size_t i=0; i<number_of_threads; ++i)
		{
			buckets[i] = new bucket_t[number_of_buckets];
			for (size_t j=0; j<=number_of_buckets; ++j) buckets[i][j]=0;
		}
	}
	
	/**
		Clean-up.
	*/
	~Siever()
	{
		for (size_t i=0; i<number_of_chunks; i++) delete[] chunks[i];
		delete[] chunks;
		delete[] circles;
		for (size_t i=0; i<number_of_threads; i++) delete[] buckets[i];
		delete[] buckets;
		delete[] st;
		for (size_t i=0; i<number_of_threads; i++) delete[] st_pairs[i];
		delete[] st_pairs;
	}

	/**
	   Implements the sieving of "small" primes.
	*/
	void soe_init()
	{
		prime_t p = 3; // first prime
		prime_t q = P2I(p); // index in st

		while (p * p < I2P(nbits)) // need to sieve only until sqrt(upper_bound)
		{
			while (GET(st,q)) q++; // search the next 0 in st, i.e. the next prime to sieve with

			p = I2P(q); // what is this next sieving prime
			prime_t i = P2I(p*p); // need to sieve only from p^2 because the smallers are already sieved

			while (i < nbits) // sieve until it is in st
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
		init_circles();
	
		std::vector<Params_for_threads> params(number_of_threads);

		for (size_t j=0; j<number_of_threads; ++j)
		{
			params[j].chunk_per_thread = j<plus_one_sieve ? chunk_per_thread+1 : chunk_per_thread;
			if (j == 0) 
			{
				params[j].starting_point = chunk_base;
				params[j].first_chunk_to_sieve = 0;
			}
			else 
			{
				params[j].starting_point = chunk_base + j*chunk_bits*params[j-1].chunk_per_thread;
				params[j].first_chunk_to_sieve = params[j-1].first_chunk_to_sieve + params[j-1].chunk_per_thread;
			}
		}
		
		init_buckets(params);
		
		for (size_t thread_id=0; thread_id<number_of_threads; ++thread_id)
		{
			threads.push_back( std::thread( [this, thread_id, &params] {
				for (size_t chunk_id=0; chunk_id<params[thread_id].chunk_per_thread; ++chunk_id) // iterate through the chunks
				{
					bool last_chunk = (chunk_id == params[thread_id].chunk_per_thread - 1);
					for (circle_t circle_id = 0; circle_id < number_of_circles; ++circle_id) // iterate through circles
					{
						#if DEBUG
							std::cout << "CIRCLE:   " << this->circles[circle_id] << std::endl << std::flush;
						#endif
						
						std::pair<bucket_t,bucket_t> actual_bucket = get_actual_bucket(thread_id, circle_id);
						
						#if DEBUG
						std::cout << "ACTUAL BUCKET:   " << actual_bucket.first << "   " << actual_bucket.second << std::endl << std::flush;
						#endif
						
						bool broken_bucket = actual_bucket.first > actual_bucket.second;
						size_t start, end;

						if (circle_id == 0)
						{
							start = 0;
							end = nbits;	
						}
						else
						{
							start = P2I(actual_bucket.first);
							if (broken_bucket) // if the bucket is broken, then we start sieving until the top of the circle
							{
								end = P2I(this->circles[circle_id]);
							}
							{
								end = P2I(actual_bucket.second);
							}					
						}
							
						#if DEBUG
							std::cout << start << "   " << end << std::endl << std::flush;
						#endif
						
						Sieve(thread_id, chunk_id, params, start, end);
						if (broken_bucket) // we must sieve the bottom of the circle as well
						{
							std::cout << "    ***   BROKEN" << std::endl << std::flush;
								// from 0 to actual_bucket.second
							start = P2I(this->circles[circle_id]+1);
							end = P2I(actual_bucket.second);
							Sieve(thread_id, chunk_id, params, start, end);
						}
					}
					if (!last_chunk)
					{
						update_buckets(thread_id);
					}
				}
			}));
		}
		
		for (size_t j=0; j<number_of_threads; ++j)
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
		Sieve with the given bucket
	*/
	void Sieve(size_t thread_id, size_t chunk_id, const std::vector<Params_for_threads> &params, size_t start, size_t end)
	{
		for (size_t index=start; index<end; ++index) // start from 1 if 1 is in primes
		{
			if (!GET( st, st_pairs[thread_id][index].prime_index )) // if it's a prime, then we sieve
			{					
				prime_t p = I2P(st_pairs[thread_id][index].prime_index); // the prime in dec
				
				#if DEBUG
					std::cout << "SIEVING WITH:  " << p << std::endl << std::flush;
				#endif
				
				prime_t offset = st_pairs[thread_id][index].offset; // get the offset of the current prime in the actual chunk
				
				#if DEBUG
					std::cout << "OFFSET:  " << offset << std::endl << std::flush;
				#endif
				
				while (offset < chunk_bits) // while we are in the chunk
				{								
					SET(chunks[params[thread_id].first_chunk_to_sieve + chunk_id], offset); // mark as composite

					#if DEBUG
						std::cout << "SIEVED OUT " << I2P(chunk_base+(chunk_bits*chunk_id) + offset) << std::endl << std::flush;
					#endif
					
					offset += p; // next multiplier
				}
			update_offset(thread_id, index, offset);
			}							
		}
	}

	/**
		Initializes the offset table for each thread.
	*/
	void init_offsets(const std::vector<Params_for_threads> &params)
	{
		for (size_t j=0; j<number_of_threads; ++j) // for all threads
		{
			for (size_t i=1; i<nbits; ++i) // start from 1 if 1 is in primes
			{	
				if (!GET( st, i )) // if it's a prime, then we calculate offset
				{
					prime_t p = I2P(i); // the prime in dec
					prime_t q = I2P(params[j].starting_point);  // the first number in the chunk
					
					#if DEBUG
						std::cout <<"PRIME " << p << "    "<<  negmodp2I(q, p) <<std::endl;
					#endif
					
					st_pairs[j][i-1].offset = negmodp2I(q, p); // calculate offset in the actual chunk
				}
				else
				{
					#if DEBUG
						std::cout <<"               NOT PRIME  " << I2P(i) <<std::endl;
					#endif
				}
			}
		}
	}
	
	/**
		Updates the offset table for the next chunk.
	*/
	inline void update_offset(size_t thread_id, size_t index, prime_t last_offset)
	{
		if (!GET( st, index )) st_pairs[thread_id][index].offset = last_offset - chunk_bits;
	}
	
	/**
		Initializes the cirlces.
	*/
	void init_circles()
	{
		#if DEBUG
			std::cout << "CIRCLES " << std::endl << std::flush;
		#endif
		
		word_t temp = I2P(chunk_bits);
		word_t border = temp;
		for (size_t i=0; i<number_of_circles; ++i)
		{
			circles[i] = MIN(border, I2P(nbits));
			
			#if DEBUG
				std::cout << circles[i] << std::endl << std::flush;
			#endif
			
			border += temp;
		}
	}
	
	/**
		Initializes the buckets.
		It also updates the offsets, because from now on it makes no sense that an offset could be
		larger than the size of a chunk.
	*/
	void init_buckets(const std::vector<Params_for_threads> &params)
	{
		init_offsets(params);
	
		std::vector<std::vector<int> > which_bucket(number_of_threads);
		for (int i=0; i<number_of_threads; ++i)
		{
			which_bucket[i].resize(nbits, -1);
		}
		
		for (size_t thread_id=0; thread_id<number_of_threads; ++thread_id)
		{
			for (int i=0; i<nbits; ++i)
			{
				for (int j=0; j<number_of_circles; ++j)
				{
					if (st_pairs[thread_id][i].offset > j*I2P(chunk_bits))
					{
						which_bucket[thread_id][i] = j;
					}
				}
		//		if (st_pairs[thread_id][i].offset) st_pairs[thread_id][i].offset -= which_bucket[thread_id][i]*I2P(chunk_bits);
			}
		}
		
		sort_b(which_bucket);
		
		#if DEBUG
			for (int i=0; i<nbits; ++i)
			{
				std::cout << "____ " << I2P(i) << " " << I2P(st_pairs[0][i].prime_index)<< " " << st_pairs[0][i].offset << " " << which_bucket[0][i] <<std::endl << std::flush;
			}
		#endif
		
		for (size_t j=0; j<number_of_threads; ++j) // for all threads
		{
			buckets[j][0] = circles[0]; // the first bucket is equals to the first circle everytime
		
			size_t b = 1;
			for (circle_t circle_id=1; circle_id<number_of_circles; ++circle_id)
			{
				size_t p = circles[circle_id-1];
				word_t temp = I2P(chunk_bits);
				for (bucket_t bucket_id=0; bucket_id<circle_id+1; ++bucket_id)
				{
					while (p < circles[circle_id]) 
					{
						#if DEBUG
							std::cout << "*** " << P2I(p) << " " << circles[circle_id] << " "  << st_pairs[j][P2I(p)-1].offset << " " << temp << std::endl <<std::flush; 
						#endif
						
						if (!GET( st, P2I(p) ))
						{
							#if DEBUG
								std::cout << "   PRIME " << std::endl << std::flush;
							#endif
								
							if (st_pairs[j][P2I(p)-1].offset >= temp)
							{
								break;
							}
						}
						p++;
					}
					// iterate through the primes until either we reach the end of the current circle,
					// or the offset indicates that we won't sieve into the next bucket
					buckets[j][b++] = p-1;
					if (bucket_id != 0)
					{
						for (size_t i=buckets[j][b-1]; i<buckets[j][b]; ++i)
						{
							st_pairs[j][i].offset -= I2P(chunk_bits); 
							// reset the offsets so we can roll the circles easier
						}
					}
					temp += I2P(chunk_bits);
				}
		//		for (size_t prime=circle_id*(circle_od+1)/2; prime<)
			}
			
			#if DEBUG			
				for (int i=0;i<number_of_buckets;i++)
				{ std::cout << " BUCKETS: " << buckets[j][i] << std::endl;}
			#endif
		}
	}
	
	/**
	
	*/
	void sort_b(std::vector<std::vector<int> > &which_bucket)
	{
		for (size_t thread_id=0; thread_id<number_of_threads; ++thread_id) 
		{
			for (int circle_id=0; circle_id<number_of_circles-1; ++circle_id)
			{
				size_t limit = MIN(P2I(circles[circle_id+1]),nbits);
				for (int p=P2I(circles[circle_id]); p<limit-1; ++p)
				{
					for (int q=p+1; q<limit; ++q)
					{
						if (which_bucket[thread_id][q] < which_bucket[thread_id][q-1])
						{
							size_t temp = which_bucket[thread_id][q];
							which_bucket[thread_id][q] = which_bucket[thread_id][q-1];
							which_bucket[thread_id][q-1] = temp;
							
							pair_t temp2 = st_pairs[thread_id][q];
							st_pairs[thread_id][q] = st_pairs[thread_id][q-1];
							st_pairs[thread_id][q-1] = temp2;
						}
					}
				}
			}
		}
	}
	
	/**
		Returns the actual sieving bucket.
	*/
	inline std::pair<bucket_t,bucket_t> get_actual_bucket(size_t thread_id, circle_t circle_id)
	{
		return std::pair<bucket_t,bucket_t> (buckets[thread_id][circle_id*(circle_id+1)/2-1], 
											 buckets[thread_id][circle_id*(circle_id+1)/2]);
	}
	
	/**
		Updates the buckets.
	*/
	void update_buckets(size_t thread_id)
	{
		for (circle_t circle_id=1; circle_id<number_of_circles; ++circle_id)
		{
			auto actual_bucket = get_actual_bucket(thread_id, circle_id);
			bool stay = false;
			bucket_t index = actual_bucket.first;
			bucket_t end = actual_bucket.second;
			word_t limit = chunk_bits * (circle_id+1);
			while (!stay && index <= MAX(end,nbits))
			{
				// find the primes that will need to move to the former bucket
				stay = st_pairs[thread_id][index].offset < limit; 
				index++;
			}
			if (actual_bucket.first > end)
			{
				index = circles[circle_id];
				while (!stay && index < end)
				{
					stay = st_pairs[thread_id][index].offset < limit;
					index++;
				}
			}
			
// bucket elejere
			
			bucket_t first_bucket_in_circle = circle_id*(circle_id+1)/2;
			if (stay)
			{
				buckets[thread_id][first_bucket_in_circle] = index-1;
			}
			
			bucket_t last_bucket_in_circle = first_bucket_in_circle + circle_id;
			bucket_t temp = buckets[thread_id][last_bucket_in_circle];
			for (bucket_t bucket_id=last_bucket_in_circle; bucket_id>first_bucket_in_circle+1; --bucket_id)
			{
				// roll the circle, it means that in any given circle the 1st and 
				// 2nd ([0] and [1]) pointers give us the acutal bucket
				buckets[thread_id][bucket_id] = buckets[thread_id][bucket_id-1];
			}
			buckets[thread_id][first_bucket_in_circle+1] = temp;
		}	
	}
	
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






















