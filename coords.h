#ifndef COORDS_H
#define COORDS_H

#include <iostream>
#include <vector>

class Spot {
	public:
	Spot();
	Spot(int x, int y);
	int row;
	int col;
};

std::ostream& operator<<(std::ostream& out, Spot& p);

int idx_of_spot(Spot p);
int row_of_idx(int idx);
int col_of_idx(int idx);
Spot spot_of_idx(int idx);

int boxid_of_spot(Spot p);
int row_of_boxid(int boxid, int which);
int col_of_boxid(int boxid, int which);
Spot spot_of_boxid(int boxid, int which);

int idx_of_row(int row, int which);
int idx_of_col(int col, int which);

bool spot_equal(Spot a, Spot b);
bool spot_visible(Spot a, Spot b);

bool in_spots(std::vector<Spot>& spots, Spot p);
int idx_of_spots(std::vector<Spot>& spots, Spot p);
std::vector<Spot> unique_spots(std::vector<Spot>& spots);
std::vector<Spot> spots_union(std::vector<Spot>& a, std::vector<Spot>& b);
std::vector<Spot> spots_intersect(std::vector<Spot>& a, std::vector<Spot>& b);
std::vector<Spot> spots_subtract(std::vector<Spot>& a, std::vector<Spot>& b);

std::vector<Spot> spots_of_row(int row);
std::vector<Spot> spots_of_col(int col);
std::vector<Spot> spots_of_box(int boxid);

std::vector<int> horiz_adj_boxes(int boxid);
std::vector<int> vert_adj_boxes(int boxid);

std::ostream& operator<<(std::ostream& out, std::vector<Spot>& spots);

// BIT stuff
std::vector<int> vals_of_int(int bit);

int first_bit_int(int bit);
int count_bits_int(int bit);
int bit_subtract_int(int a, int b);
bool int_has_candidate(int bit, int val);
int complement_bit(int bit);
bool in_ints(std::vector<int>& ints, int val);

#endif
