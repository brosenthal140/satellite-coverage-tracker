#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/GraphModel.h"
#include "../src/TLEParser.h"

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

TEST_CASE("Insert an observation", "[GraphModel][Insert]")
{
	// Example Tle
	string line1 = "ISS (ZARYA)";
	string line2 = "1 25544U 98067A   24336.59759280  .00020297  00000-0  36103-3 0  9999";
	string line3 = "2 25544  51.6397 205.9875 0006915 289.9483 151.1375 15.50113255484516";
	auto tle = TLEParser::parse(line1, line2, line3);

	// Parameters for test
	string dataDirectory = "../data";
	double wpSepThreshDeg = 0.9;
	const GraphModel::Vertex vertex = { 0, false, { 0.88899156265206469, -0.073645485312217928, 426.37173319160553, true }};

	REQUIRE(GraphModel::testInsert(dataDirectory, wpSepThreshDeg, tle, vertex));
}

TEST_CASE("Filter edges by weight", "[GraphModel][Search]")
{
	const vector<GraphModel::Edge> edges = {
			{ 0, 10 },
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 5 },
			{ 4, 6 },
			{ 5, 1 },
			{ 6, 1 },
			{ 7, 5 },
			{ 8, 3 },
			{ 9, 10 }
	};
	const double maxWeight = 5;
	const vector<GraphModel::Edge> refEdges = {
			{ 1, 2 },
			{ 2, 3 },
			{ 5, 1 },
			{ 6, 1 },
			{ 8, 3 },
	};

	REQUIRE(GraphModel::testFilterEdges(edges, maxWeight, refEdges));
}