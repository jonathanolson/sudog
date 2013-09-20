#ifndef NFISH_H
#define NFISH_H

#include <vector>
#include "pattern.h"
#include "bits.h"

extern const int NFISH_ROWS;
extern const int NFISH_COLS;

class NFish : public Pattern {
	public:
	
	NFish(std::vector<Target> _targets, std::vector<Key> _keys, std::vector<Spot> _subset, std::vector<int> _units, int _val, int _type);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	std::vector<Target> targets;
	std::vector<Key> keys;
	std::vector<Spot> subset;
	std::vector<int> units;
	int val;
	int type;
};

std::vector<NFish> find_nfishes(Bits* bits);

std::ostream& operator<<(std::ostream& out, NFish* nf);

#endif
