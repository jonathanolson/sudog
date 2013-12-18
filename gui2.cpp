
#include <gdk/gdk.h>
#include <gtkmm/expander.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <cairomm/context.h>
#include <string>
#include <stack>
#include <iostream>
#include <sstream>
#include <gdk/gdkkeysyms.h>

#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>

#include "coords.h"
#include "psout.h"
#include "backtrack_common.h"
#include "backtrack_naive_unique.h"
#include "bitmatrix_to_ps.h"
#include "bits.h"

#include "simple_naked_single.h"
#include "naked_single.h"
#include "hidden_single.h"
#include "nh_subset.h"
#include "pointing.h"
#include "boxline.h"
#include "nfish.h"
#include "simple_coloring.h"
#include "ywing.h"

#include "pretty_solver.h"
#include "pattern.h"
#include "backtrack_to_bits.h"
#include "permutation.h"

#include "cairo_out.h"
#include "bits_to_output.h"

#include "strong_link.h"

#include "simple_complete_gen.h"
#include "naive_gen.h"

using namespace std;

std::stack<Bits> bitstack;
Bits bits;
Spot selected_spot;
bool selected;
GridArea* grid;
Gtk::Label* status;
Gtk::Label* pattern_description;
Gtk::Label* backtracker_status;
std::vector<Pattern*> patterns;
int cur_pattern;
bool show_candidates;
int candidate_mask;
bool show_backtrack;
bool show_keys;
bool show_links;
bool left_down;

int num_solns;
bool backtrack_valid;
Bits backtrack_bits;


void do_after_new_puzzle();

void paint() {
	grid->queue_draw_area(0,0,gridsize + 20, gridsize + 20);
}

void write_grid() {
	grid->init();
	if(show_backtrack) {
		write_numbers(&backtrack_bits, grid);
		paint();
		return;
	}
	write_numbers(&bits, grid);
	if(show_candidates) {
		if(candidate_mask == 0) {
			write_candidates(&bits, grid);
			if(show_links) {
				for(int i=1; i <= 9; i++) {
					std::vector<StrongLink> links = strong_links_of_val(&bits, i);
					for(int k=0; k < links.size(); k++) {
						strong_link_display(links[k], grid, 0.7, 0.7, 1);
					}
				}
			}
		} else {
			for(int i=1; i <= 9; i++) {
				if(int_has_candidate(candidate_mask, i)) {
					write_val_candidates(&bits, grid, i);
					////////////////////////////////
					if(show_links) {
						std::vector<StrongLink> links = strong_links_of_val(&bits, i);
						for(int k=0; k < links.size(); k++) {
							strong_link_display(links[k], grid, 0.7, 0.7, 1);
						}
					}
					//////////////////////////////////
					
				}
			}
		}
	}
	if(selected) {
		grid->highlight_cell(selected_spot, 0.8, 1.0, 0.5);
	}
	
	
	//////////////////////
	/*
	std::vector<StrongLink> links = strong_links_of_val(&bits, 6);
	for(int i=0; i < links.size(); i++) {
		strong_link_display(links[i], grid, 0.7, 0.7, 1);
	}
	*/
	//////////////////////
	
	
	if(cur_pattern != -1) {
		patterns[cur_pattern]->full_display(grid);
		if(show_keys) {
			patterns[cur_pattern]->display_keys(grid);
		}
		
		std::ostringstream sout;
		patterns[cur_pattern]->describe(sout);
		std::string str = sout.str();
		const char* chars = str.c_str();
		pattern_description->set_text(chars);
	} else {
		pattern_description->set_text("No Pattern Selected");
	}
	paint();
}

void bitstack_save() {
	bitstack.push(bits);
}

void bitstack_restore() {
	bits = bitstack.top();
	bitstack.pop();
}

void delete_patterns() {
	for(int i=0; i < patterns.size(); i++) {
		delete patterns[i];
	}
	patterns.clear();
}

void add_pattern(Pattern* pat) {
	patterns.push_back(pat->clone());
}

void find_patterns() {
	cur_pattern = -1;
	delete_patterns();
	std::vector<SimpleNakedSingle> snss = find_simple_naked_singles(&bits);
	std::vector<NakedSingle> nss = find_naked_singles(&bits);
	std::vector<HiddenSingle> hss = find_hidden_singles(&bits);
	std::vector<NHSubset> nhs = find_nh_subsets(&bits);
	std::vector<Pointing> points = find_pointings(&bits);
	std::vector<BoxLine> boxlines = find_boxlines(&bits);
	std::vector<NFish> nfishes = find_nfishes(&bits);
	std::vector<SimpleColoring> scs = find_simple_colorings(&bits);
	std::vector<YWing> ywings = find_ywings(&bits);
	for(int i=0; i < snss.size(); i++) {
		if(!pattern_novel(patterns, &(snss[i]))) { continue; }
		add_pattern(&(snss[i]));
	}
	for(int i=0; i < nss.size(); i++) {
		if(!pattern_novel(patterns, &(nss[i]))) { continue; }
		add_pattern(&(nss[i]));
	}
	for(int i=0; i < hss.size(); i++) {
		if(!pattern_novel(patterns, &(hss[i]))) { continue; }
		add_pattern(&(hss[i]));
	}
	for(int i=0; i < nhs.size(); i++) {
		if(!pattern_novel(patterns, &(nhs[i]))) { continue; }
		add_pattern(&(nhs[i]));
	}
	for(int i=0; i < points.size(); i++) {
		if(!pattern_novel(patterns, &(points[i]))) { continue; }
		add_pattern(&(points[i]));
	}
	for(int i=0; i < boxlines.size(); i++) {
		if(!pattern_novel(patterns, &(boxlines[i]))) { continue; }
		add_pattern(&(boxlines[i]));
	}
	for(int i=0; i < nfishes.size(); i++) {
		if(!pattern_novel(patterns, &(nfishes[i]))) { continue; }
		add_pattern(&(nfishes[i]));
	}
	for(int i=0; i < scs.size(); i++) {
		if(!pattern_novel(patterns, &(scs[i]))) { continue; }
		add_pattern(&(scs[i]));
	}
	for(int i=0; i < ywings.size(); i++) {
		if(!pattern_novel(patterns, &(ywings[i]))) { continue; }
		add_pattern(&(ywings[i]));
	}
	
	
	if(patterns.size() == 0) {
		status->set_label("No patterns found");
	} else {
		char buf[256];
		sprintf(buf, "%d patterns found", patterns.size());
		status->set_text(std::string(buf));
	}
}

void bits_before_change() {
	bitstack_save();
	selected = false;
}

void bits_after_change() {
	find_patterns();
}

void show_current_pattern() {
	if(cur_pattern != -1) {
		patterns[cur_pattern]->full_display(grid);
	}
}

void next_pattern() {
	if(patterns.size() == 0) {
		status->set_label("No patterns found");
		return;
	}
	cur_pattern++;
	if(cur_pattern >= patterns.size()) {
		cur_pattern = -1;
	}
	write_grid();
}

void previous_pattern() {
	if(patterns.size() == 0) {
		status->set_label("No patterns found");
		return;
	}
	cur_pattern--;
	if(cur_pattern < -1) {
		cur_pattern = patterns.size() - 1;
	}
	write_grid();
}

void apply_pattern() {
	if(cur_pattern != -1) {
		bits_before_change();
		patterns[cur_pattern]->apply(&bits);
		//init_grid();
		bits_after_change();
		write_grid();
		//paint();
	}
}

void do_mouse_left(int row, int col, int val) {
	selected = true;
	selected_spot = Spot(row, col);
	write_grid();
}

void do_mouse_double_left(int row, int col, int val) {
	Spot p = Spot(row, col);
	if(!bits.has_candidate(p, val)) {
		// has no candidate: may be clicking on naked single cell
		if(!bits.has_value(p) && bits.count_candidates(p) == 1) {
			bits_before_change();
			bits.set_value(p, bits.first_bit(p));
			bits_after_change();
			write_grid();
		}
		return;
	}
	if(backtrack_valid && backtrack_bits.get_value(p) != val) { return; }
	bits_before_change();
	bits.set_value(p, val);
	bits_after_change();
	write_grid();
}

void do_mouse_right(int row, int col, int val) {
	if(backtrack_valid && backtrack_bits.get_value(Spot(row, col)) == val) { return; }
	bits_before_change();
	bits.remove_candidate(Spot(row, col), val);
	bits_after_change();
	write_grid();
}

void do_key_number(int val) {
	if(selected) {
		if(!bits.has_candidate(selected_spot, val)) { return; }
		bits_before_change();
		bits.set_value(selected_spot, val);
		bits_after_change();
		write_grid();
	} else {
		candidate_mask |= (1 << (val - 1));
		write_grid();
	}
}

void do_release_key_number(int val) {
	if(!selected) {
		candidate_mask &= (0x1FF - (1 << (val - 1)));
		write_grid();
	}
}

void undo() {
	if(bitstack.size() > 0) {
		bitstack_restore();
	}
	bits_after_change();
	selected = false;
	write_grid();
}

void full_undo() {
	while(bitstack.size() > 0) {
		bitstack_restore();
	}
	bits_after_change();
	selected = false;
	write_grid();
}

void full_logic_solve() {
	while(patterns.size() > 0) {
		next_pattern();
		apply_pattern();
	}
}

void toggle_show_candidates() {
	show_candidates = !show_candidates;
	write_grid();
}

void toggle_show_backtrack() {
	if(!backtrack_valid) { return; }
	show_backtrack = !show_backtrack;
	write_grid();
}

void toggle_show_keys() {
	show_keys = !show_keys;
	write_grid();
}

void toggle_show_links() {
	show_links = !show_links;
	write_grid();
}

void do_random_permute() {
	Permutation perm;
	perm.randomize();
	bits_before_change();
	bits = perm.permute_bits(&bits);
	bits_after_change();
	write_grid();
	if(backtrack_valid) {
		backtrack_bits = perm.permute_bits(&backtrack_bits);
	}
}

void do_canonical() {
	Permutation perm = find_canonical_permutation(&bits);
	bits_before_change();
	bits = perm.permute_bits(&bits);
	bits_after_change();
	write_grid();
	if(backtrack_valid) {
		backtrack_bits = perm.permute_bits(&backtrack_bits);
	}
}

void do_random_complete() {
	Grid grid = simple_complete_gen();
	bits_before_change();
	backtrack_to_bits(grid, &bits);
	bits_after_change();
	write_grid();
}

void do_naive_gen() {
	bits = naive_gen_bits();
	do_after_new_puzzle();
	write_grid();
}

void do_regular_output() {
	std::cout << bits;
}

void do_candidate_output() {
	std::cout <<= bits;
}

int KeyPress(Gtk::Widget *w, GdkEventKey *e) {
	if(e->type == GDK_KEY_PRESS) {
		switch(e->keyval) {
			case GDK_1:
				do_key_number(1); break;
			case GDK_2:
				do_key_number(2); break;
			case GDK_3:
				do_key_number(3); break;
			case GDK_4:
				do_key_number(4); break;
			case GDK_5:
				do_key_number(5); break;
			case GDK_6:
				do_key_number(6); break;
			case GDK_7:
				do_key_number(7); break;
			case GDK_8:
				do_key_number(8); break;
			case GDK_9:
				do_key_number(9); break;
			case GDK_Right:
				next_pattern(); break;
			case GDK_Left:
				previous_pattern(); break;
			case GDK_a:
				apply_pattern(); break;
			case GDK_c:
				toggle_show_candidates(); break;
			case GDK_C:
				do_canonical(); break;
			case GDK_f:
				full_logic_solve(); break;
			case GDK_F:
				toggle_show_backtrack(); break;
			case GDK_G:
				do_random_complete(); break;
			case GDK_k:
				toggle_show_keys(); break;
			case GDK_l:
				toggle_show_links(); break;
			case GDK_o:
				do_regular_output(); break;
			case GDK_O:
				do_candidate_output(); break;
			case GDK_p:
				do_random_permute(); break;
			case GDK_s:
				next_pattern(); break;
			case GDK_r:
				do_naive_gen(); break;
			case GDK_u:
				undo(); break;
			case GDK_U:
				full_undo(); break;
		}
	} else if(e->type == GDK_KEY_RELEASE) {
		switch(e->keyval) {
			case GDK_1:
				do_release_key_number(1); break;
			case GDK_2:
				do_release_key_number(2); break;
			case GDK_3:
				do_release_key_number(3); break;
			case GDK_4:
				do_release_key_number(4); break;
			case GDK_5:
				do_release_key_number(5); break;
			case GDK_6:
				do_release_key_number(6); break;
			case GDK_7:
				do_release_key_number(7); break;
			case GDK_8:
				do_release_key_number(8); break;
			case GDK_9:
				do_release_key_number(9); break;
		}
	}
	return 1;
}

void* do_backtrack(void *ptr) {
	/////
	gdk_threads_enter();
	backtracker_status->set_text("Backtracking for Solution");
	gdk_threads_leave();
	/////
	Grid grid = Grid();
	backtrack_of_bits(grid, &bits);
	num_solns = soln_count_solve(grid);
	// NOTE: BAD THREAD BEHAVIOR
	gdk_threads_enter();
	if(num_solns == 1) {
		backtrack_valid = true;
		backtrack_to_bits(grid, &backtrack_bits);
		backtracker_status->set_text("Unique Solution");
	} else if(num_solns == 0) {
		backtracker_status->set_text("No Solution");
		backtracker_status->modify_fg(Gtk::STATE_NORMAL, Gdk::Color("red"));
	} else {
		char buf[256];
		sprintf(buf, "%d Solutions", num_solns);
		backtracker_status->set_text(std::string(buf));
		backtracker_status->modify_fg(Gtk::STATE_NORMAL, Gdk::Color("red"));
	}
	backtracker_status->queue_draw();
	gdk_flush();
	gdk_threads_leave();
}

void do_after_new_puzzle() {
	backtrack_valid = false;
	selected = false;
	show_backtrack = false;
	while(!bitstack.empty()) {
		bitstack.pop();
	}
	bits_after_change();
	
	pthread_t backtrack_thread;
	int backtrack_thread_ret = pthread_create(&backtrack_thread, NULL, do_backtrack, (void*) NULL);
}

int main(int argc, char** argv) {
	srand((unsigned) time(0));
	selected = false;
	show_candidates = false;
	candidate_mask = 0;
	backtrack_valid = false;
	show_backtrack = false;
	show_keys = false;
	show_links = false;
	left_down = false;
	
	Glib::thread_init();
	//g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();
	
	bits = Bits();
	cin >> bits;
	
	Gtk::Main kit(argc, argv);
	
	Gtk::Window win;
	win.set_default_size(gridsize + 20 + 300, gridsize + 20);
	win.set_title("Sudog GUI #2 (Cairo)");
	
	status = new Gtk::Label("Puzzle Loaded");
	pattern_description = new Gtk::Label("No Pattern Loaded");
	pattern_description->set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER);
	Gtk::Frame* pattern_frame = new Gtk::Frame();
	Gtk::Expander* pattern_expander = new Gtk::Expander("Pattern");
	pattern_frame->add(*pattern_description);
	pattern_expander->add(*pattern_frame);
	backtracker_status = new Gtk::Label("Searching for Solutions");
	
	grid = new GridArea();
	
	
	
	bits_after_change();
	write_grid();
	
	Gtk::VBox mvbox;
	Gtk::HBox mhbox;
	Gtk::VBox display_box;
	win.add(mvbox);
	
	display_box.pack_start(*backtracker_status, Gtk::PACK_SHRINK, 0);
	display_box.pack_start(*pattern_expander, Gtk::PACK_SHRINK, 0);
	
	
	mhbox.pack_end(display_box, Gtk::PACK_EXPAND_WIDGET, 0);
	mhbox.pack_end(*grid, Gtk::PACK_SHRINK, 0);
	
	mvbox.add(mhbox);
	mvbox.add(*status);
	
	
	// Show Widgets
	grid->show();
	status->show();
	pattern_description->show();
	pattern_frame->show();
	pattern_expander->show();
	backtracker_status->show();
	display_box.show();
	mhbox.show();
	mvbox.show();
	
	kit.signal_key_snooper().connect(&KeyPress);
	
	pthread_t backtrack_thread;
	int backtrack_thread_ret = pthread_create(&backtrack_thread, NULL, do_backtrack, (void*) NULL);
	
	Gtk::Main::run(win);
	
	
	delete grid;
	delete status;
	delete pattern_description;
	delete pattern_frame;
	
	//pthread_join(backtrack_thread_ret, NULL);
	gdk_threads_leave();
	
	return 0;
}

bool GridArea::on_button_press_event(GdkEventButton* event) {
	int x = ((int) event->x);
	int y = ((int) event->y);
	if(x < 10 || x >= gridsize + 10 || y < 10 || y >= gridsize + 10) { return false; }
	x -= 10;
	y -= 10;
	int row = y / cellsize;
	int col = x / cellsize;
	int candrow = (y % cellsize) / candidatesize;
	int candcol = (x % cellsize) / candidatesize;
	int val = candrow * 3 + candcol + 1;
	if(event->button == 1) {
		// left
		if(event->type != GDK_BUTTON_PRESS) { std::cout << "warning not GDK_BUTTON_PRESS" << std::endl; }
		left_down = true;
		if(show_candidates) {
			do_mouse_double_left(row, col, val);
		} else {
			do_mouse_left(row, col, val);
		}
	} else if(event->button == 3) {
		// right
		do_mouse_right(row, col, val);
	}
	return true;
}

bool GridArea::on_button_release_event(GdkEventButton* event) {
	int x = ((int) event->x);
	int y = ((int) event->y);
	if(x < 10 || x >= gridsize + 10 || y < 10 || y >= gridsize + 10) { return false; }
	x -= 10;
	y -= 10;
	int row = y / cellsize;
	int col = x / cellsize;
	int candrow = (y % cellsize) / candidatesize;
	int candcol = (x % cellsize) / candidatesize;
	int val = candrow * 3 + candcol + 1;
	if(event->button == 1) {
		// left
		left_down = false;
//std::cout << "release" << std::endl;
	} else if(event->button == 3) {
	}
	return true;
}

bool GridArea::on_scroll_event(GdkEventScroll* event) {
	if(event->direction == GDK_SCROLL_UP) {
		previous_pattern();
	} else if(event->direction == GDK_SCROLL_DOWN) {
		next_pattern();
	}
	return true;
}

bool GridArea::on_motion_notify_event(GdkEventMotion* event) {
	if(!left_down) { return true; }
	int x = ((int) event->x);
	int y = ((int) event->y);
	//std::cout << "motion ";
	//std::cout << x << " " << y << std::endl;
	return true;
}

