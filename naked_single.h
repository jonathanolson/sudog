#ifndef NAKED_SINGLE_H
#define NAKED_SINGLE_H

#include "pattern.h"
#include "bits.h"
#include "target.h"
#include "key.h"

class NakedSingle : public Pattern {
	public:
	
	NakedSingle(Target targ, std::vector<Key> keyss);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	Target target;
	std::vector<Key> keys;
};

std::vector<NakedSingle> find_naked_singles(Bits* bits);

std::ostream& operator<<(std::ostream& out, NakedSingle* ns);

#endif
