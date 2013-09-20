
#include "dlx.h"

/*
Code modified from freely distributed Dancing Links code by Ruud/Havard on the sudoku forums
Adapted to handle bits format, and removed memory links
*/

const int SOLUTION_LIMIT = 100000;
const int BOX_WIDTH = 3;
const int BOX_HEIGHT = 3;
const int UNIT_SIZE = 9;
const int GRID_SIZE = 81;

CNode::CNode(bool head) {
   IsHead = head;
}

CNode::~CNode() {
}

CNode::CNode(CHead *head, CCandidate *cand) {
	IsHead = false;
	Head = head;
	up = Head->up;
	down = Head;
	up->down = this;
	down->up = this;
	Head->size++;
	Candidate = cand;
}

CHead::CHead() : CNode(true) {
	IsRoot = true;
	IsHead = true;
	size = 99;
	up = down = left = right = this;
}

CHead::~CHead() {
}

CHead::CHead(CHead *root) : CNode(true) {
	IsRoot = false;
	IsHead = true;
	size = 0;
	left = root->left;
	right = root;
	up = down = left->right = right->left = this;
}

void CHead::Cover() {
	left->right = right;
	right->left = left;
	
	for (CNode *b = down; !b->IsHead ; b = b->down) {
		b->Peer1->down->up = b->Peer1->up;
		b->Peer1->up->down = b->Peer1->down;
		b->Peer1->Head->size--;
		
		b->Peer2->down->up = b->Peer2->up;
		b->Peer2->up->down = b->Peer2->down;
		b->Peer2->Head->size--;
		
		b->Peer3->down->up = b->Peer3->up;
		b->Peer3->up->down = b->Peer3->down;
		b->Peer3->Head->size--;
	}
}

void CHead::Uncover() {
	for (CNode *b = up; !b->IsHead; b = b->up) {
		b->Peer3->down->up = b->Peer3->up->down = b->Peer3;
		b->Peer3->Head->size++;
		
		b->Peer2->down->up = b->Peer2->up->down = b->Peer2;
		b->Peer2->Head->size++;
		
		b->Peer1->down->up = b->Peer1->up->down = b->Peer1;
		b->Peer1->Head->size++;
	}
	
	left->right = right->left = this;
}

CCandidate::CCandidate(CCell *cell, int dx) {
	this->Cell = cell;
	Digit = dx+1;
	
	Disabled = false;
}

CCandidate::~CCandidate() {
	delete CelNode;
	delete RowNode;
	delete ColNode;
	delete BoxNode;
}

void CCandidate::Disable() {
	if (!Disabled) {
		Disabled = true;
		
		CelNode->up->down = CelNode->down;
		CelNode->down->up = CelNode->up;
		CelNode->up = CelNode->down = CelNode;
		CelNode->Head->size--;
		
		RowNode->up->down = RowNode->down;
		RowNode->down->up = RowNode->up;
		RowNode->up = RowNode->down = RowNode;
		RowNode->Head->size--;
		
		ColNode->up->down = ColNode->down;
		ColNode->down->up = ColNode->up;
		ColNode->up = ColNode->down = ColNode;
		ColNode->Head->size--;
		
		BoxNode->up->down = BoxNode->down;
		BoxNode->down->up = BoxNode->up;
		BoxNode->up = BoxNode->down = BoxNode;
		BoxNode->Head->size--;
	}
}

void CCandidate::Enable() {
	if (Disabled) {
		BoxNode->up = BoxNode->Head->up;
		BoxNode->down = BoxNode->Head;
		BoxNode->up->down = BoxNode->down->up = BoxNode;
		BoxNode->Head->size++;
		
		ColNode->up = ColNode->Head->up;
		ColNode->down = ColNode->Head;
		ColNode->up->down = ColNode->down->up = ColNode;
		ColNode->Head->size++;
		
		RowNode->up = RowNode->Head->up;
		RowNode->down = RowNode->Head;
		RowNode->up->down = RowNode->down->up = RowNode;
		RowNode->Head->size++;
		
		CelNode->up = CelNode->Head->up;
		CelNode->down = CelNode->Head;
		CelNode->up->down = CelNode->down->up = CelNode;
		CelNode->Head->size++;
		
		Disabled = false;
	}
}

CCell::CCell(int index) {
	Index = index;
	int rx = index / UNIT_SIZE;
	int cx = index % UNIT_SIZE;
	RowOffset = rx * UNIT_SIZE;
	ColOffset = cx * UNIT_SIZE;
	BoxOffset = (BOX_HEIGHT * (rx / BOX_HEIGHT) + cx / BOX_WIDTH) * UNIT_SIZE;
	Given = 0;
	Selected = 0;
	Solution = 0;
	Candidates = new CCandidate*[UNIT_SIZE];
}

CCell::~CCell() {
	delete[] Candidates;
}

void CCell::SetGiven(char digit) {
	Given = digit;
	for (int dx = 0; dx < UNIT_SIZE; dx++) {
		if (dx != (Given-1)) Candidates[dx]->Disable();
	}
}

void CCell::ClearGiven() {
	for (int dx = UNIT_SIZE-1; dx >= 0; dx--) {
		if (dx != (Given-1)) Candidates[dx]->Enable();
	}
	Given = 0;
}

CSolver::CSolver() {
	Root = new CHead();
	
	for(int n = 0; n < GRID_SIZE; ++n) {
		Cells[n] = new CCell(n);
		celHdr[n] = new CHead(Root);
		rowHdr[n] = new CHead(Root);
		colHdr[n] = new CHead(Root);
		boxHdr[n] = new CHead(Root);
	}
	
	for (int cx = 0; cx < GRID_SIZE; cx++) {
		for (int dx = 0; dx < UNIT_SIZE; dx++) {
			Cells[cx]->Candidates[dx] = new CCandidate(Cells[cx],dx);
			Cells[cx]->Candidates[dx]->CelNode = new CNode(celHdr[cx],Cells[cx]->Candidates[dx]);
			Cells[cx]->Candidates[dx]->RowNode = new CNode(rowHdr[Cells[cx]->RowOffset+dx],Cells[cx]->Candidates[dx]);
			Cells[cx]->Candidates[dx]->ColNode = new CNode(colHdr[Cells[cx]->ColOffset+dx],Cells[cx]->Candidates[dx]);
			Cells[cx]->Candidates[dx]->BoxNode = new CNode(boxHdr[Cells[cx]->BoxOffset+dx],Cells[cx]->Candidates[dx]);
			
			Cells[cx]->Candidates[dx]->CelNode->Peer1 = Cells[cx]->Candidates[dx]->RowNode;
			Cells[cx]->Candidates[dx]->CelNode->Peer2 = Cells[cx]->Candidates[dx]->ColNode;
			Cells[cx]->Candidates[dx]->CelNode->Peer3 = Cells[cx]->Candidates[dx]->BoxNode;
			
			Cells[cx]->Candidates[dx]->RowNode->Peer1 = Cells[cx]->Candidates[dx]->CelNode;
			Cells[cx]->Candidates[dx]->RowNode->Peer2 = Cells[cx]->Candidates[dx]->ColNode;
			Cells[cx]->Candidates[dx]->RowNode->Peer3 = Cells[cx]->Candidates[dx]->BoxNode;
			
			Cells[cx]->Candidates[dx]->ColNode->Peer1 = Cells[cx]->Candidates[dx]->CelNode;
			Cells[cx]->Candidates[dx]->ColNode->Peer2 = Cells[cx]->Candidates[dx]->RowNode;
			Cells[cx]->Candidates[dx]->ColNode->Peer3 = Cells[cx]->Candidates[dx]->BoxNode;
			
			Cells[cx]->Candidates[dx]->BoxNode->Peer1 = Cells[cx]->Candidates[dx]->CelNode;
			Cells[cx]->Candidates[dx]->BoxNode->Peer2 = Cells[cx]->Candidates[dx]->RowNode;
			Cells[cx]->Candidates[dx]->BoxNode->Peer3 = Cells[cx]->Candidates[dx]->ColNode;
		}
	}
}

CSolver::~CSolver() {
	for (int cx = 0; cx < GRID_SIZE; cx++) {
		for (int dx = 0; dx < UNIT_SIZE; dx++) {
			delete Cells[cx]->Candidates[dx];
		}
	}
	delete Root;
	for(int n=0; n < GRID_SIZE; ++n) {
		delete Cells[n];
		delete celHdr[n];
		delete rowHdr[n];
		delete colHdr[n];
		delete boxHdr[n];
	}
}

int CSolver::Solve(bool all) {
	if (all) SolutionLimit = SOLUTION_LIMIT; else SolutionLimit = 2;
	Abort = false;
	solution_count = 0;
	Recurse();
	return solution_count;
}

void CSolver::Recurse() {
	if (Root->right->IsRoot) {
		solution_count++;
		if (solution_count>1)
			Abort = (solution_count >= SolutionLimit);
		else
			for (int cx = 0; cx < GRID_SIZE; cx++) Cells[cx]->Solution = Cells[cx]->Selected;
		return;
	}
	
	CHead *col = Root->right;
	
	if (col->size > 1) {
		for (CHead *c = col->right; !c->IsRoot; c = c->right) {
			if (c->size < col->size) {
				col = c;
				if (col->size <= 1) break;
			}
		}
	}
	
	if (col->size == 0) return;
	
	col->Cover();
	
	for (CNode *rn = col->down; !rn->IsHead; rn = rn->down) {
		
		rn->Candidate->Cell->Selected = rn->Candidate->Digit;
		rn->Peer1->Head->Cover();
		rn->Peer2->Head->Cover();
		rn->Peer3->Head->Cover();
		
		Recurse();
		
		rn->Peer3->Head->Uncover();
		rn->Peer2->Head->Uncover();
		rn->Peer1->Head->Uncover();
		
		if (Abort) break;
	
	}
	
	col->Uncover();
}

void csolver_load_bits(CSolver* solver, Bits* bits) {
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(bits->has_value(p)) {
			solver->Cells[i]->SetGiven(bits->get_value(p));
		}
	}
}

void csolver_unload_bits(CSolver* solver, Bits* bits) {
	for(int i=0; i < 81; i++) {
		Spot p = spot_of_idx(i);
		if(bits->has_value(p)) {
			solver->Cells[i]->ClearGiven();
		}
	}
}

bool csolver_unique_check(CSolver* solver, Bits* bits) {
	csolver_load_bits(solver, bits);
	int result = solver->Solve(false);
	csolver_unload_bits(solver, bits);
	return (result == 1);
}

int csolver_soln_count(CSolver* solver, Bits* bits) {
	csolver_load_bits(solver, bits);
	int result = solver->Solve(true);
	csolver_unload_bits(solver, bits);
	return result;
}




void csolver_load_bits(CSolver* solver, Grid& grid) {
	for(int i=0; i < 81; i++) {
		if(grid.getmanual(i) != 0) {
			solver->Cells[i]->SetGiven(grid.getmanual(i));
		}
	}
}

void csolver_unload_bits(CSolver* solver, Grid& grid) {
	for(int i=0; i < 81; i++) {
		if(grid.getmanual(i) != 0) {
			solver->Cells[i]->ClearGiven();
		}
	}
}

bool csolver_unique_check(CSolver* solver, Grid& grid) {
	csolver_load_bits(solver, grid);
	int result = solver->Solve(false);
	csolver_unload_bits(solver, grid);
	return (result == 1);
}

int csolver_soln_count(CSolver* solver, Grid& grid) {
	csolver_load_bits(solver, grid);
	int result = solver->Solve(true);
	csolver_unload_bits(solver, grid);
	return result;
}

