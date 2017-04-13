#include "generate.h"

#include "tinyxml2.h"

#include "core/log.h"

#include "cr_globals.h"
#include "cr_util.h"
#include "languages/generator.h"

using namespace tinyxml2;

namespace cr {

// generateAll
void generateAll( shared_ptr<GenConfig> config ) {

	for ( Language language : config->_languages ) {
		shared_ptr<CodeGenerator> generator = g_codeGenerators[language];
		if ( ! generator ) {
			throw CRException( "No known generator for language" );
		}

		generator->generate( config );
	}

};

}
