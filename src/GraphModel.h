#pragma once
#include <string>
#include <map>
#include <unordered_set>
#include <vector>
#include <map>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;

class GraphModel {
public:
	/* ========== NESTED STRUCTS & ENUMS ========== */
	struct Vertex {
		int index;
		bool isWaypoint;
		CoordGeodetic position;

		bool operator==(const Vertex &rhs) const;
	};

	struct Edge {
		int index;
		double weight;
	};

	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	GraphModel(string &directory, const double &sepThresh);

	/* ========== PUBLIC MUTATORS ========== */
	void insertVertex(Tle &tle);

	/* ========== PUBLIC ACCESSORS ========== */
	const Vertex& getVertex(const int &index);
	const vector<Edge>& getWaypointEdges(const int &index);

	/* ========== PUBLIC TEST METHODS ========== */
	static bool testFindClosestWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos, const Vertex &refWaypoint);
	static bool testFindClosestWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos, const vector<CoordGeodetic> &waypoints, const Vertex &refWaypoint);
	static bool testConnectWaypoint(string &dataDirectory, const double &wpSepThresh, const vector<CoordGeodetic> &positions, const Vertex &waypoint, const vector<Edge> &refEdges);

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;

	unordered_set<int> _indices;
	map<int, Vertex> _vertices;
	map<int, vector<Edge>> _vertexAdjList;
	int _vertexCount;

	map<int, vector<Edge>> _wpAdjList;
	unordered_set<int> _waypoints;
	map<double, int> _wpLatMap;
	map<double, int> _wpLongMap;
	double _wpSeparation;
	int _wpCount;

	/* ========== PRIVATE INSERTION METHODS ========== */
	int _getNextVertexIndex();
	int _insertVertex(const CoordGeodetic &pos, bool isWaypoint);
	const Vertex& _insertWaypoint(const CoordGeodetic &pos);
	void _insertEdge(const int &from, const int &to);
	void _insertWpEdge(const int &from, const int &to);
	void _connectWaypoint(const int &index);

	/* ========== PRIVATE SEARCH METHODS ========== */
	static int _getIndexWithMinimumDistance(map<int, double> &distances);
	map<int, double> _dijkstra(const CoordGeodetic &pos, const map<int, vector<Edge>> &graph);
	int _findNearestVertex(const CoordGeodetic &pos);
	int _findNearestWaypoint(const CoordGeodetic &pos);
	unordered_set<int> _findVerticesWithinRange(const CoordGeodetic &pos, const double &range);
};
