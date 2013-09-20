#ifndef POINTING_H
#define POINTING_H

#include <vector>
#include "pattern.h"
#include "bits.h"

extern const int POINT_ROW;
extern const int POINT_COL;

class Pointing : public Pattern {
	public:
	
	Pointing(std::vector<Target> _targets, std::vector<Key> _keys, std::vector<Spot> _pointers, std::vector<int> _vals, int _type);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	std::vector<Target> targets;
	std::vector<Key> keys;
	std::vector<Spot> pointers;
	std::vector<int> vals;
	int type;
};

std::vector<Pointing> find_pointings(Bits* bits);

std::ostream& operator<<(std::ostream& out, Pointing* p);

#endif
