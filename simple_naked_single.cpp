
#include "simple_naked_single.h"

const int SNS_ROW = 1;
const int SNS_COL = 2;
const int SNS_BOX = 3;

SimpleNakedSingle::SimpleNakedSingle(Target targ, int typ, std::vector<Key> keyss) {
	target = targ;
	type = typ;
	keys = keyss;
}

void SimpleNakedSingle::apply(BitMatrix* bits) {
	target.apply(bits);
}

void SimpleNakedSingle::full_display(Output* out) {
	// highlight rule
	if(type == SNS_ROW) {
		out->highlight_row(target.spot.row, 0.9, 0.9, 0.9);
		//out->highlight_cell(target.spot, 0.8, 0.8, 1.0);
	} else if(type == SNS_COL) {
		out->highlight_col(target.spot.col, 0.9, 0.9, 0.9);
		//out->highlight_cell(target.spot, 0.8, 0.8, 1.0);
	} else if(type == SNS_BOX) {
		out->highlight_box(boxid_of_spot(target.spot), 0.9, 0.9, 0.9);
		//out->highlight_cell(target.spot, 0.8, 0.8, 1.0);
	}
	
	// highlight cell
	out->highlight_cell(target.spot, 0.8, 0.8, 1.0);
}

void SimpleNakedSingle::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> SimpleNakedSingle::target_list() {
	// package in vector since we only have one
	std::vector<Target> ret;
	ret.push_back(target);
	return ret;
}

std::vector<Key> SimpleNakedSingle::key_list() {
	return keys;
}

Pattern* SimpleNakedSingle::clone() {
	return (new SimpleNakedSingle(*this));
}

void SimpleNakedSingle::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, SimpleNakedSingle* sns) {
	out << "Simple Naked Single (" << (sns->type == SNS_ROW ? "row" : (sns->type == SNS_COL ? "col" : "box")) << ")" << std::endl;
	out << "\tTarget: " << sns->target;
	return out;
}

std::vector<SimpleNakedSingle> find_simple_naked_singles(Bits* bits) {
	std::vector<SimpleNakedSingle> ret;
	for(int i=0; i < 9; i++) {
		int rowcount = 0;
		int colcount = 0;
		int boxcount = 0;
		std::vector<Spot> rowlist, collist, boxlist;
		Spot rowspot, colspot, boxspot;
		
		// rows
		for(int j=0; j < 9; j++) {
			Spot p = Spot(i,j);
			if(bits->has_value(p)) {
				rowlist.push_back(p);
			} else {
				rowcount++;
				rowspot = p;
			}
		}
		if(rowcount == 1) {
			// we have a SNS at rowspot
			std::vector<Key> keys;
			for(int i=0; i < rowlist.size(); i++) {
				if(! bits->is_possible(rowlist[i])) { continue; }
				keys.push_back(Key(rowlist[i], 1));
			}
			ret.push_back(SimpleNakedSingle(Target(rowspot, bits->first_bit(rowspot)), SNS_ROW, keys));
		}
		
		// cols
		for(int j=0; j < 9; j++) {
			Spot p = Spot(j,i);
			if(bits->has_value(p)) {
				collist.push_back(p);
			} else {
				colcount++;
				colspot = p;
			}
		}
		if(colcount == 1) {
			// we have a SNS at colspot
			std::vector<Key> keys;
			for(int i=0; i < collist.size(); i++) {
				if(! bits->is_possible(collist[i])) { continue; }
				keys.push_back(Key(collist[i], 1));
			}
			ret.push_back(SimpleNakedSingle(Target(colspot, bits->first_bit(colspot)), SNS_COL, keys));
		}
		
		// boxes
		for(int j=0; j < 9; j++) {
			Spot p = spot_of_boxid(i, j);
			if(bits->has_value(p)) {
				boxlist.push_back(p);
			} else {
				boxcount++;
				boxspot = p;
			}
		}
		if(boxcount == 1) {
			// we have a SNS at boxspot
			std::vector<Key> keys;
			for(int i=0; i < boxlist.size(); i++) {
				if(! bits->is_possible(boxlist[i])) { continue; }
				keys.push_back(Key(boxlist[i], 1));
			}
			ret.push_back(SimpleNakedSingle(Target(boxspot, bits->first_bit(boxspot)), SNS_BOX, keys));
		}
	}
	return ret;
}
