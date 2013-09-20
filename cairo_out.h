#ifndef CAIRO_OUT_H
#define CAIRO_OUT_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <cairomm/context.h>
#include <string>
#include <vector>

#include "coords.h"
#include "output.h"

extern const int candidatesize;
extern const int cellsize;
extern const int boxsize;
extern const int gridsize;

class Cairo_Number {
	public:
	Cairo_Number(Spot _p, int _val) { p = _p; val = _val; }
	Spot p;
	int val;
};

class Cairo_Highlight {
	public:
	Cairo_Highlight(Spot _p, int _val, float _r, float _g, float _b) { p = _p; val = _val; r = _r; g = _g; b = _b;}
	Spot p;
	int val;
	float r;
	float g;
	float b;
};

class Cairo_Outline {
	public:
	Cairo_Outline(Spot _p, float _width, float _r, float _g, float _b) { p = _p; width = _width; r = _r; g = _g; b = _b;}
	Spot p;
	float width;
	float r;
	float g;
	float b;
};

class Cairo_Link {
	public:
	Cairo_Link(Spot _spot_a, int _val_a, Spot _spot_b, int _val_b, float _width, float _r, float _g, float _b) { spot_a = _spot_a; val_a = _val_a; spot_b = _spot_b; val_b = _val_b; width = _width; r = _r; g = _g; b = _b; }
	Spot spot_a;
	Spot spot_b;
	int val_a;
	int val_b;
	float width;
	float r;
	float g;
	float b;
};

class GridArea : public Gtk::DrawingArea, public Output {
	public:
	GridArea();
	virtual ~GridArea();
	
	void init();
	
	void set_fullnumber(Spot p, int val);
	void add_candidate(Spot p, int val);
	void highlight_candidate(Spot p, int val, float r, float g, float b);
	void highlight_cell(Spot p, float r, float g, float b);
	void highlight_row(int row, float r, float g, float b);
	void highlight_col(int col, float r, float g, float b);
	void highlight_box(int boxid, float r, float g, float b);
	void outline_cell(Spot p, float width, float r, float g, float b);
	void link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b);
	
	protected:
	virtual bool on_expose_event(GdkEventExpose* event);
	virtual bool on_button_press_event(GdkEventButton* event);
	virtual bool on_button_release_event(GdkEventButton* event);
	virtual bool on_scroll_event(GdkEventScroll* event);
	virtual bool on_motion_notify_event(GdkEventMotion* event);
	
	private:
	std::vector<Cairo_Number> fullnumbers;
	std::vector<Cairo_Number> candidates;
	std::vector<Cairo_Highlight> highlighted_candidates;
	std::vector<Cairo_Highlight> highlighted_cells;
	std::vector<Cairo_Highlight> highlighted_boxes;
	std::vector<Cairo_Highlight> highlighted_rows;
	std::vector<Cairo_Highlight> highlighted_cols;
	std::vector<Cairo_Outline> outlined_cells;
	std::vector<Cairo_Link> links;
};

std::string str_of(int val);
void cairo_move_to_box(Cairo::RefPtr<Cairo::Context> cr, int boxid);
void cairo_move_to_spot(Cairo::RefPtr<Cairo::Context> cr, Spot p);
void cairo_rel_move_to_candidate(Cairo::RefPtr<Cairo::Context> cr, int val);
void cairo_draw_fullnumber(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val);
void cairo_draw_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val);
void cairo_highlight_box(Cairo::RefPtr<Cairo::Context> cr, int boxid, float r, float g, float b, float a);
void cairo_highlight_row(Cairo::RefPtr<Cairo::Context> cr, int row, float r, float g, float b, float a);
void cairo_highlight_col(Cairo::RefPtr<Cairo::Context> cr, int col, float r, float g, float b, float a);
void cairo_highlight_cell(Cairo::RefPtr<Cairo::Context> cr, Spot p, float r, float g, float b, float a);
void cairo_highlight_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot p, int val, float r, float g, float b, float a);
void cairo_outline_cell(Cairo::RefPtr<Cairo::Context> cr, Spot p, float width, float r, float g, float b, float a);
void cairo_link_candidate(Cairo::RefPtr<Cairo::Context> cr, Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b, float a);

void cairo_draw_background(Cairo::RefPtr<Cairo::Context> cr);
void cairo_draw_gridlines(Cairo::RefPtr<Cairo::Context> cr);

#endif
