#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>
#include "coords.h"
#include "backtrack_common.h"
#include "bits.h"

extern const int num_relabels;
extern const int num_rebands;
extern const int num_rerows;

class Permutation {
	public:
	
	// Permutation() does not initialize!
	Permutation();
	Permutation(std::vector<int>& labs, int row, int col, bool trans);
	//Permutation(int plab, int pband, int prow, int pstack, int pcol, bool trans);
	
	void randomize();
	
	Spot permute_spot(Spot p);
	int permute_val(int val);
	
	//int permute_grid_val(Grid& grid, Spot p);
	//int permute_bits_val(Bits* bits, Spot p);
	
	//Grid permute_grid(Grid& grid);
	Bits permute_bits(Bits* bits);
	
	/*
	std::vector<int> labels;
	std::vector<int> bands;
	std::vector< std::vector<int> > rows;
	std::vector<int> stacks;
	std::vector< std::vector<int> > cols;
	bool transpose;
	*/
	std::vector<int> labels;
	int rows[18];
	int cols[18];
	bool transpose;
};

void init_digit_array(std::vector<int>& digits);
void permute_digit_array(std::vector<int>& digits, int k);

Permutation find_canonical_permutation(Bits* bits);

#endif
