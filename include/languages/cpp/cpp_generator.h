#ifndef __CR_CPP_GENERATORH_
#define __CR_CPP_GENERATORH_

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

};

};

#endif
