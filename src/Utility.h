#pragma once
#include <string>
#include <unordered_set>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;

class Utility {

public:
	/* ========== PUBLIC CONST MEMBER Variables ========== */
	static const double EARTH_RADIUS;

	/* ========== PUBLIC STATIC FUNCTIONS ========== */
	static double getDistance(const CoordGeodetic &pos1, const CoordGeodetic &pos2);
	static string toString(const unordered_set<int> &set);
};
