#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/GraphModel.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Check for a waypoint for a new graph", "[GraphModel][Waypoint][Insertion]")
{
	double waypointThresholdDegrees = 0.5;
	string dataDirectory = "../data";

	GraphModel graph(dataDirectory, waypointThresholdDegrees);
	CoordGeodetic position(0.0, 0.0, 0.0, false);

	graph.testGetNearestWaypoint(position);

	REQUIRE(graph.waypointCount() == 1);
}

TEST_CASE("Check for a waypoint when one already exists", "[GraphModel][Waypoint][Insertion]")
{
	double waypointThresholdDegrees = 0.5;
	string dataDirectory = "../data";

	GraphModel graph(dataDirectory, waypointThresholdDegrees);
	CoordGeodetic position(0.0, 0.0, 0.0, false);
	graph.testGetNearestWaypoint(position);

	REQUIRE(graph.waypointCount() == 1);

	CoordGeodetic position2(0.24, 0.24, 0, true);
	graph.testGetNearestWaypoint(position2);

	REQUIRE(graph.waypointCount() == 1);
}
