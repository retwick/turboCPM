# turboCPM

Project planning software.

## TODO
```
Fix calendar operations -change weekly working days.(Currently, Sat and Sun always holidays)

Do backward parsing.

Account for in-progress activities

Solve for parent nodes

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

First install c++ Boost Library and then install QuantLib

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

## Built With

* [QuantLib](https://github.com/lballabio/quantlib) - Calendar operations
* [C++ Boost Library](https://www.boost.org/)
