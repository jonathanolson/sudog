
#include "nh_subset.h"
#include "keyrank.h"

const int NH_ROW = 1;
const int NH_COL = 2;
const int NH_BOX = 3;

NHSubset::NHSubset(std::vector<Target> targetss, std::vector<Key> keyss, std::vector<int> valss, std::vector<Spot> subsetx, int t, int empties) {
	targets = targetss;
	keys = keyss;
	vals = valss;
	subset = subsetx;
	type = t;
	empty = empties;
}

void NHSubset::apply(BitMatrix* bits) {
	for(int i=0; i < targets.size(); i++) {
		targets[i].apply(bits);
	}
}

void NHSubset::full_display(Output* out) {
	if(type == NH_ROW) {
		out->highlight_row(targets[0].spot.row, 0.9, 0.9, 0.9);
	} else if(type == NH_COL) {
		out->highlight_col(targets[0].spot.col, 0.9, 0.9, 0.9);
	} else if(type == NH_BOX) {
		out->highlight_box(boxid_of_spot(targets[0].spot), 0.9, 0.9, 0.9);
	}
	for(int i=0; i < targets.size(); i++) {
		Target target = targets[i];
		for(int j=0; j < target.candidates.size(); j++) {
			out->highlight_candidate(target.spot, target.candidates[j], 0.5, 1.0, 0.5);
		}
	}
	for(int i=0; i < subset.size(); i++) {
		out->highlight_cell(subset[i], 0.8, 0.8, 0.8);
	}
}

void NHSubset::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> NHSubset::target_list() {
	return targets;
}

std::vector<Key> NHSubset::key_list() {
	return keys;
}

Pattern* NHSubset::clone() {
	return (new NHSubset(*this));
}

void NHSubset::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, NHSubset* nh) {
	out << "Naked Subset (" << (nh->type == NH_BOX ? "box" : (nh->type == NH_ROW ? "row" : "col")) << ")" << std::endl;
	
	int naked = nh->subset.size();
	int hidden = nh->empty - naked;
	
	if(naked < 5) {
		out << "\"Naked ";
		switch(naked) {
			case 1:
				out << "Single"; break;
			case 2:
				out << "Pair"; break;
			case 3:
				out << "Triple"; break;
			case 4:
				out << "Quad"; break;
		}
		out << "\"";
		if(hidden < 5) {
			out << " / ";
		}
	}
	if(hidden < 5) {
		out << "\"Hidden ";
		switch(hidden) {
			case 1:
				out << "Single"; break;
			case 2:
				out << "Pair"; break;
			case 3:
				out << "Triple"; break;
			case 4:
				out << "Quad"; break;
		}
		out << "\"";
	}
	
	out << std::endl << "\tSubset:";
	for(int i=0; i < nh->subset.size(); i++) {
		out << std::endl << "\t\t" << nh->subset[i];
	}
	out << std::endl << "\tTargets:";
	for(int i=0; i < nh->targets.size(); i++) {
		out << std::endl << "\t\t" << nh->targets[i];
	}
	return out;
}

void nh_process_nsubsets(Bits* bits, std::vector<NHSubset>* ret, std::vector<Spot>* spots, int len, int num, int cur, int bit, std::vector<Spot>* st, int type, std::vector<Spot>* blockers) {
	if(num == 0) {
		// recursion base case, st holds our subset to test
		if(count_bits_int(bit) == st->size()) {
			// detected subset, now does it remove candidates?
			
			// complement will hold all empty non-subset spots
			std::vector<Spot> complement;
			
			// complementbits will contain candidate bits for all complements
			int complementbits = 0;
			for(int i=0; i < spots->size(); i++) {
				bool test = true;
				for(int j=0; j < st->size(); j++) {
					if(spot_equal((*st)[j], (*spots)[i])) { test = false; }
				}
				if(test) {
					complement.push_back((*spots)[i]);
					complementbits |= bits->bit_of_spot((*spots)[i]);
				}
			}
			
			if(bit_subtract_int(complementbits, bit) == complementbits) { return; }
			// we now know there are candidates to remove
			
			// naked spots: subset
			// hidden spots: complement
			std::vector<Spot> subset = *st;
			
			int nakedbits = bits->bit_of_spots(subset);
			int compbits = bits->bit_of_spots(complement);
			int hiddenbits = bit_subtract_int(compbits, nakedbits);
			std::vector<int> nakedbitlist = vals_of_int(nakedbits);
			std::vector<int> hiddenbitlist = vals_of_int(hiddenbits);
			std::vector<int> key_bits = vals_of_int(complement_bit(nakedbits));
			
			std::vector<Spot> all_spots = spots_union(*spots, *blockers);
			std::vector<Spot> not_hidden = spots_subtract(all_spots, complement);
			
			
			KeyRank keyrank;
			
			for(int i=0; i < not_hidden.size(); i++) {
				Spot spot = not_hidden[i];
				// NOTE: sloppy fix!!!!!!!!!! (only need to handle naked spots)
				if(bits->has_value(spot)) { continue; }
				for(int j=0; j < key_bits.size(); j++) {
					int hbit = key_bits[j];
					keyrank.add_possible_covers(bits, spot, hbit);
				}
			}
			
			std::vector<Target> targets;
			for(int i=0; i < complement.size(); i++) {
				Spot hiddenspot = complement[i];
				std::vector<int> candidates;
				for(int j=0; j < nakedbitlist.size(); j++) {
					int nbit = nakedbitlist[j];
					if(bits->has_candidate(hiddenspot, nbit)) {
						candidates.push_back(nbit);
					}
				}
				if(candidates.size() > 0) {
					targets.push_back(Target(hiddenspot, candidates, 0));
				}
			}
			
			
			
			ret->push_back(NHSubset(targets, keyrank.keys, nakedbitlist, subset, type, spots->size()));
		}
	} else {
		// add more cells and call recursively
		for(int i=cur;i <= len-num; i++) {
			// only add from [cur, len-num]
			st->push_back((*spots)[i]);
			nh_process_nsubsets(bits, ret, spots, len, num - 1, i+1, bit | bits->bit_of_spot((*spots)[i]), st, type, blockers);
			st->pop_back();
		}
	}
}

void nh_process_subsets(Bits* bits, std::vector<NHSubset>* ret, std::vector<Spot>* spots, int type, std::vector<Spot>* blockers) {
	for(int i=1; i < spots->size(); i++) {
		// check for i-subset patterns in spots
		
		// st used as a stack for current spots
		std::vector<Spot> st;
		nh_process_nsubsets(bits, ret, spots, spots->size(), i, 0, 0, &st, type, blockers);
	}
}

std::vector<NHSubset> find_nh_subsets(Bits* bits) {
	std::vector<NHSubset> ret;
	for(int i=0; i < 9; i++) {
		// rows
		std::vector<Spot> rowpts;
		std::vector<Spot> rowblockers;
		for(int j=0; j < 9; j++) {
			Spot p = Spot(i, j);
			if(bits->has_value(p)) {
				rowblockers.push_back(p);
			} else {
				rowpts.push_back(p);
			}
		}
		if(rowpts.size() > 0) { nh_process_subsets(bits, &ret, &rowpts, NH_ROW, &rowblockers); }
		
		// cols
		std::vector<Spot> colpts;
		std::vector<Spot> colblockers;
		for(int j=0; j < 9; j++) {
			Spot p = Spot(j, i);
			if(bits->has_value(p)) {
				colblockers.push_back(p);
			} else {
				colpts.push_back(p);
			}
		}
		if(colpts.size() > 0) { nh_process_subsets(bits, &ret, &colpts, NH_COL, &colblockers); }
		
		// rows
		std::vector<Spot> boxpts;
		std::vector<Spot> boxblockers;
		for(int j=0; j < 9; j++) {
			Spot p = spot_of_boxid(i, j);
			if(bits->has_value(p)) {
				boxblockers.push_back(p);
			} else {
				boxpts.push_back(p);
			}
		}
		if(boxpts.size() > 0) { nh_process_subsets(bits, &ret, &boxpts, NH_BOX, &boxblockers); }
	}
	return ret;
}

