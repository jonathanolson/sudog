#ifndef YWING_H
#define YWING_H

#include <vector>
#include "pattern.h"
#include "bits.h"

class YWing : public Pattern {
	public:
	
	YWing(std::vector<Target> _targets, std::vector<Key> _keys, Spot _hinge, std::vector<Spot> wings);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	std::vector<Target> targets;
	std::vector<Key> keys;
	Spot hinge;
	std::vector<Spot> wings;
};

std::vector<YWing> find_ywings(Bits* bits);

std::ostream& operator<<(std::ostream& out, YWing* nf);

#endif
