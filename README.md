# satellite-coverage-tracker
***
COP3530 Data Structures &amp; Algorithm - Project 3

**Group Name:** The Orbital Police

**Group Members:** Kamarii Austin, Ryan McGlinn, Brandon Rosenthal

# Class Documentation
***
The following section provides a summary of each custom class used in the project

## SatelliteCoverageTracker

## TLEParser Class

## DataModel Class
***
This is a template class which both the LinearModel and GraphModel classes inherit from.

### Construction
- `DataModel()` - a constructor that takes in a path to the directory where the data files are located

### Pure Virtual Functions
- `import()` - performs the data import process
- `insert()` - takes in a reference to a Tle object and inserts it into the data structure
- `search()` - takes in a position and a radius used to perform the search for matching data points

## GraphModel Class
***
### Construction
- `GraphModel()` - constructor that takes in a path to the data directory and a waypoint separation threshold

### Mutators
- `import()` - Performs the import process for all .tle files found in the data directory
- `insert()` - Inserts a single data point into the data structure

### Accessors
- `search()` - performs a search of the data structure to find datapoint that are in range relative to a position

### Static Tests
- `testFindClosestWaypoint()` - Takes in a position, checks the positions and inserts a waypoint because one does not exist, then compares the waypoint to a reference
- `testFindClosestWaypoint()` - Takes in a position and and a vector of waypoints, and compares the closes waypoint to a reference waypoint
- `testInsert()` - Tests inserting a two line element into the Graph and compares the Vertex that was generated to a reference vertex

## LinearModel Class

## Utility Class
- `getDistance()` - computes the haversine distance between two geodetic coordinate positions

# Library Documentation
***
The following section describes external libraries that are used in this project

## libsgp4

###
