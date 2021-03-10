#include "Header.h"
#include "Error.h"
#include "NFA.h"
#include "NFH.h"
#include "Ntree.h"
#include "NFH_LanguageBelonging.h"
#include <iostream>
#include <chrono>
using namespace std::chrono;

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
//NFH_LanguageBelonging_test_returnWord
// Change to main() for testing
void NFH_LanguageBelonging_test_returnWord()
{
	// NFA A Build
	int startState_A = 0;
	char alphabet_NFA_A[2] = { 'a', 'b' };
	int* final_states_NFA_A = new int(0);
	int moveMatrix1_A[2][2] = { {-1, 1}, {0, -1 } };
	int* pointerToMoveMatrix1_A[2] = { moveMatrix1_A[0], moveMatrix1_A[1] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A[1] = { pointerToMoveMatrix1_A };
	NFA* A = new NFA(pointerToMoveMatrix_NonDeterministic_A, alphabet_NFA_A, startState_A, final_states_NFA_A, 1, 2, 2, 1);
	// NFH B Build
	int sizeOfAlphaCondition_B = 2;
	int startState_B = 0;
	char alphabet_NFH_B[2] = { 'a', 'b' };
	int* final_states_NFH_B = new int(0);
	char branch1_B[2] = { 'a', '#' };
	char* pointerToBranch1_B = branch1_B;
	char branch2_B[2] = { 'a', 'a' };
	char* pointerToBranch2_B = branch2_B;
	Transition* state1_transition1_B = new Transition(pointerToBranch1_B, 0);
	Transition* state1_transition2_B = new Transition(pointerToBranch2_B, 0);
	Transition* state1_givenTransition_B[2] = { state1_transition1_B, state1_transition2_B };
	StateTransition* state1_B = new StateTransition(state1_givenTransition_B, 2, sizeOfAlphaCondition_B);
	Ntree* moveArray_B[1] = { new Ntree(state1_B) };
	Quantifiers quantifiers_B[2] = { EVERY, EACH };
	AlphaCondition* alphaCondition_B = new AlphaCondition(quantifiers_B, sizeOfAlphaCondition_B);
	NFH* B = new NFH(moveArray_B, alphabet_NFH_B, startState_B, final_states_NFH_B, *alphaCondition_B, 1, 2, 1);
	/* Example 7 - {B, A}															*/
	// The language of A: { epsilon }
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A2's language is not belonging to B's language
	// This time isBelong returns a word from A2 language that does'nt exists in the
	// language of the final NFA (the NFA that creates from intersections until NFH
	// alpha condition's size is 1).
	auto start = std::chrono::high_resolution_clock::now();
	char** word = new char*();
	int size = 0;
	cout << NFH_LanguageBelonging::isBelong(B, A, word, &size); // False
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
}
