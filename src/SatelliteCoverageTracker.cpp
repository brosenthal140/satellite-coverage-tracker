#include <iostream>
#include <chrono>
#include "SatelliteCoverageTracker.h"
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
	double latitude, longitude;
	string directory, strLongitude, strLatitude, strRadius;
	cout << "Welcome to the Satellite Coverage Tracker CLI! This process will ask you several questions to get started:" << endl << endl;

	// Get the path to the data directory
	cout << "This process will require some .tle files that contain two-line elements representing satellite observations and a .csv with the current satellite situation report . These files can be downloaded from space-track.org" << endl;
	cout << "Enter the path to where your data can be found: ";
	cin >> directory;

	// Get the location
	cout << endl << endl << "To perform the search, you need to reference a location. This process will assume a geodetic location in latitude and longitude." << endl;
	cout << "Enter the latitude (in degrees): ";
	cin >> strLatitude;
	cout << "Enter the longitude (in degrees): ";
	cin >> strLongitude;

	latitude = stod(strLatitude);
	longitude = stod(strLongitude);
	CoordGeodetic position(latitude, longitude, 0, false);

	// Get the radius
	cout << endl << endl << "Enter a radius (in kilometers) to perform the search: ";
	cin >> strRadius;
	double radius = stod(strRadius);

	// Create an instance of the SatelliteCoverageTracker class
	SatelliteCoverageTracker tracker(directory, position, radius);

	// Import the data and output the execution time
	cout << endl << endl << "Importing the data into the tracker..." << endl;
	auto start = chrono::high_resolution_clock::now();
	tracker._importData();
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;
	cout << "Import process execution time: " << elapsed.count() << " second(s)" << endl << endl;

	// Init the graph model and output the execution time
	cout << "Initializing the graph model... This could take several minutes" << endl;
	start = chrono::high_resolution_clock::now();
	tracker._initModel(tracker._graphModel);
	end = chrono::high_resolution_clock::now();
	elapsed = end - start;
	cout << "Graph model init execution time: " << elapsed.count() << " second(s)" << endl << endl;

	// Init the linear model and output the execution time
	cout << "Initializing the linear model... This could take several minutes" << endl;
	start = chrono::high_resolution_clock::now();
	tracker._initModel(tracker._linearModel);
	end = chrono::high_resolution_clock::now();
	elapsed = end - start;
	cout << "Linear model init execution time: " << elapsed.count() << " second(s)" << endl << endl;

	// Perform a search on the graph data model
	cout << "Performing a search using the graph model..." << endl;
	start = chrono::high_resolution_clock::now();
	tracker._performSearch(tracker._graphModel);
	end = chrono::high_resolution_clock::now();
	elapsed = end - start;
	cout << "Graph search execution time: " << elapsed.count() << " second(s)" << endl << endl;
	// TODO: Output information about the graph search

	// Perform a search on the linear data model
	cout << "Performing a search using the graph model..." << endl;
	start = chrono::high_resolution_clock::now();
	tracker._performSearch(tracker._linearModel);
	end = chrono::high_resolution_clock::now();
	elapsed = end - start;
	cout << "Linear search execution time: " << elapsed.count() << " second(s)" << endl << endl;
	// TODO: Output information about the linear search
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

/**
 * Performs a search using the data model passed to the function
 * @param model the pointer that points to the data model to perform the search with
 * @return an unordered set of NORAD category numbers
 */
unordered_set<int> SatelliteCoverageTracker::_performSearch(DataModel *model)
{
	return model->search(_locationRef, _radius);
}
