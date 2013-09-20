
#include <cstdlib>
#include <ctime>

#include <iostream>
#include "pretty_solver.h"
#include "bitmatrix_to_ps.h"
#include "simple_naked_single.h"
#include "naked_single.h"
#include "hidden_single.h"
#include "nh_subset.h"
#include "pointing.h"
#include "boxline.h"

void pretty_solve(Bits* bits, char* file) {
	Psout ps = Psout();
	ps.start_file(file);
	bool running = true;
	while( running ) {
		ps.init();
		
		std::vector<SimpleNakedSingle> snss = find_simple_naked_singles(bits);
		if(snss.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			for(int i=0; i < snss.size(); i++) {
				snss[i].full_display(&ps);
				snss[i].apply(bits);
			}
			ps.append_file(file);
			continue;
		}
		
		std::vector<NakedSingle> nss = find_naked_singles(bits);
		if(nss.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			for(int i=0; i < nss.size(); i++) {
				nss[i].full_display(&ps);
				nss[i].apply(bits);
			}
			ps.append_file(file);
			continue;
		}
		
		std::vector<HiddenSingle> hss = find_hidden_singles(bits);
		if(hss.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			for(int i=0; i < hss.size(); i++) {
				hss[i].full_display(&ps);
				hss[i].apply(bits);
			}
			ps.append_file(file);
			continue;
		}
		
		std::vector<NHSubset> nhs = find_nh_subsets(bits);
		if(nhs.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			
			for(int i=0; i < nhs.size(); i++) {
				nhs[i].full_display(&ps);
				nhs[i].apply(bits);
			}
			ps.append_file(file);
			continue;
		}
		
		std::vector<Pointing> points = find_pointings(bits);
		if(points.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			
			for(int i=0; i < points.size(); i++) {
				points[i].full_display(&ps);
				points[i].apply(bits);
			}
			ps.append_file(file);
			continue;
		}
		
		ps_write_numbers(bits, &ps);
		ps_write_candidates(bits, &ps);
		ps.append_file(file);
		break;
	}
}

int rand_lower_than(int x) {
	return (rand() % x);
}

void pretty_solve_single_random(Bits* bits, char* file) {
	Psout ps = Psout();
	ps.start_file(file);
	bool running = true;
	while( running ) {
//std::cout << "Solve Step" << std::endl;
		ps.init();
		
//std::cout << "\tChecking SNS" << std::endl;
		std::vector<SimpleNakedSingle> snss = find_simple_naked_singles(bits);
		if(snss.size() > 0) {
//std::cout << "\tFound SNS?" << std::endl;
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			int idx = rand_lower_than(snss.size());
			snss[idx].full_display(&ps);
			snss[idx].apply(bits);
			ps.append_file(file);
			continue;
		}
		
//std::cout << "\tChecking NS" << std::endl;
		std::vector<NakedSingle> nss = find_naked_singles(bits);
		if(nss.size() > 0) {
//std::cout << "\tFound NS?" << std::endl;
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			int idx = rand_lower_than(nss.size());
			nss[idx].full_display(&ps);
			nss[idx].apply(bits);
			nss[idx].display_keys(&ps);
			ps.append_file(file);
			continue;
		}
		
//std::cout << "\tChecking HS" << std::endl;
		std::vector<HiddenSingle> hss = find_hidden_singles(bits);
		if(hss.size() > 0) {
//std::cout << "\tFound HS?" << std::endl;
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			//int idx = rand_lower_than(hss.size());
			///// NOTE: this picks box HS before other HS
			bool hasbox = false;
			for(int i=0; i < hss.size(); i++) {
				if(hss[i].type == HS_BOX) { hasbox = true; }
			}
			int idx;
			if(hasbox) {
				idx = rand_lower_than(hss.size());
				while(hss[idx].type != HS_BOX) {
					idx = rand_lower_than(hss.size());
				}
			} else {
				idx = rand_lower_than(hss.size());
			}
			/////
			hss[idx].full_display(&ps);
			hss[idx].apply(bits);
			hss[idx].display_keys(&ps);
			ps.append_file(file);
			continue;
		}
		
//std::cout << "\tChecking NHSubset" << std::endl;
		std::vector<NHSubset> nhs = find_nh_subsets(bits);
		if(nhs.size() > 0) {
//std::cout << "\tFound NHSubset?" << std::endl;
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			int idx = rand_lower_than(nhs.size());
			nhs[idx].full_display(&ps);
			nhs[idx].apply(bits);
			nhs[idx].display_keys(&ps);
			ps.append_file(file);
			continue;
		}
		
		std::vector<Pointing> points = find_pointings(bits);
		if(points.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			int idx = rand_lower_than(points.size());
			points[idx].full_display(&ps);
			points[idx].apply(bits);
			points[idx].display_keys(&ps);
			ps.append_file(file);
			continue;
		}
		
		std::vector<BoxLine> boxlines = find_boxlines(bits);
		if(boxlines.size() > 0) {
			ps_write_numbers(bits, &ps);
			ps_write_candidates(bits, &ps);
			int idx = rand_lower_than(boxlines.size());
			boxlines[idx].full_display(&ps);
			boxlines[idx].apply(bits);
			boxlines[idx].display_keys(&ps);
			ps.append_file(file);
			continue;
		}
		
		ps_write_numbers(bits, &ps);
		ps_write_candidates(bits, &ps);
		ps.append_file(file);
		break;
	}
std::cout << "Done solving" << std::endl;
}
