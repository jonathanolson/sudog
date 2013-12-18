
#include <iostream>
#include <fstream>

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
#include "ywing.h"

int main(int argc, char* argv[]) {
	
	Bits bits;
	CSolver solver;
	
	while(true) {
		std::cin >> bits;
		if(std::cin.eof()) { break; }
		if(!csolver_unique_check(&solver, &bits)) { continue; }
		
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
			
			std::vector<YWing> ywings = find_ywings(&bits);
			if(ywings.size() > 0) {
				for(int i=0; i < ywings.size(); i++) {
					ywings[i].apply(&bits);
				}
				continue;
			}
			
			solving = false;
		}
		
		if(!bits.complete()) {
			continue;
		}
		
		std::cout << bits;
	}
	
	return 0;
}
