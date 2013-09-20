
#include <iostream>

#include "libplot_out.h"

const int candidatesize = 20;
const int cellsize = candidatesize * 3;
const int boxsize = cellsize * 3;
const int gridsize = boxsize * 3;



void draw_grid(Plotter* plotter) {
	plotter->flinewidth(1);
	plotter->pencolor(0x8888, 0x8888, 0x8888);
	plotter->fline(0,cellsize,gridsize,cellsize);
	plotter->fline(0,2*cellsize,gridsize,2*cellsize);
	plotter->fline(0,4*cellsize,gridsize,4*cellsize);
	plotter->fline(0,5*cellsize,gridsize,5*cellsize);
	plotter->fline(0,7*cellsize,gridsize,7*cellsize);
	plotter->fline(0,8*cellsize,gridsize,8*cellsize);
	plotter->fline(cellsize,0,cellsize,gridsize);
	plotter->fline(2*cellsize,0,2*cellsize,gridsize);
	plotter->fline(4*cellsize,0,4*cellsize,gridsize);
	plotter->fline(5*cellsize,0,5*cellsize,gridsize);
	plotter->fline(7*cellsize,0,7*cellsize,gridsize);
	plotter->fline(8*cellsize,0,8*cellsize,gridsize);
	
	plotter->flinewidth(2);
	plotter->pencolor(0,0,0);
	plotter->fbox(0,0,gridsize,gridsize);
	plotter->fline(0,boxsize,gridsize,boxsize);
	plotter->fline(0,2*boxsize,gridsize,2*boxsize);
	plotter->fline(boxsize,0,boxsize,gridsize);
	plotter->fline(2*boxsize,0,2*boxsize,gridsize);
}

std::string str_of(int val) {
	switch(val) {
		case 1:
			return std::string("1");
		case 2:
			return std::string("2");
		case 3:
			return std::string("3");
		case 4:
			return std::string("4");
		case 5:
			return std::string("5");
		case 6:
			return std::string("6");
		case 7:
			return std::string("7");
		case 8:
			return std::string("8");
		case 9:
			return std::string("9");
		default:
			return std::string("E");
	}
}

void draw_fullnumber(Plotter* plotter, Spot spot, int val) {
	plotter->fontsize((5 * cellsize) / 8);
	plotter->fmove(cellsize * spot.col, cellsize * (8 - spot.row));
	plotter->fmoverel(((float)cellsize)/2, ((float)cellsize)/2);
	std::string str = str_of(val);
	plotter->alabel('c','c',str.c_str());
}

void draw_candidate(Plotter* plotter, Spot spot, int val) {
	//plotter->savestate();
	/*
	plotter->fmove(cellsize * spot.col, cellsize * (8 - spot.row));
	plotter->fscale(0.33333, 0.33333);
	plotter->fmoverel(cellsize * ((val - 1) % 3), cellsize * (2 - ((val - 1) / 3)));
	plotter->fmoverel(((float)cellsize)/2, ((float)cellsize)/2);
	*/
	plotter->fmove(cellsize * spot.col + candidatesize * ((val - 1) % 3), cellsize * (8 - spot.row) + candidatesize * (2 - ((val - 1) / 3)));
	plotter->fmoverel(((float)candidatesize)/2, ((float)candidatesize)/2);
	//plotter->fscale(0.33333, 0.33333);
	plotter->fontsize((cellsize) / 4);
	std::string str = str_of(val);
	plotter->alabel('c','c',str.c_str());
	//plotter->restorestate();
}

LPOut::LPOut(Plotter* plotter_) {
	plotter = plotter_;
	
	fullnumbers = std::vector<Out_Number>();
	candidates = std::vector<Out_Number>();
	highlighted_candidates = std::vector<Out_Highlight>();
	highlighted_cells = std::vector<Out_Highlight>();
	highlighted_boxes = std::vector<Out_Highlight>();
	highlighted_rows = std::vector<Out_Highlight>();
	highlighted_cols = std::vector<Out_Highlight>();
	outlined_cells = std::vector<Out_Outline>();
	links = std::vector<Out_Link>();
}

LPOut::~LPOut() {
}

void LPOut::init() {
	fullnumbers.clear();
	candidates.clear();
	highlighted_candidates.clear();
	highlighted_cells.clear();
	highlighted_boxes.clear();
	highlighted_rows.clear();
	highlighted_cols.clear();
	outlined_cells.clear();
	links.clear();
}

int lpscale(float f) {
	return (int)(65535.0 * f);
}

void LPOut::draw() {
	if(plotter->openpl() < 0) {
		std::cerr << "Couldn't open plotter" << std::endl;
	}
	plotter->fspace(0.0, 0.0, gridsize + 20, gridsize + 20);
	plotter->ftranslate(10, 10);
	
	
	for(int i=0; i < highlighted_boxes.size(); i++) {
		Out_Highlight h = highlighted_boxes[i];
	}
	
	for(int i=0; i < highlighted_rows.size(); i++) {
		Out_Highlight h = highlighted_rows[i];
	}
	
	for(int i=0; i < highlighted_cols.size(); i++) {
		Out_Highlight h = highlighted_cols[i];
	}
	
	for(int i=0; i < highlighted_cells.size(); i++) {
		Out_Highlight h = highlighted_cells[i];
	}
	
	for(int i=0; i < links.size(); i++) {
		Out_Link h = links[i];
	}
	
	for(int i=0; i < highlighted_candidates.size(); i++) {
		Out_Highlight h = highlighted_candidates[i];
	}
	
	for(int i=0; i < outlined_cells.size(); i++) {
		Out_Outline h = outlined_cells[i];
	}
	
	for(int i=0; i < candidates.size(); i++) {
		Out_Number n = candidates[i];
		draw_candidate(plotter, n.p, n.val);
	}
	
	for(int i=0; i < fullnumbers.size(); i++) {
		Out_Number n = fullnumbers[i];
		draw_fullnumber(plotter, n.p, n.val);
	}
	
	draw_grid(plotter);
	
	if(plotter->closepl() < 0) {
		std::cerr << "Couldn't close plotter" << std::endl;
	}
}


void LPOut::set_fullnumber(Spot p, int val) {
	fullnumbers.push_back(Out_Number(p, val));
}

void LPOut::add_candidate(Spot p, int val) {
	candidates.push_back(Out_Number(p, val));
}

void LPOut::highlight_candidate(Spot p, int val, float r, float g, float b) {
	highlighted_candidates.push_back(Out_Highlight(p, val, r, g, b));
}

void LPOut::highlight_cell(Spot p, float r, float g, float b) {
	highlighted_cells.push_back(Out_Highlight(p, 0, r, g, b));
}

void LPOut::highlight_row(int row, float r, float g, float b) {
	highlighted_rows.push_back(Out_Highlight(Spot(0,0), row, r, g, b));
}

void LPOut::highlight_col(int col, float r, float g, float b) {
	highlighted_cols.push_back(Out_Highlight(Spot(0,0), col, r, g, b));
}

void LPOut::highlight_box(int boxid, float r, float g, float b) {
	highlighted_boxes.push_back(Out_Highlight(Spot(0,0), boxid, r, g, b));
}

void LPOut::outline_cell(Spot p, float width, float r, float g, float b) {
	outlined_cells.push_back(Out_Outline(p, 3.5, r, g, b));
}

void LPOut::link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b) {
	links.push_back(Out_Link(spot_a, val_a, spot_b, val_b, width, r, g, b));
}

