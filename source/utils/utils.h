/*
	author: Peter Huszti
 */

#ifndef UTILS
#define UTILS

#include <iostream>
#include <fstream>

#define CONFIG "../utils/config4.txt"

std::pair<word_t,word_t> read_config()
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;

	/**
		Read config file
	*/
	std::ifstream reader;
	reader.open(CONFIG);	
	char line[64];
	reader.getline(line, 64, ' ');
	reader >> lower_bound;
	reader.getline(line, 64, ' ');
	reader >> upper_bound;
	reader.close();

	return std::pair<word_t,word_t>(lower_bound, upper_bound);
}

#endif
