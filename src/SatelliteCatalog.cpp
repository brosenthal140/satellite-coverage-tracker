#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include "SatelliteCatalog.h"

/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
SatelliteCatalog::SatelliteCatalog(string pathToCatalogFile) : _pathToCatalogFile(std::move(pathToCatalogFile)), _count(0)
{
	_loadCatalog();
}

/* ========== PRIVATE HELPER FUNCTIONS ========== */
void SatelliteCatalog::_loadCatalog()
{
	try
	{
		// Attempt to open the file
		ifstream catalogFile(_pathToCatalogFile);

		if (!catalogFile.is_open())
			throw runtime_error("File could not be opened! Catalog data was not imported!");

		// Read the file line by line
		string line, cell;
		getline(catalogFile, line);
		while (getline(catalogFile, line))
		{
			// Create a string stream and split it by a ',' delimiter
			vector<string> result;
			stringstream lineStream(line);

			while (getline(lineStream, cell, ','))
			{
				result.emplace_back(_removeQuotes(cell));
			}

			// Check for a trailing comma
			if (!lineStream && cell.empty())
				result.emplace_back("");

			// Create the CatalogEntry and add it to the catalog
			CatalogEntry catEntry;
			catEntry.objectID = result[0];
			catEntry.objectName = result[1];
			catEntry.catalogNumber = cellToInt(result[2]);
			catEntry.country = result[3];
			catEntry.period = cellToDouble(result[4]);
			catEntry.inclination = cellToDouble(result[5]);
			catEntry.apogee = cellToInt(result[6]);
			catEntry.perigee = cellToInt(result[7]);

			// Determine the radar cross-section size
			string rcs = result[8];
			if (rcs == "LARGE")
				catEntry.rcsSize = large;
			else if (rcs == "MEDIUM")
				catEntry.rcsSize = medium;
			else if (rcs == "SMALL")
				catEntry.rcsSize = small;
			else
				catEntry.rcsSize = unknown;

			catEntry.rcsValue = cellToInt(result[9]);

			// Create a DateTime object from the results[11]
			string launchDateStr = result[10];
			if (!launchDateStr.empty())
			{
				auto year = stoi(launchDateStr.substr(0, 4));
				auto month = stoi(launchDateStr.substr(5, 2));
				auto day = stoi(launchDateStr.substr(8, 2));
				catEntry.launchDate = DateTime(year, month, day);
			}
			else
				catEntry.launchDate = DateTime();

			catEntry.comment = result[11];

			_catalog[catEntry.catalogNumber] = catEntry;
			_count++;
		}
	}
	catch (const runtime_error& error)
	{
		cerr << "Error: " << error.what() << endl;
	}
}

string SatelliteCatalog::_removeQuotes(string &str)
{
	return str.substr(1, str.size() - 2);
}
double SatelliteCatalog::cellToDouble(string &cell)
{
	if (cell.empty())
		return 0.0;
	else
		return stod(cell);
}
int SatelliteCatalog::cellToInt(string &cell)
{
	if (cell.empty())
		return 0;
	else
		return stoi(cell);
}


/* ========== PUBLIC ACCESSORS ========== */
const SatelliteCatalog::CatalogEntry& SatelliteCatalog::getCatalogEntry(const int &catNum)
{
	return _catalog[catNum];
}
int SatelliteCatalog::count() const
{
	return _count;
}