#include "generate.h"

#include "cr_globals.h"
#include "languages/generator.h"

namespace cr {

// generateAll
void generateAll( const GenConfig& config ) {

	for ( Language language : config._languages ) {
		shared_ptr<CodeGenerator> generator = g_codeGenerators[language];
		if ( ! generator ) {
			throw RollerException( "No known generator for language" );
		}

		generator->generate( config );
	}

};

}
