#ifndef __CR_CR_CONSTANTS_H_
#define __CR_CR_CONSTANTS_H_

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

};

#endif
