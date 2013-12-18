
#include <iostream>
#include "ywing.h"
#include "keyrank.h"

YWing::YWing(std::vector<Target> _targets, std::vector<Key> _keys, Spot _hinge, std::vector<Spot> _wings) {
	targets = _targets;
	keys = _keys;
	hinge = _hinge;
	wings = _wings;
}

void YWing::apply(BitMatrix* bits) {
	for(int i=0; i < targets.size(); i++) {
		targets[i].apply(bits);
	}
}

void YWing::full_display(Output* out) {
	for(int i=0; i < targets.size(); i++) {
		Target target = targets[i];
		for(int j=0; j < target.candidates.size(); j++) {
			out->highlight_candidate(target.spot, target.candidates[j], 0.4, 0.7, 1.0);
		}
	}
	for(int i=0; i < wings.size(); i++) {
		out->highlight_cell(wings[i], 0.9, 0.9, 0.9);
	}
	out->highlight_cell(hinge, 0.8, 0.8, 0.8);
}

void YWing::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> YWing::target_list() {
	return targets;
}

std::vector<Key> YWing::key_list() {
	return keys;
}

Pattern* YWing::clone() {
	return (new YWing(*this));
}

void YWing::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, YWing* ywing) {
	out << "Y-wing" << std::endl;
	out << "\tHinge:" << std::endl << "\t\t" << ywing->hinge;
	out << "\tWings:";
	for(int i=0; i < ywing->wings.size(); i++) {
		out << std::endl << "\t\t" << ywing->wings[i];
	}
	out << std::endl << "\tTargets:";
	for(int i=0; i < ywing->targets.size(); i++) {
		out << std::endl << "\t\t" << ywing->targets[i];
	}
	return out;
}

void ywing_process_hinge(Bits* bits, Spot hinge, std::vector<Spot>* double_spots, std::vector<YWing>* ret) {
	int hinge_bits = bits->bit_of_spot(hinge);

	for(int i=0; i < double_spots->size(); i++) {
		Spot wing1 = (*double_spots)[i];
		int wing1_bits = bits->bit_of_spot(wing1);

		// only consider wings that aren't the hinge, but are visible to the hinge, and have exactly 1 candidate in common with the hinge
		if(spot_equal(wing1,hinge) || !spot_visible(wing1,hinge) || count_bits(hinge_bits & wing1_bits) != 1 ) {
			continue;
		}

		for(int j=i+1; j < double_spots->size(); j++) {
			Spot wing2 = (*double_spots)[j];
			int wing2_bits = bits->bit_of_spot(wing2);

			// only consider wings that aren't the hinge, but are visible to the hinge, and have exactly 1 candidate in common with the hinge and the other wing
			if(spot_equal(wing2,hinge) || !spot_visible(wing2,hinge) || count_bits(hinge_bits & wing2_bits) != 1 || count_bits(wing1_bits & wing2_bits) != 1 || count_bits(hinge_bits & wing1_bits & wing2_bits) != 0) {
				continue;
			}

			// now we have two unique wings that can see the hinge, and each has only one candidate shared between each other

			// the candidate we can exclude is the shared candidate between the two wings
			int shared_candidate = first_bits(wing1_bits & wing2_bits);

			// determine all spots that are visible to both wing1 and wing2 (same row, column, or box)
			std::vector<Spot> shared_spots;
			for(int ia=0; ia < 9; ia++) {
				for(int ib=0; ib < 9; ib++) {
					Spot p = Spot(ia,ib);

					// exclude the hinge and wing spots
					if(spot_equal(p,hinge) || spot_equal(p,wing1) || spot_equal(p,wing2)) { continue; }

					if((p.row == wing1.row || p.col == wing1.col || boxid_of_spot(p) == boxid_of_spot(wing1)) && (p.row == wing2.row || p.col == wing2.col || boxid_of_spot(p) == boxid_of_spot(wing2))) {
						shared_spots.push_back(p);
					}
				}
			}

			// NOTE: potentially more-efficient version commented out, didn't figure out the best way to make this compile.
			// if(wing1.row == wing2.row) {
			// 	shared_spots = spots_union(shared_spots,spots_of_row(wing1.row));
			// }
			// if(wing1.col == wing2.col) {
			// 	shared_spots = spots_union(shared_spots,spots_of_col(wing1.col));
			// }
			// if(boxid_of_spot(wing1) == boxid_of_spot(wing2)) {
			// 	shared_spots = spots_union(shared_spots,spots_of_box(boxid_of_spot(wing1)));
			// }
			// remove any excluded spots (hinge,wings) from our list
			// std::vector<Spot> excluded_spots;
			// excluded_spots.push_back(hinge);
			// excluded_spots.push_back(wing1);
			// excluded_spots.push_back(wing2);
			// shared_spots = spots_subtract(shared_spots,excluded_spots);

			// candidate array (if necessary)
			std::vector<int> candidates;
			candidates.push_back(shared_candidate);

			std::vector<Target> targets;
			for(int k=0; k < shared_spots.size(); k++) {
				Spot target_spot = shared_spots[k];
				if(bits->has_candidate(target_spot,shared_candidate) && bits->count_candidates(target_spot) > 1) {
					targets.push_back(Target(target_spot, candidates, 0));
				}
			}
			if(targets.size() > 0) {
				KeyRank keyrank;
				std::vector<Spot> wings;
				wings.push_back(wing1);
				wings.push_back(wing2);
				ret->push_back(YWing(targets, keyrank.keys, hinge, wings));
			}
		}
	}
}

std::vector<YWing> find_ywings(Bits* bits) {
	std::vector<YWing> ret;

	std::vector<Spot> double_spots; // spots with only two candidates
	for(int i=0; i < 9; i++) {
		for(int j=0; j < 9; j++) {
			Spot p = Spot(i, j);
			if(bits->count_candidates(p) == 2) {
				double_spots.push_back(p);
			}
		}
	}

	for(int i=0; i < double_spots.size(); i++){
		ywing_process_hinge(bits, double_spots[i], &double_spots, &ret);
	}
	return ret;
}

