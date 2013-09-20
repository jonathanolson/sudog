#ifndef BOXLINE_H
#define BOXLINE_H

#include <vector>
#include "pattern.h"
#include "bits.h"

extern const int BOXLINE_ROW;
extern const int BOXLINE_COL;

class BoxLine : public Pattern {
	public:
	
	BoxLine(std::vector<Target> _targets, std::vector<Key> _keys, std::vector<Spot> _subset, std::vector<int> _vals, int _type);
	
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
	std::vector<int> vals;
	int type;
};

std::vector<BoxLine> find_boxlines(Bits* bits);

std::ostream& operator<<(std::ostream& out, BoxLine* bl);

#endif
