#ifndef __CR_CR_GLOBALS_H_
#define __CR_CR_GLOBALS_H_

#include <memory>
#include <map>

#include "core/file.h"

#include "languages/generator.h"

using std::map;
using std::shared_ptr;

namespace cr {

// command line config
extern File g_inputFile;
extern File g_outputDir;

/**
 * Store generators for each language
 */
extern map<Language, shared_ptr<CodeGenerator>> g_codeGenerators;

};

#endif
