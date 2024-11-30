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

## GraphModel Class
***
### Construction
- `GraphModel()` - constructor that takes in a path to the data directory and a waypoint separation threshold

### Mutators
- `insertVertex()` - takes in a Tle object and inserts a vertex

### Accessors
- `getVertex()` - returns a reference to a vertex object based on the index passed to the function
- `getWaypointEdges()` - returns a reference to a vector containing the edges for a waypoint whose index matches the index passed to the function

## LinearModel Class

## Utility Class
- `getDistance()` - computes the haversine distance between two geodetic coordinate positions

# Library Documentation
***
The following section describes external libraries that are used in this project

## libsgp4

###
