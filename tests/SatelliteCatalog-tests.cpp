#include <string>
#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include "../src/SatelliteCatalog.h"

using namespace std;
using namespace libsgp4;

TEST_CASE("Import SSR CSV file from space-track.org", "[SatelliteCatalog]")
{
	// This test will fail unless you have a valid SSR file in CSV format in the specified directory
	string directory = "../data";

	// Test to see if the directory exists


	// Test to see if there is a file with the .csv extension on the directory

	string pathToCatalogFile = "../data/ssr_20241127_743439133.csv";

	SatelliteCatalog catalog(pathToCatalogFile);

	REQUIRE(catalog.count() > 0);
}
