#ifndef NH_SUBSET_H
#define NH_SUBSET_H

#include <vector>
#include "pattern.h"
#include "bits.h"

extern const int NH_ROW;
extern const int NH_COL;
extern const int NH_BOX;

class NHSubset : public Pattern {
	public:
	
	NHSubset(std::vector<Target> targetss, std::vector<Key> keyss, std::vector<int> valss, std::vector<Spot> subsetx, int t, int empties);
	
	void apply(BitMatrix* bits);
	void full_display(Output* out);
	void display_keys(Output* out);
	void describe(std::ostream& out);
	
	std::vector<Target> target_list();
	std::vector<Key> key_list();
	
	Pattern* clone();
	
	std::vector<Target> targets;
	std::vector<Key> keys;
	std::vector<int> vals;
	std::vector<Spot> subset;
	int type;
	int empty;
};

std::ostream& operator<<(std::ostream& out, NHSubset* nh);

std::vector<NHSubset> find_nh_subsets(Bits* bits);

#endif
