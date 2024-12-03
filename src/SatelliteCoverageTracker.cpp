#include "SatelliteCoverageTracker.h"
#include "TLEParser.h"
#include <fstream>
using namespace std; 

/* =============== PUBLIC METHODS =============== */
/* ---------------------------------------------- */
void SatelliteCoverageTracker::importDataFromLocalFile(const string& inputFilePath) {
    TLEParser parser;
    const string outputPath = "data/tle_latest.txt";

    //--- Retrieves TLE data from file ---------//
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
SatelliteCoverageTracker::SatelliteCoverageTracker(const string& pathToData, const DateTime& dateTime, const CoordGeodetic& location)
    : _pathToDataDirectory(pathToData), _dateTimeRef(dateTime), _locationRef(location)
{
    _importData();
}

/* =============== PRIVATE METHODS =============== */
/* ---------------------------------------------- */

void SatelliteCoverageTracker::_importData()
{
    TLEParser parser;
    const string tleFilePath = _pathToDataDirectory + "/tle_data.txt";

    ifstream tleFile(tleFilePath);
    if (!tleFile.is_open()) {
        string error_statement;
        error_statement = "Could not open TLE file: ";
        cerr << error_statement << tleFilePath << endl;
        return;
    }

    string TLE_Line1, TLE_Line2, TLE_Line3;
    while (getline(tleFile, TLE_Line1)) {
        if (getline(tleFile, TLE_Line2) && getline(tleFile, TLE_Line3)) {
            
            Tle tle = TLEParser::parse(TLE_Line1, TLE_Line2, TLE_Line3);
            (*_dataModel).insert(tle);
        }
        else {
            string error_statement;
            error_statement = "TLE record found, but incomplete: ";
            cerr << error_statement << tleFilePath << endl;
            break;
        }
    }

    tleFile.close();
}
