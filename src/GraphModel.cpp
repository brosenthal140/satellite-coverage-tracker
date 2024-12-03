#include <limits>
#include <algorithm>
#include "GraphModel.h"
#include "TLEParser.h"
#include "Utility.h"

/* ========== NESTED STRUCTS & ENUMS ========== */
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

/* ============== Edge STRUCT =============== */
/* ============= PUBLIC OPERATORS ============= */
/**
 * Equality operator for the Edge struct
 * @param rhs the reference to compare the invoking object to
 * @return a boolean value indicating if the two structs are equal
 */
bool GraphModel::Edge::operator==(const Edge &rhs) const
{
	if (this->index == rhs.index)
		return this->weight == rhs.weight;
	else
		return false;
}


/* ============== GraphModel CLASS ============== */
/* ---------------------------------------------- */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
GraphModel::GraphModel(string &directory) : _dataDirectory(directory), _wpCount(0), _vertexCount(0), DataModel(directory) {}

/* ========== PUBLIC MUTATORS ========== */
/**
 * Performs the import process of the data files found in the data directory
 */
void GraphModel::import()
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
 * Inserts a single datapoint into the data structure
 * @param tle the Tle object that defines the datapoint to insert
 */
void GraphModel::insert(const Tle &tle)
{
	// Get the position for the Tle passed to the function
	auto pos = TLEParser::getCoordGeodetic(tle);

	// Insert the observation into the graph
	auto index = _insertVertex(pos, false);

	// Update the observations map to associate the vertex number with the Tle
	_observations.insert({index, tle});
}


/* ========== PUBLIC ACCESSORS ========== */
/**
 * Performs a search of the data to return the catalog number of satellites that are within the radius of the position
 * @param position the position used to check for satellites
 * @param radius the range used to determine if a satellite is in range
 * @return a vector containing the unique catalog numbers of the satellites that are in range
 */
unordered_set<int> GraphModel::search(const CoordGeodetic &position, const double &radius)
{
	// Traverse the wpAdjList to find any other waypoints that are in range
	auto waypoints = _findWaypointsWithinRange(position, radius);

	// For each waypoint filter the associated cluster to find candidate vertices
	unordered_set<int> candidates;
	for (const auto &wpIndex: waypoints)
	{
		auto tempCandidates = _filterCluster(wpIndex, radius);

		candidates.insert(tempCandidates.begin(), tempCandidates.end());
	}

	// For each candidate verify the actual distance between the position and the candidate and filter indices whose distance from the candidate is below the radius threshold
	double distance;
	CoordGeodetic tempPos;
	unordered_set<int> filteredCandidates;
	for (const auto &index: candidates)
	{
		tempPos = _vertices[index].position;
		distance = Utility::getDistance(position, tempPos);

		if (distance < radius)
			filteredCandidates.insert(index);
	}

	// For each of the filtered candidates, look it up in _observations map to get the original Tle object
	unordered_set<int> catalogNumbers(filteredCandidates.size());
	for (const auto &filteredIndex : filteredCandidates)
		catalogNumbers.insert(_observations.at(filteredIndex).NoradNumber());

	return catalogNumbers;
}


/* ========== PUBLIC TEST METHODS ========== */
/**
 * A test function used to test the _getNearestWaypoint() function
 * @param dataDirectory the path to the data source for the GraphModel
 * @param wpSepThresh the threshold that causes a new waypoint to be generated
 * @param pos the position used to check for a waypoint
 * @param refWaypoint the required value that must be returned for the test to pass
 * @return a boolean value indicating if the test passes
 */
bool GraphModel::testFindClosestWaypoint(string &dataDirectory, const CoordGeodetic &pos, const Vertex &refWaypoint)
{
	GraphModel graph(dataDirectory);
	auto wpIndex = graph._findNearestWaypoint(pos, true);
	auto waypoint = graph._vertices[wpIndex];

	return waypoint == refWaypoint;
}

/**
 * A test function used to test the _findClosestWaypointDijkstra() function
 * @param dataDirectory the path to the data source for the GraphModel
 * @param wpSepThresh the threshold that causes a new waypoint to be generated
 * @param pos the position used to check for a waypoint
 * @param waypoints the waypoints the graph should contain for the test
 * @param refWaypoint the waypoint that should be returned
 * @return a boolean value indicating if the test passes
 */
bool GraphModel::testFindClosestWaypoint(string &dataDirectory, const CoordGeodetic &pos, const vector<CoordGeodetic> &waypoints, const Vertex &refWaypoint)
{
	GraphModel graph(dataDirectory);

	// Insert the waypoints into the graph
	for (const auto &wpPos : waypoints)
		graph._insertWaypoint(wpPos);

	// Get the index of the closest waypoint to pos
	auto wpIndex = graph._findNearestWaypoint(pos, true);

	// Compare the waypoint to refWaypoint
	auto waypoint = graph._vertices[wpIndex];

	return waypoint == refWaypoint;
}

/**
 * A test function used to test the insert() function
 * @param dataDirectory the path to the data source for the GraphModel
 * @param wpSepThresh the threshold that causes a new waypoint to be generated
 * @param tle a reference to a Tle object that will be inserted into the graph
 * @param refVertex a reference to a Vertex object used to test if the pass succeeded
 * @return a boolean value indicating if the test passes
 */
bool GraphModel::testInsert(string &dataDirectory, const Tle &tle, const Vertex &refVertex)
{
	// Create an instance of the GraphModel class
	GraphModel graph(dataDirectory);

	// Insert an observation
	graph.insert(tle);

	// Get the newly a reference to the newly inserted vertex
	auto vertex = graph._vertices[0];

	return refVertex == vertex;
}

/**
 * Tests the _filterByWeight() function
 * @param edges a reference to the vector of edges to be filtered
 * @param maxWeight the value used to determine the filtering
 * @param refEdges a reference to the vector of edges used to determine if the test passed
 * @return a boolean value indicating if the test passes
 */
bool GraphModel::testFilterEdges(const vector<Edge> &edges, const double &maxWeight, const vector<Edge> &refEdges)
{
	auto filteredEdges = GraphModel::_filterByWeight(edges, maxWeight);

	return (filteredEdges.size() == refEdges.size()) && equal(refEdges.begin(), refEdges.end(), filteredEdges.begin());
}

/**
 * Tests the search() function
 * @param dataDirectory the path to the data source for the GraphModel
 * @param wpSepThresh the threshold that causes a new waypoint to be generated
 * @param observations a vector of Tle objects to insert into the graph
 * @param pos the position to search for observations
 * @param radius the range of positions from the position that are valid
 * @return an unordered set containing the NORAD category numbers within range of the position
 */
unordered_set<int> GraphModel::testSearch(string &dataDirectory, const vector<Tle> &observations, const CoordGeodetic &pos, const double &radius)
{
	// Create an instance of the GraphModel class
	GraphModel graph(dataDirectory);

	// For each observation in observations, insert it into the graph
	for (const auto &tle : observations)
		graph.insert(tle);

	// Perform the search
	auto result = graph.search(pos, radius);

	// Compare the search result to the refSatCatNums
	return result;
}

/**
 * Tests the import() function
 * @param dataDirectory the path to the data source for the GraphModel
 * @param wpSepThresh the threshold that causes a new waypoint to be generated
 */
void GraphModel::testImport(string &dataDirectory)
{
	// Create an instance of the GraphModel class
	GraphModel graph(dataDirectory);

	// Perform the import process
	graph.import();
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
	int index = _getNextVertexIndex();
	Vertex vertex = { index, isWaypoint, pos };

	_vertices[index] = vertex;
	_indices.insert(index);

	// If the vertex is not a waypoint, insert an edge for it's cluster
	if (!isWaypoint)
	{
		auto wpIndex = _findNearestWaypoint(pos, true);
		_insertEdge(wpIndex, index);
	}

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

	// Initialize the waypoint in the waypoint adjacency list
	vector<Edge> edges;
	_wpAdjList[wpIndex] = edges;

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
	Edge edge = { to, Utility::getDistance(_vertices[from].position, _vertices[to].position) };

	_vertexAdjList[from].emplace_back(edge);
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
		if (wpIndex != index)
		{
			_insertWpEdge(index, wpIndex);
			_insertWpEdge(wpIndex, index);
		}
	}
}

/* ========== PRIVATE SEARCH METHODS ========== */
/**
 * Gets the minimum distance from a map passed to the function
 * @param distances a map of vertex indices and distances
 * @return the index of the vertex that has the minimum distance
 */
int GraphModel::_getIndexWithMinimumDistance(map<int, double> &distances)
{
	int closestVertex = -1;
	double minDistance = numeric_limits<double>::infinity();
	for (const auto& [index, distance] : distances)
	{
		if (distance < minDistance)
		{
			minDistance = distance;
			closestVertex = index;
		}
	}

	return closestVertex;
}

/**
 * Finds the shortest path distance between a source position and all vertices in the graph
 * @param pos the position used to make the distance comparisons
 * @param graph the adjacency list to determine the closest vertex for
 * @return a map connecting all the indices and the minimum distance
 */
map<int, double> GraphModel::_dijkstra(const CoordGeodetic& pos, const map<int, vector<Edge>> &graph)
{
	priority_queue<pair<double, int>, vector<pair<double, int>>,greater<>> pq;
	map<int, double> distances;
	map<int, bool> visited;

	for (const auto& [index, edges] : graph)
	{
		distances[index] = numeric_limits<double>::infinity();
		visited[index] = false;
	}

	int startVertex = -1;
	double minInitialDistance = numeric_limits<double>::infinity();
	for (const auto& [ index, edges] : graph)
	{
		double distance = Utility::getDistance(pos, _vertices[index].position);
		if (distance < minInitialDistance)
		{
			minInitialDistance = distance;
			startVertex = index;
		}
	}

	distances[startVertex] = 0.0;
	pq.emplace(0.0, startVertex);

	while (!pq.empty())
	{
		int currentVertex = pq.top().second;
		pq.pop();

		if (visited[currentVertex]) continue;
		visited[currentVertex] = true;

		for (const auto& edge : graph.at(currentVertex))
		{
			int neighbor = edge.index;
			double weight = edge.weight;

			if (distances[currentVertex] + weight < distances[neighbor])
			{
				distances[neighbor] = distances[currentVertex] + weight;
				pq.emplace(distances[neighbor], neighbor);
			}
		}
	}

	return distances;
}

/**
 * Finds the closest vertex index to a position for a group of indices and an adjacency list
 * @param pos the position used to make the distance comparisons
 * @return the index of the vertex closest to the position passed to the function
 */
int GraphModel::_findNearestVertex(const CoordGeodetic &pos)
{
	auto distMap = _dijkstra(pos, _vertexAdjList);

	return _getIndexWithMinimumDistance(distMap);
}

/**
 * Finds the closest waypoint in the waypoint adjacency list
 * @param pos the position used to make the distance comparisons
 * @param insertOnFailure if a valid waypoint is not found, insert a new one
 * @return the index of the vertex closest to the position passed to the function
 */
int GraphModel::_findNearestWaypoint(const CoordGeodetic &pos, bool insertOnFailure)
{
	if (_wpCount != 0)
	{
		auto distMap = _dijkstra(pos, _wpAdjList);
		auto wpIndex = _getIndexWithMinimumDistance(distMap);

		// Determine if the nearest waypoint is within the threshold or if a new waypoint should be inserted
		auto waypoint = _vertices[wpIndex];
		auto distFromWaypoint = Utility::getDistance(pos, waypoint.position);

		if (distFromWaypoint < _wpSeparation)
			return wpIndex;
	}

	// If a valid waypoint was not found, and insertOnFailure is true, insert a new one
	if (insertOnFailure)
		return _insertWaypoint(pos).index;
	else
		return -1;
}

/**
 * Finds the vertices that are within a certain range of a position
 * @param pos the position used to make the distance comparisons
 * @param range the threshold used to filter the distances to the vertices
 * @return
 */
unordered_set<int> GraphModel::_findVerticesWithinRange(const CoordGeodetic &pos, const double &range)
{
	auto distances = _dijkstra(pos, _vertexAdjList);

	unordered_set<int> inRange;
	for (const auto &[index, distance] : distances)
	{
		if (distance < range)
			inRange.insert(index);
	}

	return inRange;
}

/**
 * Finds the waypoints that are within a certain range of a position
 * @param pos the position used to make the distance comparisons
 * @param range the threshold used to filter the distances to the waypoints
 * @return
 */
unordered_set<int> GraphModel::_findWaypointsWithinRange(const CoordGeodetic &pos, const double &range)
{
	auto distances = _dijkstra(pos, _wpAdjList);

	unordered_set<int> inRange;
	for (const auto &[index, distance] : distances)
	{
		if (distance < range)
			inRange.insert(index);
	}

	return inRange;
}

/**
 * Traverses all the edges in a cluster and returns a set of the vertex indices within a specific range of a waypoint
 * @param waypoint a reference to the waypoint object for the cluster
 * @param range the value to filter the edges to associated with a waypoint
 * @return a set containing the indices that are within range of the waypoint
 */
unordered_set<int> GraphModel::_filterCluster(const int &wpIndex, const double &range)
{
	// Get the edges for the waypoint
	auto edges = _vertexAdjList[wpIndex];
	auto filteredEdges = _filterByWeight(edges, 2 * range); // 2 * range is used to capture all vertices that could be in range

	// Create a set of the indices
	unordered_set<int> indices;
	for (const auto &edge: filteredEdges)
	{
		indices.insert(edge.index);
	}

	return indices;
}

/* ========== PRIVATE HELPER METHODS ========== */
/**
 * Takes in a vector of edges and returns a filtered vector containing edges that are below a max weight
 * @param edges the vector of edges to filter
 * @param maxWeight the maximum value for an edge weight
 * @return a vector of edges
 */
vector<GraphModel::Edge> GraphModel::_filterByWeight(const vector<Edge> &edges, const double &maxWeight)
{
	vector<Edge> filteredEdges;
	copy_if(edges.begin(), edges.end(), back_inserter(filteredEdges), [&maxWeight] (Edge e) { return e.weight < maxWeight; });
	return filteredEdges;
}