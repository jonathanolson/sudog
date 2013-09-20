#ifndef DLX_H
#define DLX_H

/*
Code modified from freely distributed Dancing Links code by Ruud/Havard on the sudoku forums
Adapted to handle bits format, and removed memory links
*/


#include <iostream>
#include "bits.h"
#include "backtrack_common.h"

class CHead;
class CCandidate;
class CCell;

class CNode {
	protected:
	CNode(bool head);
	
	public:
	~CNode();
	CNode(CHead *head, CCandidate *candidate);
	CNode *up;
	CNode *down;
	bool IsHead;
	
	CNode *Peer1;
	CNode *Peer2;
	CNode *Peer3;
	CHead *Head;
	CCandidate *Candidate;
};

class CHead : public CNode {
	public:
	CHead();
	CHead(CHead *root);
	~CHead();
	CHead *left;
	CHead *right;
	bool IsRoot;
	void Cover();
	void Uncover();
	int size;
};

class CCandidate {
	public:
	CCandidate(CCell *cell, int digit);
	~CCandidate();
	CCell *Cell;
	int Digit;
	CNode *CelNode;
	CNode *RowNode;
	CNode *ColNode;
	CNode *BoxNode;
	void Disable();
	void Enable();
	bool Disabled;
};

class CCell {
	public:
	CCell(int index);
	~CCell();
	int Index;
	int RowOffset;
	int ColOffset;
	int BoxOffset;
	int Given;     // Nonzero for a cell with a given digit
	int Selected;  // Currently selected digit in the DLX algorithm
	int Solution;  // Digit recorded in the (first) solution
	CCandidate **Candidates;
	void SetGiven(char digit);
	void ClearGiven();
};

class CSolver {
	private:
	CHead *Root;
	CHead *celHdr[81];
	CHead *rowHdr[81];
	CHead *colHdr[81];
	CHead *boxHdr[81];
	
	int solution_count;
	int SolutionLimit;
	bool Abort;
	void Recurse();
	
	public:
	
	CSolver();
	~CSolver();
	
	int Solve(bool all);
	CCell *Cells[81];
};

void csolver_load_bits(CSolver* solver, Bits* bits);
void csolver_unload_bits(CSolver* solver, Bits* bits);

bool csolver_unique_check(CSolver* solver, Bits* bits);
int csolver_soln_count(CSolver* solver, Bits* bits);

void csolver_load_bits(CSolver* solver, Grid& grid);
void csolver_unload_bits(CSolver* solver, Grid& grid);

bool csolver_unique_check(CSolver* solver, Grid& grid);
int csolver_soln_count(CSolver* solver, Grid& grid);

#endif




