#include "languages/cpp.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>

#include "core/aver.h"
#include "core/util.h"

#include "cr_constants.h"

using namespace roller;
using std::unique_ptr;
using std::shared_ptr;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::endl;

namespace cr {

// Constructor
CPPGenerator::CPPGenerator() :
				_hashPosition(-1) {
}

// Destructor
CPPGenerator::~CPPGenerator() {
}

// generate
void CPPGenerator::generate( shared_ptr<GenConfig> config ) {

	for ( Class c : config->_package._classes ) {

		ostringstream outBufferStream;

		writeMainHeader( outBufferStream );

		// print out "class Foo {"
		outBufferStream << "class " << c._name << " {" << endl;
		outBufferStream << endl;

		list<Field> publicFields;
		list<Field> protectedFields;
		list<Field> privateFields;

		for ( Field f : c._fields ) {

			AccessPrivacy privacy = resolvePrivacy( config->_package, c, f );

			switch ( privacy ) {

				case AccessPrivacy::PRIVATE:
					privateFields.push_back( f );
					break;

				case AccessPrivacy::PROTECTED:
					protectedFields.push_back( f );
					break;

				case AccessPrivacy::PUBLIC:
					publicFields.push_back( f );
					break;

				case AccessPrivacy::NONE:
				default:
					throw MissingAttributeException( "Must specify a root level defaultMemberPrivacy, a class defaultMemberPrivacy, or a field memberPrivacy setting" );
			}
		}

		// add fields by type
		if ( publicFields.size() > 0 ) {
			
			outBufferStream << "public:" << endl;
			outBufferStream << endl;

			for ( Field f : publicFields ) {
				writeField( outBufferStream, f );
			}

			outBufferStream << endl;
		}

		if ( protectedFields.size() > 0 ) {
			
			outBufferStream << "protected:" << endl;
			outBufferStream << endl;

			for ( Field f : protectedFields ) {
				writeField( outBufferStream, f );
			}

			outBufferStream << endl;
		}

		if ( privateFields.size() > 0 ) {
			
			outBufferStream << "private:" << endl;
			outBufferStream << endl;

			for ( Field f : privateFields ) {
				writeField( outBufferStream, f );
			}

			outBufferStream << endl;
		}

		// close class
		outBufferStream << "};" << endl;
		outBufferStream << endl;

		// take hash of the entire buffer, then go back and fill in hash
		// TODO: this involves at least one big copy...
		// NOTE: this needs to be updated if the "class Foo..." and the hash move with respect to each other (etc)
		i32 hashCode = hash( outBufferStream.str().c_str() + _hashPosition + sizeof( "00000000\n */") );
		// Log::i( "class text starts with: %s", outBufferStream.str().c_str() + _hashPosition + sizeof( "00000000\n */") );

		outBufferStream.seekp( _hashPosition, std::ios_base::beg );

		// print in hex
		outBufferStream << std::hex << hashCode;
		outBufferStream << std::dec;

		// seek back to end
		outBufferStream.seekp( 0, std::ios_base::end );

		char filename[128]; snprintf( filename, 128, "%s.gen.h", c._name.c_str() );
		File outputFile( config->_outputDir, filename );
		ofstream outFileStream( outputFile.getFullPath() );
		outFileStream << outBufferStream.str();

		outFileStream.close();

	}
}

// writeMainHeader
void CPPGenerator::writeMainHeader( ostream& stream ) {
	stream	<< "/**" << endl
			<< " * CodeRoller generated class. This class is automatically generated; do not modify it" << endl
			<< " * because it will be overwritten." << endl
			<< " *" << endl
			<< " * CodeRoller git hash: " << GIT_HASH << endl
			<< " * CodeRoller git branch: " << GIT_BRANCH << endl
			<< " *" << endl
			<< " * Generated on " << getTime() << endl
			<< " * Class hash: 00000000";
	
	// grab position here; we will return to it to overwrite class hash once we can compute it.
	_hashPosition = (i64)stream.tellp() - 8;

	// finish writing header

	stream	<< endl
			<< " */" << endl;
}

// writeField
void CPPGenerator::writeField( ostream& stream, const Field& f ) {
	stream << "\t" << getDataTypeName( f._dataType ) << " _" << f._name << ";" << endl;
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
			throw RollerException( "Unrecognized DataType" );

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

		default:
			throw RollerException( "Unrecognized AccessPrivacy" );
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
