#ifndef NFH_LANGUAGEBELONGING_H
#define NFH_LANGUAGEBELONGING_H

#include "Header.h"
#include "Error.h"
#include "NFH.h"
#include "NFA.h"
#include "DFA.h"
#include "DFH.h"
#include "EmptinessChecking.h"
#include "Convert.h"
#include "Intersection.h"

class NFH_LanguageBelonging{
public:
	// isBelong checks if the language of the NFA is belonging to the
	// language of the NFH. Return true if belong, otherwise - false.
	bool static isBelong(NFH* nfh, NFA* nfa) {
		/* Stop condition, if SIZE_OF_ALPHA_CONDITION == 1 */
		if (nfh->alphaCondition.numberOfQuantifiers == 1) {
			NFA* nfh_Converted = Convert::convert_NFH_to_NFA(nfh);
			if (nfh->alphaCondition.quantifiers[0] == EACH) {
				nfh->~NFH();
				NFA* intersection_output = Intersection::intersectionComplementation_NFA_to_NFA(nfh_Converted, nfa);
				nfh_Converted->~NFA();
				nfa->~NFA();
				EmptinessChecking emptinessChecking(intersection_output, intersection_output->starterState, intersection_output->finalStates);
				return emptinessChecking.checkEmptiness(intersection_output, intersection_output->starterState);
			}
			else if (nfh->alphaCondition.quantifiers[0] == EVERY) {
				nfh->~NFH();
				DFA* nfh_Convert_DFA_version = Convert::convert_NFA_to_DFA(*nfh_Converted);
				DFA* nfh_Convert_DFA_version_complement = nfh_Convert_DFA_version->complement();
				NFA* nfh_for_intersection = Convert::convert_DFA_to_NFA(*nfh_Convert_DFA_version_complement);
				NFA* intersection_output = Intersection::intersectionComplementation_NFA_to_NFA(nfh_for_intersection, nfa);
				nfa->~NFA();
				EmptinessChecking emptinessChecking(intersection_output, intersection_output->starterState, intersection_output->finalStates);
				bool answer = emptinessChecking.checkEmptiness(intersection_output, intersection_output->starterState);
				if (answer) return true;
				return false;
			}
		}
		/* General case */
		else {
			if (nfh->alphaCondition.quantifiers[nfh->alphaCondition.numberOfQuantifiers - 1] == EACH) {
				NFH* intersection_output = Intersection::intersectionComplementation_NFH_to_NFA(nfh, nfa);
				nfh->~NFH();
				return isBelong(intersection_output, nfa);
			}
			else if (nfh->alphaCondition.quantifiers[nfh->alphaCondition.numberOfQuantifiers - 1] == EVERY) {
				DFA* nfaConvert = Convert::convert_NFA_to_DFA(*nfa);
				DFA* nfaConvertComplement = nfaConvert->complement();
				NFA* nfaConvertComplementConvert = Convert::convert_DFA_to_NFA(*nfaConvertComplement);
				NFH* intersection_output = Intersection::intersectionComplementation_NFH_to_NFA(nfh, nfaConvertComplementConvert);
				nfh->~NFH();
				DFH* intersection_output_deterministic = Convert::convert_NFH_to_DFH(intersection_output);
				intersection_output->~NFH();
				DFH* intersection_output_deterministic_complement = intersection_output_deterministic->complement();
				NFH* intersection_output_deterministic_complement_convertNFH = Convert::convert_DFH_to_NFH(intersection_output_deterministic_complement);
				return isBelong(intersection_output_deterministic_complement_convertNFH, nfa);
			}
		}
	}
	// isBelong checks if the language of the NFA is belonging to the language of the NFH.
	// Return true if belong
	// Otherwise - false.
	// *Also			- Returns a word in case that isBelong return false.
	// *Assumption 1	- word initializes to NULL
	// *Assumption 2	- size initializes to 0
	// *Assumption 3	- The alpha condition starts with Quantifier EVERY
	bool static isBelong(NFH* nfh, NFA* nfa, char** word, int* size) {
		/* Stop condition, if SIZE_OF_ALPHA_CONDITION == 1 */
		if (nfh->alphaCondition.numberOfQuantifiers == 1) {
			NFA* nfh_Converted = Convert::convert_NFH_to_NFA(nfh);
			if (nfh->alphaCondition.quantifiers[0] == EACH) {
				nfh->~NFH();
				NFA* intersection_output = Intersection::intersectionComplementation_NFA_to_NFA(nfh_Converted, nfa);
				nfh_Converted->~NFA();
				nfa->~NFA();
				EmptinessChecking emptinessChecking(intersection_output, intersection_output->starterState, intersection_output->finalStates);
				bool answer;
				word = emptinessChecking.checkEmptiness(intersection_output, intersection_output->starterState, word, size, &answer);
				return answer;
			}
			else if (nfh->alphaCondition.quantifiers[0] == EVERY) {
				nfh->~NFH();
				DFA* nfh_Convert_DFA_version = Convert::convert_NFA_to_DFA(*nfh_Converted);
				DFA* nfh_Convert_DFA_version_complement = nfh_Convert_DFA_version->complement();
				NFA* nfh_for_intersection = Convert::convert_DFA_to_NFA(*nfh_Convert_DFA_version_complement);
				NFA* intersection_output = Intersection::intersectionComplementation_NFA_to_NFA(nfh_for_intersection, nfa);
				nfa->~NFA();
				EmptinessChecking emptinessChecking(intersection_output, intersection_output->starterState, intersection_output->finalStates);
				// Emptiness Checking with a return word
				bool answer;
				word = emptinessChecking.checkEmptiness(intersection_output, intersection_output->starterState, word, size, &answer);
				if (answer) return true;
				return false;
			}
		}
		/* General case */
		else {
			if (nfh->alphaCondition.quantifiers[nfh->alphaCondition.numberOfQuantifiers - 1] == EACH) {
				NFH* intersection_output = Intersection::intersectionComplementation_NFH_to_NFA(nfh, nfa);
				nfh->~NFH();
				return isBelong(intersection_output, nfa, word, size);
			}
			else if (nfh->alphaCondition.quantifiers[nfh->alphaCondition.numberOfQuantifiers - 1] == EVERY) {
				DFA* nfaConvert = Convert::convert_NFA_to_DFA(*nfa);
				DFA* nfaConvertComplement = nfaConvert->complement();
				NFA* nfaConvertComplementConvert = Convert::convert_DFA_to_NFA(*nfaConvertComplement);
				NFH* intersection_output = Intersection::intersectionComplementation_NFH_to_NFA(nfh, nfaConvertComplementConvert);
				nfh->~NFH();
				DFH* intersection_output_deterministic = Convert::convert_NFH_to_DFH(intersection_output);
				intersection_output->~NFH();
				DFH* intersection_output_deterministic_complement = intersection_output_deterministic->complement();
				NFH* intersection_output_deterministic_complement_convertNFH = Convert::convert_DFH_to_NFH(intersection_output_deterministic_complement);
				return isBelong(intersection_output_deterministic_complement_convertNFH, nfa, word, size);
			}
		}
	}
	// isBelong checks if the languages of the NFAs are belonging to the
	// language of the NFH. Return true if belong, otherwise - false.
	// Assumption 1: NFH's alpha condition's size equals to NFAsIndex.
	// Assumption 2: NFAsIndex initializes to 0.
	bool static isBelong(NFH* nfh, NFA** NFAs, int NFAsIndex) {
		/* Stop condition, if SIZE_OF_ALPHA_CONDITION == 1 */
		if (nfh->alphaCondition.numberOfQuantifiers == 1) {
			NFA* nfh_Converted = Convert::convert_NFH_to_NFA(nfh);
			if (nfh->alphaCondition.quantifiers[0] == EACH) {
				nfh->~NFH();
				NFA* intersection_output = Intersection::intersectionComplementation_NFA_to_NFA(nfh_Converted, NFAs[NFAsIndex]);
				NFAs[NFAsIndex]->~NFA();
				EmptinessChecking emptinessChecking(intersection_output, intersection_output->starterState, intersection_output->finalStates);
				return emptinessChecking.checkEmptiness(intersection_output, intersection_output->starterState);
			}
			else if (nfh->alphaCondition.quantifiers[0] == EVERY) {
				nfh->~NFH();
				DFA* nfh_Convert_DFA_version = Convert::convert_NFA_to_DFA(*nfh_Converted);
				DFA* nfh_Convert_DFA_version_complement = nfh_Convert_DFA_version->complement();
				NFA* nfh_for_intersection = Convert::convert_DFA_to_NFA(*nfh_Convert_DFA_version_complement);
				NFA* intersection_output = Intersection::intersectionComplementation_NFA_to_NFA(nfh_for_intersection, NFAs[NFAsIndex]);
				NFAs[NFAsIndex]->~NFA();
				EmptinessChecking emptinessChecking(intersection_output, intersection_output->starterState, intersection_output->finalStates);
				bool answer = emptinessChecking.checkEmptiness(intersection_output, intersection_output->starterState);
				if (answer) return true;
				return false;
			}
		}
		/* General case */
		else {
			if (nfh->alphaCondition.quantifiers[nfh->alphaCondition.numberOfQuantifiers - 1] == EACH) {
				NFH* intersection_output = Intersection::intersectionComplementation_NFH_to_NFA(nfh, NFAs[NFAsIndex]);
				nfh->~NFH();
				return isBelong(intersection_output, NFAs, ++NFAsIndex);
			}
			else if (nfh->alphaCondition.quantifiers[nfh->alphaCondition.numberOfQuantifiers - 1] == EVERY) {
				DFA* nfaConvert = Convert::convert_NFA_to_DFA(*NFAs[NFAsIndex]);
				DFA* nfaConvertComplement = nfaConvert->complement();
				NFA* nfaConvertComplementConvert = Convert::convert_DFA_to_NFA(*nfaConvertComplement);
				NFH* intersection_output = Intersection::intersectionComplementation_NFH_to_NFA(nfh, nfaConvertComplementConvert);
				nfh->~NFH();
				DFH* intersection_output_deterministic = Convert::convert_NFH_to_DFH(intersection_output);
				intersection_output->~NFH();
				DFH* intersection_output_deterministic_complement = intersection_output_deterministic->complement();
				NFH* intersection_output_deterministic_complement_convertNFH = Convert::convert_DFH_to_NFH(intersection_output_deterministic_complement);
				return isBelong(intersection_output_deterministic_complement_convertNFH, NFAs, ++NFAsIndex);
			}
		}
	}
};

#endif
