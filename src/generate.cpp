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

namespace cr {

// parseXML
shared_ptr<GenConfig> parseXML( const char* filename ) {

	Log::i( "Parsing coderoller xml file %s", filename );

	shared_ptr<GenConfig> config( new GenConfig() );

	XMLDocument doc;
	doc.LoadFile( filename );

	XMLElement* root = doc.RootElement();

	// validate root node
	if (( strcmp( root->Name(), ROOT_NODE_NAME )) != 0 ) {
		throw InvalidNodeNameException( "Root node should be named %s", ROOT_NODE_NAME );
	}

	// get languages
	const char* languagesStr = root->Attribute( LANGUAGES_ATTRIBUTE_NAME );
	if ( ! languagesStr ) {
		throw MissingAttributeException( "Missing languages specification" );
	}

	// vector<string> languages = split( languagesStr, ",", true ); // there appears to be a bug in split()
	// TODO: bad hack
	vector<string> languages;
	languages.push_back( languagesStr );

	if ( languages.size() == 0 ) {
		throw MissingAttributeException( "No valid languages specified" );
	}

	for ( string& languageStr : languages ) {
		try {
			Log::i( "Adding language %s", languageStr.c_str() );
			config->_languages.push_back( toLanguage( languageStr.c_str() ));
		} catch ( ... ) {
			Log::w( "Exception caught while parsing languages specification. Rethrowing." );
		}
	}

	// parse class nodes
	XMLElement* classNode = root->FirstChildElement( CLASS_NODE_NAME );
	while ( classNode ) {
		Class c;

		const char* className = classNode->Attribute( NAME_ATTRIBUTE_NAME );
		if ( ! className ) {
			throw MissingAttributeException( "Class declaration does not contain a name" );
		}
		c._name = className;

		// parse field nodes
		XMLElement* fieldNode = classNode->FirstChildElement( FIELD_NODE_NAME );
		while ( fieldNode ) {

			Field f;
			
			const char* fieldName = fieldNode->Attribute( NAME_ATTRIBUTE_NAME );
			if ( ! className ) {
				throw MissingAttributeException( "Field declaration in class %s does not contain a name", className );
			}
			f._name = fieldName;

			const char* fieldType = fieldNode->Attribute( TYPE_ATTRIBUTE_NAME );
			if ( ! className ) {
				throw MissingAttributeException( "Field declaration in class %s does not contain a type", className );
			}
			try {
				f._dataType = toDataType( fieldType );
			} catch ( ... ) {
				Log::w( "Exception caught while parsing class %s. Rethrowing.", className );
				throw;
			}

			Log::i( "Adding field %s", f._name.c_str() );
			c._fields.push_back( f );

			fieldNode = fieldNode->NextSiblingElement( FIELD_NODE_NAME );

		}

		config->_package._classes.push_back( c );

		classNode = classNode->NextSiblingElement( CLASS_NODE_NAME );
	}

	return config;
}

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
