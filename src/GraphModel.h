#pragma once
#include <string>
#include <map>
#include <unordered_set>
#include <vector>
#include <map>
#include "SGP4.h"
#include "DataModel.h"

using namespace std;
using namespace libsgp4;

class GraphModel : public DataModel {
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

		bool operator==(const Edge &rhs) const;
	};

	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	GraphModel(string &directory);
	~GraphModel() override;

	/* ========== PUBLIC MUTATORS ========== */
	void import() override;
	void insert(const Tle &tle) override;

	/* ========== PUBLIC ACCESSORS ========== */
	unordered_set<int> search(const CoordGeodetic &position, const double &radius) override; // Returns a vector of the satellite catalog numbers

	/* ========== PUBLIC TEST METHODS ========== */
	static bool testFindClosestWaypoint(string &dataDirectory, const CoordGeodetic &pos, const Vertex &refWaypoint);
	static bool testFindClosestWaypoint(string &dataDirectory, const CoordGeodetic &pos, const vector<CoordGeodetic> &waypoints, const Vertex &refWaypoint);
	static bool testInsert(string &dataDirectory, const Tle &tle, const Vertex &refVertex);
	static bool testFilterEdges(const vector<Edge> &edges, const double &maxWeight, const vector<Edge> &refEdges);
	static unordered_set<int> testSearch(string &dataDirectory, const vector<Tle> &observations, const CoordGeodetic &pos, const double &radius);
	static void testImport(string &dataDirectory);

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;

	map<int, const Tle&> _observations; // key: vertex index, value: Tle of observation
	unordered_set<int> _indices;
	map<int, Vertex> _vertices;
	map<int, vector<Edge>> _vertexAdjList;
	int _vertexCount;

	map<int, vector<Edge>> _wpAdjList;
	unordered_set<int> _waypoints;
	double _wpSeparation = 10000; // units: kilometers
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
	int _findNearestWaypoint(const CoordGeodetic &pos, bool insertOnFailure);
	unordered_set<int> _findVerticesWithinRange(const CoordGeodetic &pos, const double &range);
	unordered_set<int> _findWaypointsWithinRange(const CoordGeodetic &pos, const double &range);
	unordered_set<int> _filterCluster(const int &wpIndex, const double &range);

	/* ========== PRIVATE HELPER METHODS ========== */
	static vector<Edge> _filterByWeight(const vector<Edge> &edges, const double &maxWeight);
};
