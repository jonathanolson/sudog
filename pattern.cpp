
#include "pattern.h"

const int num_pattern_types = 8;

const std::string pattern_type_names[num_pattern_types] = {
	"Simple Naked Single",
	"Naked Single",
	"Hidden Single",
	"Naked/Hidden Subset",
	"Pointing",
	"Box-Line Interaction",
	"N-Fish",
	"Simple Coloring"
};

bool pattern_subset(Pattern* a, Pattern* b) {
	std::vector<Target> a_targets = a->target_list();
	std::vector<Target> b_targets = b->target_list();
	
	bool matches_all = true;
	
	// for all targets in b
	for(int i=0; i < b_targets.size(); i++) {
		Target b_t = b_targets[i];
		bool matches_one = false;
		
		// for all targets in a
		for(int j=0; j < a_targets.size(); j++) {
			Target a_t = a_targets[j];
			if(!spot_equal(a_t.spot, b_t.spot)) { continue; }
			bool match_all_cands = true;
			
			// for all candidates in b's target
			for(int m=0; m < b_t.candidates.size(); m++) {
				int mbit = b_t.candidates[m];
				if(in_ints(a_t.candidates, mbit)) { continue; }
				match_all_cands = false;
				break;
			}
			if(!match_all_cands) { continue; }
			matches_one = true;
			break;
		}
		if(!matches_one) {
			matches_all = false;
			break;
		}
	}
	
	return matches_all;
}

bool pattern_novel(std::vector<Pattern*> patterns, Pattern* p) {
	for(int i=0; i < patterns.size(); i++) {
		if(pattern_subset(patterns[i], p)) { return false; }
	}
	return true;
}
