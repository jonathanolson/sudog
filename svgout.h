#ifndef SVGOUT_H
#define SVGOUT_H

#include <iostream>
#include <string>
#include <vector>

#include "coords.h"
#include "output.h"

class Out_Number {
	public:
	Out_Number(Spot _p, int _val) { p = _p; val = _val; }
	Spot p;
	int val;
};

class Out_Highlight {
	public:
	Out_Highlight(Spot _p, int _val, float _r, float _g, float _b) { p = _p; val = _val; r = _r; g = _g; b = _b;}
	Spot p;
	int val;
	float r;
	float g;
	float b;
};

class Out_Outline {
	public:
	Out_Outline(Spot _p, float _width, float _r, float _g, float _b) { p = _p; width = _width; r = _r; g = _g; b = _b;}
	Spot p;
	float width;
	float r;
	float g;
	float b;
};

class Out_Link {
	public:
	Out_Link(Spot _spot_a, int _val_a, Spot _spot_b, int _val_b, float _width, float _r, float _g, float _b) { spot_a = _spot_a; val_a = _val_a; spot_b = _spot_b; val_b = _val_b; width = _width; r = _r; g = _g; b = _b; }
	Spot spot_a;
	Spot spot_b;
	int val_a;
	int val_b;
	float width;
	float r;
	float g;
	float b;
};

class SVGOut : public Output {
	public:
	SVGOut();
	virtual ~SVGOut();
	
	void init();
	void draw(std::ostream& out);
	
	void set_fullnumber(Spot p, int val);
	void add_candidate(Spot p, int val);
	void highlight_candidate(Spot p, int val, float r, float g, float b);
	void highlight_cell(Spot p, float r, float g, float b);
	void highlight_row(int row, float r, float g, float b);
	void highlight_col(int col, float r, float g, float b);
	void highlight_box(int boxid, float r, float g, float b);
	void outline_cell(Spot p, float width, float r, float g, float b);
	void link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b);
	
	std::vector<Out_Number> fullnumbers;
	std::vector<Out_Number> candidates;
	std::vector<Out_Highlight> highlighted_candidates;
	std::vector<Out_Highlight> highlighted_cells;
	std::vector<Out_Highlight> highlighted_boxes;
	std::vector<Out_Highlight> highlighted_rows;
	std::vector<Out_Highlight> highlighted_cols;
	std::vector<Out_Outline> outlined_cells;
	std::vector<Out_Link> links;
};

#endif
