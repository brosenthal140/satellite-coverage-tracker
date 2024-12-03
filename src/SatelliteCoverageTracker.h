#pragma once
#include <string>
#include "SGP4.h"
#include "DataModel.h"
using namespace std;
using namespace libsgp4;


struct SatelliteCoverageTracker {
public:
	/* ========== CONSTRUCTORS/DESTRUCTORS ========== */
	SatelliteCoverageTracker(string pathToData, const CoordGeodetic &location, const double &radius);
	~SatelliteCoverageTracker();

	/* ========== PUBLIC METHODS ========== */
	static void runCLI();

private:
	/* ========== PRIVATE MEMBER VARIABLES ========== */
	string _dataDirectory;
	CoordGeodetic _locationRef;
	double _radius;
	vector<Tle> _observations;

	DataModel* _graphModel;
	DataModel* _linearModel;

	/* ========== PRIVATE HELPER FUNCTIONS ========== */
	void _importData();
	void _initModel(DataModel *model);
};

