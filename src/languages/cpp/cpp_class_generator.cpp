#include "languages/cpp/cpp_class_generator.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <ios>

#include "core/aver.h"
#include "core/util.h"

#include "languages/cpp/cpp_util.h"
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
CPPClassGenerator::CPPClassGenerator() :
				_hashPosition1(-1),
				_hashPosition2(-1) {
}

// Destructor
CPPClassGenerator::~CPPClassGenerator() {
}

// generate
void CPPClassGenerator::generate(
			shared_ptr<GenConfig> config, 
			const Class& c, 
			const string& includeRoot,
			const string& hFilePath, 
			const string& sourceRoot,
			const string& cppFilePath ) {

	File hFile( config->_outputDir, includeRoot + hFilePath );
	File cppFile( config->_outputDir, sourceRoot + cppFilePath );

	bool hFileDirty = (! hFile.exists() || (hFile.getMTime() < config->_mTime));
	bool cppFileDirty = (! cppFile.exists() || (cppFile.getMTime() < config->_mTime));

	if ( ! hFileDirty && ! cppFileDirty ) {
		Log::f( "class files for class %s is up to date.", c._name.c_str() );
	}

	// TODO: check mtime on file and skip all of this if possible

	bool classSerializable = resolveClassSerializable( config->_package, c );

	// make sure we can create files (and their parents)
	File hOutputFileParent = hFile.getDir();
	if ( ! hOutputFileParent.exists() ) {
		hOutputFileParent.mkdir( true );
	}

	File cppOutputFileParent = cppFile.getDir();
	if ( ! cppOutputFileParent.exists() ) {
		cppOutputFileParent.mkdir( true );
	}

	writeMainHeader();
	_hStream << endl;

	writeStartIfdefs( c );
	_hStream << endl;

	writeHHeaderInclude( classSerializable );
	_hStream << endl;

	writeHUsingDeclarations( classSerializable );
	_hStream << endl;

	writeHashDef( c );
	_hStream << endl;

	// print out "class Foo {"
	if ( classSerializable ) {
		_hStream << "class " << c._name << " : public Serializable {" << endl;
		_hStream << endl;

	} else {
		_hStream << "class " << c._name << " {" << endl;
		_hStream << endl;
	}

	// write header to _cppStream
	writeCPPHeaderInclude( hFilePath.data() );
	_cppStream << endl;

	writeCPPUsingDeclarations( classSerializable );
	_cppStream << endl;

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

	_hStream << "public:" << endl;
	_hStream << endl;

	if ( publicFields.size() > 0 ) {

		if ( publicFields.size() > 0 ) {

			for ( Field f : publicFields ) {
				writeField( f );
			}

			_hStream << endl;

			for ( Field f : publicFields ) {
				writeFieldAccessors( c, f );
				_hStream << endl;
			}
		}

		// while we're in public, write out all accessors
		if ( protectedFields.size() > 0 ) {
			for ( Field f : protectedFields ) {
				writeFieldAccessors( c, f );
				_hStream << endl;
			}
		}
		if ( privateFields.size() > 0 ) {
			for ( Field f : privateFields ) {
				writeFieldAccessors( c, f );
				_hStream << endl;
			}
		}

		// write out serialization declarations
		if ( classSerializable ) {
			writeSerializationDeclarations();
			_hStream << endl;
		}
	}

	// add other fields by access type
	if ( protectedFields.size() > 0 ) {
		
		_hStream << "protected:" << endl;
		_hStream << endl;

		for ( Field f : protectedFields ) {
			writeField( f );
		}

		_hStream << endl;
	}

	if ( privateFields.size() > 0 ) {
		
		_hStream << "private:" << endl;
		_hStream << endl;

		for ( Field f : privateFields ) {
			writeField( f );
		}

		_hStream << endl;
	}

	// write serialization function definitions
	if ( classSerializable ) {
		writeSerializationDefinitions( c, publicFields, protectedFields, privateFields );
	}

	// close class
	_hStream << "};" << endl;
	_hStream << endl;

	writeEndIfdefs( c );

	// take hash of the entire buffer, then go back and fill in hash
	// TODO: this involves at least one big copy...
	// NOTE: this needs to be updated if the "class Foo..." and the hash move with respect to each other (etc)
	i32 hashCode = hash( _hStream.str().c_str() + _hashPosition2 + sizeof( "00000000\n */") );
	// Log::i( "class text starts with: %s", _hStream.str().c_str() + _hashPosition + sizeof( "00000000\n */") );

	substituteHash( hashCode, _hashPosition1 );
	substituteHash( hashCode, _hashPosition2 );

	ofstream hOutFileStream( hFile.getFullPath() );
	hOutFileStream << _hStream.str();
	hOutFileStream.close();

	ofstream cppOutFileStream( cppFile.getFullPath() );
	cppOutFileStream << _cppStream.str();
	cppOutFileStream.close();
}

// writeStartIfdefs
void CPPClassGenerator::writeStartIfdefs( const Class& c ) {
	_hStream	<< "#ifndef __CR_GEN_" << c._name << "_H_" << endl
				<< "#define __CR_GEN_" << c._name << "_H_" << endl;
}

// writeCPPHeaderInclude
void CPPClassGenerator::writeCPPHeaderInclude( const char* headerFilename ) {
	_cppStream << "#include \"" << headerFilename << "\"" << endl;
}

// writeCPPUsingDeclarations
void CPPClassGenerator::writeCPPUsingDeclarations( bool classSerializable ) {
	_cppStream << "using namespace roller;" << endl;
	if ( classSerializable ) {
		_cppStream << "using namespace cr;" << endl;
	}
	_cppStream << "using std::string;" << endl;
}

// writeHHeaderInclude
void CPPClassGenerator::writeHHeaderInclude( bool classSerializable ) {
	_hStream		<< "#include <string>" << endl
				<< endl
				<< "#include \"core/types.h\"" << endl;
	if ( classSerializable ) {
		_hStream		<< "#include \"core/serialization.h\"" << endl
					<< endl
					<< "#include \"base/serializable.h\"" << endl;
	}
}

// writeHUsingDeclarations
void CPPClassGenerator::writeHUsingDeclarations( bool classSerializable ) {
	_hStream << "using namespace roller;" << endl;
	if ( classSerializable ) {
		_hStream << "using namespace cr;" << endl;
	}
	_hStream << "using std::string;" << endl;
}

// writeHashDef
void CPPClassGenerator::writeHashDef( const Class& c ) {
	_hStream	<< "#define __CR_HASH_" << c._name << " 0x00000000";
	_hashPosition1 = (i64)_hStream.tellp() - 8;
	_hStream	<< endl;
}

// writeMainHeader
void CPPClassGenerator::writeMainHeader() {
	_hStream	<< "/**" << endl
			<< " * CodeRoller generated class. This class is automatically generated; do not modify it" << endl
			<< " * because it will be overwritten." << endl
			<< " *" << endl
			<< " * CodeRoller git hash: " << GIT_HASH << endl
			<< " * CodeRoller git branch: " << GIT_BRANCH << endl
			<< " *" << endl
			<< " * Generated on " << getTime() << endl
			<< " * Class hash: 00000000";
	
	// grab position here; we will return to it to overwrite class hash once we can compute it.
	_hashPosition2 = (i64)_hStream.tellp() - 8;

	// finish writing header

	_hStream	<< endl
			<< " */" << endl;
}

// writeFieldAccessors
void CPPClassGenerator::writeFieldAccessors( const Class& c, const Field& f ) {

	string capitalized;
	capitalized += capitalize( f._name[0] );
	capitalized += (f._name.c_str() + 1);

	string typeName = getDataTypeName( f._dataType );

	_hStream << "\t" << typeName << " get" << capitalized << "() const;" << endl;
	_hStream << "\tvoid" << " set" << capitalized << "( " << typeName << " value );" << endl;

	// getter
	_cppStream << typeName << " " << c._name << "::" << "get" << capitalized << "() const {" << endl;
	_cppStream << "\treturn _" << f._name << ";" << endl;
	_cppStream << "}" << endl;
	_cppStream << endl;

	// setter
	_cppStream << "void " << c._name << "::" << "set" << capitalized << "( " << typeName << " value ) {" << endl;
	_cppStream << "\t_" << f._name << " = value;" << endl;
	_cppStream << "}" << endl;
	_cppStream << endl;

}

// writeField
void CPPClassGenerator::writeField( const Field& f ) {
	_hStream << "\t" << getDataTypeName( f._dataType ) << " _" << f._name << ";" << endl;
}

// writeEndIfdefs
void CPPClassGenerator::writeEndIfdefs( const Class& c ) {
	_hStream	<< "#endif // __CR_GEN_" << c._name << "_H_" << endl;

}

// substituteHash
void CPPClassGenerator::substituteHash( i32 hash, i64 streamPos ) {

	_hStream.seekp( streamPos, std::ios_base::beg );

	// print in hex
	_hStream << std::hex << hash;
	_hStream << std::dec;

	// seek back to end
	_hStream.seekp( 0, std::ios_base::end );

}

// writeSerializationDeclarations
void CPPClassGenerator::writeSerializationDeclarations() {
	_hStream << "\tvirtual i64 serialize( void* buffer ) const;" << endl
			<< "\tvirtual i64 getSerializedSize() const;" << endl
			<< "\tvirtual void internalize( void* buffer );" << endl
			<< "\tvirtual i32 getClassHash() const;" << endl;
}

// writeSerializationDeclarations
void CPPClassGenerator::writeSerializationDefinitions( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	writeSerialize( c, publicFields, protectedFields, privateFields );
	_cppStream << endl;

	writeGetSerializedSize( c, publicFields, protectedFields, privateFields );
	_cppStream << endl;

	writeInternalize( c, publicFields, protectedFields, privateFields );
	_cppStream << endl;

	writeGetClassHash( c );
	_cppStream << endl;
}

// writeSerialize
void CPPClassGenerator::writeSerialize( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	_cppStream << "i64 " << c._name << "::serialize( void* buffer ) const {" << endl;
	_cppStream << "\ti64 written = 0;" << endl;

	for ( Field f : publicFields ) {
		writeSerializeField( f );
	}
	for ( Field f : protectedFields ) {
		writeSerializeField( f );
	}
	for ( Field f : privateFields ) {
		writeSerializeField( f );
	}

	_cppStream	<< "\treturn written;" << endl
				<< "}" << endl;
}

// writeSerializeField
void CPPClassGenerator::writeSerializeField( const Field& f ) {
	_cppStream << "\twritten += Serialization::write( ((char*)buffer + written), _" << f._name << " );" << endl;
}

// writeGetSerializedSize
void CPPClassGenerator::writeGetSerializedSize( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	_cppStream	<< "i64 " << c._name << "::getSerializedSize() const {" << endl
				<< "\ti64 size = 0;" << endl;

	for ( Field f : publicFields ) {
		writeGetSerializedSizeField( f );
	}
	for ( Field f : protectedFields ) {
		writeGetSerializedSizeField( f );
	}
	for ( Field f : privateFields ) {
		writeGetSerializedSizeField( f );
	}

	_cppStream	<< "\treturn size;" << endl
				<< "}" << endl;
}

// writeGetSerializedSizeField
void CPPClassGenerator::writeGetSerializedSizeField( const Field& f ) {
	if ( f._dataType == DataType::STRING ) {
		_cppStream << "\tsize += sizeof( i64 ) + _" << f._name << ".size(); // _" << f._name << endl;
	} else {
		_cppStream << "\tsize += sizeof( " << getDataTypeName( f._dataType ) << " ); // _" << f._name << endl;
	}
}

// writeInternalize
void CPPClassGenerator::writeInternalize( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields ) {

	_cppStream << "void " << c._name << "::internalize( void* buffer ) {" << endl;
	_cppStream << "\ti64 read = 0;" << endl;

	for ( Field f : publicFields ) {
		writeInternalizeField( f );
	}
	for ( Field f : protectedFields ) {
		writeInternalizeField( f );
	}
	for ( Field f : privateFields ) {
		writeInternalizeField( f );
	}

	_cppStream << "}" << endl;
}

// writeInternalizeField
void CPPClassGenerator::writeInternalizeField( const Field& f ) {
	if ( f._dataType == DataType::STRING ) {
		_cppStream << "\tread += Serialization::read( ((char*)buffer + read), _" << f._name << " );" << endl;
	} else {
		_cppStream << "\tread += Serialization::read( ((char*)buffer + read), &_" << f._name << " );" << endl;
	}
}

// writeGetClassHash
void CPPClassGenerator::writeGetClassHash( const Class& c ) {

	_cppStream	<< "i32 " << c._name << "::getClassHash() const {" << endl
				<< "\treturn __CR_HASH_" << c._name << ";" << endl
				<< "}" << endl;
}

};
