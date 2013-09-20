
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>

#include "coords.h"
#include "psout.h"
#include "backtrack_common.h"
#include "backtrack_naive_unique.h"
#include "bitmatrix_to_ps.h"
#include "bits.h"
#include "simple_naked_single.h"
#include "naked_single.h"

#include "pretty_solver.h"

using namespace std;

int main() {
	srand((unsigned) time(0));
	
	
	Bits bits = Bits();
	cin >> bits;
	pretty_solve_single_random(&bits, "testc_07.ps");
	system("ps2pdf testc_07.ps");
	
	return 0;
}
