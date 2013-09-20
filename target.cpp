
#include "target.h"

Target::Target() {
	spot = Spot();
	to_value = 0;
}

Target::Target(Spot p, int toval) {
	spot = p;
	to_value = toval;
}

Target::Target(Spot p, std::vector<int> cands, int toval) {
	spot = p;
	candidates = cands;
	to_value = toval;
}

void Target::apply(BitMatrix* bits) {
	if(to_value != 0) {
		bits->set_value(spot, to_value);
	} else {
		for(int i=0; i < candidates.size(); i++) {
			bits->remove_candidate(spot, candidates[i]);
		}
	}
}

std::ostream& operator<<(std::ostream& out, Target& target) {
	/*
	out << "[" << target.spot << " [";
	if(target.candidates.size() != 0) {
		out << target.candidates[0];
	}
	for(int i=1; i < target.candidates.size(); i++) {
		out << " " << target.candidates[i];
	}
	out << "] -> " << target.to_value << "]";
	return out;
	*/
	
	out << target.spot;
	if(target.candidates.size() != 0) {
		out << " !=";
		for(int i=0; i < target.candidates.size(); i++) {
			out << target.candidates[i];
		}
	}
	if(target.to_value != 0) {
		out << " -> " << target.to_value;
	}
	return out;
}
