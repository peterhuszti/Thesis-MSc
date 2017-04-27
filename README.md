# MSc thesis

## Parallel implementation of cache optimized linear sieve

Compile:
	g++ -o sieve -O3 -std=c++11 main.cpp

Usage:
	sieve [params]
	One can configure the program. Params could be:
	- nothing. Default: 1 thread, config1.txt
	- sieve N, where N is the number of threads to be used
	- sieve N X, where N is as above, and X is the config file to be used

Author: Peter Huszti
Consultant: Emil Vatai