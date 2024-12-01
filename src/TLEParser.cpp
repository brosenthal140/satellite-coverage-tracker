#include <sstream>
#include <vector>
#include "TLEParser.h"

/* =========== PUBLIC STATIC METHODS ============ */
Tle TLEParser::parse(const string &tleString)
{
	// Split the string passed to the function into three lines
	vector<string> lines(3);
	string line;
	istringstream stream(tleString);
	while (getline(stream, line))
	{
		lines.emplace_back(line);
	}

	// Call private _parse() function
	if (lines.size() == 3)
		return _parse(lines[0], lines[1], lines[2]);
	else
		return _parse(lines[0], lines[1]);
}
Tle TLEParser::parse(const string &line1, const string &line2)
{
	return _parse(line1, line2);
}
Tle TLEParser::parse(const string &line1, const string &line2, const string &line3)
{
	return _parse(line1, line2, line3);
}

CoordGeodetic TLEParser::getCoordGeodetic(const Tle &tle)
{
	SGP4 sgp4(tle);

	return sgp4.FindPosition(tle.Epoch()).ToGeodetic();
}
CoordGeodetic TLEParser::getCoordGeodetic(const string &line1, const string &line2, const string &line3)
{
	auto tle = TLEParser::parse(line1, line2, line3);

	return TLEParser::getCoordGeodetic(tle);
}
CoordGeodetic TLEParser::getCoordGeodetic(const string &line1, const string &line2)
{
	auto tle = TLEParser::parse(line1, line2);

	return TLEParser::getCoordGeodetic(tle);
}

/* =============== PRIVATE METHODS ===============
 * ---------------------------------------------- */
Tle TLEParser::_parse(const string &line1, const string &line2, const string &line3)
{
	Tle tle(line1, line2, line3);
	return tle;
}
Tle TLEParser::_parse(const string &line1, const string &line2)
{
	Tle tle(line1, line2);
	return tle;
}
