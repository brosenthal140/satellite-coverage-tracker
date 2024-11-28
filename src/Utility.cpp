#include "Utility.h"

/* ========== PUBLIC STATIC FUNCTIONS ========== */
double Utility::getDistance(const CoordGeodetic &pos1, const CoordGeodetic &pos2)
{
	return sqrt(pow(pos2.latitude - pos1.latitude, 2) + pow(pos2.longitude - pos1.longitude, 2) );
}