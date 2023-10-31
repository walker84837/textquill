#include <cstring>

namespace wrappers
{
	// TODO: Add wrappers for C functions
	bool stringcheck(char* s1, char* s2) {
		if (std::strcmp(s1, s2) == 0) {
			return true;
		}
		return false;
	}
}

