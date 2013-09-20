


#include <iostream>
#include <fstream>
#include <unistd.h>
#include <getopt.h>

#include "coords.h"
#include "psout.h"
#include "backtrack_common.h"
#include "backtrack_naive_unique.h"
#include "bitmatrix_to_ps.h"
#include "bits.h"

#include "simple_naked_single.h"
#include "naked_single.h"
#include "hidden_single.h"
#include "nh_subset.h"
#include "pointing.h"
#include "boxline.h"
#include "nfish.h"
#include "simple_coloring.h"

#include "pretty_solver.h"
#include "pattern.h"
#include "backtrack_to_bits.h"
#include "permutation.h"

//#include "cairo_out.h"
#include "bits_to_output.h"

#include "strong_link.h"

#include "simple_complete_gen.h"
#include "naive_gen.h"

#include "svgout.h"

int main(int argc, char* argv[]) {
	
	Bits bits = Bits();
	std::cin >> bits;
	
	SVGOut svg = SVGOut();
	
	write_numbers(&bits, &svg);
	write_candidates(&bits, &svg);
	
	std::ofstream file;
	file.open("example.svg");
	svg.draw(file);
	file.close();
	return 0;
}

