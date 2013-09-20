
#include "naked_single.h"

NakedSingle::NakedSingle(Target targ, std::vector<Key> keyss) {
	target = targ;
	keys = keyss;
}

void NakedSingle::apply(BitMatrix* bits) {
	target.apply(bits);
}

void NakedSingle::full_display(Output* out) {
	// highlight cell
	out->highlight_cell(target.spot, 0.8, 0.8, 1.0);
}

void NakedSingle::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> NakedSingle::target_list() {
	// package in vector since we only have one
	std::vector<Target> ret;
	ret.push_back(target);
	return ret;
}

std::vector<Key> NakedSingle::key_list() {
	return keys;
}

Pattern* NakedSingle::clone() {
	return (new NakedSingle(*this));
}

void NakedSingle::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, NakedSingle* ns) {
	out << "Naked Single" << std::endl;
	out << "\tTarget: " << ns->target;
	return out;
}

std::vector<NakedSingle> find_naked_singles(Bits* bits) {
	std::vector<NakedSingle> ret;
	for(int i=0; i < 81; i++) {
		// testing spot p
		Spot p = spot_of_idx(i);
		
		// make sure it is not already filled
		if(bits->has_value(p)) { continue; }
		
		// check how many candidates it has
		int bitcount = 0;
		for(int j=1; j <= 9; j++) {
			bitcount += bits->get_candidate_int(p, j);
		}
		if(bitcount != 1) { continue; }
		
		// we have a NS!
		
		// set val to value of remaining candidate
		int val = bits->first_bit(p);
		
		// initialize our one target
		Target targ = Target(p, val);
		
		// find keys
		std::vector<Key> keylist;
		for(int k=1; k <= 9; k++) {
			// for each non-candidate value, add/modify covers to keys, with minimum multiplicity
			if(k == val) { continue; }
			std::vector<Spot> covers = bits->covers_of_spot(p, k);
			int mult = covers.size();
			
			// make sure it is possible to open up this candidate
			bool possible = true;
			for(int m=0; m < covers.size(); m++) {
				possible = possible && bits->is_possible(covers[m]);
			}
			if(!possible) { continue; }
			
			// add keys to key list
			for(int m=0; m < covers.size(); m++) {
				keylist.push_back(Key(covers[m], mult));
			}
		}
		ret.push_back(NakedSingle(targ, keylist));
	}
	return ret;
}

