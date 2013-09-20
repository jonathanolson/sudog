
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "simple_complete_gen.h"

int main(int argc, char* argv[]) {
	// note only random by the -second-
	srand((unsigned) time(0));
	
	
	if(argc < 2) { return 1; }
	std::istringstream is;
	std::string str(argv[1]);
	is.str(str);
	int num;
	is >> num;
	
	for(int i=0; i < num; i++) {
		Grid grid = simple_complete_gen();
		std::cout << grid;
	}
	
	//run_test();
	
	return 0;
}

