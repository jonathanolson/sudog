#ifndef HIDDEN_SINGLE_H
#define HIDDEN_SINGLE_H

#include "pattern.h"
#include "bits.h"

extern const int HS_ROW;
extern const int HS_COL;
extern const int HS_BOX;

class HiddenSingle : public Pattern {
	public:
	
	HiddenSingle(Target _target, int _type, std::vector<Key> _keys, std::vector<Spot> _roots, std::vector<int> _root_types, int _val);
	
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
	std::vector<Spot> roots;
	std::vector<int> root_types;
	int val;
};

std::vector<HiddenSingle> find_hidden_singles(Bits* bits);

std::ostream& operator<<(std::ostream& out, HiddenSingle* hs);

#endif
