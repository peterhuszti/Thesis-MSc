/*
	author: Peter Huszti
 */
 
#include <vector>
 
#ifndef TESTCASE_GENERATOR
#define TESTCASE_GENERATOR

struct Testcase
{
	int lower_bound;
	int upper_bound;
	int chunks;
	int threads;
	
	Testcase(int low, int up,
			 int ch, int th):
	lower_bound(low),
	upper_bound(up),
	chunks(ch),
	threads(th)
	{ }
};

class Testcase_generator
{

private:

	int lower_bound;
	int upper_bound;
	int min_chunks;
	int max_chunks;
	int min_threads;
	int max_threads;
	
	std::vector<Testcase> testcases;

public:

	Testcase_generator(int low, int up,
					   int min_ch, int max_ch,
					   int min_th, int max_th):
	lower_bound(low),
	upper_bound(up),
	min_chunks(min_ch),
	max_chunks(max_ch),
	min_threads(min_th),
	max_threads(max_th)
	{
		generate_testcases();
	}
	
	Testcase getTestcase(int j)
	{
		return testcases[j];
	}
	
	int getNumberOfTestcases()
	{
		return testcases.size();
	}
	
private:	

	void generate_testcases()
	{
		for (int j=min_chunks; j<=max_chunks; j*=2)
		{
			for (int i=min_threads; i<=max_threads; i*=2)
			{
				Testcase tc(lower_bound, upper_bound, j, i);
				testcases.push_back(tc);
			}
		}
	}

};

#endif