#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <SGP4.h>
#include "../src/LinearModel.h"
#include "../src/TLEParser.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Insert an observation into the model", "[LinearModel][Insert]")
{
	string dataDirectory = "../data";
	// Example Tle
	string line1 = "ISS (ZARYA)";
	string line2 = "1 25544U 98067A   24336.59759280  .00020297  00000-0  36103-3 0  9999";
	string line3 = "2 25544  51.6397 205.9875 0006915 289.9483 151.1375 15.50113255484516";
	auto tle = TLEParser::parse(line1, line2, line3);

	REQUIRE(LinearModel::testInsert(dataDirectory, tle));
}

TEST_CASE("Search for observations relative to position and within a range for 10 observations", "[LinearModel][Search]")
{
	// Parameters for test
	string dataDirectory = "../data";
	string pathToTestFile = "../data/test_01.tle";
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

		auto catNums = LinearModel::testSearch(dataDirectory, tleObjs, pos, radius);

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

TEST_CASE("Search for observations relative to position and within a range for 50 observations", "[LinearModel][Search]")
{
	// Parameters for test
	string dataDirectory = "../data";
	string pathToTestFile = "../data/test_02.tle";
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

		auto catNums = LinearModel::testSearch(dataDirectory, tleObjs, pos, radius);

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

TEST_CASE("Search for observations relative to position and within a range for 1000 observations", "[LinearModel][Search]")
{
	// Parameters for test
	string dataDirectory = "../data";
	string pathToTestFile = "../data/test_03.tle";
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

		auto catNums = LinearModel::testSearch(dataDirectory, tleObjs, pos, radius);

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