#include <sstream>
#include <vector>
#include "TLEParser.h"

/* =============== PUBLIC METHODS ===============
 * ---------------------------------------------- */
/* ========== CONSTRUCTORS/DESTRUCTORS ========== */

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
    return _parse(lines[0], lines[1], lines[2]);
}
Tle TLEParser::parse(const string &l1, const string &l2, const string &l3)
{
    // Call private _parse() function
    return _parse(l1, l2, l3);
}

/* =============== PRIVATE METHODS ===============
 * ---------------------------------------------- */
Tle TLEParser::_parse(const string &l1, const string &l2, const string &l3)
{
    Tle tle(l1, l2, l3);
    return tle;
}
