#ifndef OUTPUT_H
#define OUTPUT_H

#include "coords.h"

class Output {
	public:
	
	virtual void set_fullnumber(Spot p, int val) = 0;
	virtual void add_candidate(Spot p, int val) = 0;
	virtual void highlight_candidate(Spot p, int val, float r, float g, float b) = 0;
	virtual void highlight_cell(Spot p, float r, float g, float b) = 0;
	virtual void highlight_row(int row, float r, float g, float b) = 0;
	virtual void highlight_col(int col, float r, float g, float b) = 0;
	virtual void highlight_box(int boxid, float r, float g, float b) = 0;
	virtual void outline_cell(Spot p, float width, float r, float g, float b) = 0;
	virtual void link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b) = 0;
};

#endif
