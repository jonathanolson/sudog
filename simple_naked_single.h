#ifndef SIMPLE_NAKED_SINGLE_H
#define SIMPLE_NAKED_SINGLE_H

#include "pattern.h"
#include "bits.h"
#include "target.h"
#include "key.h"

extern const int SNS_ROW;
extern const int SNS_COL;
extern const int SNS_BOX;

class SimpleNakedSingle : public Pattern {
	public:
	
	SimpleNakedSingle(Target targ, int typ, std::vector<Key> keyss);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	Target target;
	int type;
	std::vector<Key> keys;
};

std::ostream& operator<<(std::ostream& out, SimpleNakedSingle* sns);

std::vector<SimpleNakedSingle> find_simple_naked_singles(Bits* bits);

#endif
