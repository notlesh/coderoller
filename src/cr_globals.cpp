#include "cr_globals.h"

#include <memory>

#include "cr_constants.h"
#include "languages/cpp.h"

using std::shared_ptr;
using std::dynamic_pointer_cast;

namespace cr {

File g_inputFile;
File g_outputDir;

map<Language, shared_ptr<CodeGenerator>> g_codeGenerators;

// static code block
struct __StaticCodeBlock {
	__StaticCodeBlock() {
		shared_ptr<CPPGenerator> cppGenerator = shared_ptr<CPPGenerator>( new CPPGenerator() );
		g_codeGenerators[Language::CPP] = dynamic_pointer_cast<CodeGenerator>( cppGenerator );
	}
};
__StaticCodeBlock dummy;

};
