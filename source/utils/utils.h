/*
	author: Peter Huszti
 */

#ifndef UTILS
#define UTILS

#include <iostream>
#include <fstream>
#include "Siever.h"

input read_config(std::string config)
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;
	int max_number_of_chunks = 0;
	int number_of_threads = 0;

	/**
		Read config file
	*/
	std::ifstream reader;
	reader.open(config);
	char line[64];
	reader.getline(line, 64, ':');
	reader >> lower_bound;
	reader.getline(line, 64, ':');
	reader >> upper_bound;
	reader.getline(line, 64, ':');
	reader >> max_number_of_chunks;
	reader.getline(line, 64, ':');
	reader >> number_of_threads;
	reader.close();

	return input(lower_bound, upper_bound, max_number_of_chunks, number_of_threads);
}

void start(int argc, char *argv[])
{
	std::string config = "utils/";
	std::string result = "../result/";
	
	if (argc == 1)
	{
		config += "config1.txt";
		result += "result.txt";
	}
	else if (argc == 2)
	{
		config += argv[1];
		result += "result.txt";
	}
	else
	{
		config += argv[2];
		result += argv[3];
	}
	
	Siever siever(read_config(config));
	
	Printer printer(&siever, result);
	#if DEBUG	
		printer.print_debug_info();
	#endif
	
	siever.soe_init();
	siever.soe_chunks();
	
	#if PRIMES
		printer.print_sieving_table();
//		printer.print_primes_found();
	#endif
	
	#if DEBUG
		printer.print_number_of_found_primes();
	#endif
}

#endif
