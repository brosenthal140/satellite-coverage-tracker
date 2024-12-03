#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/GraphModel.h"
#include "../src/TLEParser.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Check for a waypoint for a new graph", "[GraphModel][Waypoint][Search]")
{
	double wpSepThreshDeg = 2000; // kilometers
	string dataDirectory = "../data";

	CoordGeodetic position(0.0, 0.0, 0.0, false);
	GraphModel::Vertex refVertex = { 0, true, position };

	REQUIRE(GraphModel::testFindClosestWaypoint(dataDirectory, wpSepThreshDeg, position, refVertex));
}

TEST_CASE("Check for a waypoint when one already exists", "[GraphModel][Waypoint][Search]")
{
	double wpSepThreshDeg = 2000; // kilometers
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
	double wpSepThreshDeg = 2000; // kilometers
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
	double wpSepThreshDeg = 2000; // kilometers
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

TEST_CASE("Search for observations relative to position and within a range for 10 observations", "[GraphModel][Search]")
{
	// Parameters for test
	string dataDirectory = "../data";
	string pathToTestFile = "../data/test_files/test_01.tle";
	double wpSepThreshDeg = 2000; // kilometers
	CoordGeodetic pos = { 0, 158.925, 561.694, false };
	double radius = 10; // units: km
	unordered_set<int> refCatNums = { 12 };

	// Ingest the file
	vector<Tle> tleObjs;
	ifstream file(pathToTestFile);
	int count = 0;
	if (file.is_open())
	{
		string line1, line2;
		while (getline(file, line1))
		{
			if (getline(file, line2))
			{
				cout << "Observation #" << to_string(count) << endl;
				auto tle = TLEParser::parse(line1, line2);
				cout << tle << endl;
				tleObjs.emplace_back(tle);
				auto position = TLEParser::getCoordGeodetic(tle);
				cout << position << endl << endl;

				count++;
			}
		}

		auto catNums = GraphModel::testSearch(dataDirectory, wpSepThreshDeg, tleObjs, pos, radius);

		// Print out the category numbers for debugging
		for (const auto &catNum : catNums)
			INFO(to_string(catNum));

		REQUIRE(refCatNums == catNums);
	}
	else
	{
		INFO("Test file at path '" + pathToTestFile + "' could not be opened!");
		REQUIRE(false);
	}

}

TEST_CASE("Search for observations relative to position and within a range for 50 observations", "[GraphModel][Search]")
{
	// Parameters for test
	string dataDirectory = "../data";
	string pathToTestFile = "../data/test_files/test_02.tle";
	double wpSepThreshDeg = 2000; // kilometers
	CoordGeodetic pos = { 0, 153, 0, false };
	double radius = 700; // units: km
	unordered_set<int> refCatNums = { 12, 29, 51, 85, 115 };

	// Ingest the file
	vector<Tle> tleObjs;
	ifstream file(pathToTestFile);
	int count = 0;
	if (file.is_open())
	{
		string line1, line2;
		while (getline(file, line1))
		{
			if (getline(file, line2))
			{
				cout << "Observation #" << to_string(count);
				auto tle = TLEParser::parse(line1, line2);
				cout << " " << "NORAD CAT: " << tle.NoradNumber() << endl;
				tleObjs.emplace_back(tle);
				auto position = TLEParser::getCoordGeodetic(tle);
				cout << position << endl << endl;

				count++;
			}
		}

		auto catNums = GraphModel::testSearch(dataDirectory, wpSepThreshDeg, tleObjs, pos, radius);

		// Print out the category numbers for debugging
		for (const auto &catNum : catNums)
			INFO(to_string(catNum));

		REQUIRE(refCatNums == catNums);
	}
	else
	{
		INFO("Test file at path '" + pathToTestFile + "' could not be opened!");
		REQUIRE(false);
	}

}

TEST_CASE("Search for observations relative to position and within a range for 1000 observations", "[GraphModel][Search]")
{
	// Parameters for test
	string dataDirectory = "../data";
	string pathToTestFile = "../data/test_files/test_03.tle";
	double wpSepThreshDeg = 2000; // kilometers
	CoordGeodetic pos = { 29.649294, -82.339383, 0, false }; // Lat, Long of Tigert hall
	double radius = 1000; // units: km
	unordered_set<int> refCatNums = { 340, 414, 549, 745, 561, 446, 657, 268, 406, 544, 659, 648, 672, 660, 405, 704, 224, 753 };

	// Ingest the file
	vector<Tle> tleObjs;
	ifstream file(pathToTestFile);
	int count = 0;
	if (file.is_open())
	{
		string line1, line2;
		while (getline(file, line1))
		{
			if (getline(file, line2))
			{
				cout << "Observation #" << to_string(count);
				auto tle = TLEParser::parse(line1, line2);
				cout << " " << "NORAD CAT: " << tle.NoradNumber() << endl;
				tleObjs.emplace_back(tle);
				auto position = TLEParser::getCoordGeodetic(tle);
				cout << position << endl << endl;

				count++;
			}
		}

		auto catNums = GraphModel::testSearch(dataDirectory, wpSepThreshDeg, tleObjs, pos, radius);

		// Print out the category numbers for debugging
		for (const auto &catNum : catNums)
			INFO(to_string(catNum));

		REQUIRE(refCatNums == catNums);
	}
	else
	{
		INFO("Test file at path '" + pathToTestFile + "' could not be opened!");
		REQUIRE(false);
	}
}

TEST_CASE("Import all the TLE data in the data directory", "[GraphModel][Import]")
{
	string dataDirectory = "../data";
	double wpSepThreshDeg = 2000; // kilometers

	// Perform the import
	REQUIRE_NOTHROW(GraphModel::testImport(dataDirectory, wpSepThreshDeg));
}