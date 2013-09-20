
#include <iostream>
#include "nfish.h"
#include "keyrank.h"

const int NFISH_ROWS = 1;
const int NFISH_COLS = 2;

NFish::NFish(std::vector<Target> _targets, std::vector<Key> _keys, std::vector<Spot> _subset, std::vector<int> _units, int _val, int _type) {
	targets = _targets;
	keys = _keys;
	subset = _subset;
	units = _units;
	val = _val;
	type = _type;
}

void NFish::apply(BitMatrix* bits) {
	for(int i=0; i < targets.size(); i++) {
		targets[i].apply(bits);
	}
}

void NFish::full_display(Output* out) {
	for(int i=0; i < targets.size(); i++) {
		Target target = targets[i];
		for(int j=0; j < target.candidates.size(); j++) {
			out->highlight_candidate(target.spot, target.candidates[j], 1.0, 0.7, 0.4);
		}
	}
	if(type == NFISH_ROWS) {
		for(int i=0; i < units.size(); i++) {
			out->highlight_row(units[i], 0.9, 0.9, 0.9);
		}
	} else if(type == NFISH_COLS) {
		for(int i=0; i < units.size(); i++) {
			out->highlight_col(units[i], 0.9, 0.9, 0.9);
		}
	}
	for(int i=0; i < subset.size(); i++) {
		out->highlight_cell(subset[i], 0.8, 0.8, 0.8);
	}
}

void NFish::display_keys(Output* out) {
	for(int i=0; i < keys.size(); i++) {
		float weight = (keys[i].multiplicity == 1 ? 0.0 : (keys[i].multiplicity == 2 ? 0.5 : 0.8));
		out->outline_cell(keys[i].spot, 1.5, 1.0, weight, weight);
	}
}

std::vector<Target> NFish::target_list() {
	return targets;
}

std::vector<Key> NFish::key_list() {
	return keys;
}

Pattern* NFish::clone() {
	return (new NFish(*this));
}

void NFish::describe(std::ostream& out) {
	out << this;
}

std::ostream& operator<<(std::ostream& out, NFish* nf) {
	out << "N-Fish (on " << (nf->type == NFISH_ROWS ? "row" : "col") << "s)" << std::endl;
	switch(nf->units.size()) {
		case 2:
			out << "\"X-Wing\"" << std::endl; break;
		case 3:
			out << "\"Swordfish\"" << std::endl; break;
		case 4:
			out << "\"Jellyfish\"" << std::endl; break;
	}
	out << "\tSet:";
	for(int i=0; i < nf->subset.size(); i++) {
		out << std::endl << "\t\t" << nf->subset[i];
	}
	out << std::endl << "\tTargets:";
	for(int i=0; i < nf->targets.size(); i++) {
		out << std::endl << "\t\t" << nf->targets[i];
	}
	return out;
}

void nfish_process_recur_row(Bits* bits, int val, int n, std::vector<NFish>* ret, std::vector<int>* rows, std::vector< std::vector<Spot> >* candsets, std::vector<int>* stack, int cur) {
	if(stack->size() == n) {
		std::vector<int> currows;
		for(int i=0; i < n; i++) {
			currows.push_back((*rows)[(*stack)[i]]);
		}
		std::vector<Spot> subset;
		std::vector<bool> colmask;
		for(int i=0; i < 9; i++) {
			colmask.push_back(false);
		}
		for(int i=0; i < n; i++) {
			std::vector<Spot> candset = (*candsets)[(*stack)[i]];
			for(int j=0; j < (*candsets)[(*stack)[i]].size(); j++) {
				subset.push_back((*candsets)[(*stack)[i]][j]);
				int col = (*candsets)[(*stack)[i]][j].col;
				colmask[col] = true;
			}
		}
		std::vector<int> cols;
		for(int i=0; i < 9; i++) {
			if(colmask[i]) {
				cols.push_back(i);
			}
		}
		if(cols.size() > n) { return; }
		// then identify targets
		std::vector<Target> targets;
		for(int i=0; i < cols.size(); i++) {
			int col = cols[i];
			for(int j=0; j < 9; j++) {
				if(in_ints(currows, j)) { continue; }
				Spot p = Spot(j, col);
				if(!bits->has_candidate(p, val)) { continue; }
				std::vector<int> vals;
				vals.push_back(val);
				targets.push_back(Target(p, vals, 0));
			}
		}
		if(targets.size() == 0) { return; }
		
		// process keys
		
		KeyRank keyrank;
		for(int i=0; i < currows.size(); i++) {
			int row = currows[i];
			for(int j=0; j < 9; j++) {
				keyrank.add_possible_covers(bits, Spot(row, j), val);
			}
		}
		ret->push_back(NFish(targets, keyrank.keys, subset, currows, val, NFISH_ROWS));
	} else {
		for(int i=cur; i <= rows->size() - (n - stack->size()); i++) {
			stack->push_back(i);
			nfish_process_recur_row(bits, val, n, ret, rows, candsets, stack, i+1);
			stack->pop_back();
		}
	}
}

void nfish_process_recur_col(Bits* bits, int val, int n, std::vector<NFish>* ret, std::vector<int>* cols, std::vector< std::vector<Spot> >* candsets, std::vector<int>* stack, int cur) {
	if(stack->size() == n) {
		std::vector<int> curcols;
		for(int i=0; i < n; i++) {
			curcols.push_back((*cols)[(*stack)[i]]);
		}
		std::vector<Spot> subset;
		std::vector<bool> rowmask;
		for(int i=0; i < 9; i++) {
			rowmask.push_back(false);
		}
		for(int i=0; i < n; i++) {
			std::vector<Spot> candset = (*candsets)[(*stack)[i]];
			for(int j=0; j < (*candsets)[(*stack)[i]].size(); j++) {
				subset.push_back((*candsets)[(*stack)[i]][j]);
				int row = (*candsets)[(*stack)[i]][j].row;
				rowmask[row] = true;
			}
		}
		std::vector<int> rows;
		for(int i=0; i < 9; i++) {
			if(rowmask[i]) {
				rows.push_back(i);
			}
		}
		if(rows.size() > n) { return; }
		// then identify targets
		std::vector<Target> targets;
		for(int i=0; i < rows.size(); i++) {
			int row = rows[i];
			for(int j=0; j < 9; j++) {
				if(in_ints(curcols, j)) { continue; }
				Spot p = Spot(row, j);
				if(!bits->has_candidate(p, val)) { continue; }
				std::vector<int> vals;
				vals.push_back(val);
				targets.push_back(Target(p, vals, 0));
			}
		}
		if(targets.size() == 0) { return; }
		
		// process keys
		
		KeyRank keyrank;
		for(int i=0; i < curcols.size(); i++) {
			int col = curcols[i];
			for(int j=0; j < 9; j++) {
				keyrank.add_possible_covers(bits, Spot(j, col), val);
			}
		}
		ret->push_back(NFish(targets, keyrank.keys, subset, curcols, val, NFISH_COLS));
	} else {
		for(int i=cur; i <= cols->size() - (n - stack->size()); i++) {
			stack->push_back(i);
			nfish_process_recur_col(bits, val, n, ret, cols, candsets, stack, i+1);
			stack->pop_back();
		}
	}
}

void nfish_process_n(Bits* bits, int val, int n, std::vector<NFish>* ret) {
	// rows
	std::vector<int> rows;
	//std::vector< std::vector<Spot> > rowsets;
	std::vector< std::vector<Spot> > rowcandsets;
	for(int i=0; i < 9; i++) {
		//std::vector<Spot> spots;
		std::vector<Spot> candspots;
		for(int j=0; j < 9; j++) {
			Spot p = Spot(i, j);
			//spots.push_back(p);
			if(bits->has_candidate(p, val)) {
				candspots.push_back(p);
			}
		}
		if(candspots.size() < 2 || candspots.size() > n) { continue; }
		rows.push_back(i);
		//rowsets.push_back(spots);
		rowcandsets.push_back(candspots);
	}
	if(rows.size() >= n) {
		std::vector<int> idxstack;
		nfish_process_recur_row(bits, val, n, ret, &rows, &rowcandsets, &idxstack, 0);
	}
	
	// cols
	std::vector<int> cols;
	//std::vector< std::vector<Spot> > colsets;
	std::vector< std::vector<Spot> > colcandsets;
	for(int i=0; i < 9; i++) {
		//std::vector<Spot> spots;
		std::vector<Spot> candspots;
		for(int j=0; j < 9; j++) {
			Spot p = Spot(j, i);
			//spots.push_back(p);
			if(bits->has_candidate(p, val)) {
				candspots.push_back(p);
			}
		}
		if(candspots.size() < 2 || candspots.size() > n) { continue; }
		cols.push_back(i);
		//colsets.push_back(spots);
		colcandsets.push_back(candspots);
	}
	if(cols.size() >= n) {
		std::vector<int> idxstack;
		nfish_process_recur_col(bits, val, n, ret, &cols, &colcandsets, &idxstack, 0);
	}
}

std::vector<NFish> find_nfishes(Bits* bits) {
	std::vector<NFish> ret;
	for(int i=1; i <= 9; i++) {
		nfish_process_n(bits, i, 2, &ret);
		nfish_process_n(bits, i, 3, &ret);
		nfish_process_n(bits, i, 4, &ret);
	}
	return ret;
}

