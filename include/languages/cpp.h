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

	i64 _hashPosition; // TODO: this should be done per class (per file...)

	/** 
	 * Write a field
	 */
	void writeMainHeader( ostream& stream );

	/** 
	 * Write a field
	 */
	void writeField( ostream& stream, const Field& f );
};

/**
 * Resolve the desired access privacy for the given field
 */
AccessPrivacy resolvePrivacy( const Package& p, const Class& c, const Field& f );

};

#endif
