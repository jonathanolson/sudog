
#include <iostream>
#include <fstream>

#include "bits.h"
#include "dlx.h"

int main(int argc, char* argv[]) {
	/*
	if(argc < 2) {
		return 1;
	}
	std::ifstream in;
	in.open(argv[1]);
	*/
	
	Bits bits;
	CSolver solver;
	
	while(true) {
		std::cin >> bits;
		if(std::cin.eof()) { break; }
		if(csolver_unique_check(&solver, &bits)) {
			std::cout << bits;
		}
	}
	
	//in.close();
	return 0;
}
