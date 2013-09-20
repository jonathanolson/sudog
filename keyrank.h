#ifndef KEYRANK_H
#define KEYRANK_H

#include "bits.h"
#include "key.h"

class KeyRank {
	public:
	KeyRank();
	
	void add_key(Spot p, int mult);
	void add_possible_covers(Bits* bits, Spot p, int val);
	
	std::vector<Key> keys;
};

#endif
