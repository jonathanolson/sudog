
#include <iostream>
#include "digit_chain.h"

const int DC_BLACK = 0;
const int DC_WHITE = 1;

DigitVertex::DigitVertex() {
	spot = Spot();
	color = 0;
	peers = std::vector<int>();
	links = std::vector<StrongLink>();
}

DigitVertex::DigitVertex(Spot _spot, int _color) {
	spot = _spot;
	color = _color;
	peers = std::vector<int>();
	links = std::vector<StrongLink>();
}

void DigitVertex::connect(StrongLink& link, int peer) {
	peers.push_back(peer);
	links.push_back(link);
}





DigitChain::DigitChain() {
	val = 0;
	black = true;
	white = true;
	verts = std::vector<DigitVertex>();
}

DigitChain::DigitChain(int _val) {
	val = _val;
	black = true;
	white = true;
	verts = std::vector<DigitVertex>();
}

DigitChain::DigitChain(int _val, StrongLink& link) {
	val = _val;
	black = true;
	white = true;
	if(link.val != val) {
		std::cout << "DigitChain(int _val, StrongLink& link) SEVERE WARNING: creating DigitChain with wrong val?" << std::endl;
	}
	verts = std::vector<DigitVertex>();
	DigitVertex vert_a(link.a, DC_BLACK);
	DigitVertex vert_b(link.b, DC_WHITE);
	verts.push_back(vert_a);
	verts.push_back(vert_b);
	verts[0].connect(link, 1);
	verts[1].connect(link, 0);
}

bool DigitChain::has_spot(Spot p) {
	return idx_of_digit_vertex(verts, p) != -1;
}

void DigitChain::add_strong_link(StrongLink& link) {
	int idx_a = idx_of_digit_vertex(verts, link.a);
	int idx_b = idx_of_digit_vertex(verts, link.b);
	if(idx_a == -1 && idx_b == -1) {
		std::cout << "DigitChain::add_strong_link not connected" << std::endl;
		return;
	}
	if(idx_a >= 0 && idx_b >= 0) {
		if(in_ints(verts[idx_a].peers, idx_b)) {
			// already connected
			return;
		}
		verts[idx_a].connect(link, idx_b);
		verts[idx_b].connect(link, idx_a);
		return;
	}
	int idx_bridge;
	Spot new_spot;
	if(idx_a >= 0) {
		idx_bridge = idx_a;
		new_spot = link.b;
	} else {
		idx_bridge = idx_b;
		new_spot = link.a;
	}
	int idx_new = verts.size();
	verts.push_back(DigitVertex(new_spot, dc_other_color(verts[idx_bridge].color)));
	verts[idx_bridge].connect(link, idx_new);
	verts[idx_new].connect(link, idx_bridge);
}

void DigitChain::scan_false_color() {
	//rows
	for(int i=0; i < 9; i++) {
		int count_white = 0;
		int count_black = 0;
		for(int j=0; j < 9; j++) {
			int idx = idx_of_digit_vertex(verts, Spot(i, j));
			if(idx == -1) { continue; }
			if(verts[idx].color == DC_BLACK) {
				count_black++;
			} else if(verts[idx].color == DC_WHITE) {
				count_white++;
			}
		}
		if(count_black > 1) {
			black = false;
		}
		if(count_white > 1) {
			white = false;
		}
	}
	
	//cols
	for(int i=0; i < 9; i++) {
		int count_white = 0;
		int count_black = 0;
		for(int j=0; j < 9; j++) {
			int idx = idx_of_digit_vertex(verts, Spot(j, i));
			if(idx == -1) { continue; }
			if(verts[idx].color == DC_BLACK) {
				count_black++;
			} else if(verts[idx].color == DC_WHITE) {
				count_white++;
			}
		}
		if(count_black > 1) {
			black = false;
		}
		if(count_white > 1) {
			white = false;
		}
	}
	
	//boxes
	for(int i=0; i < 9; i++) {
		int count_white = 0;
		int count_black = 0;
		for(int j=0; j < 9; j++) {
			int idx = idx_of_digit_vertex(verts, spot_of_boxid(i, j));
			if(idx == -1) { continue; }
			if(verts[idx].color == DC_BLACK) {
				count_black++;
			} else if(verts[idx].color == DC_WHITE) {
				count_white++;
			}
		}
		if(count_black > 1) {
			black = false;
		}
		if(count_white > 1) {
			white = false;
		}
	}
}

std::vector<Spot> DigitChain::chain_spots() {
	std::vector<Spot> ret;
	for(int i=0; i < verts.size(); i++) {
		ret.push_back(verts[i].spot);
	}
	return ret;
}

std::vector<Spot> DigitChain::both_cover(Bits* bits) {
	std::vector<Spot> ret;
	for(int i=0; i < 81; i++) {
/*
if(row_of_idx(i) == 7 && col_of_idx(i) == 0 && val == 4) {
std::cout << "x" << std::endl;
}
*/
		Spot p = spot_of_idx(i);
		if(!bits->has_candidate(p, val)) { continue; }
		if(has_spot(p)) { continue; }
		bool touch_white = false;
		bool touch_black = false;
		for(int j=0; j < verts.size(); j++) {
			Spot px = verts[j].spot;
			if( (p.row == px.row) || (p.col == px.col) || (boxid_of_spot(p) == boxid_of_spot(px)) ) {
				if(verts[j].color == DC_BLACK) {
					touch_black = true;
				} else if(verts[j].color == DC_WHITE) {
					touch_white = true;
				}
			}
		}
		if(touch_white && touch_black) {
			ret.push_back(p);
		}
	}
	return ret;
}







DigitChains::DigitChains() {
	val = 0;
	chains = std::vector<DigitChain>();
}

DigitChains::DigitChains(int _val) {
	val = _val;
	chains = std::vector<DigitChain>();
}

void DigitChains::add_strong_link(StrongLink& link) {
//std::cout << "add_strong_link: adding: " << link.a << " " << link.b << std::endl;
	if(link.val != val) { return; }
	int idx_a = idx_of_digit_chain(chains, link.a);
	int idx_b = idx_of_digit_chain(chains, link.b);
	if(idx_a == -1 && idx_b == -1) {
//std::cout << "\t\t\tnew" << std::endl;
//std::cout << "add_strong_link new chain" << std::endl;
		// add new chain
		chains.push_back(DigitChain(link.val, link));
		return;
	}
	if(idx_a >= 0 && idx_b >= 0 && idx_a != idx_b) {
//std::cout << "\t\t\tmerge" << std::endl;
//std::cout << "add_strong_link merge chain, indexes " << idx_a << " " << idx_b << std::endl;
		// merge chains!
		chains.push_back(bridge_digit_chains(chains[idx_a], chains[idx_b], link));
//std::cout << "\tcompleted bridge_digit_chains" << std::endl;
		chains.erase(chains.begin() + (idx_a > idx_b ? idx_a : idx_b));
		chains.erase(chains.begin() + (idx_a > idx_b ? idx_b : idx_a));
		return;
	}
//std::cout << "\t\t\tappend" << std::endl;
	// only one chain matches
//std::cout << "add_strong_link one chain: linking on" << std::endl;
	int idx = (idx_a == -1 ? idx_b : idx_a);
	chains[idx].add_strong_link(link);
}

void DigitChains::add_links(Bits* bits) {
//std::cout << "add_links: val: " << val << std::endl;
	std::vector<StrongLink> links = strong_links_of_val(bits, val);
	for(int i=0; i < links.size(); i++) {
//std::cout << "DCS:add_links doing add_strong_link" << std::endl;
//std::cout << "\t\tadding: " << links[i].a << " " << links[i].b << std::endl;
		add_strong_link(links[i]);
//std::cout << "DCS:add_links completed add_strong_link" << std::endl;
//std::cout << "\t" << chains.size() << std::endl;
	}
}




int dc_other_color(int color) {
	if(color == DC_BLACK) {
		return DC_WHITE;
	} else if(color == DC_WHITE) {
		return DC_BLACK;
	}
	std::cout << "Bad color" << std::endl;
	return -1;
}

int idx_of_digit_vertex(std::vector<DigitVertex>& verts, Spot p) {
	for(int i=0; i < verts.size(); i++) {
//std::cout << "idx_of_digit_vertex, checking " << p << " = " << verts[i].spot << std::endl;
		if(spot_equal(p, verts[i].spot)) {
//std::cout << i << std::endl;
			return i;
		}
	}
//std::cout << -1 << std::endl;
	return -1;
}

DigitChain bridge_digit_chains(DigitChain& dca, DigitChain& dcb, StrongLink& link) {
//std::cout << "bridge_digit_chains" << std::endl;
	DigitChain dcc(dca.val);
	Spot aspot;
	Spot bspot;
	if(dca.has_spot(link.a)) {
		aspot = link.a;
		bspot = link.b;
	} else if(dcb.has_spot(link.b)){
		aspot = link.b;
		bspot = link.a;
	} else {
		std::cout << "!! Should not be bridging (merging) digitchains" << std::endl;
	}
	int a_idx_spot = idx_of_digit_vertex(dca.verts, aspot);
	int b_idx_spot = idx_of_digit_vertex(dcb.verts, bspot);
	
	// whether to reverse B's colors
	bool reverse = (dca.verts[a_idx_spot].color == dcb.verts[b_idx_spot].color);
	
//std::cout << "bridge_digit_chains: init complete" << std::endl;
	// add A's verts
	for(int i=0; i < dca.verts.size(); i++) {
		dcc.verts.push_back(DigitVertex(dca.verts[i].spot, dca.verts[i].color));
		for(int j=0; j < dca.verts[i].peers.size(); j++) {
			dcc.verts[i].peers.push_back(dca.verts[i].peers[j]);
			dcc.verts[i].links.push_back(dca.verts[i].links[j]);
		}
	}
//std::cout << "bridge_digit_chains: A's verts added" << std::endl;
	int offset = dca.verts.size();
	
	// add B's verts
	for(int i=0; i < dcb.verts.size(); i++) {
		dcc.verts.push_back(DigitVertex(dcb.verts[i].spot, (reverse ? dc_other_color(dcb.verts[i].color) : dcb.verts[i].color)));
		for(int j=0; j < dcb.verts[i].peers.size(); j++) {
			dcc.verts[i + offset].peers.push_back(dcb.verts[i].peers[j] + offset);
			dcc.verts[i + offset].links.push_back(dcb.verts[i].links[j]);
		}
	}
//std::cout << "bridge_digit_chains: B's verts added" << std::endl;
	// connect link
	int b_idx_off = b_idx_spot + offset;
	dcc.verts[a_idx_spot].connect(link, b_idx_off);
//std::cout << "bridge_digit_chains: X" << std::endl;
	dcc.verts[b_idx_off].connect(link, a_idx_spot);
//std::cout << "bridge_digit_chains: connected" << std::endl;
	return dcc;
}





int idx_of_digit_chain(std::vector<DigitChain>& chains, Spot p) {
//std::cout << "idx_of_digit_chain, spot: " << p << std::endl;
	for(int i=0; i < chains.size(); i++) {
		if(idx_of_digit_vertex(chains[i].verts, p) >= 0) {
//std::cout << i << std::endl;
			return i;
		}
	}
//std::cout << -1 << std::endl;
	return -1;
}


