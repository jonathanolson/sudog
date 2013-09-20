
#include <iostream>
#include "bitmatrix_to_ps.h"

void ps_write_numbers(BitMatrix* b, Psout* ps) {
	for(int i=0; i < 81; i++) {
//std::cout << "ps_write_numbers: b->get_value(pair_of_idx(i)) = " << b->get_value(pair_of_idx(i)) << std::endl;
		ps->cellvalues[i] = b->get_value(spot_of_idx(i));
	}
}

void ps_write_candidates(BitMatrix* b, Psout* ps) {
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(b->has_value(p)) { continue; }
		for(int j=1; j <=9; j++) {
			if(b->has_candidate(p, j)) {
				ps->add_candidate(p, j);
			}
		}
	}
}

void ps_write_bitmatrix(BitMatrix* b, Psout* ps) {
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		for(int j=1; j <=9; j++) {
			if(b->has_candidate(p, j)) {
				ps->add_candidate(p, j);
			}
		}
	}
}

