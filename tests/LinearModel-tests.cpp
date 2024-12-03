#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/LinearModel.h"
#include "../src/TLEParser.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Tests the insert() method for the LinearModel", "[LinearModel][Insert]")
{
	string dataDirectory = "../data";
	// Example Tle
	string line1 = "ISS (ZARYA)";
	string line2 = "1 25544U 98067A   24336.59759280  .00020297  00000-0  36103-3 0  9999";
	string line3 = "2 25544  51.6397 205.9875 0006915 289.9483 151.1375 15.50113255484516";
	auto tle = TLEParser::parse(line1, line2, line3);

	REQUIRE(LinearModel::testInsert(dataDirectory, tle));
}