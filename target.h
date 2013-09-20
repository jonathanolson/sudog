#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include "coords.h"
#include "bitmatrix.h"

class Target {
	public:
	
	Spot spot;
	std::vector<int> candidates;
	int to_value;
	
	Target();
	Target(Spot p, int toval);
	Target(Spot p, std::vector<int> cands, int toval);
	
	void apply(BitMatrix* bits);
};

std::ostream& operator<<(std::ostream& out, Target& target);



#endif
