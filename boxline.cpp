
#include "boxline.h"
#include "keyrank.h"

const int BOXLINE_ROW = 1;
const int BOXLINE_COL = 2;

BoxLine::BoxLine(std::vector<Target> _targets, std::vector<Key> _keys, std::vector<Spot> _subset, std::vector<int> _vals, int _type) {
	targets = _targets;
	keys = _keys;
	subset = _subset;
	vals = _vals;
	type = _type;
}

void BoxLine::apply(BitMatrix* bits) {
	for(int i=0; i < targets.size(); i++) {
		targets[i].apply(bits);
	}
}

void BoxLine::full_display(Output* out) {
	out->highlight_box(boxid_of_spot(subset[0]), 0.9, 0.9, 0.9);
	if(type == BOXLINE_ROW) {
		out->highlight_row(subset[0].row, 0.9, 0.9, 0.9);
	} else if(type == BOXLINE_COL) {
		out->highlight_col(subset[0].col, 0.9, 0.9, 0.9);
	}
	for(int i=0; i < targets.size(); i++) {
		Target target = targets[i];
		for(int j=0; j < target.candidates.size(); j++) {
			out->highlight_candidate(target.spot, target.candidates[j], 1.0, 0.5, 1.0);
		}
	}
	for(int i=0; i < subset.size(); i++) {
		out->highlight_cell(subset[i], 0.8, 0.8, 0.8);
	}
}

void BoxLine::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> BoxLine::target_list() {
	return targets;
}

std::vector<Key> BoxLine::key_list() {
	return keys;
}

Pattern* BoxLine::clone() {
	return (new BoxLine(*this));
}

void BoxLine::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, BoxLine* bl) {
	out << "Box Line Interaction (" << (bl->type == BOXLINE_ROW ? "row" : "col") << ")" << std::endl;
	out << "\tIntersection:";
	for(int i=0; i < bl->subset.size(); i++) {
		out << std::endl << "\t\t" << bl->subset[i];
	}
	out << std::endl << "\tTargets:";
	for(int i=0; i < bl->targets.size(); i++) {
		out << std::endl << "\t\t" << bl->targets[i];
	}
	return out;
}

std::vector<BoxLine> find_boxlines(Bits* bits) {
	std::vector<BoxLine> ret;
	for(int i = 0; i < 9; i++) {
		// for box i
		std::vector<Spot> boxspots = spots_of_box(i);
		int boxrow = boxspots[0].row;
		int boxcol = boxspots[0].col;
		
		// check rows
		for(int j=0; j < 3; j++) {
			// row is absolute row
			int row = boxrow + j;
			std::vector<Spot> others;
			std::vector<Spot> spots;
			std::vector<Spot> empty_spots;
			std::vector<Spot> complements;
			
			// classify all necessary spots
			for(int k=0; k < 9; k++) {
				Spot boxspot = spot_of_boxid(i, k);
				Spot rowspot = Spot(row, k);
				if(boxid_of_spot(rowspot) != i) {
					others.push_back(rowspot);
				}
				if(boxspot.row == row) {
					spots.push_back(boxspot);
					if(!bits->has_value(boxspot)) {
						empty_spots.push_back(boxspot);
					}
				} else {
					if(!bits->has_value(boxspot)) {
						complements.push_back(boxspot);
					}
				}
			}
			
			int emptybits = bits->bit_of_spots(empty_spots);
			int otherbits = bits->bit_of_spots(others);
			int compbits = bits->bit_of_spots(complements);
			
			int boxedbits = bit_subtract_int(emptybits, otherbits);
			if(boxedbits == 0) { continue; }
			// we have aligned
			int targetbits = boxedbits & compbits;
			if(targetbits == 0) { continue; }
			// and will remove candidates
			
			std::vector<int> vals = vals_of_int(targetbits);
			
			// identify targets
			std::vector<Target> targets;
			for(int k=0; k < complements.size(); k++) {
				Spot comp = complements[k];
				std::vector<int> candidates;
				for(int m=0; m < vals.size(); m++) {
					int mbit = vals[m];
					if(bits->has_candidate(comp, mbit)) {
						candidates.push_back(mbit);
					}
				}
				if(candidates.size() > 0) {
					targets.push_back(Target(comp, candidates, 0));
				}
			}
			
			// identify subset
			std::vector<Spot> subset;
			for(int k=0; k < empty_spots.size(); k++) {
				bool part = false;
				for(int m=0; m < vals.size(); m++) {
					int mbit = vals[m];
					if(bits->has_candidate(empty_spots[k], mbit)) {
						part = true;
					}
				}
				if(part) {
					subset.push_back(empty_spots[k]);
				}
			}
			
			// identify keys
			// OLD NOTE: mostly copied from pointing.cpp
			
			KeyRank keyrank;
			for(int k=0; k < others.size(); k++) {
				Spot spot = others[k];
				for(int m=0; m < vals.size(); m++) {
					int mbit = vals[m];
					keyrank.add_possible_covers(bits, spot, mbit);
				}
			}
			ret.push_back(BoxLine(targets, keyrank.keys, subset, vals, BOXLINE_ROW));
		}
		
		// check cols
		for(int j=0; j < 3; j++) {
			// col is absolute col
			int col = boxcol + j;
			std::vector<Spot> others;
			std::vector<Spot> spots;
			std::vector<Spot> empty_spots;
			std::vector<Spot> complements;
			
			// classify all necessary spots
			for(int k=0; k < 9; k++) {
				Spot boxspot = spot_of_boxid(i, k);
				Spot rowspot = Spot(k, col);
				if(boxid_of_spot(rowspot) != i) {
					others.push_back(rowspot);
				}
				if(boxspot.col == col) {
					spots.push_back(boxspot);
					if(!bits->has_value(boxspot)) {
						empty_spots.push_back(boxspot);
					}
				} else {
					if(!bits->has_value(boxspot)) {
						complements.push_back(boxspot);
					}
				}
			}
			
			int emptybits = bits->bit_of_spots(empty_spots);
			int otherbits = bits->bit_of_spots(others);
			int compbits = bits->bit_of_spots(complements);
			
			int boxedbits = bit_subtract_int(emptybits, otherbits);
			if(boxedbits == 0) { continue; }
			// we have aligned
			int targetbits = boxedbits & compbits;
			if(targetbits == 0) { continue; }
			// and will remove candidates
			
			std::vector<int> vals = vals_of_int(targetbits);
			
			// identify targets
			std::vector<Target> targets;
			for(int k=0; k < complements.size(); k++) {
				Spot comp = complements[k];
				std::vector<int> candidates;
				for(int m=0; m < vals.size(); m++) {
					int mbit = vals[m];
					if(bits->has_candidate(comp, mbit)) {
						candidates.push_back(mbit);
					}
				}
				if(candidates.size() > 0) {
					targets.push_back(Target(comp, candidates, 0));
				}
			}
			
			// identify subset
			std::vector<Spot> subset;
			for(int k=0; k < empty_spots.size(); k++) {
				bool part = false;
				for(int m=0; m < vals.size(); m++) {
					int mbit = vals[m];
					if(bits->has_candidate(empty_spots[k], mbit)) {
						part = true;
					}
				}
				if(part) {
					subset.push_back(empty_spots[k]);
				}
			}
			
			// identify keys
			// OLD NOTE: mostly copied from pointing.cpp
			
			KeyRank keyrank;
			for(int k=0; k < others.size(); k++) {
				Spot spot = others[k];
				for(int m=0; m < vals.size(); m++) {
					int mbit = vals[m];
					keyrank.add_possible_covers(bits, spot, mbit);
				}
			}
			ret.push_back(BoxLine(targets, keyrank.keys, subset, vals, BOXLINE_COL));
		}
	}
	return ret;
}

