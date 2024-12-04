#include <algorithm>
#include "LinearModel.h"
#include "TLEParser.h"
#include "Utility.h"

/* ============== LinearModel CLASS ============== */
/* ---------------------------------------------- */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
LinearModel::LinearModel(string &dataDirectory) : DataModel(dataDirectory), _dataDirectory(dataDirectory), _observationCount(0) {}
LinearModel::~LinearModel() {}

/* ========== PUBLIC MUTATORS ========== */
/**
 * Imports all of the .tle files that are present in the data directory
 */
void LinearModel::import()
{
	// Get the .tle files at the _dataDirectory
	auto filePaths = TLEParser::getTLEFiles(_dataDirectory);

	// For each file, process the files and insert the TLE entries into the model
	vector<Tle> observations;
	for (const auto &filePath : filePaths)
	{
		TLEParser::parseTLEFile(filePath, observations, false);
	}

	// For each observation, insert it into the model
	for (const auto &tle : observations)
	{
		insert(tle);
	}
}

/**
 * Inserts an observation into the _observations map
 * @param tle a reference to the Tle object to be inserted into the model
 */
void LinearModel::insert(const Tle &tle)
{
	// Get the NORAD category number and the position for the observation
	auto catNum = tle.NoradNumber();
	auto position = TLEParser::getCoordGeodetic(tle);

	_insert(catNum, position);
}

/* ========== PUBLIC ACCESSORS ========== */
/**
 * Performs a search of the _observations map observations within range of a position
 * @param position the position to perform the search relative to
 * @param radius the range from the position
 * @return an unordered_set of the NORAD category numbers that are within range
 */
unordered_set<int> LinearModel::search(const CoordGeodetic &position, const double &radius)
{
	unordered_set<int> catNums;
	double distance;
	for (const auto &entry : _observations)
	{
		for (const auto &pos : entry.second)
		{
			distance = Utility::getDistance(position, pos);

			if (distance < radius)
				catNums.insert(entry.first);
		}
	}

	return catNums;
}


/* ========== PRIVATE INSERTION METHODS ========== */
/**
 * Inserts an observation into the observation map
 * @param catNum the NORAD category number for the observation
 * @param pos the position to be inserted into the _observations map
 */
void LinearModel::_insert(const unsigned int &catNum, const CoordGeodetic &pos)
{
	_observations[catNum].emplace_back(pos);
	_observationCount++;
}

/* ========== PUBLIC TEST METHODS ========== */
/**
 * Tests the insert method
 * @param dataDirectory the path to the data source for the GraphModel
 * @param tle a reference to a Tle object that will be inserted into the graph
 * @return a boolean value indicating if the test passed
 */
bool LinearModel::testInsert(string &dataDirectory, const Tle &tle)
{
	// Create an instance of the GraphModel class
	LinearModel model(dataDirectory);

	// Insert an observation
	model.insert(tle);

	auto catNum = tle.NoradNumber();
	auto positions = model._observations[catNum];
	auto refPos = TLEParser::getCoordGeodetic(tle);

	if (!positions.empty())
	{
		auto iter = find(positions.begin(), positions.end(), refPos);

		return iter != positions.end();
	}
	else
		return false;
}

/**
 * Tests the search() function for the LinearModel class
 * @param dataDirectory the path to the data source for the GraphModel
 * @param observations a reference to a vector of Tle objects that represent the observation to insert into the model
 * @param pos the position to search for
 * @param radius the range from the target position the determines if the observation is valid
 * @return an unordered set containing the NORAD category numbers of the observations that were within range of the position
 */
unordered_set<int> LinearModel::testSearch(string &dataDirectory, const vector<Tle> &observations, const CoordGeodetic &pos, const double &radius)
{
	// Create an instance of the GraphModel class
	LinearModel model(dataDirectory);

	// Insert the observations into the model
	for (const auto &observation : observations)
		model.insert(observation);

	// Perform the search
	auto results = model.search(pos, radius);

	return results;
}