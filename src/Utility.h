#pragma once
#include "SGP4.h"

using namespace libsgp4;

class Utility {

public:
	/* ========== PUBLIC CONST MEMBER Variables ========== */
	static const double EARTH_RADIUS;

	/* ========== PUBLIC STATIC FUNCTIONS ========== */
	static double getDistance(const CoordGeodetic &pos1, const CoordGeodetic &pos2);
};
