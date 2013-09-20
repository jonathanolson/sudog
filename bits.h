#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <vector>
#include "coords.h"
#include "bitmatrix.h"

class Bits : public BitMatrix {
	public:
	Bits();
	~Bits();
	Bits(const Bits& bitss);
	Bits& operator=(const Bits& bitss);
	
	void init();
	
	void set_value(Spot p, int val);
	bool has_value(Spot p);
	int get_value(Spot p);
	
	void remove_candidate(Spot p, int val);
	bool has_candidate(Spot p, int val);
	int get_candidate_int(Spot p, int val);
	int count_candidates(Spot p);
	
	int first_bit(Spot p);
	
	int bit_of_spot(Spot p);
	int bit_of_spots(std::vector<Spot>& pl);
	
	int open_bit_of_row(int row);
	int open_bit_of_col(int col);
	int open_bit_of_box(int boxid);
	
	std::vector<Spot> covers_of_spot(Spot p, int val);
	
	bool is_possible(Spot p);
	void not_possible(Spot p);
	
	bool complete();
	bool valid_now();
	bool solved();
	
	int* numbers;
	int* bits;
	int* possibles;
};

std::ostream& operator << (std::ostream& os, Bits& bits);
std::istream& operator >> (std::istream& os, Bits& bits);

std::ostream& operator <<= (std::ostream& os, Bits& bits);

#endif
