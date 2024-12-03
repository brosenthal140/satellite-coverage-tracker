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


/* ========== PUBLIC METHODS ========== */
/**
 * This process guides the user through the process of using the SatelliteCoverageTracker class
 */
void SatelliteCoverageTracker::runCLI()
{
	// Get the path to the data directory

	// Get the location

	// Get the radius

	// Create an instance of the SatelliteCoverageTracker class

	// Import the data and output the execution time

	// Init the graph model and output the execution time

	// Init the linear model and output the execution time

	// Perform a search on the graph data model

	// Perform a search on the linear data model

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

/**
 * Inserts all the observations into the model that the pointer points to
 * @param model a constant pointer to the model that needs to be initialized
 */
void SatelliteCoverageTracker::_initModel(DataModel *model)
{
	// Insert each observation into the model
	for (const auto &observation : _observations)
		model->insert(observation);
}
