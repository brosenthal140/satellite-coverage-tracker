#include <limits>
#include <queue>
#include "GraphModel.h"
#include "Utility.h"

/* ========== PUBLIC STRUCTS & ENUMS ========== */
/* -------------------------------------------- */
/* ============== Vertex STRUCT =============== */
/* ============= PUBLIC OPERATORS ============= */
/**
 * Equality operator for the Vertex struct
 * @param rhs the reference to compare the invoking object to
 * @return a boolean value indicating if the two structs are equal
 */
bool GraphModel::Vertex::operator==(const Vertex &rhs) const
{
	if (this->isWaypoint == rhs.isWaypoint)
	{
		return this->position == rhs.position;
	}
	else
		return false;
}

/* ============== GraphModel CLASS ============== */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
GraphModel::GraphModel(string &directory, const double &sepThresh) : _dataDirectory(directory), _wpSeparation(sepThresh), _wpCount(0), _vertexCount(0) {}

/* ========== PUBLIC MUTATORS ========== */
/**
 * Processes a Tle object, creates a vertex, and inserts it into the graph. This function does not insert waypoints (that is automatic)
 * @param tle the Tle object used to determine the position
 */
void GraphModel::insertVertex(Tle &tle)
{
	// TODO: implement insertVertex() function
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
const GraphModel::Vertex& GraphModel::testGetNearestWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos, const Vertex &refWaypoint)
{
	GraphModel graph(dataDirectory, wpSepThresh);
	auto waypoint = graph._getNearestWaypoint(pos);
}

/**
 * A test function used to test the results of _connectWaypoint()
 * @param waypointIndex the index of the waypoint
 * @return
 */
const vector<GraphModel::Edge> GraphModel::testConnectWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos)
{

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
	Vertex vertex;
	vertex.position = pos;
	vertex.isWaypoint = isWaypoint;

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
	_waypoints.insert(wpIndex);

	// Adds the information about the new waypoint
	_wpLatMap.insert({ pos.latitude, wpIndex });
	_wpLongMap.insert({ pos.longitude, wpIndex});

	// Connect the new waypoint to all the other waypoint
	_connectWaypoint(wpIndex);

	return _vertices[wpIndex];
}

/**
 * Inserts an edge from one index to another
 * @param from the index of the source
 * @param to the index of the destination
 */
void GraphModel::_insertEdge(const int &from, const int &to)
{
	// TODO: implement _insertEdge() function
}

/**
 * Inserts an waypoint edge from one index to another
 * @param from the index of the source waypoint
 * @param to the index of the destination waypoint
 */
void GraphModel::_insertWpEdge(const int &from, const int &to)
{
	Edge edge = { to, Utility::getDistance(_vertices[from].position, _vertices[to].position) };
	_wpAdjList[from].emplace_back(edge);
}

/**
 * Connects a new waypoint to all the other waypoints in the graph
 * @param index the index of the waypoint to connect
 */
void GraphModel::_connectWaypoint(const int &index)
{
	// Connect the waypoint to all the waypoints that are within the threshold distance
	for (const auto &wpIndex : _waypoints)
	{
		_insertWpEdge(index, wpIndex);
	}
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
	auto latLowerBound = pos.latitude - _wpSeparation;
	auto latUpperBound = pos.latitude + _wpSeparation;
	auto longLowerBound = pos.longitude - _wpSeparation;
	auto longUpperBound = pos.longitude + _wpSeparation;

	auto lowerBoundIter = _wpLatMap.lower_bound(latLowerBound);
	auto upperBoundIter = _wpLatMap.upper_bound(latUpperBound);

	// Iterate through the _wpLatMap to find possible waypoints
	unordered_set<int> wpCandidates;
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

/**
 * Performs a breath first search to return waypoints that are within range
 * @param range the threshold to test the waypoints
 * @return an unordered set of the indices for the waypoints that are within range
 */
unordered_set<int> GraphModel::_getWaypointsInRange(const int &sourceIndex, const double &range)
{

}