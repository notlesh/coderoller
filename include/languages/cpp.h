#ifndef __CR_CPP_H_
#define __CR_CPP_H_

#include <memory>
#include <iostream>

#include "languages/generator.h"
#include "cr_constants.h"
#include "meta.h"

using std::shared_ptr;
using std::ostream;

namespace cr {

/**
 * A CPP generator
 */
class CPPGenerator : public CodeGenerator {

public:

	/** 
	 * Constructor
	 */
	CPPGenerator();

	/**
	 * Virtual destructor
	 */
	virtual ~CPPGenerator();

	/**
	 * Generate code
	 */
	virtual void generate( shared_ptr<GenConfig> config );

	/**
	 * Return the C++ data type string for a given DataType
	 */
	static const char* getDataTypeName( DataType type );

	/**
	 * Return the C++ access specifier for the given AccessPrivacy
	 */
	static const char* getPrivacyName( AccessPrivacy privacy );

	/**
	 * Return the C++ access specifier for the given AccessPrivacy
	 */
	static AccessPrivacy resolvePrivacy( const Package& p, const Class& c, const Field& f );

private:

	i64 _hashPosition1; // TODO: this should be done per class (per file...)
	i64 _hashPosition2; // TODO: this should be done per class (per file...)

	/** 
	 * Write a field
	 */
	void writeStartIfdefs( ostream& stream, const Class& c );

	/**
	 * writeCPPHeaderInclude
	 */
	void writeCPPHeaderInclude( ostream& cppStream, const char* headerFilename );

	/**
	 * Write CPP using declarations
	 */
	void writeCPPUsingDeclarations( ostream& cppStream, bool classSerializable );

	/**
	 * Write H Header includes
	 */
	void writeHHeaderInclude( ostream& hStream, bool classSerializable );

	/**
	 * Write H using declarations
	 */
	void writeHUsingDeclarations( ostream& hStream, bool classSerializable );

	/** 
	 * Write the hash def
	 */
	void writeHashDef( ostream& stream, const Class& c );

	/** 
	 * Write the main header
	 */
	void writeMainHeader( ostream& stream );

	/** 
	 * Write a field
	 */
	void writeField( ostream& stream, const Field& f );

	/** 
	 * Write a field accessors
	 */
	void writeFieldAccessors( ostream& hStream, ostream& cppStream, const Class& c, const Field& f );

	/**
	 * Write end ifdefs
	 */
	void writeEndIfdefs( ostream& stream, const Class& c );

	/**
	 * Substitute the hash code at the given position
	 */
	void substituteHash( i32 hash, ostream& stream, i64 streamPos );

	/**
	 * Write the serialization function declarations
	 */
	void writeSerializationDeclarations( ostream& hStream );

	/**
	 * Write the serialization function definitions
	 */
	void writeSerializationDefinitions( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );

	void writeSerialize( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );
	void writeSerializeField( ostream& cppStream, const Field& f );

	void writeGetSerializedSize( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );
	void writeGetSerializedSizeField( ostream& cppStream, const Field& f );

	void writeInternalize( ostream& cppStream, const Class& c, list<Field>& publicFields, list<Field>& protectedFields, list<Field>& privateFields );
	void writeInternalizeField( ostream& cppStream, const Field& f );

	void writeGetClassHash( ostream& cppStream, const Class& c );
};

/**
 * Resolve the desired access privacy for the given field
 */
AccessPrivacy resolvePrivacy( const Package& p, const Class& c, const Field& f );

/**
 * Resolve the desired serializable for the given class
 */
bool resolveClassSerializable( const Package& p, const Class& c );

/**
 * Resolve the desired serializable for the given field
 */
bool resolveFieldSerializable( const Package& p, const Class& c, const Field& f );

};

#endif
