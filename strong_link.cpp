
#include "strong_link.h"

StrongLink::StrongLink() {
	a = Spot();
	b = Spot();
	val = 0;
	row = col = box = false;
}

StrongLink::StrongLink(Spot _a, Spot _b, int _val, bool _row, bool _col, bool _box) {
	a = _a;
	b = _b;
	val = _val;
	row = _row;
	col = _col;
	box = _box;
}

bool strong_link_equal(StrongLink& la, StrongLink& lb) {
	// note: does not inspect row, col, box
	if(la.val != lb.val) { return false; }
	if(spot_equal(la.a, lb.a)) {
		return spot_equal(la.b, lb.b);
	} else {
		return spot_equal(la.a, lb.b) && spot_equal(la.b, lb.a);
	}
}

int idx_of_strong_link(std::vector<StrongLink>& links, StrongLink& link) {
	for(int i=0; i < links.size(); i++) {
		if(strong_link_equal(links[i], link)) {
			return i;
		}
	}
	return -1;
}

std::vector<StrongLink> strong_links_of_val(Bits* bits, int val) {
	std::vector<StrongLink> ret;
	
	// rows
	for(int i=0; i < 9; i++) {
		std::vector<Spot> spots;
		for(int j=0; j < 9; j++) {
			Spot p = Spot(i, j);
			if(bits->has_candidate(p, val)) {
				spots.push_back(p);
			}
		}
		if(spots.size() != 2) { continue; }
		ret.push_back(StrongLink(spots[0], spots[1], val, true, false, false));
	}
	
	// cols
	for(int i=0; i < 9; i++) {
		std::vector<Spot> spots;
		for(int j=0; j < 9; j++) {
			Spot p = Spot(j, i);
			if(bits->has_candidate(p, val)) {
				spots.push_back(p);
			}
		}
		if(spots.size() != 2) { continue; }
		ret.push_back(StrongLink(spots[0], spots[1], val, false, true, false));
	}
	
	// boxes
	for(int i=0; i < 9; i++) {
		std::vector<Spot> spots;
		for(int j=0; j < 9; j++) {
			Spot p = spot_of_boxid(i, j);
			if(bits->has_candidate(p, val)) {
				spots.push_back(p);
			}
		}
		if(spots.size() != 2) { continue; }
		StrongLink link = StrongLink(spots[0], spots[1], val, false, false, true);
		int idx = idx_of_strong_link(ret, link);
		if(idx == -1) {
			ret.push_back(link);
		} else {
			ret[idx].box = true;
		}
	}
	return ret;
}

std::vector< std::vector<StrongLink> > find_strong_links(Bits* bits) {
	std::vector< std::vector<StrongLink> > ret;
	for(int i=1; i <= 9; i++) {
		ret.push_back(strong_links_of_val(bits, i));
	}
	return ret;
}

void strong_link_display(StrongLink& link, Output* out, float r, float g, float b) {
	out->link_candidate(link.a, link.val, link.b, link.val, 4.0, r, g, b);
}


