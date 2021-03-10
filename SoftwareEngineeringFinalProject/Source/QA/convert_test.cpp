#include "Header.h"
#include "Error.h"
#include "NFA.h"
#include "NFH.h"
#include "Ntree.h"
#include "Convert.h"

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
//convert_test
// Change to main() for testing
void convert_test()
{
	// NFA Build
	char alphabet_NFA_4chars[4] = { 'a', 'b', 'c', 'd' };
	int* final_states = (int*)malloc(sizeof(int));
	Error::checkMalloc((void*)final_states, 1);
	final_states[0] = 1;
	int moveMatrix1_four_state[4][4] = { {NO_THOROUGHFARE, NO_THOROUGHFARE, NO_THOROUGHFARE, NO_THOROUGHFARE}, {0, 1, 1, 1}, {1, 1, 1, 3}, {2, 2, 3, 0} };
	int* pointerToMoveMatrix1_four_state[4] = { moveMatrix1_four_state[0], moveMatrix1_four_state[1], moveMatrix1_four_state[2], moveMatrix1_four_state[3] };
	int moveMatrix2_four_state[4][4] = { {NO_THOROUGHFARE, NO_THOROUGHFARE, NO_THOROUGHFARE, NO_THOROUGHFARE}, {0, 1, 1, 1}, {1, 1, 1, 0}, {2, 2, 3, 0} };
	int* pointerToMoveMatrix2_four_state[4] = { moveMatrix2_four_state[0], moveMatrix2_four_state[1], moveMatrix2_four_state[2], moveMatrix2_four_state[3] };
	int** pointerToMoveMatrix_NonDeterministic_four_state[4] = { pointerToMoveMatrix1_four_state, pointerToMoveMatrix2_four_state };
	NFA four_state(pointerToMoveMatrix_NonDeterministic_four_state, alphabet_NFA_4chars, 0, final_states, 2, 4, 4, 1);
	char alphabet_NFA[2] = { 'a', 'b' };
	int moveMatrix1_A[2][2] = { {1, 0}, {0, 1} };
	int* pointerToMoveMatrix1_A[2] = { moveMatrix1_A[0], moveMatrix1_A[1] }; // pointer to moveMatrix1
	int moveMatrix2_A[2][2] = { {1, 1}, {0, 1} };
	int* pointerToMoveMatrix2_A[2] = { moveMatrix2_A[0], moveMatrix2_A[1] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A[2] = { pointerToMoveMatrix1_A , pointerToMoveMatrix2_A };
	NFA A(pointerToMoveMatrix_NonDeterministic_A, alphabet_NFA, 0, final_states, 2, 2, 2, 1);
	// NFH Build
	char alphabet_NFH[2] = { 'a', 'b'};
	char branch1[1] = { 'a' };
	char* pointerToBranch1 = branch1;
	char branch2[1] = { 'a' };
	char* pointerToBranch2 = branch2;
	char branch3[1] = { 'b' };
	char* pointerToBranch3 = branch3;
	char branch4[1] = { 'b' };
	char* pointerToBranch4 = branch4;
	char branch5[1] = { 'b' };
	char* pointerToBranch5 = branch5;
	Transition* state1_transition1 = new Transition(pointerToBranch1, 0);
	Transition* state1_transition2 = new Transition(pointerToBranch2, 1);
	Transition* state1_givenTransition[2] = { state1_transition1 , state1_transition2 };
	StateTransition* state1 = new StateTransition(state1_givenTransition, 2, 1);
	Transition* state2_transition1 = new Transition(pointerToBranch3, 2);
	Transition* state2_transition2 = new Transition(pointerToBranch4, 1);
	Transition* state2_transition3 = new Transition(pointerToBranch5, 0);
	Transition* state2_givenTransition[3] = { state2_transition1, state2_transition2, state2_transition3 };
	StateTransition* state2 = new StateTransition(state2_givenTransition, 3, 1);
	Ntree* moveArray[3] = { new Ntree(state1), new Ntree(state2), NULL };
	Quantifiers quantifiers[2] = { EVERY };
	AlphaCondition* alphaCondition = new AlphaCondition(quantifiers, 1);
	NFH* B = new NFH(moveArray, alphabet_NFH, 0, final_states, *alphaCondition, 3, 2, 1);
	/****************************************************************************************************************/
	/* convert_NFA_to_DFA */
	DFA* A_convert = Convert::convert_NFA_to_DFA(A);
	A_convert->toString();
	DFA* four_state_convert = Convert::convert_NFA_to_DFA(four_state);
	four_state_convert->toString();
	/* convert_NFH_to_NFA */
	NFA* B_convert = Convert::convert_NFH_to_NFA(B);
	B_convert->toString();
	/* convert_NFH_to_DFH */
	DFH* B_convert_DFH = Convert::convert_NFH_to_DFH(B);
	/*  free all the allocated memory    */
	free(final_states);
}
