#include <string.h>

#include "cr_util.h"

namespace cr {

DataType toDataType( const char* str ) {

	if ( strcmp( str, "string" ) == 0 ) {
		return DataType::STRING;

	} else if ( strcmp( str, "i8 " ) == 0 ) {
		return DataType::I8;

	} else if ( strcmp( str, "ui8" ) == 0 ) {
		return DataType::UI8;

	} else if ( strcmp( str, "i16" ) == 0 ) {
		return DataType::I16;

	} else if ( strcmp( str, "ui16" ) == 0 ) {
		return DataType::UI16;

	} else if ( strcmp( str, "i32" ) == 0 ) {
		return DataType::I32;

	} else if ( strcmp( str, "ui32" ) == 0 ) {
		return DataType::UI32;

	} else if ( strcmp( str, "i64" ) == 0 ) {
		return DataType::I64;

	} else if ( strcmp( str, "ui64" ) == 0 ) {
		return DataType::UI64;

	} else if ( strcmp( str, "f32" ) == 0 ) {
		return DataType::F32;

	} else if ( strcmp( str, "f64" ) == 0 ) {
		return DataType::F64;

	} else {
		throw RollerException( "Unrecognized data type" );
	}
}

// toLanguage
Language toLanguage( const char* str ) {

	if ( (strcmp( str, "C++" ) == 0) || (strcmp( str, "CPP" ) == 0) ) {

		return Language::CPP;

	} else {

		throw RollerException( "Unrecognized language" );
	}

}

};
