#ifndef __CR_CPP_H_
#define __CR_CPP_H_

#include <memory>

#include "languages/generator.h"
#include "cr_constants.h"
#include "meta.h"

using std::shared_ptr;

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
};

};

#endif
