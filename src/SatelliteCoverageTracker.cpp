#include "SatelliteCoverageTracker.h"
#include "TLEParser.h"
#include <fstream>
using namespace std; 

/* =============== PUBLIC METHODS =============== */
/* ---------------------------------------------- */


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
