#include "languages/cpp/cpp_util.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>

#include "core/aver.h"
#include "core/util.h"

#include "cr_constants.h"
#include "cr_util.h"

using namespace roller;
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
			return "string";

		case DataType::BLOB:
			return "ByteArray";

		case DataType::I8:
			return "i8";

		case DataType::UI8:
			return "ui8";

		case DataType::I16:
			return "i16";

		case DataType::UI16:
			return "ui16";

		case DataType::I32:
			return "i32";

		case DataType::UI32:
			return "ui32";

		case DataType::I64:
			return "i64";

		case DataType::UI64:
			return "ui64";

		case DataType::F32:
			return "f32";

		case DataType::F64:
			return "f64";

		default:
			throw RollerException( "Unrecognized DataType" );

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
			throw RollerException( "Unrecognized AccessPrivacy" );
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
			throw RollerException( "Field specifiecd as serializable, but class specified as not serializable" );
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
			throw RollerException( "Field specifiecd as serializable, neither class nor package specify serialiazable" );
		}
		return false;

	}
}

};
