
#include "bits_to_output.h"

void write_numbers(Bits* b, Output* out) {
	for(int i=0; i < 81; i++) {
		Spot spot = spot_of_idx(i);
		if(b->has_value(spot)) {
			out->set_fullnumber(spot, b->get_value(spot));
		}
	}
}

void write_candidates(Bits* b, Output* out) {
	for(int i=0; i < 81; i++) {
		Spot spot = spot_of_idx(i);
		if(b->has_value(spot)) { continue; }
		for(int j=1; j <= 9; j++) {
			if(b->has_candidate(spot, j)) {
				out->add_candidate(spot, j);
			}
		}
	}
}

void write_val_candidates(Bits* b, Output* out, int val) {
	for(int i=0; i < 81; i++) {
		Spot spot = spot_of_idx(i);
		if(b->has_value(spot)) { continue; }
		if(b->has_candidate(spot, val)) {
			out->add_candidate(spot, val);
		}
	}
}
