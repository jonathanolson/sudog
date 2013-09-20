#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include <vector>
#include <string>
#include "coords.h"
#include "psout.h"
#include "bitmatrix.h"
#include "target.h"
#include "key.h"

extern const int num_pattern_types;

extern const std::string pattern_type_names[];

enum PatternType {
	PATTERN_SIMPLENAKEDSINGLE_GENERIC=0,
	PATTERN_NAKEDSINGLE_GENERIC,
	PATTERN_HIDDENSINGLE_GENERIC,
	PATTERN_NHSUBSET_GENERIC,
	PATTERN_POINTING_GENERIC,
	PATTERN_BOXLINE_GENERIC,
	PATTERN_NFISH_GENERIC,
	PATTERN_SIMPLE_COLORING_GENERIC
};

class Pattern {
	public:
	
	virtual void apply(BitMatrix* bits) = 0;
	virtual void full_display(Output* out) = 0;
	virtual void display_keys(Output* out) = 0;
	virtual void describe(std::ostream& out) = 0;
	
	virtual std::vector<Target> target_list() = 0;
	virtual std::vector<Key> key_list() = 0;
	virtual Pattern* clone() = 0;
};

bool pattern_subset(Pattern* a, Pattern* b);
bool pattern_novel(std::vector<Pattern*> patterns, Pattern* p);

#endif
