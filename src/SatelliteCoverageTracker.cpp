#include <iostream>
#include <chrono>
#include <algorithm>
#include "SatelliteCoverageTracker.h"
#include "TLEParser.h"
#include "GraphModel.h"
#include "LinearModel.h"
#include "Utility.h"

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

	// Load the catalog
	SatelliteCatalog catalog(_dataDirectory);
	_catalog = catalog;
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
	chrono::duration<double> graphInitTime{}, linearInitTime{}, graphSearchTime{}, linearSearchTime{};
	string directory, strLongitude, strLatitude, strRadius;
	cout << "Welcome to the Satellite Coverage Tracker CLI! This process will ask you several questions to get started:" << endl << endl;

	// Get the path to the data directory
	cout << "This process will require some .tle files that contain two-line elements representing satellite observations and a .csv with the current satellite situation report. These files can be downloaded from space-track.org" << endl;
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
	cout << tracker._observations.size() << " data points were imported..." << endl;
	cout << "Import process execution time: " << elapsed.count() << " second(s)" << endl << endl;

	// Init the graph model and output the execution time
	cout << "Initializing the graph model..." << endl;
	start = chrono::high_resolution_clock::now();
	tracker._initModel(tracker._graphModel);
	end = chrono::high_resolution_clock::now();
	graphInitTime = end - start;
	cout << "Graph model init execution time: " << graphInitTime.count() << " second(s)" << endl << endl;

	// Init the linear model and output the execution time
	cout << "Initializing the linear model..." << endl;
	start = chrono::high_resolution_clock::now();
	tracker._initModel(tracker._linearModel);
	end = chrono::high_resolution_clock::now();
	linearInitTime = end - start;
	cout << "Linear model init execution time: " << linearInitTime.count() << " second(s)" << endl << endl;

	// Perform a search on the graph data model
	cout << "Performing a search using the graph model..." << endl;
	start = chrono::high_resolution_clock::now();
	auto result = tracker._performSearch(tracker._graphModel);
	end = chrono::high_resolution_clock::now();

	// Filter the results for starlink satellites
	unordered_set<string> graphFilteredResults;
	std::copy_if(result.begin(), result.end(), std::inserter(graphFilteredResults, graphFilteredResults.end()),[] (const string &s) { return s.find("STARLINK") != std::string::npos; });

	graphSearchTime = end - start;
	cout << "Graph search execution time: " << graphSearchTime.count() << " second(s)" << endl;
	cout << "Number of StarLink satellites that were observed overhead within the radius: " << graphFilteredResults.size() << endl;
	cout << "Satellite names: " << Utility::toString(graphFilteredResults) << endl << endl;

	// Perform a search on the linear data model
	cout << "Performing a search using the linear model..." << endl;
	start = chrono::high_resolution_clock::now();
	result = tracker._performSearch(tracker._linearModel);
	end = chrono::high_resolution_clock::now();

	// Filter the results for starlink satellites
	unordered_set<string> linearFilteredResults;
	std::copy_if(result.begin(), result.end(), std::inserter(linearFilteredResults, linearFilteredResults.end()),[] (const string &s) { return s.find("STARLINK") != std::string::npos; });

	linearSearchTime = end - start;
	cout << "Linear search execution time: " << linearSearchTime.count() << " second(s)" << endl << endl;
	cout << "Number of Starlink satellites that were observed overhead within the radius: " << linearFilteredResults.size() << endl;
	cout << "Satellite names: " << Utility::toString(linearFilteredResults) << endl << endl;

	// Output some stats
	cout << "Statistics:" << endl;
	// Init stats
	double initRatio = graphInitTime / linearInitTime;
	cout << fixed << setprecision(1) << "Init time difference (graph init time - linear init time): " << (graphInitTime - linearInitTime).count() << " seconds" << endl;
	cout << fixed << setprecision(1) << "The graph model took " << initRatio << " times longer to initialize than the linear model" << endl << endl;

	// Search stats
	double searchRatio = linearSearchTime / graphSearchTime;
	double searchDifMilli = chrono::duration<double, milli>(linearSearchTime - graphSearchTime).count();
	cout << fixed << setprecision(1) << "Search time difference (linear search time - graph search time in milliseconds): " << searchDifMilli << " milliseconds" << endl;
	cout << fixed << setprecision(1) << "The linear model took " << searchRatio << " times longer to complete the search than the graph model" << endl << endl;

	// Output num cycles to break even
	int numCyclesToJustifyInitTime = ceil((graphInitTime - linearInitTime).count() / (linearSearchTime - graphSearchTime) .count());
	cout << "After " << numCyclesToJustifyInitTime << " searches the graph model will begin to outperform the linear model in terms of total execution time required" << endl;
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
 * @return an unordered set of names associated with the NORAD category numbers returned from the search
 */
unordered_set<string> SatelliteCoverageTracker::_performSearch(DataModel *model)
{
	auto catNums = model->search(_locationRef, _radius);

	unordered_set<string> satNames;
	string name;
	for (const auto &catNum : catNums)
	{
		auto catEntry = _catalog.getCatalogEntry(catNum);
		name = catEntry.objectName;
		if (!name.empty())
			satNames.insert(name);
	}

	return satNames;
}
