#ifndef BITMATRIX_H
#define BITMATRIX_H

#include "coords.h"

class BitMatrix {
	public:
	
	virtual void init() = 0;
	
	virtual void set_value(Spot p, int val) = 0;
	virtual bool has_value(Spot p) = 0;
	virtual int get_value(Spot p) = 0;
	
	virtual void remove_candidate(Spot p, int val) = 0;
	virtual bool has_candidate(Spot p, int val) = 0;
	virtual int get_candidate_int(Spot p, int val) = 0;
	
	virtual int first_bit(Spot p) = 0;
	
};


#endif
