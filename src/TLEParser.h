#pragma once
#include <string>
#include <SGP4.h>

using namespace std;
using namespace libsgp4;

struct TLEParser {
private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */

	/* ========== PRIVATE METHODS ========== */
	static Tle _parse(const string &line1, const string &line2, const string &line3);
	static Tle _parse(const string &line1, const string &line2);

public:
	/* ========== PUBLIC STATIC METHODS ========== */
	static Tle parse(string &tleString);
	static Tle parse(string &line1, string &line2);
	static Tle parse(string &line1, string &line2, string &line3);

	static CoordGeodetic getCoordGeodetic(const Tle &tle);
	static CoordGeodetic getCoordGeodetic(string &line1, string &line2, string &line3);
	static CoordGeodetic getCoordGeodetic(string &line1, string &line2);

	static vector<string> getTLEFiles(const string &directoryPath);
	static vector<Tle> parseTLEFile(const string& tlePath, bool isThreeLine);
	static void parseTLEFile(const string& tlePath, vector<Tle> &observations, bool isThreeLine);
};
