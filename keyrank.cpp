
#include "keyrank.h"

KeyRank::KeyRank() {
}

void KeyRank::add_key(Spot p, int mult) {
	int keyidx = idx_of_keys(keys, p);
	if(keyidx == -1) {
		keys.push_back(Key(p, mult));
	} else {
		if(keys[keyidx].multiplicity > mult) {
			keys[keyidx].multiplicity = mult;
		}
	}
}

void KeyRank::add_possible_covers(Bits* bits, Spot p, int val) {
	if(bits->has_candidate(p, val)) { return; }
	std::vector<Spot> covers = bits->covers_of_spot(p, val);
	if(bits->has_value(p)) {
		covers.push_back(p);
	}
	int mult = covers.size();
	for(int k=0; k < mult; k++) {
		if(!bits->is_possible(covers[k])) {
			return;
		}
	}
	for(int k=0; k < mult; k++) {
		add_key(covers[k], mult);
	}
}
