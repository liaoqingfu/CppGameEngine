#pragma once
#include "BoxShape.h"
#ifdef _DEBUG
#include <assert.h>
#endif // DEBUG

namespace GameEngine {
	template<typename E, typename T>
	bool hasOverlap(const E& a, const T& b);

	template<typename E, typename T>
	inline bool hasOverlap(const E& a, const T& b) {
		assert(false, "wasd");
		return false;
	}


	template<>
	inline bool hasOverlap(const BoxShape& a, const BoxShape& b)
	{
		return (a.x < b.x + b.width &&
			a.x + a.width > b.x &&
			a.y < b.y + b.height &&
			a.height + a.y > b.y);
	}
}