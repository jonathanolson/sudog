
#include "cairo_out.h"
#include <iostream>

const int candidatesize = 20;
const int cellsize = candidatesize * 3;
const int boxsize = cellsize * 3;
const int gridsize = boxsize * 3;

GridArea::GridArea() {
	set_size_request(gridsize + 20, gridsize + 20);
	add_events(Gdk::BUTTON_PRESS_MASK);
	add_events(Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::SCROLL_MASK);
	add_events(Gdk::POINTER_MOTION_MASK);
	add_events(Gdk::BUTTON1_MOTION_MASK);
	
	// do not hint until it is much more intelligent!!!!!!
	//add_events(Gdk::POINTER_MOTION_HINT_MASK);
	//add_events(Gdk::ALL_EVENTS_MASK);
	
	fullnumbers = std::vector<Cairo_Number>();
	candidates = std::vector<Cairo_Number>();
	highlighted_candidates = std::vector<Cairo_Highlight>();
	highlighted_cells = std::vector<Cairo_Highlight>();
	highlighted_boxes = std::vector<Cairo_Highlight>();
	highlighted_rows = std::vector<Cairo_Highlight>();
	highlighted_cols = std::vector<Cairo_Highlight>();
	outlined_cells = std::vector<Cairo_Outline>();
	links = std::vector<Cairo_Link>();
}

GridArea::~GridArea() {
}

void GridArea::init() {
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

bool GridArea::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		cr->rectangle(event->area.x, event->area.y,
		event->area.width, event->area.height);
		cr->clip();
		
		////////////////////////////
		cr->save();
		
		cr->translate(10.0, 10.0);
		cairo_draw_background(cr);
		
		for(int i=0; i < highlighted_boxes.size(); i++) {
			Cairo_Highlight h = highlighted_boxes[i];
			cairo_highlight_box(cr, h.val, h.r, h.g, h.b, 1);
		}
		
		for(int i=0; i < highlighted_rows.size(); i++) {
			Cairo_Highlight h = highlighted_rows[i];
			cairo_highlight_row(cr, h.val, h.r, h.g, h.b, 1);
		}
		
		for(int i=0; i < highlighted_cols.size(); i++) {
			Cairo_Highlight h = highlighted_cols[i];
			cairo_highlight_col(cr, h.val, h.r, h.g, h.b, 1);
		}
		
		for(int i=0; i < highlighted_cells.size(); i++) {
			Cairo_Highlight h = highlighted_cells[i];
			cairo_highlight_cell(cr, h.p, h.r, h.g, h.b, 1);
		}
		
		for(int i=0; i < links.size(); i++) {
			Cairo_Link h = links[i];
			cairo_link_candidate(cr, h.spot_a, h.val_a, h.spot_b, h.val_b, h.width, h.r, h.g, h.b, 1.0);
		}
		
		for(int i=0; i < highlighted_candidates.size(); i++) {
			Cairo_Highlight h = highlighted_candidates[i];
			cairo_highlight_candidate(cr, h.p, h.val, h.r, h.g, h.b, 1);
		}
		
		for(int i=0; i < outlined_cells.size(); i++) {
			Cairo_Outline h = outlined_cells[i];
			cairo_outline_cell(cr, h.p, h.width, h.r, h.g, h.b, 1);
		}
		
		for(int i=0; i < candidates.size(); i++) {
			Cairo_Number n = candidates[i];
			cairo_draw_candidate(cr, n.p, n.val);
		}
		
		for(int i=0; i < fullnumbers.size(); i++) {
			Cairo_Number n = fullnumbers[i];
			cairo_draw_fullnumber(cr, n.p, n.val);
		}
		
		cr->set_antialias(Cairo::ANTIALIAS_NONE);
		cairo_draw_gridlines(cr);
		cr->set_antialias(Cairo::ANTIALIAS_DEFAULT);
		
		
		
		cr->restore();
	}
	
	return true;
}

void GridArea::set_fullnumber(Spot p, int val) {
	fullnumbers.push_back(Cairo_Number(p, val));
}

void GridArea::add_candidate(Spot p, int val) {
	candidates.push_back(Cairo_Number(p, val));
}

void GridArea::highlight_candidate(Spot p, int val, float r, float g, float b) {
	highlighted_candidates.push_back(Cairo_Highlight(p, val, r, g, b));
}

void GridArea::highlight_cell(Spot p, float r, float g, float b) {
	highlighted_cells.push_back(Cairo_Highlight(p, 0, r, g, b));
}

void GridArea::highlight_row(int row, float r, float g, float b) {
	highlighted_rows.push_back(Cairo_Highlight(Spot(0,0), row, r, g, b));
}

void GridArea::highlight_col(int col, float r, float g, float b) {
	highlighted_cols.push_back(Cairo_Highlight(Spot(0,0), col, r, g, b));
}

void GridArea::highlight_box(int boxid, float r, float g, float b) {
	highlighted_boxes.push_back(Cairo_Highlight(Spot(0,0), boxid, r, g, b));
}

void GridArea::outline_cell(Spot p, float width, float r, float g, float b) {
	outlined_cells.push_back(Cairo_Outline(p, 3.5, r, g, b));
}

void GridArea::link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b) {
	links.push_back(Cairo_Link(spot_a, val_a, spot_b, val_b, width, r, g, b));
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

void cairo_move_to_box(Cairo::RefPtr<Cairo::Context> cr, int boxid) {
	cr->move_to(boxsize * (boxid % 3), boxsize * (boxid / 3));
}

void cairo_move_to_spot(Cairo::RefPtr<Cairo::Context> cr, Spot p) {
	cr->move_to(cellsize * (p.col), cellsize * (p.row));
}

void cairo_rel_move_to_candidate(Cairo::RefPtr<Cairo::Context> cr, int val) {
	cr->rel_move_to(candidatesize * ((val - 1) % 3), candidatesize * ((val - 1) / 3));
}

void cairo_draw_fullnumber(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val) {
	cr->save();
	
	//cr->select_font_face("Bitstream Vera Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
	cr->select_font_face("Helvetica", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
	cr->set_font_size(42);
	Cairo::FontOptions font_options;
	font_options.set_hint_style(Cairo::HINT_STYLE_NONE);
	font_options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
	font_options.set_antialias(Cairo::ANTIALIAS_GRAY);
	cr->set_font_options(font_options);
	
	Cairo::FontExtents fe;
	Cairo::TextExtents te;
	cr->get_font_extents(fe);
	std::string str = str_of(val);
	cr->get_text_extents(str, te);
	float halfcell = ((float) cellsize) / 2.0;
	cairo_move_to_spot(cr, p);
	cr->rel_move_to(0, cellsize);
	//cr->rel_move_to(halfcell - te.x_bearing - te.width / 2.0, halfcell - fe.descent + fe.height / 2.0);
	cr->rel_move_to(halfcell - te.x_bearing - te.width / 2.0, halfcell - fe.descent - fe.height / 2.0);
	cr->show_text(str);
	cr->restore();
}

void cairo_draw_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val) {
	cr->save();
	
	//cr->select_font_face("Bitstream Vera Sans", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
	cr->select_font_face("Helvetica", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
	cr->set_font_size(16);
	Cairo::FontOptions font_options;
	font_options.set_hint_style(Cairo::HINT_STYLE_NONE);
	//font_options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
	font_options.set_antialias(Cairo::ANTIALIAS_GRAY);
	cr->set_font_options(font_options);
	
	Cairo::FontExtents fe;
	Cairo::TextExtents te;
	cr->get_font_extents(fe);
	std::string str = str_of(val);
	cr->get_text_extents(str, te);
	float halfcandidate = ((float) candidatesize) / 2.0;
	cairo_move_to_spot(cr, p);
	cairo_rel_move_to_candidate(cr, val);
	cr->rel_move_to(0, candidatesize + 3);
	//cr->rel_move_to(halfcell - te.x_bearing - te.width / 2.0, halfcell - fe.descent + fe.height / 2.0);
	cr->rel_move_to(halfcandidate - te.x_bearing - te.width / 2.0, halfcandidate - fe.descent - fe.height / 2.0);
	cr->show_text(str);
	cr->restore();
}

void cairo_highlight_box(Cairo::RefPtr<Cairo::Context> cr, int boxid, float r, float g, float b, float a) {
	cr->save();
	cairo_move_to_box(cr, boxid);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(boxsize,0);
	cr->rel_line_to(0, boxsize);
	cr->rel_line_to(-boxsize, 0);
	cr->fill();
	cr->restore();
}

void cairo_highlight_row(Cairo::RefPtr<Cairo::Context> cr, int row, float r, float g, float b, float a) {
	cr->save();
	cr->move_to(0, cellsize * row);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(gridsize,0);
	cr->rel_line_to(0, cellsize);
	cr->rel_line_to(-gridsize, 0);
	cr->fill();
	cr->restore();
}

void cairo_highlight_col(Cairo::RefPtr<Cairo::Context> cr, int col, float r, float g, float b, float a) {
	cr->save();
	cr->move_to(cellsize * col, 0);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(cellsize,0);
	cr->rel_line_to(0, gridsize);
	cr->rel_line_to(-cellsize, 0);
	cr->fill();
	cr->restore();
}

void cairo_highlight_cell(Cairo::RefPtr<Cairo::Context> cr, Spot p, float r, float g, float b, float a) {
	cr->save();
	cairo_move_to_spot(cr, p);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(cellsize,0);
	cr->rel_line_to(0, cellsize);
	cr->rel_line_to(-cellsize, 0);
	cr->fill();
	cr->restore();
}

void cairo_highlight_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val, float r, float g, float b, float a) {
	cr->save();
	cairo_move_to_spot(cr, p);
	cairo_rel_move_to_candidate(cr, val);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(candidatesize,0);
	cr->rel_line_to(0, candidatesize);
	cr->rel_line_to(-candidatesize, 0);
	cr->fill();
	cr->restore();
}

void cairo_outline_cell(Cairo::RefPtr<Cairo::Context> cr, Spot p, float width, float r, float g, float b, float a) {
	float two_width = 2.0 * width;
	cr->save();
	cairo_move_to_spot(cr, p);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(cellsize,0);
	cr->rel_line_to(0, cellsize);
	cr->rel_line_to(-cellsize, 0);
	cr->close_path();
	cr->rel_move_to(width, width);
	cr->rel_line_to(cellsize - two_width, 0);
	cr->rel_line_to(0, cellsize - two_width);
	cr->rel_line_to(two_width - cellsize, 0);
	cr->rel_line_to(0, two_width - cellsize);
	cr->close_path();
	cr->set_fill_rule(Cairo::FILL_RULE_EVEN_ODD);
	cr->fill();
	cr->set_fill_rule(Cairo::FILL_RULE_WINDING);
	cr->restore();
}

/* COPIED FROM ABOVE
void cairo_move_to_spot(Cairo::RefPtr<Cairo::Context> cr, Spot p) {
	cr->move_to(cellsize * (p.col), cellsize * (p.row));
}

void cairo_rel_move_to_candidate(Cairo::RefPtr<Cairo::Context> cr, int val) {
	cr->rel_move_to(candidatesize * ((val - 1) % 3), candidatesize * ((val - 1) / 3));
}
*/ //  COPIED FROM ABOVE

void cairo_link_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b, float a) {
	cr->save();
	cr->set_source_rgba(r, g, b, a);
	
	cairo_highlight_candidate(cr, spot_a, val_a, r, g, b, a);
	cairo_highlight_candidate(cr, spot_b, val_b, r, g, b, a);
	/*
	cr->arc(cellsize * (spot_a.col) + candidatesize * ((val_a - 1) % 3) + candidatesize / 2.0, cellsize * (spot_a.row) + candidatesize * ((val_a - 1) / 3) + candidatesize / 2.0 - 1.0, candidatesize / 2.0, 0, 2 * M_PI);
	cr->fill();
	cr->arc(cellsize * (spot_b.col) + candidatesize * ((val_b - 1) % 3) + candidatesize / 2.0, cellsize * (spot_b.row) + candidatesize * ((val_b - 1) / 3) + candidatesize / 2.0 - 1.0, candidatesize / 2.0, 0, 2 * M_PI);
	cr->fill();
	*/
	
	cr->move_to(cellsize * (spot_a.col) + candidatesize * ((val_a - 1) % 3) + candidatesize / 2.0, cellsize * (spot_a.row) + candidatesize * ((val_a - 1) / 3) + candidatesize / 2.0);
	cr->line_to(cellsize * (spot_b.col) + candidatesize * ((val_b - 1) % 3) + candidatesize / 2.0, cellsize * (spot_b.row) + candidatesize * ((val_b - 1) / 3) + candidatesize / 2.0);
	cr->set_line_width(width);
	cr->stroke();
	cr->restore();
}

void cairo_draw_background(Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_source_rgb(1.0, 1.0, 1.0);
	//cr->paint();
	cr->move_to(0,0);
	cr->line_to(gridsize,0);
	cr->line_to(gridsize, gridsize);
	cr->line_to(0, gridsize);
	cr->close_path();
	cr->fill();
	cr->restore();
}

void cairo_draw_gridlines(Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_source_rgb(0.5, 0.5, 0.5);
	
	cr->set_line_width(1);
	cr->move_to(cellsize, 0);
	cr->line_to(cellsize, gridsize);
	cr->move_to(2.0 * cellsize, 0);
	cr->line_to(2.0 * cellsize, gridsize);
	cr->move_to(4.0 * cellsize, 0);
	cr->line_to(4.0 * cellsize, gridsize);
	cr->move_to(5.0 * cellsize, 0);
	cr->line_to(5.0 * cellsize, gridsize);
	cr->move_to(7.0 * cellsize, 0);
	cr->line_to(7.0 * cellsize, gridsize);
	cr->move_to(8.0 * cellsize, 0);
	cr->line_to(8.0 * cellsize, gridsize);
	cr->move_to(0, cellsize);
	cr->line_to(gridsize, cellsize);
	cr->move_to(0, 2.0 * cellsize);
	cr->line_to(gridsize, 2.0 * cellsize);
	cr->move_to(0, 4.0 * cellsize);
	cr->line_to(gridsize, 4.0 * cellsize);
	cr->move_to(0, 5.0 * cellsize);
	cr->line_to(gridsize, 5.0 * cellsize);
	cr->move_to(0, 7.0 * cellsize);
	cr->line_to(gridsize, 7.0 * cellsize);
	cr->move_to(0, 8.0 * cellsize);
	cr->line_to(gridsize, 8.0 * cellsize);
	cr->stroke();
	
	
	cr->set_source_rgb(0.0, 0.0, 0.0);
	
	cr->set_line_width(3);
	cr->move_to(0,0);
	cr->line_to(gridsize,0);
	cr->line_to(gridsize, gridsize);
	cr->line_to(0, gridsize);
	cr->close_path();
	cr->stroke();
	
	cr->move_to(boxsize, 0);
	cr->line_to(boxsize, gridsize);
	cr->move_to(2.0 * boxsize, 0);
	cr->line_to(2.0 * boxsize, gridsize);
	cr->move_to(0, boxsize);
	cr->line_to(gridsize, boxsize);
	cr->move_to(0, 2.0 * boxsize);
	cr->line_to(gridsize, 2.0 * boxsize);
	cr->stroke();
	
	cr->restore();
}


