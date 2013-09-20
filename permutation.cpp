
#include <iostream>
#include <cstdlib>
#include "permutation.h"

const int num_data_permutations = 1296;
#include "data_permutations.h"

const int num_relabels = 362880;
const int num_rebands = 6;
const int num_rerows = 216;

Permutation::Permutation() {
	// does not initialize
}

Permutation::Permutation(std::vector<int>& labs, int row, int col, bool trans) {
	transpose = trans;
	for(int i=0; i < 18; i++) {
		rows[i] = permutation_data[row][i];
		cols[i] = permutation_data[col][i];
	}
	labels = labs;
}

void Permutation::randomize() {
	transpose = ((rand() % 2) == 0 ? true : false);
	int row = rand() % num_data_permutations;
	int col = rand() % num_data_permutations;
	for(int i=0; i < 18; i++) {
		rows[i] = permutation_data[row][i];
		cols[i] = permutation_data[col][i];
	}
	labels.clear();
	for(int i=1; i <= 9; i++) {
		labels.push_back(i);
	}
	permute_digit_array(labels, rand() % num_relabels);
}

Spot Permutation::permute_spot(Spot p) {
	Spot ret = p;
	ret.row = rows[ret.row];
	ret.col = cols[ret.col];
	if(transpose) {
		int temp = ret.row;
		ret.row = ret.col;
		ret.col = temp;
	}
	return ret;
}

int Permutation::permute_val(int val) {
//if(val == 0) { std::cout << "bad error" << std::endl; }
	return labels[val-1];
}

Bits Permutation::permute_bits(Bits* bits) {
	Bits ret;
	ret.init();
	for(int i=0; i < 81; i++) {
		// p: spot in ret
		Spot p = spot_of_idx(i);
		
		// px: spot in bits
		Spot px = permute_spot(p);
		
		if(bits->has_value(px)) {
			ret.set_value(p, permute_val(bits->get_value(px)));
		} else {
			for(int k=1; k <=9; k++) {
				if(!bits->has_candidate(px, k)) {
					ret.remove_candidate(p, permute_val(k));
				}
			}
		}
	}
	return ret;
}






void init_digit_array(std::vector<int>& digits) {
	for(int i=0; i < digits.size(); i++) {
		digits[i] = i;
	}
}

void permute_digit_array(std::vector<int>& digits, int k) {
	int n = digits.size();
	int fact = 1;
	for(int j=1; j < n; j++) {
		fact *= j;
		int temp = digits[j];
		int swapidx = (k / fact) % (j+1);
		digits[j] = digits[swapidx];
		digits[swapidx] = temp;
	}
}



Permutation find_canonical_permutation(Bits* bits) {
	int best_vals[81];
	int best_map[9];
	int best_row;
	int best_col;
	bool best_trans;
	
	for(int i=0; i < 81; i++) {
		best_vals[i] = 10;
	}
	
	for(int rowidx=0; rowidx < num_data_permutations; rowidx++) {
		for(int colidx=0; colidx < num_data_permutations; colidx++) {
			for(int i=0; i < 2; i++) {
				bool trans = (i==0);
				int curmap = 1;
				int map[10] = {0, -1, -1, -1, -1, -1, -1, -1, -1, -1};
				bool is_best = false;
				for(int i=0; i < 81; i++) {
					Spot p = spot_of_idx(i);
					Spot px = p;
					px.row = permutation_data[rowidx][p.row];
					px.col = permutation_data[colidx][p.col];
					if(trans) {
						int temp = px.row;
						px.row = px.col;
						px.col = temp;
					}
					int preval = bits->get_value(px);
					int val;
					if(map[preval] == -1) {
						val = map[preval] = curmap;
						curmap++;
					} else {
						val = map[preval];
					}
					if(is_best) {
						best_vals[i] = val;
					} else {
						if(val > best_vals[i]) {
							break;
						}
						if(val < best_vals[i]) {
							best_vals[i] = val;
							is_best = true;
							for(int i=0; i < 9; i++) {
								best_map[i] = map[i+1];
							}
							best_row = rowidx;
							best_col = colidx;
							best_trans = trans;
						}
					}
				}
				if(is_best) {
					for(int i=0; i < 9; i++) {
						best_map[i] = map[i+1];
					}
				}
/*if(is_best) {
for(int i=0; i < 81; i++) {
std::cout << best_vals[i];
} std::cout << std::endl;
}*/
			}
		}
	}
	
	// replace possible -1 in best_map with remaining value
/*for(int i=0; i < 9; i++) {
std::cout << " " << best_map[i];
} std::cout << std::endl;*/
	int idx_of_none = -1;
	std::vector<bool> has;
	for(int i=0; i < 9; i++) { has.push_back(false); }
	for(int i=0; i < 9; i++) {
		if(best_map[i] == -1) {
			idx_of_none = i;
		} else {
			has[best_map[i]-1] = true;
		}
	}
	if(idx_of_none != -1) {
		for(int i=0; i < 9; i++) {
			if(!has[i]) {
				best_map[idx_of_none] = i+1;
			}
		}
	}
/*for(int i=0; i < 9; i++) {
std::cout << " " << best_map[i];
} std::cout << std::endl;*/
	
/*std::cout << std::endl;
for(int i=0; i < 9; i++) { for(int j=0; j < 9; j++) {
std::cout << best_vals[i*9+j];
} std::cout << std::endl; } std::cout << std::endl;*/
	
/*std::cout << std::endl;
for(int i=0; i < 9; i++) { for(int j=0; j < 9; j++) {
int preval = bits->get_value(best_trans ? (Spot(permutation_data[best_col][j], permutation_data[best_row][i])) : (Spot(permutation_data[best_row][i], permutation_data[best_col][j])));
if(preval == 0) {
	std::cout << preval;
} else {
	std::cout << best_map[preval - 1];
}
} std::cout << std::endl; } std::cout << std::endl;*/
	
	std::vector<int> labs;
	for(int i=0; i < 9; i++) {
		labs.push_back(best_map[i]);
	}
	return Permutation(labs, best_row, best_col, best_trans);
}








