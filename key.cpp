
#include "key.h"

Key::Key() {
	spot = Spot();
	multiplicity = 1;
}

Key::Key(Spot p, int mult) {
	spot = p;
	multiplicity = mult;
}

std::ostream& operator<<(std::ostream& out, Key& key) {
	out << "{ " << key.spot << " m" << key.multiplicity << " }";
	return out;
}

int idx_of_keys(std::vector<Key>& keys, Spot p) {
	for(int i=0; i < keys.size(); i++) {
		if(spot_equal(p, keys[i].spot)) {
			return i;
		}
	}
	return -1;
}
