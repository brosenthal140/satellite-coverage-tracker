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
	/* ========== PUBLIC STRUCTS & ENUMS ========== */
	struct Vertex {
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
	[[nodiscard]] int waypointCount() const;

	/* ========== PUBLIC TEST METHODS ========== */
	static const Vertex& testGetNearestWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos, const Vertex &refWaypoint);
	static const vector<Edge> testConnectWaypoint(string &dataDirectory, const double &wpSepThresh, const CoordGeodetic &pos);

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;

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
	const Vertex& _getNearestWaypoint(const CoordGeodetic &pos);
	unordered_set<int> _getWaypointsInRange(const int &sourceIndex, const double &range);
};

