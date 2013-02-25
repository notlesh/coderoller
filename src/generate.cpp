#include "generate.h"

#include "tinyxml2.h"

#include "core/log.h"
#include "core/util.h"

#include "cr_globals.h"
#include "cr_util.h"
#include "languages/generator.h"

using namespace tinyxml2;
using namespace roller;

#define ROOT_NODE_NAME "CodeRoller"
#define CLASS_NODE_NAME "class"
#define FIELD_NODE_NAME "field"

#define NAME_ATTRIBUTE_NAME "name"
#define TYPE_ATTRIBUTE_NAME "type"
#define LANGUAGES_ATTRIBUTE_NAME "languages"
#define DEFAULT_PRIVACY_ATTRIBUTE_NAME "defaultMemberPrivacy"
#define PRIVACY_ATTRIBUTE_NAME "memberPrivacy"

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
