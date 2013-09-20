#ifndef SIMPLE_COLORING_H
#define SIMPLE_COLORING_H

#include <vector>
#include "pattern.h"
#include "bits.h"
#include "digit_chain.h"

class SimpleColoring : public Pattern {
	public:
	
	SimpleColoring(std::vector<Target> _targets, std::vector<Key> _keys, DigitChain _chain, int _val);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	std::vector<Target> targets;
	std::vector<Key> keys;
	DigitChain chain;
	int val;
};

std::vector<SimpleColoring> find_simple_colorings(Bits* bits);

std::ostream& operator<<(std::ostream& out, SimpleColoring* sc);

#endif

