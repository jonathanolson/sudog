/*---------------------------------------------------------------------------*
* PostScript output of Sudoku grids
*----------------------------------------------------------------------------*/

#ifndef PSOUT_H
#define PSOUT_H

#include <string>
#include <vector>
#include "output.h"
#include "coords.h"

class Psout : public Output {
	public:
	Psout();
	~Psout();
	void init();
	void to_file(const char* file);
	
	void start_file(const char* file);
	void append_file(const char* file);
	void append_file_barebones(const char* file);
	
	void set_fullnumber(Spot p, int val);
	void add_candidate(Spot p, int val);
	void highlight_candidate(Spot p, int val, float r, float g, float b);
	void highlight_cell(Spot p, float r, float g, float b);
	void highlight_row(int row, float r, float g, float b);
	void highlight_col(int col, float r, float g, float b);
	void highlight_box(int boxid, float r, float g, float b);
	void outline_cell(Spot p, float width, float r, float g, float b);
	void link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b);
	
	
	int* cellvalues;
	std::vector<std::string> showcandidates;
	std::vector<std::string> highlightedcandidates;
	std::vector<std::string> highlightedcells;
	std::vector<std::string> highlightedrules;
	std::vector<std::string> outlinedcells;
};

#endif
