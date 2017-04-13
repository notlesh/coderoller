#ifndef __CR_GENERATE_H_
#define __CR_GENERATE_H_

#include <memory>

#include <roller/core/exception.h>

#include "meta.h"

using namespace roller;
using std::shared_ptr;

namespace cr {

/**
 * Generate code.
 */
void generateAll( shared_ptr<GenConfig> config );

}

#endif
