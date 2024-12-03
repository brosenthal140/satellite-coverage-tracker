#include "SatelliteCoverageTracker.h"
#include "TLEParser.h"  
#include <iostream>
#include <string>
using namespace std; 

/* =============== PUBLIC METHODS =============== */
/* ---------------------------------------------- */
void SatelliteCoverageTracker::importDataFromLocalFile(const string& inputFilePath) {
    TLEParser parser;
    const string outputPath = "data/tle_latest.txt";

    //--- Retrieves TLE datat from file ---------//
    bool success = parser.fetchTLEDataFromFile(inputFilePath, outputPath);
    if (success) {
        string successful_statement;
        successful_statement = "Successfully retrieved TLE data.";
        cout << successful_statement << endl;
        parser.parseTLE(outputPath);
    }
    else {
        string unsuccessful_statement;
        unsuccessful_statement = "Unsuccessfully retrieved TLE data";
        cerr << unsuccessful_statement << endl;
    }
}

/* ========== CONSTRUCTORS/DESTRUCTORS ========== */

SatelliteCoverageTracker::SatelliteCoverageTracker(const string &pathToData, const DateTime &dateTime, const CoordGeodetic &location)
{
	// TODO: Implement constructor
}

/* =============== PRIVATE METHODS =============== */
/* ---------------------------------------------- */

void SatelliteCoverageTracker::_importData()
{
	// TODO: Implement internal import method
}
