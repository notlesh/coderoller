Coderoller is a code generation tool written in C++ which generates data objects (think "Plain Old Data" or "Java Beans"). It aims to be modular, allowing for multiple outputs (languages, etc.) from a given input. Input files are XML.

# Compiling source

Compiling from source can be accomplished with a few steps:

*  $ git clone https://github.com/stephelton/coderoller
*  $ cd coderoller
*  $ git submodule init
*  $ git submodule update
*  $ mkdir build
*  $ cd build
*  $ cmake ../
*  $ make

# Usage

coderoller requires an input XML file, which defines what will be generated, and a directory where the output will be placed:

    $ ./coderoller --help
    coderoller code generator (c) Stephen Shelton
      version 0.1
      usage: coderoller [input file] [output base dir]
      
# Example

The repository contains examples that can be used out of the box:

    $ ./coderoller test/examples/test.cr.xml tmp/
    Parsing coderoller xml file test/examples/test.cr.xml
    Adding language CPP
    --- Stats ---
    Parse time: 0.00024360 seconds
    Gen time: 0.00050578 seconds
    Total time: 0.00074938 seconds
    
This will generate a number of C++ files:

    $ find tmp/
    tmp
    tmp/include
    tmp/include/TemperatureReading.gen.h
    tmp/include/Foo.gen.h
    tmp/src
    tmp/src/TemperatureReading.gen.cpp
    tmp/src/Foo.gen.cpp
    
This example takes the following XML:

```XML
	<!-- TemperatureReading type -->
	<class name="TemperatureReading" serializable="false">

		<field
			name="temp"
			type="f32"
			default=".0"
			memberPrivacy="public" />

		<field
			name="time"
			type="i64"
			default="0" />
	</class>
```

And generates the following C++ header (.h) and source (.cpp):

tmp/include/TemperatureReading.gen.h:

```C++
/**
 * CodeRoller generated class. This class is automatically generated; do not modify it
 * because it will be overwritten.
 *
 * CodeRoller git hash: 9a83c4b655f0f6f9d7ad45dace54999e9430063b
 * CodeRoller git branch: master
 *
 * Generated on 1492108923025
 * Class hash: 8b28f4fa
 */

#ifndef __CR_GEN_TemperatureReading_H_
#define __CR_GEN_TemperatureReading_H_

#include <string>



#define __CR_HASH_TemperatureReading 0x8b28f4fa

class TemperatureReading {

public:

	// declare default empty ctor and dtor
	TemperatureReading() = default;
	~TemperatureReading() = default;

	// declare default copy ctor/assignment and let the compiler generate it if possible
	TemperatureReading( const TemperatureReading& other ) = default;
	TemperatureReading& operator=( const TemperatureReading& other ) = default;

	// declare custom move ctor/assignment
	TemperatureReading( TemperatureReading&& other );
	TemperatureReading& operator=( TemperatureReading&& other );

	// copy convenience functions
	void copyFrom( const TemperatureReading& other );
	TemperatureReading clone() const;

	float _temp;

	float getTemp() const;
	void setTemp( float value );

	int64_t getTime() const;
	void setTime( int64_t value );

	static int32_t s_classHash;

private:

	int64_t _time;

};
```

tmp/src/TemperatureReading.gen.cpp:
```C++
#include "TemperatureReading.gen.h"


int32_t TemperatureReading::s_classHash = __CR_HASH_TemperatureReading;

TemperatureReading::TemperatureReading( TemperatureReading&& other ) {
	_temp = other._temp;
	_time = other._time;
}

TemperatureReading& TemperatureReading::operator=( TemperatureReading&& other ) {
	_temp = other._temp;
	_time = other._time;
	return *this;
}

void TemperatureReading::copyFrom( const TemperatureReading& other ) {
	_temp = other._temp;
	_time = other._time;
}

TemperatureReading TemperatureReading::clone() const {
	TemperatureReading copy;
	copy._temp = _temp;
	copy._time = _time;
	return copy;
}

float TemperatureReading::getTemp() const {
	return _temp;
}

void TemperatureReading::setTemp( float value ) {
	_temp = value;
}

int64_t TemperatureReading::getTime() const {
	return _time;
}

void TemperatureReading::setTime( int64_t value ) {
	_time = value;
}
```
    
