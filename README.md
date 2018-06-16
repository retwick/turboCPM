# turboCPM

Project planning software.

## TODO
```
Write output to file

Automate calendar operations - Setting holidays and weekly working days

Do backword parsing.

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

## Compiling
```
g++ driver.cpp -o driver -lQuantLib
```

## Running the tests

./driver

## Built With

* [QuantLib](https://github.com/lballabio/quantlib) - Calendar operations
* [C++ Boost Library](https://www.boost.org/)
