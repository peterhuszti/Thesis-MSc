/*
	author: Peter Huszti
 */

#ifndef UTILS
#define UTILS

#include <iostream>
#include <fstream>

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

void start(int argc, char *argv[], int MAX_NUMBER_OF_CHUNKS)
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;
	int number_of_threads = 0;
	
	std::string config = "../utils/";
	if (argc == 1)
	{
		config += "config1.txt";
		number_of_threads = 1;
	}
	else if (argc == 2)
	{
		config += "config1.txt";
		number_of_threads = atoi(argv[1]);
	}
	else
	{
		config += argv[2];
		number_of_threads = atoi(argv[1]);
	}
	auto bounds = read_config(config);
		
	lower_bound = bounds.first;
	upper_bound = bounds.second;
	
	Siever siever(MAX_NUMBER_OF_CHUNKS, lower_bound, upper_bound, number_of_threads);
	
	Printer printer(&siever);
	#if DEBUG	
		printer.print_debug_info();
	#endif
	
	siever.soe_init();
	siever.soe_chunks();
	
	#if PRIMES
		printer.print_sieving_table();
		printer.print_primes_found();
	#endif
	
	#if DEBUG
		printer.print_number_of_found_primes();
	#endif
}

#endif
