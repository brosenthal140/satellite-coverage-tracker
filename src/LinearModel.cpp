#include "LinearModel.h"

/* ============== LinearModel CLASS ============== */
/* ---------------------------------------------- */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
LinearModel::LinearModel(string &dataDirectory) : DataModel(dataDirectory) {}

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
 * @param tle
 */
void LinearModel::insert(const Tle &tle)
{
	// TODO: Implement insert() function in LinearModel class
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
}