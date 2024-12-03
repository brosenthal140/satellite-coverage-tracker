#include <algorithm>
#include "LinearModel.h"
#include "TLEParser.h"

/* ============== LinearModel CLASS ============== */
/* ---------------------------------------------- */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
LinearModel::LinearModel(string &dataDirectory) : DataModel(dataDirectory), _dataDirectory(dataDirectory), _observationCount(0) {}

/* ========== PUBLIC MUTATORS ========== */
/**
 * Imports all of the .tle files that are present in the data directory
 */
void LinearModel::import()
{
	// TODO: Implement import() function in LinearModel class
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
	// TODO: Implement search() function in LinearModel class
}


/* ========== PRIVATE INSERTION METHODS ========== */
/**
 * Inserts an observation into the observation map
 * @param catNum the NORAD category number for the observation
 * @param pos the position to be inserted into the _observations map
 */
void LinearModel::_insert(const int &catNum, const CoordGeodetic &pos)
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
	LinearModel graph(dataDirectory);

	// Insert an observation
	graph.insert(tle);

	auto catNum = tle.NoradNumber();
	auto positions = graph._observations[catNum];
	auto refPos = TLEParser::getCoordGeodetic(tle);

	if (!positions.empty())
	{
		auto iter = find(positions.begin(), positions.end(), refPos);

		return iter != positions.end();
	}
	else
		return false;
}