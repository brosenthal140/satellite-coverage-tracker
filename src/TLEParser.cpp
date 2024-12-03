#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "TLEParser.h"
using namespace std;

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

/**
 * Returns the file paths to all the files with the .tle extension in the directory
 * @param directoryPath the directory to search for files
 * @return a vector of strings representing the paths to the TLE files in that directory
 */
vector<string> getTLEFiles(const string& directoryPath) {
	vector<string> tleFiles;

	for (const auto& entry : filesystem::directory_iterator(directoryPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".tle")
		{
			tleFiles.push_back(entry.path().string());
		}
	}

	return tleFiles;
}

/**
 * Processes a file and outputs a vector of Tle objects
 * @param tlePath the path to the TLE file
 * @param isThreeLine indicates if the format is two-line element or three-line element
 * @return a vector of Tle objects
 */
vector<Tle> TLEParser::parseTLEFile(const string& tlePath, bool isThreeLine) {
	//------Open checks, to print error if cannot --------//
	ifstream inFile(tlePath);
	if (!inFile.is_open()) {
		string errorMsg;
		errorMsg = "Can't open TLE file: ";
		cerr << errorMsg << tlePath << endl;
	}

	string tleLine1, tleLine2, tleLine3;
	vector<Tle> observations;
	if (isThreeLine)
	{
		while (getline(inFile, tleLine1)) {
			if (getline(inFile, tleLine2) && getline(inFile, tleLine3))
				observations.emplace_back(_parse(tleLine1, tleLine2, tleLine3));
		}
	}
	else
	{
		while (getline(inFile, tleLine1)) {
			if (getline(inFile, tleLine2))
				observations.emplace_back(_parse(tleLine1, tleLine2));
		}
	}

	inFile.close();

	return observations;
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
