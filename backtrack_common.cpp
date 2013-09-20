
#include <iostream>
#include <cassert>
#include "coords.h"
#include "backtrack_common.h"

Grid::Grid() {
	data = new int[81];
}

Grid::~Grid() {
	delete [] data;
}

Grid::Grid(const Grid& grid) {
std::cout << "debugA" << std::endl;
	data = new int[81];
	for(int i=0; i < 81; i++) {
		data[i] = grid.data[i];
	}
}

Grid& Grid::operator=(const Grid& grid) {
std::cout << "debugB" << std::endl;
	if(this != &grid) {
		delete [] data;
		data = new int[81];
		for(int i=0; i < 81; i++) {
			data[i] = grid.data[i];
		}
	}
	return *this;
}

void Grid::init() {
	for(int i=0; i < 81; i++) {
		data[i] = 0;
	}
}

int Grid::get(Spot p) {
	return data[idx_of_spot(p)];
}

int Grid::getmanual(int idx) {
	return data[idx];
}

void Grid::set(Spot p, int val) {
	data[idx_of_spot(p)] = val;
}

void Grid::setmanual(int val, int idx) {
	data[idx] = val;
}

bool Grid::covered(Spot p, int val) {
	int boxid = boxid_of_spot(p);
	for(int i=0; i < 9; i++) {
		if(i != p.col) {
			if(getmanual(idx_of_row(p.row, i)) == val) {
				return true;
			}
		}
		if(i != p.row) {
			if(getmanual(idx_of_col(p.col, i)) == val) {
				return true;
			}
		}
		Spot pk = spot_of_boxid(boxid, i);
		if(pk.row != p.row || pk.col != p.col) {
			if(get(pk) == val) {
				return true;
			}
		}
	}
	return false;
}

void Grid::write_to_psout(Psout& ps) {
	for(int i=0; i < 81; i++) {
		ps.cellvalues[i] = data[i];
	}
}

std::ostream& operator << (std::ostream& os, Grid& grid) {
	for(int i=0; i < 9; i++) {
		for(int j=0; j < 9; j++) {
			int spot = grid.get(Spot(i,j));
			if(spot == 0) {
				os << ".";
			} else {
				os << spot;
			}
		}
		//os << std::endl;
	}
	os << std::endl;
	return os;
}

std::istream& operator >> (std::istream& is, Grid& grid) {
	char buf[90];
	is.getline(buf, 90);
	for(int i=0; i < 81; i++) {
		if(buf[i] == '.') {
			grid.setmanual(0, i);
		} else if (buf[i] >= 49 && buf[i] <= 57){
			grid.setmanual(buf[i] - 48, i);
		} else {
			assert(false);
		}
	}
	return is;
}
