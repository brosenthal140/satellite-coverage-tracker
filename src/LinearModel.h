#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "SGP4.h"
#include "DataModel.h"

using namespace std;
using namespace libsgp4;

class LinearModel: public DataModel {
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;
	unordered_map<int, vector<CoordGeodetic>> _observations;
	unsigned int _observationCount;

	/* ========== PRIVATE INSERTION METHODS ========== */
	void _insert(const int &catNum, const CoordGeodetic &pos);

public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	explicit LinearModel(string &dataDirectory);

	/* ========== PUBLIC MUTATORS ========== */
	void import() override;
	void insert(const Tle &tle) override;

	/* ========== PUBLIC ACCESSORS ========== */
	unordered_set<int> search(const CoordGeodetic &position, const double &radius) override;

	/* ========== PUBLIC TEST METHODS ========== */
	static bool testInsert(string &dataDirectory, const Tle &tle);

};