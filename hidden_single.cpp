
#include "hidden_single.h"

const int HS_ROW = 1;
const int HS_COL = 2;
const int HS_BOX = 3;

HiddenSingle::HiddenSingle(Target _target, int _type, std::vector<Key> _keys, std::vector<Spot> _roots, std::vector<int> _root_types, int _val) {
	target = _target;
	type = _type;
	keys = _keys;
	roots = _roots;
	root_types = _root_types;
	val = _val;
}

void HiddenSingle::apply(BitMatrix* bits) {
	bits->set_value(target.spot, val);
}

void HiddenSingle::full_display(Output* out) {
	// display roots
	for(int i=0; i < roots.size(); i++) {
		out->highlight_cell(roots[i], 0.8, 1.0, 1.0);
	}
	// display rule and spot
	if(type == HS_ROW) {
		out->highlight_row(target.spot.row, 0.9, 0.9, 0.9);
		out->highlight_cell(target.spot, 1.0, 0.8, 0.8);
		out->highlight_candidate(target.spot, val, 1.0, 0.5, 0.5);
	} else if(type == HS_COL) {
		out->highlight_col(target.spot.col, 0.9, 0.9, 0.9);
		out->highlight_cell(target.spot, 1.0, 0.8, 0.8);
		out->highlight_candidate(target.spot, val, 1.0, 0.5, 0.5);
	} else if(type == HS_BOX) {
		out->highlight_box(boxid_of_spot(target.spot), 0.9, 0.9, 0.9);
		out->highlight_cell(target.spot, 1.0, 0.8, 0.8);
		out->highlight_candidate(target.spot, val, 1.0, 0.5, 0.5);
	}
}

void HiddenSingle::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> HiddenSingle::target_list() {
	std::vector<Target> ret;
	ret.push_back(target);
	return ret;
}

std::vector<Key> HiddenSingle::key_list() {
	return keys;
}

Pattern* HiddenSingle::clone() {
	return (new HiddenSingle(*this));
}

void HiddenSingle::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, HiddenSingle* hs) {
	out << "Hidden Single (" << (hs->type == HS_BOX ? "box" : (hs->type == HS_ROW ? "row" : "col")) << ")" << std::endl;
	out << "\tTarget: " << hs->target;
	return out;
}

void unit_hidden_singles(Bits* bits, std::vector<HiddenSingle>* ret, std::vector<Spot>& spots, int type) {
	for(int i=0; i < spots.size(); i++) {
		// for each spot p in unit
		Spot p = spots[i];
		if(bits->has_value(p)) { continue; }
		
		// find candidates for all other spots in unit
		int bitsum = 0;
		for(int k=0; k < spots.size(); k++) {
			if(k == i) { continue; }
			bitsum |= bits->bit_of_spot(spots[k]);
		}
		
		// candidates for all spots in unit
		int allbits = bitsum | bits->bit_of_spot(p);
		
		if(bitsum == allbits) { continue; }
		
		// must be one bit (candidate) not in others. call it val
		int val = first_bit_int(allbits - bitsum);
		std::vector<Key> possiblekeys;
		std::vector<Spot> roots;
		std::vector<int> root_types;
		for(int k=0; k < spots.size(); k++) {
			// for each other spot in unit
			if(k == i) { continue; }
			Spot px = spots[k];
			// covers will hold all spots that would need to be removed
			std::vector<Spot> covers = bits->covers_of_spot(px, val);
			if(bits->has_value(px)) {
				// if a blocker, add it to covers
				covers.push_back(px);
			} else {
				// handle roots for display
				int pxboxid = boxid_of_spot(px);
				for(int m=0; m < covers.size(); m++) {
					if(in_spots(roots, covers[m])) { continue; }
					if(pxboxid == boxid_of_spot(covers[m])) {
						roots.push_back(covers[m]);
						root_types.push_back(HS_BOX);
					} else if(px.row == covers[m].row) {
						roots.push_back(covers[m]);
						root_types.push_back(HS_ROW);
					} else if(px.col == covers[m].col) {
						roots.push_back(covers[m]);
						root_types.push_back(HS_COL);
					}
				}
			}
			
			// make sure all covers (and if applicable the blocker) are possible
			bool possible = true;
			for(int m=0; m < covers.size(); m++) {
				possible = possible && bits->is_possible(covers[m]);
			}
			if(!possible) { continue; }
			
			// merge covers into possiblekeys
			int mult = covers.size();
			for(int m=0; m < covers.size(); m++) {
				// for each cover
				int coveridx = idx_of_keys(possiblekeys, covers[m]);
				if(coveridx >= 0) {
					// cover is already in possiblekeys
					// set multiplicity to minimum
					if(possiblekeys[coveridx].multiplicity > mult) {
						possiblekeys[coveridx].multiplicity = mult;
					}
				} else {
					// cover not already in possiblekeys
					// add to possiblekeys
					possiblekeys.push_back(Key(covers[m], mult));
				}
			}
		}
		ret->push_back(HiddenSingle(Target(p, val), type, possiblekeys, roots, root_types, val));
	}
}

std::vector<HiddenSingle> find_hidden_singles(Bits* bits) {
	std::vector<HiddenSingle> ret;
	for(int i=0; i < 9; i++) {
		// boxes
		for(int j=0; j < 9; j++) {
			std::vector<Spot> spots = spots_of_box(j);
			unit_hidden_singles(bits, &ret, spots, HS_BOX);
		}
		
		// rows
		for(int j=0; j < 9; j++) {
			std::vector<Spot> spots = spots_of_row(j);
			unit_hidden_singles(bits, &ret, spots, HS_ROW);
		}
		
		// cols
		for(int j=0; j < 9; j++) {
			std::vector<Spot> spots = spots_of_col(j);
			unit_hidden_singles(bits, &ret, spots, HS_COL);
		}
	}
	return ret;
}

