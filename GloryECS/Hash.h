#pragma once

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define FIRSTH 37 /* also prime */

namespace Hashing
{
	inline size_t Hash(const char* c)
	{
		size_t hash = FIRSTH;
		while (*c) {
			hash = (hash * A) ^ (c[0] * B);
			c++;
		}
		return hash;
	}
}

#undef A
#undef B
#undef FIRSTH
