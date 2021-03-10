#include "Intersection.h"

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
// intersection_test
// Change to main() for testing
void intersection_test()
{
    char alphabet[2] = { 'a', 'b' };
    int* final_states = (int*)malloc(sizeof(int));
    Error::checkMalloc((void*)final_states, 1);
    final_states[0] = 1;
    /*      NFA C1    */
    int moveMatrix1_C1[2][2] = { {0, -1}, {1, 0} };
    int* pointerToMoveMatrix1_C1[2] = { moveMatrix1_C1[0], moveMatrix1_C1[1] }; // pointer to moveMatrix1
    int moveMatrix2_C1[2][2] = { {1, -1}, {1, 0} };
    int* pointerToMoveMatrix2_C1[2] = { moveMatrix2_C1[0], moveMatrix2_C1[1] }; // pointer to moveMatrix1
    int** pointerToMoveMatrix_NonDeterministic_B[2] = { pointerToMoveMatrix1_C1, pointerToMoveMatrix2_C1 };
    NFA* C1 = new NFA(pointerToMoveMatrix_NonDeterministic_B, alphabet, 0, final_states, 2, 2, 2, 1);
    /*      NFA C2    */
    int moveMatrix1_C2[2][2] = { {1, 1}, {0, 0} };
    int* pointerToMoveMatrix1_C2[2] = { moveMatrix1_C2[0], moveMatrix1_C2[1] }; // pointer to moveMatrix1
    int** pointerToMoveMatrix_NonDeterministic_C[1] = { pointerToMoveMatrix1_C2 };
    NFA* C2 = new NFA(pointerToMoveMatrix_NonDeterministic_C, alphabet, 0, final_states, 1, 2, 2, 1);
    /*      NFH B      */
    char branch1[2] = { 'a', 'a' };
    char* pointerToBranch1 = branch1;
    Transition* state1_transition1 = new Transition(pointerToBranch1, 0);
    char branch2[2] = { 'a', '#' };
    char* pointerToBranch2 = branch2;
    char branch3[2] = { '#', 'b' };
    char* pointerToBranch3 = branch3;
    Transition* state1_transition2 = new Transition(pointerToBranch2, 1);
    Transition* state1_givenTransition[2] = { state1_transition1 , state1_transition2 };
    StateTransition* state1 = new StateTransition(state1_givenTransition, 2, 2);
    Transition* state2_transition1 = new Transition(pointerToBranch2, 1);
    Transition* state2_transition2 = new Transition(pointerToBranch3, 0);
    Transition* state2_givenTransition[2] = { state2_transition1, state2_transition2 };
    StateTransition* state2 = new StateTransition(state2_givenTransition, 2, 2);
    Ntree* moveArray[2] = { new Ntree(state1), new Ntree(state2) };
    Quantifiers quantifiers[2] = { EVERY, EACH };
    AlphaCondition* alphaCondition = new AlphaCondition(quantifiers, 2);
    NFH* B = new NFH(moveArray, alphabet, 0, final_states, *alphaCondition, 2, 2, 1);
    /****************************************************************************************************************/
    cout << "\n" << "Intersection Example:\n";
    // intersectionComplementation_NFA_to_NFA
    C1->toString();
    C2->toString();
    NFA* output1 = Intersection::intersectionComplementation_NFA_to_NFA(C1, C2);
    output1->toString();
    // intersectionComplementation_NFH_to_NFA
    NFH* output2 = Intersection::intersectionComplementation_NFH_to_NFA(B, C1);
    NFA* output2_convert = Convert::convert_NFH_to_NFA(output2);
    output2_convert->toString();
    NFH* output3 = Intersection::intersectionComplementation_NFH_to_NFA(B, C2);
    /*  free all the allocated memory    */
    free(final_states);
}
