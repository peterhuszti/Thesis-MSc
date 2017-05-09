/*
	author: Peter Huszti
 */

#include <fstream>
#include <vector>
 
#include "../source/utils/Siever.h"
#include "../source/utils/Printer.h"
#include "../source/utils/utils.h"
#include "testcase_generator.h"

struct res
{
	std::vector<int> primes;
	int low, up;
	
	res() {}
	res(std::vector<int> p, int l, int u): primes(p), low(l), up(u) { }
};

res start(const Testcase&, Siever*);
std::vector<int> read_real_results();
bool compare(const std::vector<int>&, const res&);

int main(int argc, char *argv[])
{
	bool fail = false;

	std::cout << "Reading primes.....\n" << std::flush;
	const std::vector<int> real_results = read_real_results();	

	std::cout << "Generating testcases.....\n" << std::flush;
//	Testcase_generator generator(65536, 131072, 1, 128, 1, 16);
	//Testcase_generator generator(2048, 4096, 64, 64, 1, 1);
	Testcase_generator generator(65536, 131072, 512, 512, 1, 1);
	//Testcase_generator generator(1024, 2048, 64, 64, 1, 1);
	
	std::cout << "Running tests.....\n" << std::flush;
	Testcase tc = generator.getTestcase(0);
	input in(tc.lower_bound, tc.upper_bound,
			 tc.chunks, tc.threads);
Siever siever(in);
	int number_of_tc = generator.getNumberOfTestcases();
	for (int j=0; j<number_of_tc; ++j)
	{
	//	Testcase tc = generator.getTestcase(j);
		std::cout << "    lower bound: " << tc.lower_bound;
		std::cout << "    upper bound: " << tc.upper_bound;
		std::cout << "    number of chunks: " << tc.chunks;
		std::cout << "    number of threads: " << tc.threads << "\n" << std::flush;
		
		if(!compare(real_results, start(tc,&siever)))
		{
			fail = true;
		}
	}
	
	if(fail)
	{
		std::cout << "\n\n\n*** The tests failed! ***\n\n\n" << std::flush;
	}
	else
	{
		std::cout << "\n\n\n*** The tests passed! ***\n\n\n" << std::flush;
	}
	
	return 0;
}

res start(const Testcase& tc, Siever* siever)
{
	/*input in(tc.lower_bound, tc.upper_bound,
			 tc.chunks, tc.threads);
	
	Siever siever(in);
*/
	siever->soe_init();
	siever->soe_chunks();

	Printer printer(siever, "../test/result.txt");
	printer.print_sieving_table();
	printer.print_primes_found();
	printer.print_debug_info();
	//return res(printer.get_primes_found(), in.low, in.up);
	return res(printer.get_primes_found(), tc.lower_bound, tc.upper_bound);
}

std::vector<int> read_real_results()
{
	std::vector<int> real;
	std::ifstream reader;
	reader.open("primes.txt");	
	
	while (!reader.eof())
	{
		int temp;
		reader >> temp;
		real.push_back(temp);
	}
	reader.close();
	
	return real;
}

bool compare(const std::vector<int> &real, const res &mine)
{
	if (mine.primes.size() == 0)
	{
		std::cout << "        ## NO RESULTS ##\n" << std::flush;
		return false;
	}

	bool equal = true;	
	
	int j;
	for (j=0; real[j]<mine.low; ++j);
	for (int i=0; i<mine.primes.size() && equal; )
	{
		if (real[j] != mine.primes[i])
		{
			std::cout << "REAL:    " << real[j] << "\nFOUND: " << mine.primes[i] << std::endl << std::flush;
			equal = false;
		}
		if (real[j]==mine.primes[i]) {i++;j++;}
		else if (real[j]>mine.primes[i]) i++;
		else j++;
	}
	
	if (equal)
	{
		std::cout << "        ## PASSED ##\n" << std::flush;
	}
	else
	{
		std::cout << "        ## FAILED ##\n" << std::flush;
	}
	
	return equal;
}








