
#include "simple_coloring.h"

SimpleColoring::SimpleColoring(std::vector<Target> _targets, std::vector<Key> _keys, DigitChain _chain, int _val) {
	targets = _targets;
	keys = _keys;
	chain = _chain;
	val = _val;
}

void SimpleColoring::apply(BitMatrix* bits) {
	for(int i=0; i < targets.size(); i++) {
		targets[i].apply(bits);
	}
}

void SimpleColoring::full_display(Output* out) {
	for(int i=0; i < targets.size(); i++) {
		Target target = targets[i];
		for(int j=0; j < target.candidates.size(); j++) {
			out->highlight_candidate(target.spot, target.candidates[j], 0.5, 0.5, 0.5);
		}
		if(target.to_value != 0) {
			out->highlight_candidate(target.spot, target.to_value, 1.0, 1.0, 1.0);
		}
	}
	for(int i=0; i < chain.verts.size(); i++) {
		DigitVertex v = chain.verts[i];
		if(v.color == DC_BLACK) {
			out->highlight_cell(v.spot, 1.0, 0.7, 0.7);
		} else if(v.color == DC_WHITE) {
			out->highlight_cell(v.spot, 0.7, 1.0, 0.7);
		}
	}
}

void SimpleColoring::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> SimpleColoring::target_list() {
	return targets;
}

std::vector<Key> SimpleColoring::key_list() {
	return keys;
}

Pattern* SimpleColoring::clone() {
	return (new SimpleColoring(*this));
}

void SimpleColoring::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, SimpleColoring* sc) {
	out << "Simple Coloring";
	return out;
}

std::vector<SimpleColoring> find_simple_colorings(Bits* bits) {
//std::cout << "SC" << std::endl;
	std::vector<SimpleColoring> ret;
	for(int digit=1; digit <= 9; digit++) {
//std::cout << digit << std::endl;
		std::vector<int> vals;
		vals.push_back(digit);
		DigitChains dchains(digit);
//std::cout << "DC initted" << std::endl;
		dchains.add_links(bits);
//std::cout << "DC added links" << std::endl;
//std::cout << digit << ": " << dchains.chains.size() << " chains" << std::endl;
		for(int chidx=0; chidx < dchains.chains.size(); chidx++) {
			std::vector<Target> targets;
			dchains.chains[chidx].scan_false_color();
			if(!dchains.chains[chidx].black) {
//std::cout << "false black" << std::endl;
				for(int vidx=0; vidx < dchains.chains[chidx].verts.size(); vidx++) {
					DigitVertex v = dchains.chains[chidx].verts[vidx];
					if(v.color == DC_BLACK) {
						targets.push_back(Target(v.spot, vals, 0));
					} else {
						targets.push_back(Target(v.spot, digit));
					}
				}
			}
			if(!dchains.chains[chidx].white) {
//std::cout << "false white" << std::endl;
				for(int vidx=0; vidx < dchains.chains[chidx].verts.size(); vidx++) {
					DigitVertex v = dchains.chains[chidx].verts[vidx];
					if(v.color == DC_WHITE) {
						targets.push_back(Target(v.spot, vals, 0));
					} else {
						targets.push_back(Target(v.spot, digit));
					}
				}
			}
			std::vector<Spot> boths = dchains.chains[chidx].both_cover(bits);
			for(int i=0; i < boths.size(); i++) {
//std::cout << "found boths, " << boths[i] << " " << digit << std::endl;
				targets.push_back(Target(boths[i], vals, 0));
			}
			if(targets.size() == 0) { continue; }
//std::cout << "\tfound colorings" << std::endl;
			// NOTE NOTE NOTE
			// need keys
			ret.push_back(SimpleColoring(targets, std::vector<Key>(), dchains.chains[chidx], digit));
		}
	}
	return ret;
}

