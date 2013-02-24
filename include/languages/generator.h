#ifndef __CR_GENERATOR_H_
#define __CR_GENERATOR_H_

#include <memory>

#include "cr_constants.h"
#include "meta.h"

using std::shared_ptr;

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
	virtual void generate( shared_ptr<GenConfig> config ) = 0;
};

};

#endif
