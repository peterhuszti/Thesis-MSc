/*
	author: Peter Huszti
 */

#include <fstream>
#include <vector>
 
#include "../source/utils/Siever.h"
#include "../source/utils/Printer.h"
#include "../source/utils/utils.h"

struct res
{
	std::vector<int> primes;
	int low, up;
	
	res() {}
	res(std::vector<int> p, int l, int u): primes(p), low(l), up(u) { }
};

res start(std::string, int, int);
void compare(std::vector<int>, res);

int main(int argc, char *argv[])
{
	res my_results;
	std::vector<int> real_results;
	
	std::cout << "Running SOE_SEQ.....\n";

	std::cout << "    config1.txt\n";
	
	my_results = start("../source/utils/config1.txt", 1, 1);
	compare(real_results, my_results);
	
	std::cout << "    config2.txt\n";
	my_results = start("../source/utils/config2.txt", 1, 1);
	compare(real_results, my_results);
	
	return 0;
}

res start(std::string config, int number_of_threads, int number_of_chunks)
{
	word_t lower_bound = 0; // both should be a power of 2
	word_t upper_bound = 0;
	
	auto bounds = read_config(config);
		
	lower_bound = bounds.first;
	upper_bound = bounds.second;
	
	Siever siever(number_of_chunks, lower_bound, upper_bound, number_of_threads);

	siever.soe_init();
	siever.soe_chunks();
	
	Printer printer(&siever);
	printer.print_sieving_table();
	printer.print_primes_found();
	return res(printer.get_primes_found(), lower_bound, upper_bound);
}

void compare(std::vector<int> real, res mine)
{
	bool equal = true;
	std::ifstream reader;
	reader.open("primes.txt");	
	
	while (!reader.eof())
	{
		int temp;
		reader >> temp;
		if (mine.low <= temp && temp <= mine.up)
		{
			real.push_back(temp);
		}
	}
	reader.close();
	
	
	for (int i=0; i<real.size() && equal; ++i)
	{
		if (real[i] != mine.primes[i])
		{
			std::cout << real[i] << "  " << mine.primes[i] << std::endl;
			equal = false;
		}
	}
	
	if (equal)
	{
		std::cout << "        ## PASSED ##\n";
	}
	else
	{
		std::cout << "        ## FAILED ##\n";
	}
}








