#pragma once
#include <string>
#include <map>
#include "SGP4.h"

using namespace std;
using namespace libsgp4;

class SatelliteCatalog {
public:
	/* ========== PUBLIC STRUCTS & ENUMS ========== */
	enum RCS_Size {
		unknown,
		small,
		medium,
		large
	};

	struct CatalogEntry {
		string objectID;
		string objectName;
		int catalogNumber;
		string country;
		double period;
		double inclination;
		int apogee;
		int perigee;
		RCS_Size rcsSize;
		int rcsValue;
		string comment;
		DateTime launchDate;
	};

	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	explicit SatelliteCatalog(string pathToDataDirectory);

	/* ========== PUBLIC ACCESSORS ========== */
	const CatalogEntry& getCatalogEntry(const int &catNum);
	[[nodiscard]] int count() const;

	/* ========== PUBLIC MUTATORS ========== */

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _directory;
	string _pathToCatalogFile;
	map<int, CatalogEntry> _catalog;
	int _count;

	/* ========== PRIVATE HELPER FUNCTIONS ========== */
	void _loadCatalog();

	/* ========== PRIVATE STATIC HELPER FUNCTIONS ========== */
	static string _removeQuotes(string &str);
	static double cellToDouble(string &cell);
	static int cellToInt(string &cell);
	static string selectCatalogFile(string &directoryPath);
};
