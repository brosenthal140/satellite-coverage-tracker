#include <limits>
#include "GraphModel.h"
#include "Utility.h"

/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
GraphModel::GraphModel(string &directory, double &threshold) : _dataDirectory(directory), _wpThreshold(threshold), _wpCount(0), _vertexCount(0) {}

/* ========== PUBLIC MUTATORS ========== */
/**
 * Processes a Tle object, creates a vertex, and inserts it into the graph. This function does not insert waypoints (that is automatic)
 * @param tle the Tle object used to determine the position
 */
void GraphModel::insertVertex(Tle &tle)
{

}

/* ========== PUBLIC ACCESSORS ========== */
/**
 * Returns the current number of waypoints in the graph
 * @return the number of waypoints
 */
int GraphModel::waypointCount() const
{
	return _wpCount;
}

/* ========== PUBLIC TEST METHODS ========== */
/**
 * A test function used to test the _getNearestWaypoint() function
 * @param pos the position to search for
 * @return a reference to the nearest waypoint
 */
const GraphModel::Vertex& GraphModel::testGetNearestWaypoint(const CoordGeodetic &pos)
{
	return _getNearestWaypoint(pos);
}


/* ========== PRIVATE INSERTION METHODS ========== */
/**
 * Increments _vertexCount and returns the new value as the next index
 * @return an int as the next vertex index
 */
int GraphModel::_getNextVertexIndex()
{
	return _vertexCount++;
}

/**
 * Creates a new vertex and inserts it into the graph
 * @param pos
 * @param isWaypoint
 * @return the index of the newly inserted index
 */
int GraphModel::_insertVertex(const CoordGeodetic &pos, bool isWaypoint)
{
	Vertex vertex = { isWaypoint, pos };
	int index = _getNextVertexIndex();
	_vertices[index] = vertex;

	return index;
}

/**
 * Creates a new vertex and as a waypoint
 * @param pos the position of the vertex to be inserted as a waypoint
 * @return a reference to the vertex that was just created
 */
const GraphModel::Vertex& GraphModel::_insertWaypoint(const CoordGeodetic &pos)
{
	int wpIndex = _insertVertex(pos, true);
	_wpCount++;

	_wpLatMap[pos.latitude] = wpIndex;
	_wpLongMap[pos.longitude] = wpIndex;

	return _vertices[wpIndex];
}

/**
 * Inserts an edge from one index to another
 * @param from the index of the source
 * @param to the index of the destination
 */
void GraphModel::_insertEdge(int &from, int &to)
{

}

/* ========== PRIVATE SEARCH METHODS ========== */
/**
 * Searches for the closest waypoint relative to position and a waypoint threshold
 * @param pos the position to search for
 * @return a reference to the waypoint
 */
const GraphModel::Vertex& GraphModel::_getNearestWaypoint(const CoordGeodetic &pos)
{
	// First, check the waypoint latitude map to determine if there is a waypoint candidate with a latitude within the threshold
	auto latLowerBound = pos.latitude - _wpThreshold;
	auto latUpperBound = pos.latitude + _wpThreshold;
	auto longLowerBound = pos.longitude - _wpThreshold;
	auto longUpperBound = pos.longitude + _wpThreshold;

	auto lowerBoundIter = _wpLatMap.lower_bound(latLowerBound);
	auto upperBoundIter = _wpLatMap.upper_bound(latUpperBound);

	// Iterate through the _wpLatMap to find possible waypoints
	set<int> wpCandidates;
	for (auto iter = lowerBoundIter; iter != upperBoundIter; ++iter)
	{
		// Check to see if the waypoint's longitude is in range
		auto wpIndex = iter->second;
		auto wp = _vertices[wpIndex];

		if (wp.position.longitude >= longLowerBound && wp.position.longitude <= longUpperBound)
			wpCandidates.insert(wpIndex);
	}


	// Second, check the waypoint longitude map to determine if there is a waypoint candidate with a longitude within the threshold
	lowerBoundIter = _wpLongMap.lower_bound(longLowerBound);
	upperBoundIter = _wpLongMap.upper_bound(longUpperBound);
	for (auto iter = lowerBoundIter; iter != upperBoundIter; ++iter)
	{
		// Check to see if the waypoint's longitude is in range
		auto wpIndex = iter->second;
		auto wp = _vertices[wpIndex];

		if (wp.position.latitude >= latLowerBound && wp.position.latitude <= latUpperBound)
			wpCandidates.insert(wpIndex);
	}

	// If a there are still waypoint candidates, get the nearest waypoint
	double minDist = numeric_limits<double>::max(), testDist;
	int selectedIndex = -1;
	for (const auto &wp : wpCandidates)
	{
		testDist = Utility::getDistance(pos, _vertices[wp].position);

		if (testDist < minDist)
		{
			minDist = testDist;
			selectedIndex = wp;
		}
	}

	// If there isn't a waypoint whose latitude is within the threshold, insert a new waypoint
	if (selectedIndex == -1)
		return _insertWaypoint(pos);
	else
		return _vertices[selectedIndex];
}