
#ifndef BACKTRACK_NAIVE_UNIQUE_H
#define BACKTRACK_NAIVE_UNIQUE_H

#include <stack>
#include "coords.h"

int soln_count(Grid& grid);
int soln_count_recurse(Grid& grid, std::stack<Spot>& spots);
int soln_count_recurse_solve(Grid& grid, Grid* solution, std::stack<Spot>& spots);
int soln_count_solve(Grid& grid);

#endif
