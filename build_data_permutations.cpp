
#include <iostream>
#include <fstream>

const int table_data[6][3] = {
	{0,1,2},
	{0,2,1},
	{1,0,2},
	{1,2,0},
	{2,0,1},
	{2,1,0}
};

int main() {
	std::ofstream out;
	out.open("data_permutations.h");
	out << "static const int permutation_data[1296][18] = {\n";
	for(int box=0; box < 6; box++) {
		for(int top=0; top < 6; top++) {
			for(int mid=0; mid < 6; mid++) {
				for(int bot=0; bot < 6; bot++) {
					int dat[18];
					for(int i=0; i < 9; i++) {
						dat[i] = 3 * table_data[box][i / 3];
					}
					for(int i=0; i < 3; i++) {
						dat[i] += table_data[top][i];
						dat[i+3] += table_data[mid][i];
						dat[i+6] += table_data[bot][i];
					}
					for(int i=0; i < 9; i++) {
						for(int k=0; k < 9; k++) {
							if(dat[k] == i) {
								dat[i+9] = k;
								break;
							}
						}
					}
					
					/* double-check reverse permutations
					int testa[9];
					int testb[9];
					for(int i=0; i < 9; i++) {
						testa[i] = i;
					}
					for(int i=0; i < 9; i++) {
						testb[i] = testa[dat[i]];
					}
					for(int i=0; i < 9; i++) {
						testa[i] = testb[dat[i+9]];
					}
					for(int i=0; i < 9; i++) {
						if(testa[i] != i) {
							std::cout << "build_data_permutations error!!!" << std::endl;
							
							std::cout << "\tdat: {" << dat[0];
							for(int i=1; i < 18; i++) {
								std::cout << ", " << dat[i];
							}
							std::cout << "}\n";
							
							std::cout << "\tstep1: {" << testb[0];
							for(int i=1; i < 9; i++) {
								std::cout << ", " << testb[i];
							}
							std::cout << "}\n";
							
							std::cout << "\tstep2: {" << testa[0];
							for(int i=1; i < 9; i++) {
								std::cout << ", " << testa[i];
							}
							std::cout << "}" << ((box != 5 || top != 5 || mid != 5 || bot != 5) ? "," : "") << "\n";
						}
					}
					*/
					
					out << "\t{" << dat[0];
					for(int i=1; i < 18; i++) {
						if(i==9) { out << "\t"; }
						out << ", " << dat[i];
					}
					out << "}" << ((box != 5 || top != 5 || mid != 5 || bot != 5) ? "," : "") << "\n";
				}
			}
		}
	}
	out << "};\n" << std::endl;
	out.close();
	return 0;
}
