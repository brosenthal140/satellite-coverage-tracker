#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;

class GraphModel {
public:
	/* ========== STRUCTS & ENUMS ========== */
	struct Vertex {
		bool isWaypoint;
		CoordGeodetic position;
	};

	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	GraphModel(string &directory, double &threshold);

	/* ========== PUBLIC MUTATORS ========== */
	void insertVertex(Tle &tle);

	/* ========== PUBLIC ACCESSORS ========== */
	[[nodiscard]] int waypointCount() const;

	/* ========== PUBLIC TEST METHODS ========== */
	const Vertex& testGetNearestWaypoint(const CoordGeodetic &pos);

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;
	unordered_map<int, Vertex> _vertices;
	unordered_map<int, vector<int>> _adjacencyList;
	set<int> _waypoints;
	map<double, int> _wpLatMap;
	map<double, int> _wpLongMap;
	double _wpThreshold;
	int _wpCount;
	int _vertexCount;

	/* ========== PRIVATE INSERTION METHODS ========== */
	int _getNextVertexIndex();
	int _insertVertex(const CoordGeodetic &pos, bool isWaypoint);
	const Vertex& _insertWaypoint(const CoordGeodetic &pos);
	void _insertEdge(int &from, int &to);

	/* ========== PRIVATE SEARCH METHODS ========== */
	const Vertex& _getNearestWaypoint(const CoordGeodetic &pos);
};

