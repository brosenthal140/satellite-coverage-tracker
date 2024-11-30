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

## LinearModel Class

## Utility Class
- `getDistance()` - computes the haversine distance between two geodetic coordinate positions

# Library Documentation
***
The following section describes external libraries that are used in this project

## libsgp4

###
