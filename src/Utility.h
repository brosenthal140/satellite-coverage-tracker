#pragma once
#include "SGP4.h"

using namespace libsgp4;

class Utility {

public:
	/* ========== PUBLIC STATIC FUNCTIONS ========== */
	static double getDistance(const CoordGeodetic &pos1, const CoordGeodetic &pos2);
};
