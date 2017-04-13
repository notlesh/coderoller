#include "generate.h"

#include "tinyxml2.h"

#include <roller/core/log.h>
#include <roller/core/util.h>

#include "cr_globals.h"
#include "cr_util.h"
#include "languages/generator.h"

using namespace tinyxml2;
using namespace roller;

namespace cr {

// generateAll
void generateAll( shared_ptr<GenConfig> config ) {

	for ( Language language : config->_languages ) {
		shared_ptr<CodeGenerator> generator = g_codeGenerators[language];
		if ( ! generator ) {
			throw RollerException( "No known generator for language" );
		}

		generator->generate( config );
	}

};

}
