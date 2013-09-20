
#include <iostream>
#include <stack>
#include "coords.h"
#include "backtrack_common.h"
#include "backtrack_naive_unique.h"

int soln_count(Grid& grid) {
	std::stack<Spot> spots;
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(grid.get(p) == 0) {
			spots.push(p);
		}
	}
	return soln_count_recurse(grid, spots);
}

int soln_count_recurse(Grid& grid, std::stack<Spot>& spots) {
	Spot p = spots.top();
	spots.pop();
	int ret = 0;
	for(int val=1; val <= 9; val++) {
		if(grid.covered(p, val)) { continue; }
		grid.set(p, val);
		if(spots.empty()) {
			ret += 1;
		} else {
			ret += soln_count_recurse(grid, spots);
		}
	}
	grid.set(p, 0);
	spots.push(p);
	return ret;
}

int soln_count_solve(Grid& grid) {
	int ret;
	Grid solution;
	std::stack<Spot> spots;
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(grid.get(p) == 0) {
			spots.push(p);
		}
	}
	ret = soln_count_recurse_solve(grid, &solution, spots);
	for(int i=0; i < 81; i++) {
		grid.setmanual(solution.getmanual(i), i);
	}
	return ret;
}

int soln_count_recurse_solve(Grid& grid, Grid* solution, std::stack<Spot>& spots) {
	Spot p = spots.top();
	spots.pop();
	int ret = 0;
	for(int val=1; val <= 9; val++) {
		if(grid.covered(p, val)) { continue; }
		grid.set(p, val);
		if(spots.empty()) {
			for(int i=0; i < 81; i++) {
				solution->setmanual(grid.getmanual(i), i);
			}
			ret += 1;
		} else {
			ret += soln_count_recurse_solve(grid, solution, spots);
		}
	}
	grid.set(p, 0);
	spots.push(p);
	return ret;
}

