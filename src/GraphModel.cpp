#include <vector>
#include <set>
#include <limits>
#include "GraphModel.h"
#include "Utility.h"

/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
GraphModel::GraphModel(string &directory, double &threshold) : _dataDirectory(directory), _wpThreshold(threshold), _wpCount(0) {}

/* ========== PUBLIC MUTATORS ========== */
void GraphModel::insertEdge(Tle &tle)
{
	_insertEdge(tle);
}

/* ========== PUBLIC ACCESSORS ========== */
int GraphModel::waypointCount() const
{
	return _wpCount;
}

/* ========== PUBLIC TEST METHODS ========== */
GraphModel::Vertex GraphModel::testGetNearestWaypoint(const CoordGeodetic &pos)
{
	return _getNearestWaypoint(pos);
}


/* ========== PRIVATE INSERTION METHODS ========== */
// This function should only be called from the _getNearestWaypoint() if there is not a waypoint within the threshold
const GraphModel::Vertex& GraphModel::_insertWaypoint(const CoordGeodetic &pos)
{
	Vertex vertex = { pos };
	int wpIndex = _wpCount;
	_waypoints[wpIndex] = vertex;
	_wpCount++;

	_wpLatMap[pos.latitude] = wpIndex;
	_wpLongMap[pos.longitude] = wpIndex;

	return _waypoints[wpIndex];
}
void GraphModel::_insertEdge( Tle &tle)
{

}

/* ========== PRIVATE SEARCH METHODS ========== */
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
		auto wp = _waypoints[wpIndex];

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
		auto wp = _waypoints[wpIndex];

		if (wp.position.latitude >= latLowerBound && wp.position.latitude <= latUpperBound)
			wpCandidates.insert(wpIndex);
	}

	// If a there are still waypoint candidates, get the nearest waypoint
	double minDist = numeric_limits<double>::max(), testDist;
	int selectedIndex = -1;
	for (const auto &wp : wpCandidates)
	{
		testDist = Utility::getDistance(pos, _waypoints[wp].position);

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
		return _waypoints[selectedIndex];
}