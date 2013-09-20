
#include "pointing.h"
#include "keyrank.h"

const int POINT_ROW = 1;
const int POINT_COL = 2;

Pointing::Pointing(std::vector<Target> _targets, std::vector<Key> _keys, std::vector<Spot> _pointers, std::vector<int> _vals, int _type) {
	targets = _targets;
	keys = _keys;
	pointers = _pointers;
	vals = _vals;
	type = _type;
}

void Pointing::apply(BitMatrix* bits) {
	for(int i=0; i < targets.size(); i++) {
		targets[i].apply(bits);
	}
}

void Pointing::full_display(Output* out) {
	if(type == POINT_ROW) {
		out->highlight_row(targets[0].spot.row, 0.9, 0.9, 0.9);
	} else if(type == POINT_COL) {
		out->highlight_col(targets[0].spot.col, 0.9, 0.9, 0.9);
	}
	for(int i=0; i < targets.size(); i++) {
		Target target = targets[i];
		for(int j=0; j < target.candidates.size(); j++) {
			out->highlight_candidate(target.spot, target.candidates[j], 0.5, 0.5, 1.0);
		}
	}
	for(int i=0; i < pointers.size(); i++) {
		out->highlight_cell(pointers[i], 0.8, 0.8, 0.8);
	}
}

void Pointing::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> Pointing::target_list() {
	return targets;
}

std::vector<Key> Pointing::key_list() {
	return keys;
}

Pattern* Pointing::clone() {
	return (new Pointing(*this));
}

void Pointing::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, Pointing* p) {
	out << "Pointing (" << (p->type == POINT_ROW ? "row" : "col") << ")" << std::endl;
	switch(p->pointers.size()) {
		case 1:
			out << "\"Pointing Single\""; break;
		case 2:
			out << "\"Pointing Pair\""; break;
		case 3:
			out << "\"Pointing Triple\""; break;
	}
	
	out << std::endl << "\tPointers:";
	for(int i=0; i < p->pointers.size(); i++) {
		out << std::endl << "\t\t" << p->pointers[i];
	}
	out << std::endl << "\tTargets:";
	for(int i=0; i < p->targets.size(); i++) {
		out << std::endl << "\t\t" << p->targets[i];
	}
	return out;
}

std::vector<Pointing> find_pointings(Bits* bits) {
	std::vector<Pointing> ret;
	for(int i = 0; i < 9; i++) {
		// for box i
		std::vector<Spot> boxspots = spots_of_box(i);
		int boxrow = boxspots[0].row;
		int boxcol = boxspots[0].col;
		
		// check rows
		for(int j=0; j < 3; j++) {
			// row is absolute row
			int row = boxrow + j;
			int spotbits = 0;
			int compbits = 0;
			std::vector<Spot> spots_empty;
			std::vector<Spot> comps_empty;
			for(int k=0; k < 9; k++) {
				if(bits->has_value(boxspots[k])) { continue; }
				if(boxspots[k].row == row) {
					spotbits |= bits->bit_of_spot(boxspots[k]);
					spots_empty.push_back(boxspots[k]);
				} else {
					compbits |= bits->bit_of_spot(boxspots[k]);
					comps_empty.push_back(boxspots[k]);
				}
			}
			int solos = bit_subtract_int(spotbits, compbits);
			std::vector<int> solobitlist = vals_of_int(solos);
			if(solos != 0) {
				// we have aligned values
				std::vector<Spot> others_empty;
				for(int k=0; k < 9; k++) {
					Spot p = Spot(row, k);
					if(boxid_of_spot(p) == i) { continue; }
					if(bits->has_value(p)) { continue; }
					others_empty.push_back(p);
				}
				int otherbit = bits->bit_of_spots(others_empty);
				if(bit_subtract_int(solos, otherbit) == solos) { continue; }
				// at least one candidate will be removed
				std::vector<Target> targets;
				std::vector<int> vals;
				for(int k=0; k < others_empty.size(); k++) {
					std::vector<int> removals;
					for(int m=0; m < solobitlist.size(); m++) {
						int mbit = solobitlist[m];
						if(bits->has_candidate(others_empty[k], mbit)) {
							if(!in_ints(vals, mbit)) { vals.push_back(mbit); }
							removals.push_back(mbit);
						}
					}
					if(removals.size() > 0) {
						targets.push_back(Target(others_empty[k], removals, 0));
					}
				}
				if(targets.size() == 0) { continue; } // double check candidates will be removed
				std::vector<Spot> pointers;
				for(int k=0; k < spots_empty.size(); k++) {
					bool hasvals = false;
					for(int m=0; m < vals.size(); m++) {
						if(bits->has_candidate(spots_empty[k], vals[m])) {
							hasvals = true;
							break;
						}
					}
					if(hasvals) {
						pointers.push_back(spots_empty[k]);
					}
				}
				KeyRank keyrank;
				for(int k=0; k < 9; k++) {
					Spot spot = boxspots[k];
					if(spot.row == row) { continue; }
					// not on pointing row, but inside box
					for(int m=0; m < vals.size(); m++) {
						int mbit = vals[m];
						keyrank.add_possible_covers(bits, spot, mbit);
					}
				}
				ret.push_back(Pointing(targets, keyrank.keys, pointers, vals, POINT_ROW));
			}
		}
		
		// check cols
		for(int j=0; j < 3; j++) {
			// col is absolute col
			int col = boxcol + j;
			int spotbits = 0;
			int compbits = 0;
			std::vector<Spot> spots_empty;
			std::vector<Spot> comps_empty;
			for(int k=0; k < 9; k++) {
				if(bits->has_value(boxspots[k])) { continue; }
				if(boxspots[k].col == col) {
					spotbits |= bits->bit_of_spot(boxspots[k]);
					spots_empty.push_back(boxspots[k]);
				} else {
					compbits |= bits->bit_of_spot(boxspots[k]);
					comps_empty.push_back(boxspots[k]);
				}
			}
			int solos = bit_subtract_int(spotbits, compbits);
			std::vector<int> solobitlist = vals_of_int(solos);
			if(solos != 0) {
				// we have aligned values
				std::vector<Spot> others_empty;
				for(int k=0; k < 9; k++) {
					Spot p = Spot(k, col);
					if(boxid_of_spot(p) == i) { continue; }
					if(bits->has_value(p)) { continue; }
					others_empty.push_back(p);
				}
				int otherbit = bits->bit_of_spots(others_empty);
				if(bit_subtract_int(solos, otherbit) == solos) { continue; }
				// at least one candidate will be removed
				std::vector<Target> targets;
				std::vector<int> vals;
				for(int k=0; k < others_empty.size(); k++) {
					std::vector<int> removals;
					for(int m=0; m < solobitlist.size(); m++) {
						int mbit = solobitlist[m];
						if(bits->has_candidate(others_empty[k], mbit)) {
							if(!in_ints(vals, mbit)) { vals.push_back(mbit); }
							removals.push_back(mbit);
						}
					}
					if(removals.size() > 0) {
						targets.push_back(Target(others_empty[k], removals, 0));
					}
				}
				if(targets.size() == 0) { continue; } // double check candidates will be removed
				std::vector<Spot> pointers;
				for(int k=0; k < spots_empty.size(); k++) {
					bool hasvals = false;
					for(int m=0; m < vals.size(); m++) {
						if(bits->has_candidate(spots_empty[k], vals[m])) {
							hasvals = true;
							break;
						}
					}
					if(hasvals) {
						pointers.push_back(spots_empty[k]);
					}
				}
				KeyRank keyrank;
				for(int k=0; k < 9; k++) {
					Spot spot = boxspots[k];
					if(spot.col == col) { continue; }
					// not on pointing col, but inside box
					for(int m=0; m < vals.size(); m++) {
						int mbit = vals[m];
						keyrank.add_possible_covers(bits, spot, mbit);
					}
				}
				ret.push_back(Pointing(targets, keyrank.keys, pointers, vals, POINT_COL));
			}
		}
	}
	return ret;
}

