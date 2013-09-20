
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <plotter.h>

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

#include "libplot_out.h"

const int OUT_PS = 0;
const int OUT_SVG = 1;
const int OUT_PNG = 2;
const int OUT_GIF = 3;

Plotter* plotter;

int output_format = 0;

bool show_candidates = false;

std::string resolution = "560x560";
std::string font = "Helvetica";
PlotterParams params;

void init_ps() {
	//params.setplparam("PAGESIZE",(char*)"letter");
	plotter = new PSPlotter(std::cin, std::cout, std::cerr, params);
}

void init_svg() {
	plotter = new SVGPlotter(std::cin, std::cout, std::cerr, params);
}

void init_png() {
	std::cout << "WARNING: PNGPlotter will not compile!" << std::endl;
	//plotter = new PNGPlotter(std::cin, std::cout, std::cerr, params);
}

void init_gif() {
	params.setplparam("BITMAPSIZE",(char*)"560x560");
	plotter = new GIFPlotter(std::cin, std::cout, std::cerr, params);
}

int main(int argc, char* argv[]) {
	int c;
	while(true) {
		static struct option long_options[] = {
			{"ps", no_argument, 0, 'p'},
			{"svg", no_argument, 0, 's'},
			{"png", no_argument, 0, 'n'},
			{"gif", no_argument, 0, 'g'},
			{"candidates", no_argument, 0, 'c'},
			{"font", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "psngcf:", long_options, &option_index);
		if(c == -1) { break; }
		switch(c) {
			/*
			case 0:
				*/
			case 'p':
				output_format = OUT_PS; break;
			case 's':
				output_format = OUT_SVG; break;
			case 'n':
				output_format = OUT_PNG; break;
			case 'g':
				output_format = OUT_GIF; break;
			case 'c':
				show_candidates = true; break;
			case 'f':
				font = std::string(optarg); break;
		}
	}
	
	if(optind < argc) {
		/*
		while(optind < argc) {
			
			printf ("%s ", argv[optind++]);
		}
		*/
	}
	
	if(output_format == OUT_PS) {
		init_ps();
	} else if(output_format == OUT_SVG) {
		init_svg();
	} else if(output_format == OUT_PNG) {
		init_png();
	} else if(output_format == OUT_GIF) {
		init_gif();
	} else {
		std::cerr << "Not yet supported" << std::endl;
		return 1;
	}
	
	Bits bits = Bits();
	std::cin >> bits;
	
	LPOut lp = LPOut(plotter);
	
	write_numbers(&bits, &lp);
	
	if(show_candidates) {
		write_candidates(&bits, &lp);
	}
	
	lp.draw();
	
	delete plotter;
	return 0;
}

