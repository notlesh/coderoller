#ifndef __CR_CR_CONSTANTS_H_
#define __CR_CR_CONSTANTS_H_

#include "core/exception.h"
#include "core/file.h"

// each should be kept to 8 bits
#define CR_MAJOR_VERSION 0
#define CR_MINOR_VERSION 1
#define CR_PATCH_VERSION 1

namespace cr {

/**
 * Access Privacy
 */
enum class AccessPrivacy {
	NONE = 0,
	PRIVATE,
	PROTECTED,
	PUBLIC
};

/** 
 * Enum representing basic data types.
 */
enum class DataType {
	NONE = 0,

	STRING,
	BLOB,
	SERIALIZABLE,

	// integer types
	I8, UI8,
	I16, UI16,
	I32, UI32,
	I64, UI64,

	F32,
	F64,

	BOOLEAN
};

/**
 * Enum representing supported languages
 */
enum class Language {
	NONE = 0,
	CPP
};

/**
 * Enum representing serialization options (yes, no, unspecified)
 */
enum class SerializableSpecification {
	NONE = 0,
	YES,
	NO
};

/** 
 * InvalidNodeNameException
 */
class InvalidNodeNameException : public CRException {

public:

	// TODO: replace with C++11 Inherited Constructors if they ever get implemented
	template<class ... Args> 
	InvalidNodeNameException(Args&& ... args) : CRException(args...) { } 
};

/** 
 * MissingAttributeException
 */
class MissingAttributeException : public CRException {

public:

	// TODO: replace with C++11 Inherited Constructors if they ever get implemented
	template<class ... Args> 
	MissingAttributeException(Args&& ... args) : CRException(args...) { } 
};

};

#endif
