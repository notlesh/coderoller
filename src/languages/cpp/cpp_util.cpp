#include "languages/cpp/cpp_util.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <chrono>

#include "cr_constants.h"
#include "cr_util.h"

#define HASH_MULTIPLIER 37

using std::unique_ptr;
using std::shared_ptr;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::endl;

namespace cr {

// getDataTypeName
const char* getDataTypeName( DataType type ) {

	switch ( type ) {

		case DataType::NONE:
			return "";

		case DataType::STRING:
			return "std::string";

		case DataType::BLOB:
			return "ByteArray";

		case DataType::I8:
			return "int8_t";

		case DataType::UI8:
			return "uint8_t";

		case DataType::I16:
			return "int16_t";

		case DataType::UI16:
			return "uint16_t";

		case DataType::I32:
			return "int32_t";

		case DataType::UI32:
			return "uint32_t";

		case DataType::I64:
			return "int64_t";

		case DataType::UI64:
			return "uint64_t";

		case DataType::F32:
			return "float";

		case DataType::F64:
			return "double";

		case DataType::BOOLEAN:
			return "bool";

		default:
			throw CRException( "Unrecognized DataType" );

	}

}

// getPrivacyName
const char* getPrivacyName( AccessPrivacy privacy ) {

	switch ( privacy ) {

		case AccessPrivacy::NONE:
			return "";

		case AccessPrivacy::PRIVATE:
			return "private";

		case AccessPrivacy::PROTECTED:
			return "protected";

		case AccessPrivacy::PUBLIC:
			return "public";

		default:
			throw CRException( "Unrecognized AccessPrivacy" );
	}
}

// resolvePrivacy
AccessPrivacy resolvePrivacy( const Package& p, const Class& c, const Field& f ) {

	if ( f._memberPrivacy != AccessPrivacy::NONE ) {
		return f._memberPrivacy;

	} else if ( c._defaultMemberPrivacy != AccessPrivacy::NONE ) {
		return c._defaultMemberPrivacy;

	} else {
		return p._defaultMemberPrivacy;
	}
}

// resolveClassSerializable
bool resolveClassSerializable( const Package& p, const Class& c ) {

	if ( c._serializable == SerializableSpecification::YES ) {
		return true;
	} else if ( c._serializable == SerializableSpecification::NO ) {
		return false;
	} else if ( p._defaultSerializable == SerializableSpecification::YES ) {
		return true;
	} else {
		return false;
	}
}

// resolveFieldSerializable
bool resolveFieldSerializable( const Package& p, const Class& c, const Field& f ) {

	if ( c._serializable == SerializableSpecification::NO ) {

		// class explicitly says no (field should not say yes)

		if ( f._serializable == SerializableSpecification::YES ) {
			throw CRException( "Field specifiecd as serializable, but class specified as not serializable" );
		}
		return false;

	} else if ( c._serializable == SerializableSpecification::NONE 
			&& p._defaultSerializable != SerializableSpecification::YES ) {

		// class doesn't specify, but package says yes

		if ( f._serializable == SerializableSpecification::NO ) {
			return false;
		} else {
			return true;
		}

	} else if ( c._serializable == SerializableSpecification::YES ) {

		// class explicitly says yes

		if ( f._serializable == SerializableSpecification::NO ) {
			return false;
		} else {
			return true;
		}

	} else {

		// nothing specifies yes, so if field specifies yes, we have error, otherwise no serializable.

		if ( f._serializable == SerializableSpecification::YES ) {
			throw CRException( "Field specifiecd as serializable, neither class nor package specify serialiazable" );
		}
		return false;

	}
}

// hash
ui32 hash( const char* str, ui32 h ) {
	char *p;

	for ( p = (char*)str; *p != '\0'; p++ ) {
		h = HASH_MULTIPLIER * h + *p;
	}
	return h; // or, h % ARRAY_SIZE;
}

// getTimeMillis
ui64 getTimeMillis() {
	// here's a taste of the T in STL...
	std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
}

};
