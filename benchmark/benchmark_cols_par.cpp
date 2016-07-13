/*
	author: Peter Huszti
 */
 
#define BENCHPRESS_CONFIG_MAIN
#include "benchpress/benchpress.hpp"

#include <fstream>

#include "../source/utils/Siever.h"

#define MAX_NUMBER_OF_CHUNKS 128
#define THREADS_2 2
#define THREADS_4 4
#define THREADS_8 8
#define THREADS_16 16
#define THREADS_32 32

#define CONFIG1 "../source/utils/config1.txt"
#define CONFIG2 "../source/utils/config2.txt"
#define CONFIG3 "../source/utils/config3.txt"
#define CONFIG4 "../source/utils/config4.txt"
#define CONFIG5 "../source/utils/config5.txt"

std::pair<word_t,word_t> read_config(std::string config)
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;

	/**
		Read config file
	*/
	std::ifstream reader;
	reader.open(config);	
	char line[64];
	reader.getline(line, 64, ' ');
	reader >> lower_bound;
	reader.getline(line, 64, ' ');
	reader >> upper_bound;
	reader.close();

	return std::pair<word_t,word_t>(lower_bound, upper_bound);
}

BENCHMARK("Sieve chunks 1 (2 threads): ", [](benchpress::context* ctx) {	
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_2);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 1 (4 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_4);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 1 (8 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_8);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 1 (16 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_16);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 1 (32 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_32);
		siever.soe_init();
			
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 2 (2 threads): ", [](benchpress::context* ctx) {	
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_2);
		siever.soe_init();
		
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 2 (4 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_4);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 2 (8 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_8);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 2 (16 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_16);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 2 (32 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_32);
		siever.soe_init();
				
		siever.soe_chunks();
	}
})
/*
BENCHMARK("Sieve chunks 3 (2 threads): ", [](benchpress::context* ctx) {	
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_2);
		siever.soe_init();
		
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 3 (4 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_4);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 3 (8 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_8);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 3 (16 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_16);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 3 (32 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_32);
		siever.soe_init();
			
		siever.soe_chunks();
	}
})
*/
BENCHMARK("Sieve chunks 4 (2 threads): ", [](benchpress::context* ctx) {	
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_2);
		siever.soe_init();
		
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 4 (4 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_4);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 4 (8 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_8);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 4 (16 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_16);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 4 (32 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_32);
		siever.soe_init();
				
		siever.soe_chunks();
	}
})


BENCHMARK("Sieve chunks 5 (2 threads): ", [](benchpress::context* ctx) {	
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_2);
		siever.soe_init();
		
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 5 (4 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_4);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 5 (8 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_8);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 5 (16 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_16);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 5 (32 threads): ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, THREADS_32);
		siever.soe_init();
			
		siever.soe_chunks();
	}
})