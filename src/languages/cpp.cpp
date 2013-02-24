#include "languages/cpp.h"

#include <memory>
#include <iostream>
#include <fstream>

#include "core/aver.h"

#include "cr_constants.h"

using namespace roller;
using std::unique_ptr;
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
void CPPGenerator::generate( const GenConfig& config ) {

	ofstream out( "/tmp/out.h" );

	for ( Class c : config._package._classes ) {

		// print out "class Foo {"
		out << "class " << c._name << " {" << endl;
		out << endl;
		out << "public:" << endl;
		out << endl;

		for ( Field f : c._fields ) {

			// print out variable declaration
			out << "\t" << getDataTypeName( f._dataType ) << " _" << f._name << ";" << endl;
		}

		// close class
		out << "};" << endl;

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

};
