#include "SatelliteCoverageTracker.h"

#include <utility>
#include "TLEParser.h"
#include "GraphModel.h"
#include "LinearModel.h"

using namespace std;
using namespace libsgp4;

/* =============== PUBLIC METHODS =============== */
/* ---------------------------------------------- */


/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
/**
 * A constructor for the SatelliteCoverageTracker class
 * @param pathToData the path to the directory containing the data file
 * @param location a reference to the location used for the search
 * @param radius a range used to filter observations adjacent to the target location
 */
SatelliteCoverageTracker::SatelliteCoverageTracker(string  pathToData, const CoordGeodetic& location, const double &radius)
    : _dataDirectory(std::move(pathToData)), _radius(radius), _locationRef(location)
{
	// Import all the .tle files
    _importData();

	// Initialize the two models
	_graphModel = new GraphModel(_dataDirectory);
	_linearModel = new LinearModel(_dataDirectory);
}

/**
 * Destructor that deletes dynamically allocated models
 */
SatelliteCoverageTracker::~SatelliteCoverageTracker()
{
	delete _graphModel;
	delete _linearModel;
}

/* =============== PRIVATE METHODS =============== */
/* ---------------------------------------------- */
/**
 * Imports all the .tle file found in the data directory
 */
void SatelliteCoverageTracker::_importData()
{
    // Get the file names with the .tle extension in the _dataDirectory
	auto paths = TLEParser::getTLEFiles(_dataDirectory);

	// For each file, process the files and insert the TLE entries into the model
	for (const auto &filePath : paths)
		TLEParser::parseTLEFile(filePath, _observations, false);
}
