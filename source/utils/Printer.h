/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */
 
#include <fstream>
#include <vector>
 
#ifndef PRINTER
#define PRINTER

class Printer
{

private:

	Siever* siever;
	
	std::ofstream out;

public:

	Printer(Siever* s): siever(s) 
	{
		out.open("result.txt", std::ofstream::out);
	}
	
	~Printer()
	{
		out.close();
	}

	void print_debug_info()
	{
		std::cout << "Sieve of Eratosthenese\n\n";
		
		std::cout << "lower bound " << siever->lower_bound << std::endl;
		std::cout << "upper bound " << siever->upper_bound << std::endl;
		
		std::cout << "number of chunks " << siever->number_of_chunks << std::endl;
		
		std::cout << "chunk_size " << siever->chunk_size << std::endl;
		std::cout << "chunk_bits " << siever->chunk_bits << std::endl;
		
		std::cout << "last number " << siever->last_number << std::endl;
		std::cout << "log upper bound " << siever->log_upper_bound << std::endl;
			
		std::cout << "size of st " << siever->size_of_st << std::endl;
		std::cout << "nbits " << siever->nbits << "\n\n";
		
		std::cout << "number of threads " << siever->number_of_threads << std::endl;
		std::cout << "chunk per thread " << siever->chunk_per_thread << std::endl;
		std::cout << "plus one sieve " << siever->plus_one_sieve << std::endl;
	}
	
	std::vector<int> get_primes_found()
	{
		std::vector<int> primes;
		
		prime_t chunk_base_temp = siever->chunk_base;
		
		for (size_t j=0; j<siever->number_of_chunks; ++j)
		{
			for (size_t i=0; i<siever->chunk_bits; ++i)
				if (!GET( siever->chunks[j], i )) 
				{
					primes.push_back(I2P(i + chunk_base_temp));
				}			
			chunk_base_temp += siever->chunk_bits;
		}
		
		return primes;
	}
	
	void print_primes_found()
	{
		out << "The found primes in the given interval:\n";
		
		prime_t chunk_base_temp = siever->chunk_base;
		
		for (size_t j=0; j<siever->number_of_chunks; ++j)
		{
			for (size_t i=0; i<siever->chunk_bits; ++i)
				if (!GET( siever->chunks[j], i )) 
				{
					out << I2P(i + chunk_base_temp) << ", ";
				}
			out << std::endl;
			
			chunk_base_temp += siever->chunk_bits;
		}
		
		out << std::endl;
	}
	
	void print_sieving_table()
	{
		out << "The sieving table:\n";
		
		for (size_t i=0; i<siever->nbits; i++) 
			if (! GET(siever->st,i)) out << I2P(i) << ", ";
		out << "\n\n";
	}
	
	void print_number_of_found_primes()
	{
		std::cout << "The number of found primes: ";
	
		int number_of_found_primes = 0;
		prime_t chunk_base_temp = siever->chunk_base;
		
		for (size_t j=0; j<siever->number_of_chunks; ++j)
		{
			for (size_t i=0; i<siever->chunk_bits; ++i)
				if (! GET( siever->chunks[j], i )) 
				{
					number_of_found_primes++;
				}
			chunk_base_temp += siever->chunk_bits;
		}
		
		std::cout << number_of_found_primes << std::endl;
	}

};

#endif