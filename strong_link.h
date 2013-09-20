#ifndef STRONG_LINK_H
#define STRONG_LINK_H

#include <vector>
#include "bits.h"
#include "coords.h"
#include "output.h"



class StrongLink {
	public:
	StrongLink();
	StrongLink(Spot _a, Spot _b, int _val, bool _row, bool _col, bool _box);
	
	
	int val;
	Spot a, b;
	bool row, col, box;
};

// note: does not inspect row, col, box
bool strong_link_equal(StrongLink& la, StrongLink& lb);

int idx_of_strong_link(std::vector<StrongLink>& links, StrongLink& link);
std::vector<StrongLink> strong_links_of_val(Bits* bits, int val);
std::vector< std::vector<StrongLink> > find_strong_links(Bits* bits);

void strong_link_display(StrongLink& link, Output* out, float r, float g, float b);

#endif
