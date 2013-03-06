#include <string.h>

#include "core/log.h"

#include "cr_util.h"

using namespace roller;

namespace cr {

DataType toDataType( const char* str ) {

	if ( nullptr == str || strcmp( str, "" ) == 0 ) {
		return DataType::NONE;

	} else if ( strcmp( str, "string" ) == 0 ) {
		return DataType::STRING;

	} else if ( strcmp( str, "blob" ) == 0 ) {
		return DataType::BLOB;

	} else if ( strlen( str ) > 12 && memcmp( str, "serializable", 12 ) == 0 ) {
		return DataType::SERIALIZABLE;

	} else if ( strcmp( str, "i8" ) == 0 ) {
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
		throw RollerException( "Unrecognized DataType string" );
	}
}

// toLanguage
Language toLanguage( const char* str ) {


	if ( nullptr == str || strcmp( str, "" ) == 0 ) {
		return Language::NONE;

	} else if ( (strcmp( str, "C++" ) == 0) || (strcmp( str, "CPP" ) == 0) ) {

		return Language::CPP;

	} else {

		throw RollerException( "Unrecognized Language string" );
	}

}

// toAccessPrivacy
AccessPrivacy toAccessPrivacy( const char* str ) {

	if ( nullptr == str || strcmp( str, "" ) == 0 ) {
		return AccessPrivacy::NONE;

	} else if ( strcmp( str, "private" ) == 0 ) {
		return AccessPrivacy::PRIVATE;

	} else if ( strcmp( str, "protected" ) == 0 ) {
		return AccessPrivacy::PROTECTED;

	} else if ( strcmp( str, "public" ) == 0 ) {
		return AccessPrivacy::PUBLIC;

	} else {
		throw RollerException( "Unrecognized AccessPrivacy string" );
	}

}

// toSerializableSpecification
SerializableSpecification toSerializableSpecification( const char* str ) {
	if ( str == nullptr || str[0] == 0x00 ) {
		return SerializableSpecification::NONE;
	} else if ( (strcmp( str, "yes" ) == 0 ) || (strcmp( str, "true" )) ) {
		return SerializableSpecification::YES;
	} else if ( (strcmp( str, "no" ) == 0 ) || (strcmp( str, "false" )) ) {
		return SerializableSpecification::NO;
	} else {
		return SerializableSpecification::NONE;
	}
}

// capitalize
char capitalize( char c ) {
	return c - (97 - 65);
}
};
