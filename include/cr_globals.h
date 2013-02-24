#ifndef __CR_CR_GLOBALS_H_
#define __CR_CR_GLOBALS_H_

#include <memory>
#include <map>

#include "languages/generator.h"

using std::map;
using std::shared_ptr;

namespace cr {

/**
 * Store generators for each language
 */
extern map<Language, shared_ptr<CodeGenerator>> g_codeGenerators;

};

#endif
