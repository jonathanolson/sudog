#ifndef BITMATRIX_TO_PS
#define BITMATRIX_TO_PS

#include "psout.h"
#include "bitmatrix.h"

void ps_write_numbers(BitMatrix* b, Psout* ps);
void ps_write_candidates(BitMatrix* b, Psout* ps);
void ps_write_bitmatrix(BitMatrix* b, Psout* ps);

#endif
