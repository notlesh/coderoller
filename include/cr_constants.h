#ifndef __CR_CR_CONSTANTS_H_
#define __CR_CR_CONSTANTS_H_

#include "core/exception.h"
#include "core/file.h"

using namespace roller;

#define CR_MAJOR_VERSION 0
#define CR_MINOR_VERSION 1

namespace cr {

/** 
 * Enum representing basic data types.
 */
enum class DataType {
	NONE = 0,
	STRING,

	// integer types
	I8, UI8,
	I16, UI16,
	I32, UI32,
	I64, UI64,

	F32,
	F64
};

/**
 * Enum representing supported languages
 */
enum class Language {
	NONE = 0,
	CPP
};

/** 
 * InvalidNodeNameException
 */
class InvalidNodeNameException : public RollerException {

public:

	// TODO: replace with C++11 Inherited Constructors if they ever get implemented
	template<class ... Args> 
	InvalidNodeNameException(Args&& ... args) : RollerException(args...) { } 
};

/** 
 * MissingAttributeException
 */
class MissingAttributeException : public RollerException {

public:

	// TODO: replace with C++11 Inherited Constructors if they ever get implemented
	template<class ... Args> 
	MissingAttributeException(Args&& ... args) : RollerException(args...) { } 
};

};

#endif
