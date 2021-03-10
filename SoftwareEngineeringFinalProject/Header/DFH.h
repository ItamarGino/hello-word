#ifndef DFH_H
#define DFH_H
#include "Header.h"
#include "NFH.h"
#include "Ntree.h"

/*	Non-deterministic Finite Hyper-Automaton							*/
/*	MoveArray: cells indicates states.
			   each cell represents a legitimate sequence of letters.	*/
class DFH {
public:
	NFH* nfh;
	// Constructor - Initializes NFH
	DFH(Ntree** givenMoveArray, char* givenAlphaBet, int givenStarterState, int* givenFinalStates, AlphaCondition givenAlphaCondition,
		int givenNuberOfStates, int givenSizeOfAlphaBet, int givenSizeOfFinalStates) {
		nfh = new NFH(givenMoveArray, givenAlphaBet, givenStarterState, givenFinalStates, givenAlphaCondition,
			givenNuberOfStates, givenSizeOfAlphaBet, givenSizeOfFinalStates);
	}
	//	Destructor
	~DFH() { delete nfh; }
	// Switch the current state
	int switchState(char* letters) { nfh->switchState(letters); }
	/*	Just for QA!
		(This function does'nt include alpha condition)	*/
		// Gets a word and return if the word belong to the language of the NFA
	bool isBelong(char** hyperWord, int numberOfWords) { nfh->isBelong(hyperWord, numberOfWords); }
	// Creates and returns the complement DFA
	DFH* complement() {
		int* newFinalStates = (int*)malloc(sizeof(int) * (nfh->numberOfStates - nfh->sizeOfFinalStates));
		int newFinalStatesIndex = 0;
		Error::checkMalloc(newFinalStates, (nfh->numberOfStates - nfh->sizeOfFinalStates));
		// Swaps DFA's accepting states with its non-accepting states and vice versa
		for (int stateIndex = 0; stateIndex < nfh->numberOfStates; stateIndex++) {
			bool isNewFinalState = true;
			for (int finalStateIndex = 0; finalStateIndex < nfh->sizeOfFinalStates; finalStateIndex++)
				if (nfh->finalStates[finalStateIndex] == stateIndex) isNewFinalState = false;
			if (isNewFinalState) newFinalStates[newFinalStatesIndex++] = stateIndex;
		}
		// New alphabet
		char* newAlphaBet = (char*)malloc(sizeof(char) * nfh->sizeOfAlphaBet);
		Error::checkMalloc(newAlphaBet, nfh->sizeOfAlphaBet);
		for (int charIndex = 0; charIndex < nfh->sizeOfAlphaBet; charIndex++) newAlphaBet[charIndex] = nfh->alphaBet[charIndex];
		for (int stateIndex = 0; stateIndex < nfh->sizeOfFinalStates; stateIndex++) newFinalStates[stateIndex] = nfh->finalStates[stateIndex];
		DFH* returnDFH = new DFH(nfh->moveArray, newAlphaBet, nfh->starterState, newFinalStates, nfh->alphaCondition,
									nfh->numberOfStates, nfh->sizeOfAlphaBet, (nfh->numberOfStates - nfh->sizeOfFinalStates));
		return returnDFH;
	}
};

#endif
