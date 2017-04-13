#include <string.h>
#include <chrono>

#include "core/log.h"
#include "core/types.h"

#include "cr_util.h"

#define OBJ_STR_LEN 6

namespace cr {

DataType toDataType(const std::string& str) {

	if (str.size() == 0) {
		return DataType::NONE;

	} else if (str == "string") {
		return DataType::STRING;

	} else if (str == "blob") {
		return DataType::BLOB;

	} else if (str.size() > OBJ_STR_LEN && str.substr(0, OBJ_STR_LEN) == "object") {
		return DataType::OBJECT;

	} else if (str == "i8" || str == "int8_t") {
		return DataType::I8;

	} else if (str == "ui8" || str == "uint8_t" || str == "byte") {
		return DataType::UI8;

	} else if (str == "i16" || str == "int16_t") {
		return DataType::I16;

	} else if (str == "ui16" || str == "uint16_t") {
		return DataType::UI16;

	} else if (str == "i32" || str == "int32_t" || str == "int") {
		return DataType::I32;

	} else if (str == "ui32" || str == "uint32_t") {
		return DataType::UI32;

	} else if (str == "i64" || str == "int64_t") {
		return DataType::I64;

	} else if (str == "ui64" || str == "uint64_t") {
		return DataType::UI64;

	} else if (str == "f32" || str == "float") {
		return DataType::F32;

	} else if (str == "f64" || str == "double") {
		return DataType::F64;

	} else if (str == "bool" || str == "boolean") {
		return DataType::BOOLEAN;

	} else {
		throw CRException( "Unrecognized DataType: %s", str.c_str() );
	}
}

// toLanguage
Language toLanguage( const char* str ) {


	if ( nullptr == str || strcmp( str, "" ) == 0 ) {
		return Language::NONE;

	} else if ( (strcmp( str, "C++" ) == 0) || (strcmp( str, "CPP" ) == 0) ) {

		return Language::CPP;

	} else {

		throw CRException( "Unrecognized Language: %s", str );
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
		throw CRException( "Unrecognized AccessPrivacy: %s", str );
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

// getTimeMillis
ui64 getTimeMillis() {
	// here's a taste of the T in STL...
	std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
}

};
