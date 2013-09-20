#ifndef BACKTRACK_COMMON_H
#define BACKTRACK_COMMON_H

#include <iostream>
#include "coords.h"
#include "psout.h"

class Grid {
	public:
	Grid();
	~Grid();
	Grid(const Grid& grid);
	Grid& operator=(const Grid& grid);
	
	void init();
	
	int get(Spot p);
	int getmanual(int idx);
	void set(Spot p, int col);
	void setmanual(int val, int idx);
	
	bool covered(Spot p, int val);
	
	void write_to_psout(Psout& ps);
	
	private:
	int *data;
};

std::ostream& operator << (std::ostream& os, Grid& grid);
std::istream& operator >> (std::istream& os, Grid& grid);


#endif
