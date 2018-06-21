# turboCPM

Project planning software.

## TODO
Fix calendar operations
```
change weekly working days.(Currently, Sat and Sun always holidays)
Start date of project is hard coded.
```
Account for in-progress activities.
```
Set duration of activity as (fraction completed)*actual duration.
Iterate over all activities and remove activity if it is finished. 
Reconstruct graph and recompute E* and L* dates.
```

Solve for parent nodes
```
--
```
## Getting Started
The output of the program is early start, early finish, late start and late finish dates.
It gives slack available in the activities.

### Prerequisites
You will need the following:
```
C++ Boost Library 
QuantLib - A free/open-source library for quantitative finance
```

### Installing

First install c++ Boost Library, then install QuantLib.

## Running the tests
Input files are
```
input/tasks.csv
input/relationships.csv
input/calendar.csv
```
To run program
```
make
./driver
```
Result is written to text file
```
output.txt
```
## Improvements possible
Use hashtable instead of map for graph data structure for faster execution.

## Built With

* [QuantLib](https://github.com/lballabio/quantlib) - Calendar operations
* [C++ Boost Library](https://www.boost.org/)
