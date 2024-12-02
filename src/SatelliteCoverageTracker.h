#pragma once
#include <string>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;


struct SatelliteCoverageTracker {
public: 
	void importDataFromLocalFile(const std::string& inputFilePath);
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	SatelliteCoverageTracker(const string &pathToData, const DateTime &dateTime, const CoordGeodetic &location);

	/* ========== PUBLIC ACCESSORS ========== */


	/* ========== PUBLIC MUTATORS ========== */

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _pathToDataDirectory;
	DateTime _dateTimeRef;
	CoordGeodetic _locationRef;

	/* ========== PRIVATE HELPER FUNCTIONS ========== */
	void _importData();
};

