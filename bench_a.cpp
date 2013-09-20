
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "bits.h"
#include "dlx.h"

#include "simple_naked_single.h"
#include "naked_single.h"
#include "hidden_single.h"
#include "nh_subset.h"
#include "pointing.h"
#include "boxline.h"
#include "nfish.h"
#include "simple_coloring.h"

#include "naive_gen.h"

int main(int argc, char* argv[]) {
	srand((unsigned) time(0));
	
	Bits bits;
	//CSolver solver;
	
	int num = 100000;
	int solved = 0;
	
	clock_t start, finish;
	start = clock();
	
	//bits = naive_gen_bits();
	
	for(int i=0; i < num; i++) {
		
		//std::cin >> bits;
		//if(std::cin.eof()) { break; }
		//if(!csolver_unique_check(&solver, &bits)) { continue; }
		
		
		bits = naive_gen_bits();
		
		//Bits newbits;
		//newbits = bits;
		//CSolver solver;
		
		
		bool solving = true;
		while(solving) {
			
			std::vector<SimpleNakedSingle> snss = find_simple_naked_singles(&bits);
			if(snss.size() > 0) {
				for(int i=0; i < snss.size(); i++) {
					snss[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<NakedSingle> nss = find_naked_singles(&bits);
			if(nss.size() > 0) {
				for(int i=0; i < nss.size(); i++) {
					nss[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<HiddenSingle> hss = find_hidden_singles(&bits);
			if(hss.size() > 0) {
				for(int i=0; i < hss.size(); i++) {
					hss[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<NHSubset> nhs = find_nh_subsets(&bits);
			if(nhs.size() > 0) {
				for(int i=0; i < nhs.size(); i++) {
					nhs[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<Pointing> points = find_pointings(&bits);
			if(points.size() > 0) {
				for(int i=0; i < points.size(); i++) {
					points[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<BoxLine> boxlines = find_boxlines(&bits);
			if(boxlines.size() > 0) {
				for(int i=0; i < boxlines.size(); i++) {
					boxlines[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<NFish> nfishes = find_nfishes(&bits);
			if(nfishes.size() > 0) {
				for(int i=0; i < nfishes.size(); i++) {
					nfishes[i].apply(&bits);
				}
				continue;
			}
			
			std::vector<SimpleColoring> scs = find_simple_colorings(&bits);
			if(scs.size() > 0) {
				for(int i=0; i < scs.size(); i++) {
					scs[i].apply(&bits);
				}
				continue;
			}
			
			solving = false;
		}
		
		if(bits.complete()) {
			solved++;
			continue;
		}
		/*
		std::cout << bits;
		*/
	}
	
	finish = clock();
	
	printf("time: %2.5f sec\n",(double)(finish - start)/CLOCKS_PER_SEC);
	printf("%d problems processed\n", num);
	printf("%2.5f sec per grid average\n", (double)(finish - start)/((double)(CLOCKS_PER_SEC * num)));
	printf("%1.5f percent solvable.\n", (double)(solved)/(double)(num));
	
	return 0;
}
