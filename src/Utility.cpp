#include <cmath>
#include "Utility.h"

/* ========== PUBLIC CONST MEMBER Variables ========== */
const double Utility::EARTH_RADIUS = 6371.0;

/* ========== PUBLIC STATIC FUNCTIONS ========== */
double Utility::getDistance(const CoordGeodetic &pos1, const CoordGeodetic &pos2)
{
	double lat1 = Util::DegreesToRadians(pos1.latitude);
	double lon1 = Util::DegreesToRadians(pos1.longitude);
	double lat2 = Util::DegreesToRadians(pos2.latitude);
	double lon2 = Util::DegreesToRadians(pos2.longitude);

	double dlat = lat2 - lat1;
	double dlon = lon2 - lon1;

	double a = sin(dlat / 2) * sin(dlat / 2) +
			   cos(lat1) * cos(lat2) *
			   sin(dlon / 2) * sin(dlon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return EARTH_RADIUS * c;
}