#ifndef __CR_CPP_CLASS_GENERATOR_H_
#define __CR_CPP_CLASS_GENERATOR_H_

#include <memory>
#include <iostream>
#include <sstream>

#include "languages/generator.h"
#include "cr_constants.h"
#include "meta.h"

using std::shared_ptr;
using std::ostringstream;

namespace cr {

/**
 * A CPP generator
 */
class CPPClassGenerator {

public:

	/** 
	 * Constructor
	 */
	CPPClassGenerator();

	/**
	 * Virtual destructor
	 */
	virtual ~CPPClassGenerator();

	/**
	 * Generate code
	 */
	virtual void generate( 
			shared_ptr<GenConfig> config, 
			const Class& c, 
			const string& includeRoot,
			const string& hFilePath, 
			const string& sourceRoot,
			const string& cppFilePath );

private:

	ostringstream _hStream;
	ostringstream _cppStream;
	i64 _hashPosition1;
	i64 _hashPosition2;

	/** 
	 * Write a field
	 */
	void writeStartIfdefs( const Class& c );

	/**
	 * writeCPPHeaderInclude
	 */
	void writeCPPHeaderInclude( const char* headerFilename );

	/**
	 * Write CPP using declarations
	 */
	void writeCPPUsingDeclarations( bool classSerializable );

	/**
	 * Write H Header includes
	 */
	void writeHHeaderInclude( bool classSerializable );

	/**
	 * Write H using declarations
	 */
	void writeHUsingDeclarations( bool classSerializable );

	/** 
	 * Write the hash def
	 */
	void writeHashDef( const Class& c );

	/** 
	 * Write the main header
	 */
	void writeMainHeader();

	/** 
	 * Write a field
	 */
	void writeField( const Field& f );

	/** 
	 * Write a field accessors
	 */
	void writeFieldAccessors( const Class& c, const Field& f );

	/**
	 * Write end ifdefs
	 */
	void writeEndIfdefs( const Class& c );

	/**
	 * Substitute the hash code at the given position
	 */
	void substituteHash( i32 hash, i64 streamPos );

	/**
	 * Write the serialization function declarations
	 */
	void writeSerializationDeclarations();

	/**
	 * Write the serialization function definitions
	 */
	void writeSerializationDefinitions( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );

	void writeSerialize( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );
	void writeSerializeField( const Field& f );

	void writeGetSerializedSize( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );
	void writeGetSerializedSizeField( const Field& f );

	void writeInternalize( const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );
	void writeInternalizeField( const Field& f );

	void writeGetClassHash( const Class& c );
};

};

#endif
