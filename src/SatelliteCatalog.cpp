#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <filesystem>
#include "SatelliteCatalog.h"

namespace fs = filesystem;

/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
SatelliteCatalog::SatelliteCatalog(string pathToDataDirectory) : _directory(std::move(pathToDataDirectory)), _count(0)
{
	_pathToCatalogFile = selectCatalogFile(_directory);
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

		// Require that the first link contain the appropriate column titles
		string line, cell;
		getline(catalogFile, line);
		auto correctLineZero = "OBJECT_ID,OBJECT_NAME,NORAD_CAT_ID,COUNTRY,PERIOD,INCLINATION,APOGEE,PERIGEE,RCS_SIZE,RCSVALUE,LAUNCH,COMMENT";

		if (line != correctLineZero)
			throw runtime_error("The CSV file does not contain the columns that are expected by this process! The catalog could not be imported!");

		// Read the file line by line
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


/* ========== PRIVATE STATIC HELPER FUNCTIONS ========== */
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
string SatelliteCatalog::selectCatalogFile(string &directoryPath)
{
	try {
		if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
		{
			throw std::runtime_error("Directory does not exist or is not accessible.");
		}

		string newestCSV;
		filesystem::file_time_type newestTime;

		for (const auto& entry : fs::directory_iterator(directoryPath))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".csv") {
				auto currentFileTime = fs::last_write_time(entry);

				if (newestCSV.empty() || newestTime < currentFileTime)
				{
					newestTime = currentFileTime;
					newestCSV = entry.path().string();
				}
			}
		}

		if (newestCSV.empty())
		{
			throw runtime_error("No CSV files found in the directory.");
		}

		return newestCSV;
	}
	catch (const exception& error)
	{
		cerr << "Error: " << error.what() << endl;
		return "";
	}
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