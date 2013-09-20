
#include "backtrack_to_bits.h"

void backtrack_to_bits(Grid& grid, Bits* bits) {
	for(int i=0; i < 81; i++) {
		int a = grid.getmanual(i);
		if(a != 0) {
			bits->set_value(spot_of_idx(i), a);
		}
	}
}

void backtrack_of_bits(Grid& grid, Bits* bits) {
	for(int i=0; i < 81; i++) {
		grid.setmanual(bits->get_value(spot_of_idx(i)), i);
	}
}
