#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/Utility.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Haversine distance calculation between two coordinates", "[Utility][Static]")
{
	CoordGeodetic pos1(30.4382, 84.2806, 0, true); // Coordinate position of TLH airport
	CoordGeodetic pos2(33.9422, 118.4036, 0, true); // Coordinate position of LAX airport
	double reqDistanceKm = 3219.8;
	double tolerance = 0.1;

	auto calcDistanceKm = Utility::getDistance(pos1, pos2);
	auto diff = abs(reqDistanceKm - calcDistanceKm);

	REQUIRE(diff < tolerance);
}
