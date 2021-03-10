#include "Header.h"
#include "Error.h"
#include "DFA.h"
#include <iostream>
#include <string>
using namespace std;

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
// Change to main() for testing
void complement_test()
{
	char alphabet_NFA[2] = { 'a', 'b' };
	int moveMatrix1_A[2][2] = { {1, 0}, {0, 1} };
	int* pointerToMoveMatrix1_A[2] = { moveMatrix1_A[0], moveMatrix1_A[1] }; // pointer to moveMatrix1
	int moveMatrix2_A[2][2] = { {1, 1}, {0, 1} };
	int* pointerToMoveMatrix2_A[2] = { moveMatrix2_A[0], moveMatrix2_A[1] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A[2] = { pointerToMoveMatrix1_A , pointerToMoveMatrix2_A };
	int* final_states = (int*)malloc(sizeof(int));
	Error::checkMalloc((void*)final_states, 1);
	final_states[0] = 1;
	DFA dfa1(pointerToMoveMatrix1_A, alphabet_NFA, 0, final_states, 2, 2, 1);
	DFA dfa2(pointerToMoveMatrix2_A, alphabet_NFA, 0, final_states, 2, 2, 1);
	/****************************************************************************************************************/
	/* Complement DFA Example  */
	dfa1.complement()->toString();
	dfa2.complement()->toString();
	/*  free all the allocated memory    */
	free(final_states);
}
