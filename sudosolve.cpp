
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include "coords.h"
#include "psout.h"
#include "svgout.h"
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

#include "bits_to_output.h"

#include "strong_link.h"

#include "simple_complete_gen.h"
#include "naive_gen.h"

// constants
const int OUT_PS = 0;
const int OUT_SVG = 1;

// initial variables
int output_format = 0;

bool show_patterns = false;
bool show_candidates = false;
bool show_keys = false;

std::string font = "Helvetica";
std::string prefix = "sudoku";

Pattern* find_pattern(Bits& bits) {
	// finds the first possible pattern, and returns a copy of it to process
	std::vector<SimpleNakedSingle> snss = find_simple_naked_singles(&bits);
	if(snss.size() > 0) {
		return snss[0].clone();
	}
	
	std::vector<NakedSingle> nss = find_naked_singles(&bits);
	if(nss.size() > 0) {
		return nss[0].clone();
	}
	
	std::vector<HiddenSingle> hss = find_hidden_singles(&bits);
	if(hss.size() > 0) {
		return hss[0].clone();
	}
	
	std::vector<NHSubset> nhs = find_nh_subsets(&bits);
	if(nhs.size() > 0) {
		return nhs[0].clone();
	}
	
	std::vector<Pointing> points = find_pointings(&bits);
	if(points.size() > 0) {
		return points[0].clone();
	}
	
	std::vector<BoxLine> boxlines = find_boxlines(&bits);
	if(boxlines.size() > 0) {
		return boxlines[0].clone();
	}
	
	std::vector<NFish> nfishes = find_nfishes(&bits);
	if(nfishes.size() > 0) {
		return nfishes[0].clone();
	}
	
	std::vector<SimpleColoring> scs = find_simple_colorings(&bits);
	if(scs.size() > 0) {
		return scs[0].clone();
	}
	
	return NULL;
}

void do_help() {
	// help message
	std::cerr << "Usage: sudosolve [options]\n";
	std::cerr << "Outputs the logical solving process for a Sudoku puzzle in postscript or SVG.\n";
	std::cerr << "\n";
	std::cerr << "Options:\n";
	std::cerr << "  -p, --postscript     Output six grids a page in postscript format\n";
	std::cerr << "                         Saved in <PREFIX>.ps\n";
	std::cerr << "  -s, --svg            Output in multiple SVG files of the form:\n";
	std::cerr << "                         <PREFIX>000.svg, <PREFIX>001.svg, ...\n";
	std::cerr << "  -c, --candidates     Display possible candidates in each cell\n";
	std::cerr << "  -t, --patterns       Display a visual indication of the pattern used\n";
	std::cerr << "                         to solve each step. Also turns on candidates.\n";
	std::cerr << "  -k, --keys           Outline cells that the current pattern depends upon\n";
	std::cerr << "  -h, --help           Will display this message\n";
	std::cerr << "  -f, --font=FONT      Will use FONT for postscript output\n";
	std::cerr << "                         Default: Helvetica\n";
	std::cerr << "  -o, --prefix=PREFIX  Output file(s) will be given this prefix.\n";
	std::cerr << "                         Default: 'sudoku'\n";
	std::cerr << "\n";
	std::cerr << "Input will be taken from standard input in the dotted form, for example:\n";
	std::cerr << ".4398.25.6..425...2....1.949....4.7.3..6.8...41.2.9..382.5.........4...553489.71.\n";
	std::cerr << "\n";
	std::cerr << "is equivalent to the grid:\n";
	std::cerr << ".4398.25.\n";
	std::cerr << "6..425...\n";
	std::cerr << "2....1.94\n";
	std::cerr << "9....4.7.\n";
	std::cerr << "3..6.8...\n";
	std::cerr << "41.2.9..3\n";
	std::cerr << "82.5.....\n";
	std::cerr << "....4...5\n";
	std::cerr << "53489.71.\n";
	std::cerr << "\n";
	std::cerr << "where in both cases, .'s represent blank cells in the Sudoku grid.\n";
	
	
	exit(1);
}

int main(int argc, char* argv[]) {
	// parse command line arguments
	int c;
	while(true) {
		static struct option long_options[] = {
			{"postscript", no_argument, 0, 'p'},
			{"svg", no_argument, 0, 's'},
			{"patterns", no_argument, 0, 't'},
			{"candidates", no_argument, 0, 'c'},
			{"keys", no_argument, 0, 'k'},
			{"help", no_argument, 0, 'h'},
			{"font", required_argument, 0, 'f'},
			{"prefix", required_argument, 0, 'o'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "pstckf:o:", long_options, &option_index);
		if(c == -1) { break; }
		switch(c) {
			/*
			case 0:
				*/
			case 'p':
				output_format = OUT_PS; break;
			case 's':
				output_format = OUT_SVG; break;
			case 't':
				show_patterns = true; show_candidates = true; break;
			case 'c':
				show_candidates = true; break;
			case 'k':
				show_keys = true; break;
			case 'h':
				do_help(); break;
			case 'f':
				font = std::string(optarg); break;
			case 'o':
				prefix = std::string(optarg); break;
		}
	}
	
	if(output_format == OUT_SVG && font != "Helvetica") {
		std::cerr << "WARNING: SVG output currently supports only Helvetica fonts." << std::endl;
		std::cerr << "WARNING: Overriding and using Helvetica anyways." << std::endl;
	}
	
	// load puzzle from standard input
	Bits bits = Bits();
	std::cin >> bits;
	
	// initialize variables needed for output
	bool running = true;
	Psout ps = Psout();
	ps.init();
	SVGOut svg = SVGOut();
	svg.init();
	
	if(output_format == OUT_PS) {
		prefix.append(".ps");
		ps.start_file(prefix.c_str());
	}
	
	int pageidx = 0;
	int frameidx = 0;
	std::ofstream out;
	
	while(running) {
		Pattern* pat = find_pattern(bits);
		
		if(pat == NULL) {
			running = false;
		}
		
		if(output_format == OUT_PS) {
			ps.init();
			write_numbers(&bits, &ps);
			if(show_candidates) {
				write_candidates(&bits, &ps);
			}
			if(show_patterns && running) {
				pat->full_display(&ps);
			}
			if(show_keys && running) {
				pat->display_keys(&ps);
			}
			out.open(prefix.c_str(), std::ios::app | std::ios::out);
			
			// override font from header.
			out << "/fullfont {/" << font << " findfont 20 scalefont setfont} def\n/candidatefont {/" << font << " findfont 8 scalefont setfont} def\n";
			
			out << "gsave\n";
			if(pageidx % 2 == 0) {
				out << "58 ";
			} else {
				out << "311 ";
			}
			if(pageidx / 2 == 0) {
				out << "527 translate\n";
			} else if(pageidx / 2 == 1) {
				out << "274 translate\n";
			} else {
				out << "21 translate\n";
			}
			out.close();
			ps.append_file_barebones(prefix.c_str());
			out.open(prefix.c_str(), std::ios::app | std::ios::out);
			out << "makegrid\n";
			out << "grestore\n";
			pageidx++;
			if(pageidx == 6) {
				pageidx = 0;
				out << "showpage\n";
			}
			out.close();
		} else if(output_format == OUT_SVG) {
			svg.init();
			write_numbers(&bits, &svg);
			if(show_candidates) {
				write_candidates(&bits, &svg);
			}
			if(show_patterns && running) {
				pat->full_display(&svg);
			}
			if(show_keys && running) {
				pat->display_keys(&svg);
			}
			char buf[10];
			sprintf(buf, "%03d.svg",frameidx);
			frameidx++;
			std::string filename = prefix;
			filename.append(buf);
			out.open(filename.c_str(), std::ios::out);
			
			svg.draw(out);
			
			out.close();
		}
		
		if(running) {
			pat->apply(&bits);
			
			delete pat;
		}
	}
	
	if(output_format == OUT_PS) {
		if(pageidx != 0) {
			out.open(prefix.c_str(), std::ios::app | std::ios::out);
			out << "showpage\n";
			out.close();
		}
	}
	
	
	return 0;
}

