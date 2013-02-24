#include "languages/cpp.h"

#include <memory>

#include <memory>
#include <iostream>
#include <fstream>

#include "core/aver.h"

#include "cr_constants.h"

using namespace roller;
using std::unique_ptr;
using std::shared_ptr;
using std::ofstream;
using std::endl;

namespace cr {

// Constructor
CPPGenerator::CPPGenerator() {
}

// Destructor
CPPGenerator::~CPPGenerator() {
}

// generate
void CPPGenerator::generate( shared_ptr<GenConfig> config ) {

	// TODO: this should be passed to generate...?
	File outputFile( config->_outputDir, "out.h" );
	Log::i( "Writting CPP to %s", outputFile.getFullPath().c_str() );
	ofstream out( outputFile.getFullPath() );

	for ( Class c : config->_package._classes ) {

		// print out "class Foo {"
		out << "class " << c._name << " {" << endl;
		out << endl;

		for ( Field f : c._fields ) {

			AccessPrivacy privacy = resolvePrivacy( config->_package, c, f );
			if ( privacy == AccessPrivacy::NONE ) {
				throw MissingAttributeException( "Must specify a root level defaultMemberPrivacy, a class defaultMemberPrivacy, or a field memberPrivacy setting" );
			}

			Log::i( "Processing field %s", f._name.c_str() );

			// print out variable declaration
			// TODO: need to group by access first, then build later
			out << "\t" << getPrivacyName( privacy ) << ": " << getDataTypeName( f._dataType ) << " _" << f._name << ";" << endl;
		}

		// close class
		out << "};" << endl;
		out << endl;

	}

	out.close();
}

// getDataTypeName
const char* CPPGenerator::getDataTypeName( DataType type ) {

	switch ( type ) {

		case DataType::NONE:
			return "";

		case DataType::STRING:
			return "string";

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
			throw RollerException( "Unhandled DataType" );

	}

}

// getPrivacyName
const char* CPPGenerator::getPrivacyName( AccessPrivacy privacy ) {

	switch ( privacy ) {

		case AccessPrivacy::NONE:
			return "";

		case AccessPrivacy::PRIVATE:
			return "private";

		case AccessPrivacy::PROTECTED:
			return "protected";

		case AccessPrivacy::PUBLIC:
			return "public";
	}
}

// resolvePrivacy
AccessPrivacy CPPGenerator::resolvePrivacy( const Package& p, const Class& c, const Field& f ) {

	if ( f._memberPrivacy != AccessPrivacy::NONE ) {
		return f._memberPrivacy;

	} else if ( c._defaultMemberPrivacy != AccessPrivacy::NONE ) {
		return c._defaultMemberPrivacy;

	} else {
		return p._defaultMemberPrivacy;
	}
}

};
