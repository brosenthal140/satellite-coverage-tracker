#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "TLEParser.h"
using namespace std;

/* =========== PUBLIC METHODS ============ */
bool TLEParser::fetchTLEDataFromFile(const string& inputFilePath, const string& outputPath) {
	//-------- takes input and output from file ------------//
	ifstream inFile(inputFilePath);
	ofstream outFile(outputPath);

	//-------- checks to see if input and output files are opened, if not, print statement ------//
	if (!inFile.is_open() || !outFile.is_open()) {
		string statement1, statement2;
		statement1 = "Open input failed: ", statement2 = " Open output failed: ";
		if (!inFile.is_open()) {
			cerr << statement1 << inputFilePath << endl;
		}
		if (!outFile.is_open()) {
			cerr << statement2 << outputPath << endl;
		}
		return false;
	}

	outFile << inFile.rdbuf();

	inFile.close();
	outFile.close();

	return true;
}

void TLEParser::parseTLE(const string& tlePath) {
	//------Open checks, to print error if cannot --------//
	ifstream inFile(tlePath);
	if (!inFile.is_open()) {
		string errorstatement;
		errorstatement = "Can't open TLE file: ";
		cerr << errorstatement << tlePath << endl;
		return;
	}

	string objectName, tleLine1, tleLine2;
	while (getline(inFile, objectName)) {
		if (getline(inFile, tleLine1) && getline(inFile, tleLine2)) {
			string ObjectName, TLE1, TLE2;
			ObjectName = "Object Name: ", TLE1 = "TLE Line 1: ", TLE2 = "TLE Line 2: ";
			//----- Output the description of Two Line Element -------//
			cout << ObjectName << objectName << endl;
			cout << TLE1 << tleLine1 << endl;
			cout << TLE2 << tleLine2 << endl;
		}
	}

	inFile.close();
}

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
