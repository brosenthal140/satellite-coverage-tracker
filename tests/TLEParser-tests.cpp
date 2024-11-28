#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "../src/TLEParser.h"

TEST_CASE("TLEParser parse() single string input", "[TLEParser][Method]")
{
	// Construct two strings that represent the two lines of the element set
	const string name = "ISS (ZARYA)";
	const string lineOne = "1 25544U 98067A   24332.90817323  .00022340  00000-0  39860-3 0  9997";
	const string lineTwo = "2 25544  51.6388 224.2748 0006855 275.7348  84.2859 15.49960541483940";

	// Required position
	CoordGeodetic reqPos(-2.5254170250617857E-7, -2.9664422763591514, 417.12408784880648, true);

	auto position = TLEParser::getCoordGeodetic(name, lineOne, lineTwo);
	INFO(position.ToString());

	REQUIRE(position == reqPos);
}