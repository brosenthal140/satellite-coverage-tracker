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
	unordered_map<int, vector<CoordGeodetic>> _observations;

	/* ========== PRIVATE INSERTION METHODS ========== */
	void _insert(const int &catNum, const CoordGeodetic &pos);

public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	LinearModel(string &dataDirectory);

	/* ========== PUBLIC MUTATORS ========== */
	void import() override;
	void insert(const Tle &tle) override;

	/* ========== PUBLIC ACCESSORS ========== */
	unordered_set<int> search(const CoordGeodetic &position, const double &radius) override;

};