#pragma once
#include <string>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;

class DataModel {
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;

public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	explicit DataModel(string &directory);

	/* ========== PUBLIC MUTATORS ========== */
	virtual void import() = 0;
	virtual void insert(const Tle &tle) = 0;

	/* ========== PUBLIC ACCESSORS ========== */
	virtual vector<int> search(const CoordGeodetic &position, const double &radius) = 0;
};
