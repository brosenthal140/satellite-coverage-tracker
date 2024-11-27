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

	SatelliteCatalog catalog(directory);

	REQUIRE(catalog.count() > 0);
}
