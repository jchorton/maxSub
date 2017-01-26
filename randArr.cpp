/*
 * This should generate some random numbers for us of a given size.
 * Suggested usage:
 * ./randNum n > outputFile
 */

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <ctime>
using namespace std;

int main(int argc, char *argv[])
{
	// check usage
	if (argc < 2) {
		cout << "USAGE: " << argv[0] << " n" << endl;
		cout << "or:    " << argv[0] << " n > outputFile" << endl;
		return 0;
	}

	// bring in target value from command line
	unsigned long int targetN;
	sscanf(argv[1], "%llu", &targetN);

	// seed random number gen
	srand((unsigned)time(0));
	int ran;

	for(unsigned long int count = 0; count < targetN; count++) {
		ran = (rand()%256)-128;
		cout << ran << " ";
	}
	cout << endl;

	return 0;
}
