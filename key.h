#ifndef KEY_H
#define KEY_H

#include <vector>
#include "coords.h"

#define SHOW_KEYS

class Key {
	public:
	
	Spot spot;
	int multiplicity;
	
	Key();
	Key(Spot p, int mult);
};

std::ostream& operator<<(std::ostream& out, Key& key);

int idx_of_keys(std::vector<Key>& keys, Spot p);

#endif
