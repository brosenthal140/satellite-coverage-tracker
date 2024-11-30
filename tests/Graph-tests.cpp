#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/GraphModel.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Check for a waypoint for a new graph", "[GraphModel][Waypoint][Search]")
{
	double wpSepThreshDeg = 1;
	string dataDirectory = "../data";

	CoordGeodetic position(0.0, 0.0, 0.0, false);
	GraphModel::Vertex refVertex = { 0, true, position };

	REQUIRE(GraphModel::testFindClosestWaypoint(dataDirectory, wpSepThreshDeg, position, refVertex));
}

TEST_CASE("Check for a waypoint when one already exists", "[GraphModel][Waypoint][Search]")
{
	double wpSepThreshDeg = 1;
	string dataDirectory = "../data";

	vector<CoordGeodetic> positions = {
			{0.0, 0.0, 0.0, true},
			{0.5, 0.5, 0, true}
	};
	vector<GraphModel::Vertex> refVertices = {
			{ 0, true, positions[0] },
			{ 1, true, positions[1] }
	};

	for (int i = 0; i < positions.size(); i++)
	{
		REQUIRE(GraphModel::testFindClosestWaypoint(dataDirectory, wpSepThreshDeg, positions[i], refVertices[i]));
	}
}

TEST_CASE("Get the waypoint nearest to a position", "[GraphModel][Waypoint][Search]")
{
	double wpSepThreshDeg = 0.9;
	string dataDirectory = "../data";
	vector<CoordGeodetic> wpPositions = {
			{ 0, 0, 0, true },
			{ 1, 1, 0, true },
			{ 1, -1, 0, true },
			{ -1, -1, 0, true },
			{ -1,  1, 0, true }
	};
	CoordGeodetic refPosition(-2, -2, 0, true );
	GraphModel::Vertex refWaypoint = {
			3,
			true,
			{-1, -1, 0, true}
	};

	// Test the waypoints to add them to the graph
	REQUIRE(GraphModel::testFindClosestWaypoint(dataDirectory, wpSepThreshDeg, refPosition, wpPositions, refWaypoint));
}