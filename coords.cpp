
#include "coords.h"

Spot::Spot() {
	row = 0;
	col = 0;
}

Spot::Spot(int x, int y) {
	row = x;
	col = y;
}

std::ostream& operator<<(std::ostream& out, Spot& p) {
	//out << "(" << p.row + 1 << "," << p.col + 1 << ")";
	out << "r" << p.row + 1 << "c" << p.col + 1;
	return out;
}

int idx_of_spot(Spot p) {
	return p.row * 9 + p.col;
}

int row_of_idx(int idx) {
	return idx / 9;
}

int col_of_idx(int idx) {
	return idx % 9;
}

Spot spot_of_idx(int idx) {
	return Spot(row_of_idx(idx), col_of_idx(idx));
}

int boxid_of_spot(Spot p) {
	return 3 * (p.row / 3) + p.col / 3;
}

int row_of_boxid(int boxid, int which) {
	return 3 * (boxid / 3) + which / 3;
}

int col_of_boxid(int boxid, int which) {
	return 3 * (boxid % 3) + which % 3;
}

Spot spot_of_boxid(int boxid, int which) {
	return Spot(row_of_boxid(boxid, which), col_of_boxid(boxid, which));
}

int idx_of_row(int row, int which) {
	return row * 9 + which;
}

int idx_of_col(int col, int which) {
	return which * 9 + col;
}

bool spot_equal(Spot a, Spot b) {
	return (a.row == b.row && a.col == b.col);
}

bool spot_visible(Spot a, Spot b) {
	return (a.row == b.row || a.col == b.col || boxid_of_spot(a) == boxid_of_spot(b));
}

int idx_of_spots(std::vector<Spot>& spots, Spot p) {
	for(int i=0; i < spots.size(); i++) {
		if(spot_equal(p, spots[i])) { return i; }
	}
	return -1;
}

bool in_spots(std::vector<Spot>& spots, Spot p) {
	return (idx_of_spots(spots, p) >= 0);
}

std::vector<Spot> unique_spots(std::vector<Spot>& spots) {
	std::vector<Spot> ret;
	for(int i=0; i < spots.size(); i++) {
		if(in_spots(ret, spots[i])) { continue; }
		ret.push_back(spots[i]);
	}
	return ret;
}

std::vector<Spot> spots_union(std::vector<Spot>& a, std::vector<Spot>& b) {
	std::vector<Spot> ret;
	for(int i=0; i < a.size(); i++) {
		if(in_spots(ret, a[i])) { continue; }
		ret.push_back(a[i]);
	}
	for(int i=0; i < b.size(); i++) {
		if(in_spots(ret, b[i])) { continue; }
		ret.push_back(b[i]);
	}
	return ret;
}

std::vector<Spot> spots_intersect(std::vector<Spot>& a, std::vector<Spot>& b) {
	std::vector<Spot> ret;
	for(int i=0; i < a.size(); i++) {
		if(! in_spots(b, a[i])) { continue; }
		ret.push_back(a[i]);
	}
	return ret;
}

std::vector<Spot> spots_subtract(std::vector<Spot>& a, std::vector<Spot>& b) {
	std::vector<Spot> ret;
	for(int i=0; i < a.size(); i++) {
		if(in_spots(b, a[i])) { continue; }
		ret.push_back(a[i]);
	}
	return ret;
}

std::vector<Spot> spots_of_row(int row) {
	std::vector<Spot> ret;
	for(int i=0; i < 9; i++) {
		ret.push_back(Spot(row, i));
	}
	return ret;
}

std::vector<Spot> spots_of_col(int col) {
	std::vector<Spot> ret;
	for(int i=0; i < 9; i++) {
		ret.push_back(Spot(i, col));
	}
	return ret;
}

std::vector<Spot> spots_of_box(int boxid) {
	std::vector<Spot> ret;
	for(int i=0; i < 9; i++) {
		ret.push_back(spot_of_boxid(boxid, i));
	}
	return ret;
}

std::vector<int> horiz_adj_boxes(int boxid) {
	std::vector<int> ret;
	int base = 3 * (boxid / 3);
	for(int i=0; i < 3; i++) {
		if( i+base != boxid ) { ret.push_back(i + base); }
	}
	return ret;
}

std::vector<int> vert_adj_boxes(int boxid) {
	std::vector<int> ret;
	int base = boxid % 3;
	for(int i=0; i < 3; i++) {
		if( 3*i+base != boxid ) { ret.push_back(3*i + base); }
	}
	return ret;
}

std::ostream& operator<<(std::ostream& out, std::vector<Spot>& spots) {
	for(int i=0; i < spots.size(); i++) {
		out << spots[i];
	}
	return out;
}

std::vector<int> vals_of_int(int bit) {
	std::vector<int> ret;
	int val = 1;
	int cur = bit;
	while(cur != 0) {
		if(cur % 2 == 1) { ret.push_back(val); }
		val++;
		cur = cur >> 1;
	}
	return ret;
}

int first_bit_int(int bit) {
	int ret = 1;
	int cur = bit;
	while(cur != 0) {
		if(cur % 2 == 1) { return ret; }
		ret++;
		cur = cur >> 1;
	}
	return 0;
}

int count_bits_int(int bit) {
	int ret = 0;
	int cur = bit;
	while(cur != 0) {
		ret += cur % 2;
		cur = cur >> 1;
	}
	return ret;
}

int bit_subtract_int(int a, int b) {
	return (a | b) - b;
}

bool int_has_candidate(int bit, int val) {
	return ((bit >> (val - 1)) % 2 == 1);
}

int complement_bit(int bit) {
	return (0x1FF - bit);
}

bool in_ints(std::vector<int>& ints, int val) {
	for(int i=0; i < ints.size(); i++) {
		if(ints[i] == val) { return true; }
	}
	return false;
}


