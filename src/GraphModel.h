#pragma once
#include <string>
#include <map>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;

class GraphModel {
public:
	/* ========== STRUCTS & ENUMS ========== */
	struct Vertex {
		CoordGeodetic position;
	};

	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	GraphModel(string &directory, double &threshold);

	/* ========== PUBLIC MUTATORS ========== */
	void insertEdge(Tle &tle);

	/* ========== PUBLIC ACCESSORS ========== */
	int waypointCount() const;

	/* ========== PUBLIC TEST METHODS ========== */
	Vertex testGetNearestWaypoint(const CoordGeodetic &pos);

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;
	map<int, Vertex> _waypoints;
	map<double, int> _wpLatMap;
	map<double, int> _wpLongMap;
	double _wpThreshold;
	int _wpCount;

	/* ========== PRIVATE INSERTION METHODS ========== */
	const Vertex& _insertWaypoint(const CoordGeodetic &pos);
	void _insertEdge(Tle &tle);

	/* ========== PRIVATE SEARCH METHODS ========== */
	const Vertex& _getNearestWaypoint(const CoordGeodetic &pos);
};

