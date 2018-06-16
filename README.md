# turboCPM

Project planning software.

## TODO
```
Automate calendar operations - Setting holidays and weekly working days

Do backward parsing.

Account for in-progress activities

Solve for parent nodes

```
## Getting Started
The output of the program is early start, early finish, late start and late finish dates.
It also aims to give slack available in given activities.

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
dummy_input/tasks.csv
dummy_input/relationships.csv
```
To run program
```
g++ driver.cpp -o driver -lQuantLib
./driver
```

## Built With

* [QuantLib](https://github.com/lballabio/quantlib) - Calendar operations
* [C++ Boost Library](https://www.boost.org/)
