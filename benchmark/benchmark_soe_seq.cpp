/*
	author: Peter Huszti
 */
 
#define BENCHPRESS_CONFIG_MAIN
#include "benchpress/benchpress.hpp"

#include <fstream>

#include "../source/utils/Siever.h"

#define MAX_NUMBER_OF_CHUNKS 1

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

BENCHMARK("Read config file: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
	
		auto bounds = read_config(CONFIG1);
		
		lower_bound = bounds.first;
		upper_bound = bounds.second;
    }
})

BENCHMARK("Initialize sieving 1: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;

		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	}
})

BENCHMARK("Initialize sieving 2: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
	
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	}
})
/*
BENCHMARK("Initialize sieving 3: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;

		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	}
})*/

BENCHMARK("Initialize sieving 4: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
	
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	}
})

BENCHMARK("Initialize sieving 5: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
	
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	}
})

BENCHMARK("Sieve chunks 1: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG1);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
		
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 2: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG2);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})
/*
BENCHMARK("Sieve chunks 3: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG3);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})*/

BENCHMARK("Sieve chunks 4: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG4);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})

BENCHMARK("Sieve chunks 5: ", [](benchpress::context* ctx) {
	for (size_t i = 0; i < ctx->num_iterations(); ++i)
	{
		word_t lower_bound = 0; // both should be a power of 2
		word_t upper_bound = 0;
		
		auto bounds = read_config(CONFIG5);
			
		lower_bound = bounds.first;
		upper_bound = bounds.second;
		
		Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, 1);
		siever.soe_init();
	
		siever.soe_chunks();
	}
})