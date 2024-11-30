#include <limits>
#include "GraphModel.h"
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


/* ============== GraphModel CLASS ============== */
/* ---------------------------------------------- */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
GraphModel::GraphModel(string &directory, const double &sepThresh) : _dataDirectory(directory), _wpSeparation(sepThresh), _wpCount(0), _vertexCount(0), DataModel(directory) {}

/* ========== PUBLIC MUTATORS ========== */
/**
 * Performs the import process of the data files found in the data directory
 */
void GraphModel::import()
{
	// TODO: Implement the import() function for the GraphModel class
}

/**
 * Inserts a single datapoint into the data structure
 * @param tle the Tle object that defines the datapoint to insert
 */
void GraphModel::insert(const Tle &tle)
{
	// TODO: Implement the insert() function for the GraphModel class
}

/**
 * Processes a Tle object, creates a vertex, and inserts it into the graph. This function does not insert waypoints (that is automatic)
 * @param tle the Tle object used to determine the position
 */
void GraphModel::insertVertex(Tle &tle)
{
	// TODO: implement insertVertex() function for the GraphModel class
}

/* ========== PUBLIC ACCESSORS ========== */
/**
 * Performs a search of the data to return the catalog number of satellites that are within the radius of the position
 * @param position the position used to check for satellites
 * @param radius the range used to determine if a satellite is in range
 * @return a vector containing the unique catalog numbers of the satellites that are in range
 */
vector<int> GraphModel::search(const CoordGeodetic &position, const double &radius)
{
	// TODO: Implement search() function
}

/**
 * Gets a vertex by index by index from the graph
 * @param index the index of the vertex to be returned
 * @return a reference to the Vertex matching the index passes to the function
 */
const GraphModel::Vertex& GraphModel::getVertex(const int &index)
{
	return _vertices[index];
}

/**
 * Gets the edges for a given waypoint index
 * @param index the index of the waypoint whose edges should be returned
 * @return a reference to a vector of edges
 */
const vector<GraphModel::Edge>& GraphModel::getWaypointEdges(const int &index)
{
	return _wpAdjList[index];
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
bool GraphModel::testFindClosestWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos, const Vertex &refWaypoint)
{
	GraphModel graph(dataDirectory, wpSepThresh);
	auto wpIndex = graph._findNearestWaypoint(pos);
	auto waypoint = graph.getVertex(wpIndex);

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
bool GraphModel::testFindClosestWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos, const vector<CoordGeodetic> &waypoints, const Vertex &refWaypoint)
{
	GraphModel graph(dataDirectory, wpSepThresh);

	// Insert the waypoints into the graph
	for (const auto &wpPos : waypoints)
		graph._insertWaypoint(wpPos);

	// Get the index of the closest waypoint to pos
	auto wpIndex = graph._findNearestWaypoint(pos);

	// Compare the waypoint to refWaypoint
	auto waypoint = graph.getVertex(wpIndex);

	return waypoint == refWaypoint;
}

/**
 * A test function used to test the _connectWaypoint() function
 * @param dataDirectory the path to the data source for the GraphModel
 * @param wpSepThresh the threshold that causes a new waypoint to be generated
 * @param positions the positions used to setup the waypoints in the graph
 * @param waypoint the specific waypoint to check for edges
 * @param refEdges the required values for the edges for a given waypoint
 * @return a boolean value indicating if the test passes
 */
bool GraphModel::testConnectWaypoint(string &dataDirectory, const double &wpSepThresh, const vector<CoordGeodetic>& positions, const Vertex &waypoint, const vector<Edge> &refEdges)
{
	// Create an instance of the GraphModel class
	GraphModel graph(dataDirectory, wpSepThresh);

	// Test the positions passed to the function to generate waypoints in the graph
	for (const auto &pos : positions)
		graph._findNearestWaypoint(pos);

	// Get the edges for the waypoint passed to the function
	auto edges = graph.getWaypointEdges(waypoint.index);

	// Compare the edges to the refEdges
	for (const auto &edge : edges)
	{

	}
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
 * @return the index of the vertex closest to the position passed to the function
 */
int GraphModel::_findNearestWaypoint(const CoordGeodetic &pos)
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

	// If a valid waypoint was not found, insert a new one
	return _insertWaypoint(pos).index;
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