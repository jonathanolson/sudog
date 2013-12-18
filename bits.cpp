
#include <iostream>
#include <iomanip>
#include <cassert>
#include "bits.h"
#include "coords.h"

Bits::Bits() {
	numbers = new int[81];
	bits = new int[81];
	possibles = new int[81];
}

Bits::~Bits() {
	delete [] numbers;
	delete [] bits;
	delete [] possibles;
}

Bits::Bits(const Bits& bitss) {
	numbers = new int[81];
	bits = new int[81];
	possibles = new int[81];
	for(int i=0; i < 81; i++) {
		numbers[i] = bitss.numbers[i];
		bits[i] = bitss.bits[i];
		possibles[i] = bitss.possibles[i];
	}
}

Bits& Bits::operator=(const Bits& bitss) {
	if (this != &bitss) {
		delete [] numbers;
		delete [] bits;
		delete [] possibles;
		numbers = new int[81];
		bits = new int[81];
		possibles = new int[81];
		for(int i=0; i < 81; i++) {
			numbers[i] = bitss.numbers[i];
			bits[i] = bitss.bits[i];
			possibles[i] = bitss.possibles[i];
		}
	}
	return *this;
}

void Bits::init() {
	for(int i=0; i<81; i++) {
		numbers[i] = 0;
		bits[i] = 0x1FF;
		possibles[i] = true;
	}
}

bool Bits::has_candidate(Spot p, int val) {
	return ((bits[idx_of_spot(p)] >> (val - 1)) % 2 == 1);
}

void Bits::remove_candidate(Spot p, int val) {
	if(has_candidate(p, val)) {
		bits[idx_of_spot(p)] -= (1 << (val-1));
	}
}

int Bits::get_candidate_int(Spot p, int val) {
	return ((bits[idx_of_spot(p)] >> (val - 1)) % 2);
}

int Bits::count_candidates(Spot p) {
	return count_bits(bits[idx_of_spot(p)]);
}

// how many bits are set in the first 9 positions
int count_bits(int bit) {
	int num = 0;
	for(int i=0; i < 9; i++) {
		if((bit >> i) % 2 == 1) {
			num++;
		}
	}
	return num;
}

// the 1-based index of the first set bit
int first_bits(int bit) {
	for(int i=0; i < 9; i++) {
		if((bit >> i) % 2 == 1) {
			return i + 1;
		}
	}
}

bool Bits::has_value(Spot p) {
	return (numbers[idx_of_spot(p)] != 0);
}

int Bits::get_value(Spot p) {
	return numbers[idx_of_spot(p)];
}

void Bits::set_value(Spot p, int val) {
	numbers[idx_of_spot(p)] = val;
	int boxid = boxid_of_spot(p);
	for(int i=0; i < 9; i++) {
		remove_candidate(Spot(p.row, i), val);
		remove_candidate(Spot(i, p.col), val);
		remove_candidate(spot_of_boxid(boxid, i), val);
	}
	bits[idx_of_spot(p)] = (1 << (val - 1));
}

int Bits::first_bit(Spot p) {
	return first_bits(bits[idx_of_spot(p)]);
}

int Bits::bit_of_spot(Spot p) {
	return bits[idx_of_spot(p)];
}

int Bits::bit_of_spots(std::vector<Spot>& pl) {
	int ret = 0;
	for(int i=0; i < pl.size(); i++) {
		ret |= bits[idx_of_spot(pl[i])];
	}
	return ret;
}

int Bits::open_bit_of_row(int row) {
	int ret = 0;
	for(int i=0; i < 9; i++) {
		Spot p = Spot(row, i);
		if(has_value(p)) { continue; }
		ret |= bits[idx_of_spot(p)];
	}
	return ret;
}

int Bits::open_bit_of_col(int col) {
	int ret = 0;
	for(int i=0; i < 9; i++) {
		Spot p = Spot(i, col);
		if(has_value(p)) { continue; }
		ret |= bits[idx_of_spot(p)];
	}
	return ret;
}

int Bits::open_bit_of_box(int boxid) {
	int ret = 0;
	for(int i=0; i < 9; i++) {
		Spot p = spot_of_boxid(boxid, i);
		if(has_value(p)) { continue; }
		ret |= bits[idx_of_spot(p)];
	}
	return ret;
}

std::vector<Spot> Bits::covers_of_spot(Spot p, int val) {
	std::vector<Spot> ret;
	int boxid = boxid_of_spot(p);
	for(int i=0; i < 9; i++) {
		Spot boxspot = spot_of_boxid(boxid, i);
		Spot rowspot = Spot(p.row, i);
		Spot colspot = Spot(i, p.col);
		// box
		if( get_value(boxspot) == val ) {
			ret.push_back(boxspot);
		}
		if( boxid_of_spot(rowspot) != boxid ) {
			if( get_value(rowspot) == val ) {
				ret.push_back(rowspot);
			}
		}
		if( boxid_of_spot(colspot) != boxid ) {
			if( get_value(colspot) == val ) {
				ret.push_back(colspot);
			}
		}
	}
	return ret;
}

bool Bits::is_possible(Spot p) {
	return possibles[idx_of_spot(p)];
}

void Bits::not_possible(Spot p) {
	possibles[idx_of_spot(p)] = false;
}

bool Bits::complete() {
	for(int i=0; i < 81; i++) {
		if(numbers[i] == 0) { return false; }
	}
	return true;
}

bool Bits::valid_now() {
	// rows
	for(int i=0; i < 9; i++) {
		std::vector<bool> exist;
		for(int j=0; j < 9; j++) {
			exist.push_back(false);
		}
		for(int j=0; j < 9; j++) {
			Spot p = Spot(i, j);
			if(has_value(p)) {
				int val = get_value(p);
				if(exist[val] == false) {
					exist[val] == true;
				} else {
					return false;
				}
			}
		}
	}
	
	// cols
	for(int i=0; i < 9; i++) {
		std::vector<bool> exist;
		for(int j=0; j < 9; j++) {
			exist.push_back(false);
		}
		for(int j=0; j < 9; j++) {
			Spot p = Spot(j, i);
			if(has_value(p)) {
				int val = get_value(p);
				if(exist[val] == false) {
					exist[val] == true;
				} else {
					return false;
				}
			}
		}
	}
	
	// boxes
	for(int i=0; i < 9; i++) {
		std::vector<bool> exist;
		for(int j=0; j < 9; j++) {
			exist.push_back(false);
		}
		for(int j=0; j < 9; j++) {
			Spot p = spot_of_boxid(i, j);
			if(has_value(p)) {
				int val = get_value(p);
				if(exist[val] == false) {
					exist[val] == true;
				} else {
					return false;
				}
			}
		}
	}
	
	
	return true;
}

bool Bits::solved() {
	return complete() && valid_now();
}



std::ostream& operator << (std::ostream& os, Bits& bits) {
	for(int i=0; i < 9; i++) {
		for(int j=0; j < 9; j++) {
			int spot = bits.get_value(Spot(i,j));
			if(spot == 0) {
				os << ".";
			} else if(spot > 0 && spot <= 9) {
				os << spot;
			} else {
				os << "?";
			}
		}
	}
	os << std::endl;
	return os;
}

std::istream& operator >> (std::istream& is, Bits& bits) {
	char buf[90];
	is.getline(buf, 90);
	bits.init();
	for(int i=0; i < 81; i++) {
		if(buf[i] == '.') {
		} else if (buf[i] >= 49 && buf[i] <= 57){
			bits.set_value(spot_of_idx(i), ((int)buf[i]) - 48);
		} else {
			//assert(false);
		}
	}
	return is;
}

std::ostream& operator <<= (std::ostream& os, Bits& bits) {
	os << std::hex;
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		int dabits = bits.bit_of_spot(p);
		if(bits.has_value(p)) {
			dabits += 0x400;
		} else {
			dabits += 0x200;
		}
		os << dabits;
	}
	os << std::dec << std::endl;
	return os;
}
