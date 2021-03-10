#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>
using namespace std;

class Error{
public:
	void static checkMalloc(void* allocatedMemory, int size) {
		if (allocatedMemory == NULL) {
			fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", size);
			abort();
		}
	}
};

#endif
