
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "psout.h"

Psout::Psout() {
	cellvalues = new int[81];
	showcandidates = std::vector<std::string>();
	highlightedcandidates = std::vector<std::string>();
	highlightedcells = std::vector<std::string>();
	highlightedrules = std::vector<std::string>();
	outlinedcells = std::vector<std::string>();
}

Psout::~Psout() {
	delete [] cellvalues;
}

void Psout::init() {
	for(int i=0; i < 81; i++) {
		cellvalues[i] = 0;
	}
	showcandidates.clear();
	highlightedcandidates.clear();
	highlightedcells.clear();
	highlightedrules.clear();
	outlinedcells.clear();
}

void Psout::to_file(const char* file) {
	std::string filestr = std::string(file);
	std::string introstr = std::string("cat t11_intro > ");
	introstr.append(filestr);
	system(introstr.c_str());
	std::ofstream out;
	out.open(file, std::ios::app | std::ios::out);
	
	for(int i=0; i < highlightedrules.size(); i++) {
		out << highlightedrules[i];
	}
	
	for(int i=0; i < highlightedcells.size(); i++) {
		out << highlightedcells[i];
	}
	
	for(int i=0; i < highlightedcandidates.size(); i++) {
		out << highlightedcandidates[i];
	}
	
	for(int i=0; i < outlinedcells.size(); i++) {
		out << outlinedcells[i];
	}
	
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(cellvalues[i] != 0) {
			out << cellvalues[i] << " " << p.row + 1 << " " << p.col + 1 << " fullnumber" << std::endl;
		}
	}
	
	for(int i=0; i < showcandidates.size(); i++) {
		out << showcandidates[i];
	}
	
	out << std::endl << "makegrid" << std::endl;
	out << std::endl << "showpage" << std::endl;
	out.flush();
	out.close();
}

void Psout::start_file(const char* file) {
	std::string filestr = std::string(file);
	std::string introstr = std::string("cat t20_intro > ");
	introstr.append(filestr);
	system(introstr.c_str());
}

void Psout::append_file(const char* file) {
	std::ofstream out;
	out.open(file, std::ios::app | std::ios::out);
	out << "10 510 translate" << std::endl;
	for(int i=0; i < highlightedrules.size(); i++) {
		out << highlightedrules[i];
	}
	
	for(int i=0; i < highlightedcells.size(); i++) {
		out << highlightedcells[i];
	}
	
	for(int i=0; i < highlightedcandidates.size(); i++) {
		out << highlightedcandidates[i];
	}
	
	for(int i=0; i < outlinedcells.size(); i++) {
		out << outlinedcells[i];
	}
	
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(cellvalues[i] != 0) {
			out << cellvalues[i] << " " << p.row + 1 << " " << p.col + 1 << " fullnumber" << std::endl;
		}
	}
	
	for(int i=0; i < showcandidates.size(); i++) {
		out << showcandidates[i];
	}
	
	out << std::endl << "makegrid" << std::endl;
	out << std::endl << "showpage" << std::endl;
	out.flush();
	out.close();
}

void Psout::append_file_barebones(const char* file) {
	std::ofstream out;
	out.open(file, std::ios::app | std::ios::out);
	for(int i=0; i < highlightedrules.size(); i++) {
		out << highlightedrules[i];
	}
	
	for(int i=0; i < highlightedcells.size(); i++) {
		out << highlightedcells[i];
	}
	
	for(int i=0; i < highlightedcandidates.size(); i++) {
		out << highlightedcandidates[i];
	}
	
	for(int i=0; i < outlinedcells.size(); i++) {
		out << outlinedcells[i];
	}
	
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(cellvalues[i] != 0) {
			out << cellvalues[i] << " " << p.row + 1 << " " << p.col + 1 << " fullnumber" << std::endl;
		}
	}
	
	for(int i=0; i < showcandidates.size(); i++) {
		out << showcandidates[i];
	}
	
	out << std::endl << "makegrid" << std::endl;
	out.flush();
	out.close();
}

void Psout::set_fullnumber(Spot p, int val) {
	cellvalues[idx_of_spot(p)] = val;
}

void Psout::add_candidate(Spot p, int val) {
	std::string str;
	std::stringstream outter;;
	outter << val << " " << p.row + 1 << " " << p.col + 1 << " showcandidate" << std::endl;
	str = outter.str();
	showcandidates.push_back(str);
}

void Psout::highlight_candidate(Spot p, int val, float r, float g, float b) {
	std::string str;
	std::stringstream outter;;
	outter << val << " " << p.row + 1 << " " << p.col + 1 << " " << r << " "<< g << " " << b << " highlightcandidate" << std::endl;
	str = outter.str();
	highlightedcandidates.push_back(str);
}

void Psout::highlight_cell(Spot p, float r, float g, float b) {
	std::string str;
	std::stringstream outter;;
	outter << p.row + 1 << " " << p.col + 1 << " " << r << " "<< g << " " << b << " highlightcell" << std::endl;
	str = outter.str();
	highlightedcells.push_back(str);
}

void Psout::highlight_row(int row, float r, float g, float b) {
	std::string str;
	std::stringstream outter;;
	outter << row + 1 << " " << r << " "<< g << " " << b << " highlightrow" << std::endl;
	str = outter.str();
	highlightedrules.push_back(str);
}

void Psout::highlight_col(int col, float r, float g, float b) {
	std::string str;
	std::stringstream outter;;
	outter << col + 1 << " " << r << " "<< g << " " << b << " highlightcol" << std::endl;
	str = outter.str();
	highlightedrules.push_back(str);
}

void Psout::highlight_box(int boxid, float r, float g, float b) {
	std::string str;
	std::stringstream outter;;
	outter << boxid + 1 << " " << r << " "<< g << " " << b << " highlightbox" << std::endl;
	str = outter.str();
	highlightedrules.push_back(str);
}

void Psout::outline_cell(Spot p, float width, float r, float g, float b) {
	std::string str;
	std::stringstream outter;;
	outter << width << " " << p.row + 1 << " " << p.col + 1 << " " << r << " "<< g << " " << b << " outlinecell" << std::endl;
	str = outter.str();
	outlinedcells.push_back(str);
}

void Psout::link_candidate(Spot spot_a, int val_a, Spot spot_b, int val_b, float width, float r, float g, float b) {
	// NEEDED
}
