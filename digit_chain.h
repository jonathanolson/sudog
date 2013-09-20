#ifndef DIGIT_CHAIN_H
#define DIGIT_CHAIN_H

#include "bits.h"
#include "coords.h"
#include "strong_link.h"

extern const int DC_BLACK;
extern const int DC_WHITE;

class DigitVertex {
	public:
	
	DigitVertex();
	DigitVertex(Spot _spot, int _color);
	
	void connect(StrongLink& link, int peer);
	
	Spot spot;
	int color;
	std::vector<int> peers;
	std::vector<StrongLink> links;
};

class DigitChain {
	public:
	
	DigitChain();
	DigitChain(int _val);
	DigitChain(int _val, StrongLink& link);
	
	bool has_spot(Spot p);
	void add_strong_link(StrongLink& link);
	void scan_false_color();
	std::vector<Spot> chain_spots();
	std::vector<Spot> both_cover(Bits* bits);
	
	
	int val;
	bool black;
	bool white;
	std::vector<DigitVertex> verts;
};

class DigitChains {
	public:
	
	DigitChains();
	DigitChains(int _val);
	
	void add_strong_link(StrongLink& link);
	void add_links(Bits* bits);
	
	int val;
	std::vector<DigitChain> chains;
};


int dc_other_color(int color);

int idx_of_digit_vertex(std::vector<DigitVertex>& verts, Spot p);
DigitChain bridge_digit_chains(DigitChain& dca, DigitChain& dcb, StrongLink& link);


int idx_of_digit_chain(std::vector<DigitChain>& chains, Spot p);


#endif
