/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */
 
#ifndef PRINTER
#define PRINTER

#include "Siever.h"

class Printer
{

private:

	Siever* siever;

public:

	Printer(Siever* s): siever(s) {}

	void print_debug_info()
	{
		std::cout << "Sieve of Eratosthenese\n\n";
		
		std::cout << "lower bound " << LOWER_BOUND << std::endl;
		std::cout << "upper bound " << UPPER_BOUND << std::endl;
		
		std::cout << "number of chunks " << siever->number_of_chunks << std::endl;
		
		std::cout << "chunk_size " << siever->chunk_size << std::endl;
		std::cout << "chunk_bits " << siever->chunk_bits << std::endl;
		
		std::cout << "last number " << siever->last_number << std::endl;
		std::cout << "log upper bound " << siever->log_upper_bound << std::endl;
			
		std::cout << "size of st " << siever->size_of_st << std::endl;
		std::cout << "nbits " << siever->nbits << "\n\n";
	}
	
	void print_primes_found()
	{
		std::cout << "The found primes in the given interval:\n";
		
		prime_t chunk_base_temp = siever->chunk_base;
		
		for(size_t j=0; j<siever->number_of_chunks; ++j)
		{
			for(size_t i=0; i<siever->chunk_bits; ++i)
				if(! GET( siever->chunks[j], i )) 
				{
					std::cout << I2P(i + chunk_base_temp) << ", ";
				}
			std::cout << std::endl;
			
			chunk_base_temp += siever->chunk_bits;
		}
		
		std::cout << std::endl;
	}
	
	void print_sieving_table()
	{
		std::cout << "The sieving table:\n";
		
		for(size_t i=0; i<siever->nbits; i++) 
			if(! GET(siever->st,i)) std::cout << I2P(i) << ", ";
		std::cout << "\n\n";
	}
	
	void print_number_of_found_primes()
	{
		std::cout << "The number of found primes: ";
	
		int number_of_found_primes = 0;
		prime_t chunk_base_temp = siever->chunk_base;
		
		for(size_t j=0; j<siever->number_of_chunks; ++j)
		{
			for(size_t i=0; i<siever->chunk_bits; ++i)
				if(! GET( siever->chunks[j], i )) 
				{
					number_of_found_primes++;
				}
			chunk_base_temp += siever->chunk_bits;
		}
		
		std::cout << number_of_found_primes << std::endl;
	}

};

#endif