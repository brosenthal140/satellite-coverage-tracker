#pragma once
#include <SGP4.h>
using namespace std;
using namespace libsgp4;

class TLEParser {
    /* ========== PRIVATE MEMBER VARIABLES ========== */
    const string _dataFilePath;

    /* ========== PRIVATE METHODS ========== */
    static Tle _parse(const string &l1, const string &l2, const string &l3);

public:
    /* ========== CONSTRUCTORS/DESTRUCTORS ========== */
    TLEParser() = default;
    explicit TLEParser(const string &filePath) : _dataFilePath(filePath) {};

    /* ========== PUBLIC STATIC METHODS ========== */
    static Tle parse(const string &tleString);
    static Tle parse(const string &l1, const string &l2, const string &l3);
};
