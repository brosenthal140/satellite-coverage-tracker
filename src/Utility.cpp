#include <cmath>
#include <sstream>
#include "Utility.h"

/* ========== PUBLIC CONST MEMBER Variables ========== */
const double Utility::EARTH_RADIUS = 6371.0;

/* ========== PUBLIC STATIC FUNCTIONS ========== */
/**
 * Takes in two positions and returns the great circle distance in kilometers
 * @param pos1 a reference to CoordGeodetic object whose lat/long are in radians
 * @param pos2 a reference to CoordGeodetic object whose lat/long are in radians
 * @return a double representing the great circle distance in kilometers between the two positions on earth's surface at sea level
 */
double Utility::getDistance(const CoordGeodetic &pos1, const CoordGeodetic &pos2)
{
	double lat1 = pos1.latitude;
	double lon1 = pos1.longitude;
	double lat2 = pos2.latitude;
	double lon2 = pos2.longitude;

	double dlat = lat2 - lat1;
	double dlon = lon2 - lon1;

	double a = sin(dlat / 2) * sin(dlat / 2) +
			   cos(lat1) * cos(lat2) *
			   sin(dlon / 2) * sin(dlon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return EARTH_RADIUS * c;
}

/**
 * Creates a string containing the members of the set
 * @param set the set to convert to a string
 * @return a string containing the numbers that were in the set
 */
string Utility::toString(const unordered_set<int> &set)
{
	ostringstream oss;
	for (auto it = set.begin(); it != set.end(); ++it)
	{
		if (it != set.begin())
		{
			oss << ", ";
		}
		oss << *it;
	}
	return oss.str();
}

/**
 * Creates a string containing the members of the set
 * @param set the set to convert to a string
 * @return a string containing the numbers that were in the set
 */
string Utility::toString(const unordered_set<string> &set)
{
	ostringstream oss;
	for (auto it = set.begin(); it != set.end(); ++it)
	{
		if (it != set.begin())
		{
			oss << ", ";
		}
		oss << *it;
	}
	return oss.str();
}