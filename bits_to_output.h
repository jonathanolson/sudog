#ifndef BITS_TO_OUTPUT_H
#define BITS_TO_OUTPUT_H

#include "bits.h"
#include "output.h"

void write_numbers(Bits* b, Output* out);
void write_candidates(Bits* b, Output* out);
void write_val_candidates(Bits* b, Output* out, int val);

#endif
