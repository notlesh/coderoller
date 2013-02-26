#include "languages/cpp.h"

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

// Constructor
CPPGenerator::CPPGenerator() :
				_hashPosition1(-1),
				_hashPosition2(-1) {
}

// Destructor
CPPGenerator::~CPPGenerator() {
}

// generate
void CPPGenerator::generate( shared_ptr<GenConfig> config ) {

	for ( Class c : config->_package._classes ) {

		bool classSerializable = resolveClassSerializable( config->_package, c );

		char hFilename[128]; snprintf( hFilename, 128, "%s.gen.h", c._name.c_str() );
		char cppFilename[128]; snprintf( cppFilename, 128, "%s.gen.cpp", c._name.c_str() );

		File includeDir = string("include/");
		File srcDir = string("src/");

		// make sure we can create files (and their parents)
		File hOutputFile( config->_outputDir, File( includeDir, hFilename ) );
		File hOutputFileParent = hOutputFile.getDir();
		if ( ! hOutputFileParent.exists() ) {
			hOutputFileParent.mkdir( true );
		}

		File cppOutputFile( config->_outputDir, File( srcDir, cppFilename ) );
		File cppOutputFileParent = cppOutputFile.getDir();
		if ( ! cppOutputFileParent.exists() ) {
			cppOutputFileParent.mkdir( true );
		}

		ostringstream hOutStream;
		ostringstream cppOutStream;

		writeMainHeader( hOutStream );
		hOutStream << endl;

		writeStartIfdefs( hOutStream, c );
		hOutStream << endl;

		writeHHeaderInclude( hOutStream, classSerializable );
		hOutStream << endl;

		writeHUsingDeclarations( hOutStream, classSerializable );
		hOutStream << endl;

		writeHashDef( hOutStream, c );
		hOutStream << endl;

		// print out "class Foo {"
		if ( classSerializable ) {
			hOutStream << "class " << c._name << " : public Serializable {" << endl;
			hOutStream << endl;

		} else {
			hOutStream << "class " << c._name << " {" << endl;
			hOutStream << endl;
		}

		// write header to cppOutStream
		writeCPPHeaderInclude( cppOutStream, hFilename );
		cppOutStream << endl;

		writeCPPUsingDeclarations( cppOutStream, classSerializable );
		cppOutStream << endl;

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

		hOutStream << "public:" << endl;
		hOutStream << endl;

		if ( publicFields.size() > 0 ) {

			if ( publicFields.size() > 0 ) {

				for ( Field f : publicFields ) {
					writeField( hOutStream, f );
				}

				hOutStream << endl;

				for ( Field f : publicFields ) {
					writeFieldAccessors( hOutStream, cppOutStream, c, f );
					hOutStream << endl;
				}
			}

			// while we're in public, write out all accessors
			if ( protectedFields.size() > 0 ) {
				for ( Field f : protectedFields ) {
					writeFieldAccessors( hOutStream, cppOutStream, c, f );
					hOutStream << endl;
				}
			}
			if ( privateFields.size() > 0 ) {
				for ( Field f : privateFields ) {
					writeFieldAccessors( hOutStream, cppOutStream, c, f );
					hOutStream << endl;
				}
			}

			// write out serialization declarations
			if ( classSerializable ) {
				writeSerializationDeclarations( hOutStream );
				hOutStream << endl;
			}
		}

		// add other fields by access type
		if ( protectedFields.size() > 0 ) {
			
			hOutStream << "protected:" << endl;
			hOutStream << endl;

			for ( Field f : protectedFields ) {
				writeField( hOutStream, f );
			}

			hOutStream << endl;
		}

		if ( privateFields.size() > 0 ) {
			
			hOutStream << "private:" << endl;
			hOutStream << endl;

			for ( Field f : privateFields ) {
				writeField( hOutStream, f );
			}

			hOutStream << endl;
		}

		// write serialization function definitions
		if ( classSerializable ) {
			writeSerializationDefinitions( cppOutStream, c, publicFields, protectedFields, privateFields );
		}

		// close class
		hOutStream << "};" << endl;
		hOutStream << endl;

		writeEndIfdefs( hOutStream, c );

		// take hash of the entire buffer, then go back and fill in hash
		// TODO: this involves at least one big copy...
		// NOTE: this needs to be updated if the "class Foo..." and the hash move with respect to each other (etc)
		i32 hashCode = hash( hOutStream.str().c_str() + _hashPosition2 + sizeof( "00000000\n */") );
		// Log::i( "class text starts with: %s", hOutStream.str().c_str() + _hashPosition + sizeof( "00000000\n */") );

		substituteHash( hashCode, hOutStream, _hashPosition1 );
		substituteHash( hashCode, hOutStream, _hashPosition2 );

		ofstream hOutFileStream( hOutputFile.getFullPath() );
		hOutFileStream << hOutStream.str();
		hOutFileStream.close();

		ofstream cppOutFileStream( cppOutputFile.getFullPath() );
		cppOutFileStream << cppOutStream.str();
		cppOutFileStream.close();


	}
}

// writeStartIfdefs
void CPPGenerator::writeStartIfdefs( ostream& stream, const Class& c ) {
	stream	<< "#ifndef __CR_GEN_" << c._name << "_H_" << endl
			<< "#define __CR_GEN_" << c._name << "_H_" << endl;
}

// writeCPPHeaderInclude
void CPPGenerator::writeCPPHeaderInclude( ostream& cppStream, const char* headerFilename ) {
	cppStream << "#include \"" << headerFilename << "\"" << endl;
}

// writeCPPUsingDeclarations
void CPPGenerator::writeCPPUsingDeclarations( ostream& cppStream, bool classSerializable ) {
	cppStream << "using namespace roller;" << endl;
	if ( classSerializable ) {
		cppStream << "using namespace cr;" << endl;
	}
	cppStream << "using std::string;" << endl;
}

// writeHHeaderInclude
void CPPGenerator::writeHHeaderInclude( ostream& hStream, bool classSerializable ) {
	hStream		<< "#include <string>" << endl
				<< endl
				<< "#include \"core/types.h\"" << endl;
	if ( classSerializable ) {
		hStream		<< "#include \"core/serialization.h\"" << endl
					<< endl
					<< "#include \"base/serializable.h\"" << endl;
	}
}

// writeHUsingDeclarations
void CPPGenerator::writeHUsingDeclarations( ostream& hStream, bool classSerializable ) {
	hStream << "using namespace roller;" << endl;
	if ( classSerializable ) {
		hStream << "using namespace cr;" << endl;
	}
	hStream << "using std::string;" << endl;
}

// writeHashDef
void CPPGenerator::writeHashDef( ostream& stream, const Class& c ) {
	stream	<< "#define __CR_HASH_" << c._name << " 0x00000000";
	_hashPosition1 = (i64)stream.tellp() - 8;
	stream	<< endl;
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
	_hashPosition2 = (i64)stream.tellp() - 8;

	// finish writing header

	stream	<< endl
			<< " */" << endl;
}

// writeFieldAccessors
void CPPGenerator::writeFieldAccessors( ostream& hStream, ostream& cppStream, const Class& c, const Field& f ) {

	string capitalized;
	capitalized += capitalize( f._name[0] );
	capitalized += (f._name.c_str() + 1);

	string typeName = getDataTypeName( f._dataType );

	hStream << "\t" << typeName << " get" << capitalized << "() const;" << endl;
	hStream << "\tvoid" << " set" << capitalized << "( " << typeName << " value );" << endl;

	// getter
	cppStream << typeName << " " << c._name << "::" << "get" << capitalized << "() const {" << endl;
	cppStream << "\treturn _" << f._name << ";" << endl;
	cppStream << "}" << endl;
	cppStream << endl;

	// setter
	cppStream << "void " << c._name << "::" << "set" << capitalized << "( " << typeName << " value ) {" << endl;
	cppStream << "\t_" << f._name << " = value;" << endl;
	cppStream << "}" << endl;
	cppStream << endl;

}

// writeField
void CPPGenerator::writeField( ostream& stream, const Field& f ) {
	stream << "\t" << getDataTypeName( f._dataType ) << " _" << f._name << ";" << endl;
}

// writeEndIfdefs
void CPPGenerator::writeEndIfdefs( ostream& stream, const Class& c ) {
	stream	<< "#endif // __CR_GEN_" << c._name << "_H_" << endl;

}

// substituteHash
void CPPGenerator::substituteHash( i32 hash, ostream& stream, i64 streamPos ) {

	stream.seekp( streamPos, std::ios_base::beg );

	// print in hex
	stream << std::hex << hash;
	stream << std::dec;

	// seek back to end
	stream.seekp( 0, std::ios_base::end );

}

// writeSerializationDeclarations
void CPPGenerator::writeSerializationDeclarations( ostream& hStream ) {
	hStream << "\tvirtual i64 serialize( void* buffer ) const;" << endl
			<< "\tvirtual i64 getSerializedSize() const;" << endl
			<< "\tvirtual void internalize( void* buffer );" << endl
			<< "\tvirtual i32 getClassHash() const;" << endl;
}

// writeSerializationDeclarations
void CPPGenerator::writeSerializationDefinitions( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	writeSerialize( cppStream, c, publicFields, protectedFields, privateFields );
	cppStream << endl;

	writeGetSerializedSize( cppStream, c, publicFields, protectedFields, privateFields );
	cppStream << endl;

	writeInternalize( cppStream, c, publicFields, protectedFields, privateFields );
	cppStream << endl;

	writeGetClassHash( cppStream, c );
	cppStream << endl;
}

// writeSerialize
void CPPGenerator::writeSerialize( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	cppStream << "i64 " << c._name << "::serialize( void* buffer ) const {" << endl;
	cppStream << "\ti64 written = 0;" << endl;

	for ( Field f : publicFields ) {
		writeSerializeField( cppStream, f );
	}
	for ( Field f : protectedFields ) {
		writeSerializeField( cppStream, f );
	}
	for ( Field f : privateFields ) {
		writeSerializeField( cppStream, f );
	}

	cppStream	<< "\treturn written;" << endl
				<< "}" << endl;
}

// writeSerializeField
void CPPGenerator::writeSerializeField( ostream& cppStream, const Field& f ) {
	cppStream << "\twritten += Serialization::write( ((char*)buffer + written), _" << f._name << " );" << endl;
}

// writeGetSerializedSize
void CPPGenerator::writeGetSerializedSize( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	cppStream	<< "i64 " << c._name << "::getSerializedSize() const {" << endl
				<< "\ti64 size = 0;" << endl;

	for ( Field f : publicFields ) {
		writeGetSerializedSizeField( cppStream, f );
	}
	for ( Field f : protectedFields ) {
		writeGetSerializedSizeField( cppStream, f );
	}
	for ( Field f : privateFields ) {
		writeGetSerializedSizeField( cppStream, f );
	}

	cppStream	<< "\treturn size;" << endl
				<< "}" << endl;
}

// writeGetSerializedSizeField
void CPPGenerator::writeGetSerializedSizeField( ostream& cppStream, const Field& f ) {
	if ( f._dataType == DataType::STRING ) {
		cppStream << "\tsize += sizeof( i64 ) + _" << f._name << ".size(); // _" << f._name << endl;
	} else {
		cppStream << "\tsize += sizeof( " << getDataTypeName( f._dataType ) << " ); // _" << f._name << endl;
	}
}

// writeInternalize
void CPPGenerator::writeInternalize( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	cppStream << "void " << c._name << "::internalize( void* buffer ) {" << endl;
	cppStream << "\ti64 read = 0;" << endl;

	for ( Field f : publicFields ) {
		writeInternalizeField( cppStream, f );
	}
	for ( Field f : protectedFields ) {
		writeInternalizeField( cppStream, f );
	}
	for ( Field f : privateFields ) {
		writeInternalizeField( cppStream, f );
	}

	cppStream << "}" << endl;
}

// writeInternalizeField
void CPPGenerator::writeInternalizeField( ostream& cppStream, const Field& f ) {
	if ( f._dataType == DataType::STRING ) {
		cppStream << "\tread += Serialization::read( ((char*)buffer + read), _" << f._name << " );" << endl;
	} else {
		cppStream << "\tread += Serialization::read( ((char*)buffer + read), &_" << f._name << " );" << endl;
	}
}

// writeGetClassHash
void CPPGenerator::writeGetClassHash( ostream& cppStream, const Class& c ) {

	cppStream	<< "i32 " << c._name << "::getClassHash() const {" << endl
				<< "\treturn __CR_HASH_" << c._name << ";" << endl
				<< "}" << endl;
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
