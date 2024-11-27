#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
using namespace std;
using namespace libsgp4;

// Just a basic test of the sgp4 library
TEST_CASE("Basic SGP4 Test", "[lib][sgp4]")
{
	// Get the current time
	auto now = DateTime::Now();

	// Construct two strings that represent the two lines of the element set
	const string name = "ISS (ZARYA)";
	const string lineOne = "1 25544U 98067A   24295.53713652  .00021607  00000-0  38288-3 0  9992";
	const string lineTwo = "2 25544  51.6410  49.5583 0009334  92.8582 330.0637 15.50158115478144";

	// Construct a Tle object
	const Tle tle(name, lineOne, lineTwo);

	// Initialize the SGP4 object
	SGP4 sgp4(tle);

	// Set the time since epoch, which is the difference from the current time to the time since the orbital measurements were taken
	const auto timeMeasured =  tle.Epoch();
	const double timeSinceEpoch = 0.0;

	// Get the Earth-centered inertial position of the object
	Eci position = sgp4.FindPosition(timeSinceEpoch);

	// Get the geodetic position
	CoordGeodetic geodetic = position.ToGeodetic();

	// get the latitude, longitude in radians and the altitude in kilometers
	double latitude = geodetic.latitude;
	double longitude = geodetic.longitude;
	double altitude = geodetic.altitude;

	cout << "The current geodetic position of the object with name '" << name << "' is:" << endl;
	cout << geodetic.ToString() << endl;
}