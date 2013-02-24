#ifndef __CR_GENERATOR_H_
#define __CR_GENERATOR_H_

#include "cr_constants.h"
#include "meta.h"

namespace cr {

/**
 * A generator can generate code in the given language.
 */
class CodeGenerator {

public:

	/**
	 * Virtual destructor
	 */
	virtual ~CodeGenerator() {};

	/**
	 * Generate code
	 */
	virtual void generate( const GenConfig& config ) = 0;
};

};

#endif
