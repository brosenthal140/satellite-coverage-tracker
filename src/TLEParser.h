#pragma once
#include <string>
#include <SGP4.h>

using namespace std;
using namespace libsgp4;

class TLEParser {
	/* ========== PRIVATE MEMBER VARIABLES ========== */

	/* ========== PRIVATE METHODS ========== */
	static Tle _parse(const string &line1, const string &line2, const string &line3);
	static Tle _parse(const string &line1, const string &line2);

public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	TLEParser() = default;

	/* ========== PUBLIC STATIC METHODS ========== */
	static Tle parse(const string &tleString);
	static Tle parse(const string &line1, const string &line2);
	static Tle parse(const string &line1, const string &line2, const string &line3);
	static CoordGeodetic getCoordGeodetic(const string &line1, const string &line2, const string &line3);
	static CoordGeodetic getCoordGeodetic(const string &line1, const string &line2);
};
