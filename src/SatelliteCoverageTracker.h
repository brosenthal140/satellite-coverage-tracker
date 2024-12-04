#pragma once
#include <string>
#include "SGP4.h"
#include "DataModel.h"
#include "SatelliteCatalog.h"

using namespace std;
using namespace libsgp4;


struct SatelliteCoverageTracker {
public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	SatelliteCoverageTracker(string pathToData, const CoordGeodetic &location, const double &radius);
	~SatelliteCoverageTracker();

	/* ========== PUBLIC STATIC METHODS ========== */
	static void runCLI();

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;
	CoordGeodetic _locationRef;
	double _radius;
	vector<Tle> _observations;
	SatelliteCatalog _catalog;

	DataModel* _graphModel;
	DataModel* _linearModel;

	/* ========== PRIVATE HELPER FUNCTIONS ========== */
	void _importData();
	void _initModel(DataModel *model);
	unordered_set<string> _performSearch(DataModel *model);
};

