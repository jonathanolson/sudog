
#include <gtkmm/drawingarea.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <cairomm/context.h>
#include <string>

#include "coords.h"

const int candidatesize = 20;
const int cellsize = candidatesize * 3;
const int boxsize = cellsize * 3;
const int gridsize = boxsize * 3;

class MyArea : public Gtk::DrawingArea {
	public:
	MyArea();
	virtual ~MyArea();
	
	protected:
	//Override default signal handler:
	virtual bool on_expose_event(GdkEventExpose* event);
};



int main(int argc, char** argv) {
	Gtk::Main kit(argc, argv);
	
	Gtk::Window win;
	win.set_default_size(cellsize * 9 + 20, cellsize * 9 + 20);
	win.set_title("Sudog GUI Test (Cairo)");
	
	MyArea area;
	win.add(area);
	area.show();
	
	Gtk::Main::run(win);
	
	return 0;
}

MyArea::MyArea() {
}

MyArea::~MyArea() {
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

void move_to_box(Cairo::RefPtr<Cairo::Context> cr, int boxid) {
	cr->move_to(boxsize * (boxid % 3), boxsize * (boxid / 3));
}

void move_to_spot(Cairo::RefPtr<Cairo::Context> cr, Spot p) {
	cr->move_to(cellsize * (p.col), cellsize * (p.row));
}

void rel_move_to_candidate(Cairo::RefPtr<Cairo::Context> cr, int val) {
	cr->rel_move_to(candidatesize * ((val - 1) % 3), candidatesize * ((val - 1) / 3));
}

void draw_fullnumber(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val) {
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
	move_to_spot(cr, p);
	//cr->rel_move_to(halfcell - te.x_bearing - te.width / 2.0, halfcell - fe.descent + fe.height / 2.0);
	cr->rel_move_to(halfcell - te.x_bearing - te.width / 2.0, halfcell - fe.descent - fe.height / 2.0);
	cr->show_text(str);
	cr->restore();
}

void draw_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val) {
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
	move_to_spot(cr, p);
	rel_move_to_candidate(cr, val);
	cr->rel_move_to(0, candidatesize + 3);
	//cr->rel_move_to(halfcell - te.x_bearing - te.width / 2.0, halfcell - fe.descent + fe.height / 2.0);
	cr->rel_move_to(halfcandidate - te.x_bearing - te.width / 2.0, halfcandidate - fe.descent - fe.height / 2.0);
	cr->show_text(str);
	cr->restore();
}

void highlight_box(Cairo::RefPtr<Cairo::Context> cr, int boxid, float r, float g, float b, float a) {
	cr->save();
	move_to_box(cr, boxid);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(boxsize,0);
	cr->rel_line_to(0, boxsize);
	cr->rel_line_to(-boxsize, 0);
	cr->fill();
	cr->restore();
}

void highlight_row(Cairo::RefPtr<Cairo::Context> cr, int row, float r, float g, float b, float a) {
	cr->save();
	cr->move_to(0, cellsize * row);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(gridsize,0);
	cr->rel_line_to(0, cellsize);
	cr->rel_line_to(-gridsize, 0);
	cr->fill();
	cr->restore();
}

void highlight_col(Cairo::RefPtr<Cairo::Context> cr, int col, float r, float g, float b, float a) {
	cr->save();
	cr->move_to(cellsize * col, 0);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(cellsize,0);
	cr->rel_line_to(0, gridsize);
	cr->rel_line_to(-cellsize, 0);
	cr->fill();
	cr->restore();
}

void highlight_cell(Cairo::RefPtr<Cairo::Context> cr, Spot p, float r, float g, float b, float a) {
	cr->save();
	move_to_spot(cr, p);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(cellsize,0);
	cr->rel_line_to(0, cellsize);
	cr->rel_line_to(-cellsize, 0);
	cr->fill();
	cr->restore();
}

void highlight_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val, float r, float g, float b, float a) {
	cr->save();
	move_to_spot(cr, p);
	rel_move_to_candidate(cr, val);
	cr->set_source_rgba(r, g, b, a);
	cr->rel_line_to(candidatesize,0);
	cr->rel_line_to(0, candidatesize);
	cr->rel_line_to(-candidatesize, 0);
	cr->fill();
	cr->restore();
}

void outline_cell(Cairo::RefPtr<Cairo::Context> cr, Spot p, float width, float r, float g, float b, float a) {
	float two_width = 2.0 * width;
	cr->save();
	move_to_spot(cr, p);
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

void draw_background(Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_source_rgb(1.0, 1.0, 1.0);
	cr->paint();
	cr->restore();
}

void draw_gridlines(Cairo::RefPtr<Cairo::Context> cr) {
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

bool MyArea::on_expose_event(GdkEventExpose* event) {
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		
		// coordinates for the center of the window
		//int xc, yc;
		//xc = width / 2;
		//yc = height / 2;
		
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		//cr->set_line_width(10.0);
		
		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y,
		event->area.width, event->area.height);
		cr->clip();
		
		
		////////////////////////////
		cr->save();
		draw_background(cr);
		cr->translate(10.0, 10.0);
		
		
		highlight_box(cr, 7, 0.9, 0.9, 0.9, 1);
		
		highlight_row(cr, 4, 0.8, 0.8, 1.0, 1);
		highlight_col(cr, 6, 0.8, 0.8, 1.0, 1);
		
		highlight_cell(cr, Spot(1,2), 1.0, 0.0, 0.0, 1);
		
		highlight_candidate(cr, Spot(6,1), 1, 1.0, 0.0, 0.0, 1);
		highlight_candidate(cr, Spot(6,1), 9, 1.0, 0.0, 1.0, 1);
		
		outline_cell(cr, Spot(7,7), 3.5, 1.0, 0.0, 0.0, 1);
		outline_cell(cr, Spot(7,1), 3.5, 1.0, 0.0, 0.0, 1);
		outline_cell(cr, Spot(1,4), 3.5, 1.0, 0.0, 0.0, 1);
		
		draw_candidate(cr, Spot(6,1), 1);
		draw_candidate(cr, Spot(6,1), 2);
		draw_candidate(cr, Spot(6,1), 3);
		draw_candidate(cr, Spot(6,1), 4);
		draw_candidate(cr, Spot(6,1), 5);
		draw_candidate(cr, Spot(6,1), 6);
		draw_candidate(cr, Spot(6,1), 7);
		draw_candidate(cr, Spot(6,1), 8);
		draw_candidate(cr, Spot(6,1), 9);
		
		draw_candidate(cr, Spot(4,4), 1);
		draw_candidate(cr, Spot(4,4), 2);
		draw_candidate(cr, Spot(4,4), 3);
		draw_candidate(cr, Spot(4,4), 4);
		draw_candidate(cr, Spot(4,4), 5);
		draw_candidate(cr, Spot(4,4), 6);
		draw_candidate(cr, Spot(4,4), 7);
		draw_candidate(cr, Spot(4,4), 8);
		draw_candidate(cr, Spot(4,4), 9);
		
		draw_candidate(cr, Spot(0,7), 1);
		draw_candidate(cr, Spot(0,7), 2);
		draw_candidate(cr, Spot(0,7), 3);
		draw_candidate(cr, Spot(0,7), 4);
		draw_candidate(cr, Spot(0,7), 5);
		draw_candidate(cr, Spot(0,7), 6);
		draw_candidate(cr, Spot(0,7), 7);
		draw_candidate(cr, Spot(0,7), 8);
		draw_candidate(cr, Spot(0,7), 9);
		
		draw_fullnumber(cr, Spot(1,3), 9);
		draw_fullnumber(cr, Spot(2,4), 1);
		draw_fullnumber(cr, Spot(3,5), 7);
		
		cr->set_antialias(Cairo::ANTIALIAS_NONE);
		draw_gridlines(cr);
		cr->set_antialias(Cairo::ANTIALIAS_DEFAULT);
		
		
		
		cr->restore();
	}
	
	return true;
}
