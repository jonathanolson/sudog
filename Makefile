
FLAGS = -O3
#FLAGS = -O3 -march=prescott
#FLAGS = -O3 -march=prescott -g -pg
#FLAGS = -O3 -march=pentium3
#FLAGS = -O3 -march=pentium4
#FLAGS = -O3 -march=prescott -fomit-frame-pointer -DNTRACE

TESTS = testa gui2 print_complete invalid_puzzles valid_puzzles unsolved solved unsolve_six bench_a sudoprint sudosolve
GUIFILES = cairo_out.o
TECHNIQUES = simple_naked_single.o naked_single.o hidden_single.o nh_subset.o pointing.o boxline.o nfish.o simple_coloring.o
TECHHEADERS = coords.h psout.h bits.h target.h key.h pattern.h digit_chain.h
OBJS = \
	coords.o psout.o backtrack_common.o backtrack_naive_unique.o bitmatrix_to_ps.o bits_to_output.o bits.o pretty_solver.o \
	target.o key.o backtrack_to_bits.o pattern.o keyrank.o strong_link.o digit_chain.o simple_complete_gen.o permutation.o \
	dlx.o naive_gen.o svgout.o

all: $(OBJS) $(TECHNIQUES) $(TESTS)

testa: testa.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o testa $(OBJS) $(TECHNIQUES) testa.cpp

guitest: guitest.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o guitest $(OBJS) $(TECHNIQUES) guitest.cpp `pkg-config gtkmm-2.4 --cflags --libs`

cairo_out.o: cairo_out.h cairo_out.cpp coords.h output.h
	g++ $(FLAGS) -c cairo_out.cpp `pkg-config gtkmm-2.4 --cflags`

gui2: gui2.cpp cairo_out.o $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o gui2 cairo_out.o $(OBJS) $(TECHNIQUES) gui2.cpp -lpthread `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0

print_complete: print_complete.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o print_complete $(OBJS) $(TECHNIQUES) print_complete.cpp

invalid_puzzles: invalid_puzzles.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o invalid_puzzles $(OBJS) $(TECHNIQUES) invalid_puzzles.cpp

valid_puzzles: valid_puzzles.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o valid_puzzles $(OBJS) $(TECHNIQUES) valid_puzzles.cpp

unsolved: unsolved.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o unsolved $(OBJS) $(TECHNIQUES) unsolved.cpp

solved: solved.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o solved $(OBJS) $(TECHNIQUES) solved.cpp

unsolve_six: unsolve_six.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o unsolve_six $(OBJS) $(TECHNIQUES) unsolve_six.cpp

bench_a: bench_a.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o bench_a $(OBJS) $(TECHNIQUES) bench_a.cpp

sudoprint: sudoprint.cpp libplot_out.o $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o sudoprint libplot_out.o $(OBJS) $(TECHNIQUES) sudoprint.cpp -lplotter

testcmd: testcmd.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o testcmd $(OBJS) $(TECHNIQUES) testcmd.cpp

sudosolve: sudosolve.cpp $(OBJS) $(TECHNIQUES)
	g++ $(FLAGS) -o sudosolve $(OBJS) $(TECHNIQUES) sudosolve.cpp



coords.o: coords.cpp coords.h
	g++ $(FLAGS) -c coords.cpp

psout.o: psout.cpp psout.h
	g++ $(FLAGS) -c psout.cpp

backtrack_common.o: backtrack_common.cpp backtrack_common.h coords.h psout.h
	g++ $(FLAGS) -c backtrack_common.cpp

backtrack_naive_unique.o: backtrack_naive_unique.cpp backtrack_naive_unique.h backtrack_common.h coords.h
	g++ $(FLAGS) -c backtrack_naive_unique.cpp

bitmatrix_to_ps.o: bitmatrix_to_ps.h bitmatrix_to_ps.cpp bitmatrix.h coords.h psout.h
	g++ $(FLAGS) -c bitmatrix_to_ps.cpp

bits_to_output.o: bits_to_output.h bits_to_output.cpp bits.h coords.h output.h
	g++ $(FLAGS) -c bits_to_output.cpp

backtrack_to_bits.o: backtrack_to_bits.h backtrack_to_bits.cpp bits.h coords.h backtrack_common.h
	g++ $(FLAGS) -c backtrack_to_bits.cpp

bits.o: bitmatrix.h bits.h bits.cpp coords.h
	g++ $(FLAGS) -c bits.cpp

target.o: bitmatrix.h target.h target.cpp coords.h
	g++ $(FLAGS) -c target.cpp

key.o: key.h key.cpp coords.h
	g++ $(FLAGS) -c key.cpp

keyrank.o: keyrank.h keyrank.cpp key.h coords.h
	g++ $(FLAGS) -c keyrank.cpp

pattern.o: pattern.h pattern.cpp target.h coords.h
	g++ $(FLAGS) -c pattern.cpp

strong_link.o: strong_link.h strong_link.cpp bits.h coords.h
	g++ $(FLAGS) -c strong_link.cpp

digit_chain.o: digit_chain.h digit_chain.cpp strong_link.h bits.h coords.h
	g++ $(FLAGS) -c digit_chain.cpp

simple_complete_gen.o: simple_complete_gen.h simple_complete_gen.cpp backtrack_common.h coords.h
	g++ $(FLAGS) -c simple_complete_gen.cpp

permutation.o: permutation.h permutation.cpp backtrack_common.h coords.h data_permutations.h
	g++ $(FLAGS) -c permutation.cpp

build_data_permutations: build_data_permutations.cpp
	g++ $(FLAGS) -o build_data_permutations build_data_permutations.cpp

data_permutations.h: build_data_permutations
	./build_data_permutations

dlx.o: dlx.cpp dlx.h
	g++ $(FLAGS) -c dlx.cpp

naive_gen.o: naive_gen.cpp naive_gen.h bits.h dlx.h backtrack_common.h backtrack_to_bits.h
	g++ $(FLAGS) -c naive_gen.cpp





simple_naked_single.o: simple_naked_single.h simple_naked_single.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c simple_naked_single.cpp

naked_single.o: naked_single.h naked_single.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c naked_single.cpp

hidden_single.o: hidden_single.h hidden_single.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c hidden_single.cpp

nh_subset.o: nh_subset.h nh_subset.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c nh_subset.cpp

pointing.o: pointing.h pointing.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c pointing.cpp

boxline.o: boxline.h boxline.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c boxline.cpp

nfish.o: nfish.h nfish.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c nfish.cpp

simple_coloring.o: simple_coloring.h simple_coloring.cpp $(TECHHEADERS)
	g++ $(FLAGS) -c simple_coloring.cpp






pretty_solver.o: bits.h pretty_solver.h pretty_solver.cpp $(TECHNIQUES)
	g++ $(FLAGS) -c pretty_solver.cpp

libplot_out.o: libplot_out.h coords.h output.h libplot_out.cpp $(TECHNIQUES)
	g++ $(FLAGS) -c libplot_out.cpp

svgout.o: svgout.h coords.h output.h svgout.cpp $(TECHNIQUES)
	g++ $(FLAGS) -c svgout.cpp



clean:
	@rm -f $(TESTS)
	@rm -f $(OBJS)
	@rm -f $(TECHNIQUES)
	@rm -f $(GUIFILES)
	@rm -f build_data_permutations data_permutations.h
