#pragma once
#include <string>
#include "SGP4.h"
#include "DataModel.h"
using namespace std;
using namespace libsgp4;


struct SatelliteCoverageTracker {
public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	SatelliteCoverageTracker(const string &pathToData, const CoordGeodetic &location, const double &radius);

	/* ========== PUBLIC ACCESSORS ========== */


	/* ========== PUBLIC MUTATORS ========== */

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _pathToDataDirectory;
	CoordGeodetic _locationRef;
	double _radius;
	DataModel* _dataModel;

	/* ========== PRIVATE HELPER FUNCTIONS ========== */
	void _importData();
};

