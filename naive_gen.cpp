
#include <vector>
#include <stdlib.h>

#include "naive_gen.h"
#include "simple_complete_gen.h"
#include "dlx.h"
#include "backtrack_to_bits.h"

Grid naive_gen_grid() {
	Grid grid = simple_complete_gen();
	CSolver solver;
	std::vector<Spot> removable;
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		removable.push_back(p);
	}
	while(removable.size() > 0) {
		int idx = rand() % (removable.size());
		Spot p = removable[idx];
		removable.erase(removable.begin() + idx);
		int val = grid.get(p);
		grid.set(p, 0);
		if(!csolver_unique_check(&solver, grid)) {
			grid.set(p, val);
		}
	}
	return grid;
}

Bits naive_gen_bits() {
	Grid grid = naive_gen_grid();
	Bits ret;
	ret.init();
	backtrack_to_bits(grid, &ret);
	return ret;
}

