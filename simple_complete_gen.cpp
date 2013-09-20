
#include <iostream>
#include <cstdlib>
#include <list>
#include "simple_complete_gen.h"
#include "coords.h"



bool simple_complete_gen_recur(Grid* grid, int pos) {
	if(pos == 81) {
		// found complete grid
		return true;
	} else {
		Spot p = spot_of_idx(pos);
		std::list<int> digits;
		std::list<int> vals;
		for(int i=1; i <= 9; i++) {
			digits.push_back(i);
		}
		for(int i=9; i > 0; i--) {
			std::list<int>::iterator idx = digits.begin();
			int r = rand() % i;
			for(int j=0; j < r; j++) { idx++; }
			vals.push_back(*idx);
			digits.erase(idx);
		}
		for(std::list<int>::iterator idx = vals.begin(); idx != vals.end(); idx++) {
			int val = *idx;
			if(grid->covered(p, val)) { continue; }
			grid->setmanual(val, pos);
			bool found = simple_complete_gen_recur(grid, pos + 1);
			if(found) { return true; }
			grid->setmanual(0, pos);
		}
		return false;
	}
}

Grid simple_complete_gen() {
	Grid grid;
	grid.init();
	simple_complete_gen_recur(&grid, 0);
	return grid;
}
