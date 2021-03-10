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
//NFH_LanguageBelonging_test
// Change to main() for testing
void main()
{
	/* Build for example 1 */
	// NFA A1 Build
	int startState_A1 = 0;
	char alphabet_NFA_A1[1] = { 'a' };
	int* final_states_NFA_A1 = new int(0);
	int moveMatrix1_A1[1][1] = { {0} };
	int* pointerToMoveMatrix1_A1[1] = { moveMatrix1_A1[0] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A1[1] = { pointerToMoveMatrix1_A1 };
	NFA* A1 = new NFA(pointerToMoveMatrix_NonDeterministic_A1, alphabet_NFA_A1, startState_A1, final_states_NFA_A1, 1, 1, 1, 1);
	// NFA A2 Build
	int startState_A2 = 0;
	char alphabet_NFA_A2[1] = { 'a' };
	int* final_states_NFA_A2 = NULL;
	int moveMatrix1_A2[1][1] = { {0} };
	int* pointerToMoveMatrix1_A2[1] = { moveMatrix1_A2[0] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A2[1] = { pointerToMoveMatrix1_A2 };
	NFA* A2 = new NFA(pointerToMoveMatrix_NonDeterministic_A2, alphabet_NFA_A2, startState_A2, final_states_NFA_A2, 1, 1, 1, 0);
	// NFA A3 Build
	int startState_A3 = 0;
	char alphabet_NFA_A3[1] = { 'a' };
	int* final_states_NFA_A3 = new int(2);
	int moveMatrix1_A3[3][1] = { {1}, {2}, {2} };
	int* pointerToMoveMatrix1_A3[3] = { moveMatrix1_A3[0], moveMatrix1_A3[1], moveMatrix1_A3[2] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A3[1] = { pointerToMoveMatrix1_A3 };
	NFA* A3 = new NFA(pointerToMoveMatrix_NonDeterministic_A3, alphabet_NFA_A3, startState_A3, final_states_NFA_A3, 1, 3, 1, 1);
	// NFA A4 Build
	int startState_A4 = 0;
	char alphabet_NFA_A4[1] = { 'a' };
	int* final_states_NFA_A4 = new int(1);
	int moveMatrix1_A4[2][1] = { {1}, {1} };
	int* pointerToMoveMatrix1_A4[2] = { moveMatrix1_A4[0], moveMatrix1_A4[1] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A4[1] = { pointerToMoveMatrix1_A4 };
	NFA* A4 = new NFA(pointerToMoveMatrix_NonDeterministic_A4, alphabet_NFA_A4, startState_A4, final_states_NFA_A4, 1, 2, 1, 1);
	// NFA A5 Build
	int startState_A5 = 0;
	char alphabet_NFA_A5[1] = { 'a' };
	int* final_states_NFA_A5 = new int(2);
	int moveMatrix1_A5[3][1] = { {1}, {2}, {-1} };
	int* pointerToMoveMatrix1_A5[3] = { moveMatrix1_A5[0], moveMatrix1_A5[1], moveMatrix1_A5[2] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A5[1] = { pointerToMoveMatrix1_A5 };
	NFA* A5 = new NFA(pointerToMoveMatrix_NonDeterministic_A5, alphabet_NFA_A5, startState_A5, final_states_NFA_A5, 1, 3, 1, 1);
	// NFA A6 Build
	int startState_A6 = 0;
	char alphabet_NFA_A6[1] = { 'a' };
	int* final_states_NFA_A6 = new int(3);
	int moveMatrix1_A6[4][1] = { {1}, {2}, {3}, {3} };
	int* pointerToMoveMatrix1_A6[4] = { moveMatrix1_A6[0], moveMatrix1_A6[1], moveMatrix1_A6[2], moveMatrix1_A6[3] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A6[1] = { pointerToMoveMatrix1_A6 };
	NFA* A6 = new NFA(pointerToMoveMatrix_NonDeterministic_A6, alphabet_NFA_A6, startState_A6, final_states_NFA_A6, 1, 4, 1, 1);
	// NFA A7 Build
	int startState_A7 = 0;
	char alphabet_NFA_A7[1] = { 'a' };
	int* final_states_NFA_A7 = new int(4);
	int moveMatrix1_A7[5][1] = { {1}, {2}, {3}, {4}, {4} };
	int* pointerToMoveMatrix1_A7[5] = { moveMatrix1_A7[0], moveMatrix1_A7[1], moveMatrix1_A7[2], moveMatrix1_A6[3], moveMatrix1_A6[4] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A7[1] = { pointerToMoveMatrix1_A7 };
	NFA* A7 = new NFA(pointerToMoveMatrix_NonDeterministic_A7, alphabet_NFA_A7, startState_A7, final_states_NFA_A7, 1, 5, 1, 1);
	// NFH B Build
	int sizeOfAlphaCondition_B = 2;
	int startState_B = 0;
	char alphabet_NFH_B[1] = { 'a' };
	int* final_states_NFH_B = new int(1);
	char branch1_B[2] = { 'a', 'a' };
	char* pointerToBranch1_B = branch1_B;
	char branch2_B[2] = { '#', 'a' };
	char* pointerToBranch2_B = branch2_B;
	char branch3_B[2] = { '#', 'a' };
	char* pointerToBranch3_B = branch3_B;
	Transition* state1_transition1_B = new Transition(pointerToBranch1_B, 0);
	Transition* state1_transition2_B = new Transition(pointerToBranch2_B, 1);
	Transition* state1_givenTransition_B[2] = { state1_transition1_B , state1_transition2_B };
	StateTransition* state1_B = new StateTransition(state1_givenTransition_B, 2, sizeOfAlphaCondition_B);
	Transition* state2_transition1_B = new Transition(pointerToBranch3_B, 1);
	Transition* state2_givenTransition_B[1] = { state2_transition1_B };
	StateTransition* state2_B = new StateTransition(state2_givenTransition_B, 1, sizeOfAlphaCondition_B);
	Ntree* moveArray_B[2] = { new Ntree(state1_B), new Ntree(state2_B) };
	Quantifiers quantifiers_B[2] = { EVERY, EACH };
	AlphaCondition* alphaCondition_B = new AlphaCondition(quantifiers_B, sizeOfAlphaCondition_B);
	NFH* B = new NFH(moveArray_B, alphabet_NFH_B, startState_B, final_states_NFH_B, *alphaCondition_B, 2, 1, 1);
	// NFH E Build
	int sizeOfAlphaCondition_E = 2;
	int startState_E = 0;
	char alphabet_NFH_E[1] = { 'a' };
	int* final_states_NFH_E = new int(0);
	char branch1_E[2] = { 'a', '#' };
	char* pointerToBranch1_E = branch1_E;
	Transition* state1_transition1_E = new Transition(pointerToBranch1_E, 0);
	Transition* state1_givenTransition_E[1] = { state1_transition1_E };
	StateTransition* state1_E = new StateTransition(state1_givenTransition_E, 1, sizeOfAlphaCondition_E);
	Ntree* moveArray_E[1] = { new Ntree(state1_E) };
	Quantifiers quantifiers_E[2] = { EACH, EACH };
	AlphaCondition* alphaCondition_E = new AlphaCondition(quantifiers_E, sizeOfAlphaCondition_E);
	NFH* E = new NFH(moveArray_E, alphabet_NFH_E, startState_E, final_states_NFH_E, *alphaCondition_E, 1, 1, 1);
	// NFH E1 Build
	int sizeOfAlphaCondition_E1 = 3;
	int startState_E1= 0;
	char alphabet_NFH_E1[1] = { 'a' };
	int* final_states_NFH_E1 = new int(0);
	char branch1_E1[3] = { 'a','a','#' };
	char* pointerToBranch1_E1 = branch1_E1;
	char branch2_E1[3] = { 'a', '#', '#' };
	char* pointerToBranch2_E1 = branch2_E1;
	Transition* state1_transition1_E1 = new Transition(pointerToBranch1_E1, 0);
	Transition* state1_transition2_E1 = new Transition(pointerToBranch2_E1, 0);
	Transition* state1_givenTransition_E1[2] = { state1_transition1_E1, state1_transition2_E1 };
	StateTransition* state1_E1 = new StateTransition(state1_givenTransition_E1, 2, sizeOfAlphaCondition_E1);
	Ntree* moveArray_E1[1] = { new Ntree(state1_E1) };
	Quantifiers quantifiers_E1[3] = { EACH, EACH, EACH };
	AlphaCondition* alphaCondition_E1 = new AlphaCondition(quantifiers_E1, sizeOfAlphaCondition_E1);
	NFH* E1 = new NFH(moveArray_E1, alphabet_NFH_E1, startState_E1, final_states_NFH_E1, *alphaCondition_E1, 1, 1, 1);
	// NFH E2 Build
	int sizeOfAlphaCondition_E2 = 4;
	int startState_E2 = 0;
	char alphabet_NFH_E2[1] = { 'a' };
	int* final_states_NFH_E2 = new int(0);
	char branch1_E2[4] = { 'a', 'a', 'a', '#' };
	char* pointerToBranch1_E2 = branch1_E2;
	char branch2_E2[4] = { 'a', 'a', '#', '#' };
	char* pointerToBranch2_E2 = branch2_E2;
	char branch3_E2[4] = { 'a', '#', '#', '#' };
	char* pointerToBranch3_E2 = branch3_E2;
	Transition* state1_transition1_E2 = new Transition(pointerToBranch1_E2, 0);
	Transition* state1_transition2_E2 = new Transition(pointerToBranch2_E2, 0);
	Transition* state1_transition3_E2 = new Transition(pointerToBranch3_E2, 0);
	Transition* state1_givenTransition_E2[3] = { state1_transition1_E2, state1_transition2_E2, state1_transition3_E2 };
	StateTransition* state1_E2 = new StateTransition(state1_givenTransition_E2, 3, sizeOfAlphaCondition_E2);
	Ntree* moveArray_E2[1] = { new Ntree(state1_E2) };
	Quantifiers quantifiers_E2[4] = { EACH, EACH, EACH, EACH };
	AlphaCondition* alphaCondition_E2 = new AlphaCondition(quantifiers_E2, sizeOfAlphaCondition_E2);
	NFH* E2 = new NFH(moveArray_E2, alphabet_NFH_E2, startState_E2, final_states_NFH_E2, *alphaCondition_E2, 1, 1, 1);
	/* Build for example 2 */
	// NFA C Build
	int startState_C = 0;
	char alphabet_NFA_C[2] = { 'a', 'b' };
	int* final_states_NFA_C = new int(1);
	int moveMatrix1_C[2][2] = { {1, -1}, {-1, 1} };
	int* pointerToMoveMatrix1_C[2] = { moveMatrix1_C[0], moveMatrix1_C[1] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_C[1] = { pointerToMoveMatrix1_C };
	NFA* C = new NFA(pointerToMoveMatrix_NonDeterministic_C, alphabet_NFA_C, startState_C, final_states_NFA_C, 1, 2, 2, 1);
	// NFA C1 Build
	int startState_C1 = 0;
	char alphabet_NFA_C1[2] = { 'a', 'b' };
	int* final_states_NFA_C1 = new int(0);
	int moveMatrix1_C1[1][2] = { {1, -1} };
	int* pointerToMoveMatrix1_C1[1] = { moveMatrix1_C1[0] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_C1[1] = { pointerToMoveMatrix1_C1 };
	NFA* C1 = new NFA(pointerToMoveMatrix_NonDeterministic_C1, alphabet_NFA_C1, startState_C1, final_states_NFA_C1, 1, 1, 2, 1);
	// NFH D Build
	int sizeOfAlphaCondition_D = 2;
	int startState_D = 0;
	char alphabet_NFH_D[2] = { 'a', 'b' };
	int* final_states_NFH_D = (int*)malloc(sizeof(int) * 3);
	Error::checkMalloc(final_states_NFH_D, 3);
	final_states_NFH_D[0] = 0; final_states_NFH_D[1] = 1; final_states_NFH_D[2] = 2;
	char branch1_D[2] = { 'a', 'a' };
	char* pointerToBranch1_D = branch1_D;
	char branch2_D[2] = { 'b', 'b' };
	char* pointerToBranch2_D = branch2_D;
	char branch3_D[2] = { '#', 'b' };
	char* pointerToBranch3_D = branch3_D;
	char branch4_D[2] = { 'b', '#' };
	char* pointerToBranch4_D = branch4_D;
	char branch5_D[2] = { '#', 'b' };
	char* pointerToBranch5_D = branch5_D;
	char branch6_D[2] = { 'b', '#' };
	char* pointerToBranch6_D = branch6_D;
	Transition* state1_transition1_D = new Transition(pointerToBranch1_D, 0);
	Transition* state1_transition2_D = new Transition(pointerToBranch2_D, 0);
	Transition* state1_transition3_D = new Transition(pointerToBranch3_D, 1);
	Transition* state1_transition4_D = new Transition(pointerToBranch4_D, 2);
	Transition* state1_givenTransition_D[4] = { state1_transition1_D , state1_transition2_D, state1_transition3_D, state1_transition4_D };
	StateTransition* state1_D = new StateTransition(state1_givenTransition_D, 4, sizeOfAlphaCondition_D);
	Transition* state2_transition1_D = new Transition(pointerToBranch5_D, 1);
	Transition* state2_givenTransition_D[1] = { state2_transition1_D };
	StateTransition* state2_D = new StateTransition(state2_givenTransition_D, 1, sizeOfAlphaCondition_D);
	Transition* state3_transition1_D = new Transition(pointerToBranch6_D, 2);
	Transition* state3_givenTransition_D[1] = { state3_transition1_D };
	StateTransition* state3_D = new StateTransition(state3_givenTransition_D, 1, sizeOfAlphaCondition_D);
	Ntree* moveArray_D[3] = { new Ntree(state1_D), new Ntree(state2_D), new Ntree(state3_D) };
	Quantifiers quantifiers_D[2] = { EVERY, EVERY };
	AlphaCondition* alphaCondition_D = new AlphaCondition(quantifiers_D, sizeOfAlphaCondition_D);
	NFH* D = new NFH(moveArray_D, alphabet_NFH_D, startState_D, final_states_NFH_D, *alphaCondition_D, 3, 2, 3);
	/****************************************************************************************************************/
	/* isBelong */
	cout << "NFH_LanguageBelonging Example:\n";
	std::cout << std::boolalpha; // output 1 to true; output 0 to false
	/* Example 1 - {B, A1}		-																	*/
	// The language of A1: {epsilon, a, aa, aaa, aaaa, ...}
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A1's language is belonging to B's language
	auto start = std::chrono::high_resolution_clock::now();
	cout << NFH_LanguageBelonging::isBelong(B, A1); // True
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 2 - {B, A2}		-																	*/
	// The language of A2: Empty language
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A2's language is not belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(B, A2); // False
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A2: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 3 - {B, A3}		-																	*/
	// The language of A3: { All the possible words with size bigger than 2 }
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A3's language is belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(B, A3); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A3: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 4 - {B, A4}			-																*/
	// The language of A3: { All the possible words with size bigger than 1 }
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A4's language is belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(B, A4); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A4: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 5 - {D, C}			-																*/
	// The language of C: { a, ab, abb, abbb, ... }
	// The language of D: { All languages that accepts the 'a' or 'b' position and ends with sequence of 'b' }
	// C's language is belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(D, C); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for D, C: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 6 - {D, C1}			-																*/
	// The language of C1: { epsilon, a, aa, aaa, ... }
	// The language of D: { All languages that accepts the 'a' or 'b' position and ends with sequence of 'b' }
	// C1's language is not belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(D, C1); // False
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for D, C1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
	
	/* Example 6 - {E, A1}			-																*/
	// The language of A1: { epsilon, a, aa, aaa, ... }
	// The language of E: { Languages All the languages in which Epsilon exists and
	//						also has at least one word }
	// A1's language is belonging to E's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(E, A1); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for E, A1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 7 - {B, A5}			-																*/
	// The language of A5: { a } Just the letter a
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A2's language is not belonging to B's language
	// This time isBelong returns a word from A2 language that does'nt exists in the
	// language of the final NFA (the NFA that creates from intersections until NFH
	// alpha condition's size is 1).
	start = std::chrono::high_resolution_clock::now();
	char** word = new char*();
	int* size = new int(0);
	//cout << NFH_LanguageBelonging::isBelong(B, A5, word, size); // False
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A5: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
	
	/* Example 8 - {E1, A1}																			*/
	// The language of A1: { epsilon, a, aa, aaa, ... }
	// The language of E1: { Languages All the languages in which Epsilon exists and
	//						also has at least two word }
	// A1's language is belonging to E's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(E1, A1); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for E1, A1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

	/* Example 8 - {E2, A1}																			*/
	// The language of A1: { epsilon, a, aa, aaa, ... }
	// The language of E2: { Languages All the languages in which Epsilon exists and
	//						also has at least two word }
	// A1's language is belonging to E's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(E2, A1); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for E2, A1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
	
	/* Example 3 - {B, A6}				- FAILED!!!!!												*/
	// The language of A6: { All the possible words with size bigger than 3 }
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A6's language is belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(B, A6); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A6: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
	
	/* Example 3 - {B, A7}				- FAILED!!!!!												*/
	// The language of A7: { All the possible words with size bigger than 4 }
	// The language of B: { All languages that: For every word, a bigger word exists }
	// A7's language is belonging to B's language
	start = std::chrono::high_resolution_clock::now();
	//cout << NFH_LanguageBelonging::isBelong(B, A7); // True
	finish = std::chrono::high_resolution_clock::now();
	std::cout << "\nTotal time for B, A7: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";
}
