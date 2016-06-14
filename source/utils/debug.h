/*
	author: Peter Huszti, Emil Vatai
	source: https://github.com/vatai/simple_soe/blob/master/simple_soe.c
 */

/**
   Print/debug functions.
*/
void print_table(size_t n, word_t *p)
{
	for(size_t i=n; i>0; --i)
		printf("%08lx ",p[i-1]);
	std::cout << std::endl;
}

void print_primes(size_t nbits, word_t *st)
{
	for(size_t i=0; i<nbits; i++) 
		if(! GET(st,i)) std::cout << I2P(i) << ", ";
	std::cout << std::endl;
}

void print_primes_chunk(size_t nbits, word_t *st, size_t base, int number_of_primes_found)
{
	for(size_t i=0; i<nbits; i++)
		if(! GET( st, i )) 
		{
			std::cout << I2P(i + base) << ", ";
			number_of_primes_found++;
		}
	std::cout << std::endl;
}