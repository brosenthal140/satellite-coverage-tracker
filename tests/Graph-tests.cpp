#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/GraphModel.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Check for a waypoint for a new graph", "[GraphModel][Waypoint][Search]")
{
	double waypointThresholdDegrees = 1;
	string dataDirectory = "../data";

	GraphModel graph(dataDirectory, waypointThresholdDegrees);
	CoordGeodetic position(0.0, 0.0, 0.0, false);

	graph.testGetNearestWaypoint(position);

	REQUIRE(graph.waypointCount() == 1);
}

TEST_CASE("Check for a waypoint when one already exists", "[GraphModel][Waypoint][Search]")
{
	double waypointThresholdDegrees = 1;
	double waypointConnectednessDegrees = 2;
	string dataDirectory = "../data";

	GraphModel graph(dataDirectory, waypointThresholdDegrees);
	CoordGeodetic position(0.0, 0.0, 0.0, true);
	graph.testGetNearestWaypoint(position);

	REQUIRE(graph.waypointCount() == 1);

	CoordGeodetic position2(0.5, 0.5, 0, true);
	graph.testGetNearestWaypoint(position2);

	REQUIRE(graph.waypointCount() == 1);
}

TEST_CASE("Get waypoints within the connectedness threshold", "[GraphModel][Waypoint][Search]")
{
	double waypointThresholdDegrees = 0.9;
	string dataDirectory = "../data";
	GraphModel graph(dataDirectory, waypointThresholdDegrees);

	vector<CoordGeodetic> positions = {
			{ 0, 0, 0, true },
			{ 1, 1, 0, true },
			{ 1, -1, 0, true },
			{ -1, -1, 0, true },
			{ -1,  1, 0, true }
	};

	// Test the waypoints to add them to the graph
	for (const auto &pos : positions)
	{
		graph.testGetNearestWaypoint(pos);
	}

	// Verify that all the waypoints were inserted
	REQUIRE(graph.waypointCount() == 5);
}