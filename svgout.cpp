
#include <iostream>
#include <ios>
#include <cstdio>

#include "coords.h"
#include "svgout.h"

const float candidatesize = 20;
const float cellsize = candidatesize * 3; // 60
const float boxsize = cellsize * 3; // 180
const float gridsize = boxsize * 3; // 540

const float padsize = candidatesize; // 20
const float totalwidth = gridsize + padsize * 2; // 580

const float scale = 3.0 / totalwidth;

const float outlinesize = 4;

std::string x_str_of(int val) {
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

SVGOut::SVGOut() {
	
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

SVGOut::~SVGOut() {
}

void SVGOut::init() {
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

int int_of_float(float f) {
	int x = (int) (f*255);
	if(x < 0) { x = 0; }
	if(x > 255) { x = 255; }
	return x;
}

void print_color(std::ostream& out, float r, float g, float b) {
	char buffer[80];
	sprintf(buffer, "#%02X%02X%02X",int_of_float(r), int_of_float(g), int_of_float(b));
	out << buffer;
}

void draw_line(std::ostream& out, float x1, float y1, float x2, float y2, float thick, float r, float g, float b) {
	out << "<line x1=\"" << x1 * scale << "in\" y1=\"" << y1 * scale << "in\" x2=\"" << x2 * scale << "in\" y2=\"" << y2 * scale << "in\" stroke=\"";
	print_color(out, r, g, b);
	out << "\" stroke-width=\"" << thick * scale << "in\"/>\n";
}

void draw_rect_stroke(std::ostream& out, float x, float y, float width, float height, float thick, float r, float g, float b) {
	out << "<rect x=\"" << x * scale << "in\" y=\"" << y * scale << "in\" width=\"" << width * scale << "in\" height=\"" << height * scale << "in\" stroke=\"";
	print_color(out, r, g, b);
	out << "\" stroke-width=\"" << thick * scale << "in\" fill=\"none\"/>\n";
}

void draw_rect_fill(std::ostream& out, float x, float y, float width, float height, float r, float g, float b) {
	out << "<rect x=\"" << x * scale << "in\" y=\"" << y * scale << "in\" width=\"" << width * scale << "in\" height=\"" << height * scale << "in\" fill=\"";
	print_color(out, r, g, b);
	out << "\" stroke=\"none\"/>\n";
}

void draw_bignumber(std::ostream& out, float x, float y, int val) {
	out << "<text x=\"" << (x) * scale << "in\" y=\"" << (y + 11) * scale << "in\" style=\"font-family:'Helvetica',sans-serif;font-size:16;stroke:none;fill:black;text-anchor:middle;\">" << x_str_of(val) << "</text>\n";
}

void draw_smallnumber(std::ostream& out, float x, float y, int val) {
	out << "<text x=\"" << (x) * scale << "in\" y=\"" << (y + 5) * scale << "in\" style=\"font-family:'Helvetica',sans-serif;font-size:6.5;stroke:none;fill:black;text-anchor:middle;\">" << x_str_of(val) << "</text>\n";
}

void draw_fullnumber(std::ostream& out, Spot spot, int val) {
	draw_bignumber(out, padsize + cellsize * spot.col + cellsize / 2, padsize + cellsize * spot.row + cellsize / 2, val);
}

void draw_candidate(std::ostream& out, Spot spot, int val) {
	draw_smallnumber(out, padsize + cellsize * spot.col + ((val-1) % 3) * candidatesize + candidatesize / 2, padsize + cellsize * spot.row + ((val-1) / 3) * candidatesize + candidatesize / 2, val);
}

void draw_highlight_cell(std::ostream& out, Spot spot, float r, float g, float b) {
	draw_rect_fill(out, padsize + cellsize * spot.col, padsize + cellsize * spot.row, cellsize, cellsize, r, g, b);
}

void draw_highlight_candidate(std::ostream& out, Spot spot, int val, float r, float g, float b) {
	draw_rect_fill(out, padsize + cellsize * spot.col + ((val-1) % 3) * candidatesize, padsize + cellsize * spot.row + ((val-1) / 3) * candidatesize, candidatesize, candidatesize, r, g, b);
}

void draw_highlight_box(std::ostream& out, int boxid, float r, float g, float b) {
	draw_rect_fill(out, padsize + (boxid % 3) * boxsize, padsize + (boxid / 3) * boxsize, boxsize, boxsize, r, g, b);
}

void draw_highlight_row(std::ostream& out, int row, float r, float g, float b) {
	draw_rect_fill(out, padsize, padsize + row * cellsize, gridsize, cellsize, r, g, b);
}

void draw_highlight_col(std::ostream& out, int col, float r, float g, float b) {
	draw_rect_fill(out, padsize + col * cellsize, padsize, cellsize, gridsize, r, g, b);
}

void draw_outline(std::ostream& out, Spot spot, float r, float g, float b) {
	draw_rect_stroke(out, padsize + spot.col * cellsize + outlinesize / 2, padsize + spot.row * cellsize + outlinesize / 2, cellsize - outlinesize, cellsize - outlinesize, outlinesize, r, g, b);
}

void draw_grid(std::ostream& out) {
	
	draw_line(out, padsize + 1 * cellsize, padsize, padsize + 1 * cellsize, padsize + gridsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize + 2 * cellsize, padsize, padsize + 2 * cellsize, padsize + gridsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize + 4 * cellsize, padsize, padsize + 4 * cellsize, padsize + gridsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize + 5 * cellsize, padsize, padsize + 5 * cellsize, padsize + gridsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize + 7 * cellsize, padsize, padsize + 7 * cellsize, padsize + gridsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize + 8 * cellsize, padsize, padsize + 8 * cellsize, padsize + gridsize, 1, 0.5, 0.5, 0.5);
	
	draw_line(out, padsize, padsize + 1 * cellsize, padsize + gridsize, padsize + 1 * cellsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize, padsize + 2 * cellsize, padsize + gridsize, padsize + 2 * cellsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize, padsize + 4 * cellsize, padsize + gridsize, padsize + 4 * cellsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize, padsize + 5 * cellsize, padsize + gridsize, padsize + 5 * cellsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize, padsize + 7 * cellsize, padsize + gridsize, padsize + 7 * cellsize, 1, 0.5, 0.5, 0.5);
	draw_line(out, padsize, padsize + 8 * cellsize, padsize + gridsize, padsize + 8 * cellsize, 1, 0.5, 0.5, 0.5);
	
	draw_line(out, padsize, padsize + 1 * boxsize, padsize + gridsize, padsize + 1 * boxsize, 3, 0, 0, 0);
	draw_line(out, padsize, padsize + 2 * boxsize, padsize + gridsize, padsize + 2 * boxsize, 3, 0, 0, 0);
	
	draw_line(out, padsize + 1 * boxsize, padsize, padsize + 1 * boxsize, padsize + gridsize, 3, 0, 0, 0);
	draw_line(out, padsize + 2 * boxsize, padsize, padsize + 2 * boxsize, padsize + gridsize, 3, 0, 0, 0);
	
	draw_rect_stroke(out, padsize, padsize, gridsize, gridsize, 3, 0, 0, 0);
}

void SVGOut::draw(std::ostream& out) {
	out << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
	out << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	out << "<svg width=\"3.00in\" height=\"3.00in\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
	out << "<desc>Sudoku</desc>\n";
	
	for(int i=0; i < highlighted_boxes.size(); i++) {
		Out_Highlight n = highlighted_boxes[i];
		draw_highlight_box(out, n.val, n.r, n.g, n.b);
	}
	
	for(int i=0; i < highlighted_cols.size(); i++) {
		Out_Highlight n = highlighted_cols[i];
		draw_highlight_col(out, n.val, n.r, n.g, n.b);
	}
	
	for(int i=0; i < highlighted_rows.size(); i++) {
		Out_Highlight n = highlighted_rows[i];
		draw_highlight_row(out, n.val, n.r, n.g, n.b);
	}
	
	for(int i=0; i < highlighted_cells.size(); i++) {
		Out_Highlight n = highlighted_cells[i];
		draw_highlight_cell(out, n.p, n.r, n.g, n.b);
	}
	
	for(int i=0; i < highlighted_candidates.size(); i++) {
		Out_Highlight n = highlighted_candidates[i];
		draw_highlight_candidate(out, n.p, n.val, n.r, n.g, n.b);
	}
	
	for(int i=0; i < outlined_cells.size(); i++) {
		Out_Outline n = outlined_cells[i];
		draw_outline(out, n.p, n.r, n.g, n.b);
	}
	
	for(int i=0; i < fullnumbers.size(); i++) {
		Out_Number n = fullnumbers[i];
		draw_fullnumber(out, n.p, n.val);
	}
	
	for(int i=0; i < candidates.size(); i++) {
		Out_Number n = candidates[i];
		draw_candidate(out, n.p, n.val);
	}
	
	draw_grid(out);
	
	out << "</svg>\n";
}


void SVGOut::set_fullnumber(Spot p, int val) {
	fullnumbers.push_back(Out_Number(p, val));
}

void SVGOut::add_candidate(Spot p, int val) {
	candidates.push_back(Out_Number(p, val));
}

void SVGOut::highlight_candidate(Spot p, int val, float r, float g, float b) {
	highlighted_candidates.push_back(Out_Highlight(p, val, r, g, b));
}

void SVGOut::highlight_cell(Spot p, float r, float g, float b) {
	highlighted_cells.push_back(Out_Highlight(p, 0, r, g, b));
}

void SVGOut::highlight_row(int row, float r, float g, float b) {
	highlighted_rows.push_back(Out_Highlight(Spot(0,0), row, r, g, b));
}

void SVGOut::highlight_col(int col, float r, float g, float b) {
	highlighted_cols.push_back(Out_Highlight(Spot(0,0), col, r, g, b));
}

void SVGOut::highlight_box(int boxid, float r, float g, float b) {
	highlighted_boxes.push_back(Out_Highlight(Spot(0,0), boxid, r, g, b));
}

void SVGOut::outline_cell(Spot p, float width, float r, float g, float b) {
	outlined_cells.push_back(Out_Outline(p, 3.5, r, g, b));
}

void SVGOut::link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b) {
	links.push_back(Out_Link(spot_a, val_a, spot_b, val_b, width, r, g, b));
}

