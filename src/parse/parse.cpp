#include "parse/parse.h"

#include <vector>

#include "tinyxml2.h"

#include "core/log.h"
#include "cr_globals.h"
#include "cr_util.h"
#include "languages/generator.h"

using namespace tinyxml2;

#define ROOT_NODE_NAME "CodeRoller"
#define CLASS_NODE_NAME "class"
#define FIELD_NODE_NAME "field"

#define NAME_ATTRIBUTE_NAME "name"
#define TYPE_ATTRIBUTE_NAME "type"
#define LANGUAGES_ATTRIBUTE_NAME "languages"
#define DEFAULT_PRIVACY_ATTRIBUTE_NAME "defaultMemberPrivacy"
#define PRIVACY_ATTRIBUTE_NAME "memberPrivacy"
#define DEFAULT_SERIALIZABLE_ATTRIBUTE_NAME "defaultSerializable"
#define SERIALIZABLE_ATTRIBUTE_NAME "serializable"

using std::vector;

namespace cr {

// parseXML
shared_ptr<GenConfig> parseXML( const char* filename ) {

	Log::i( "Parsing coderoller xml file %s", filename );

	shared_ptr<GenConfig> config( new GenConfig() );

	File configFile( filename );
	if ( ! configFile.exists() ) {
		throw CRException( "Config file %s does not exist", filename );
	}

	config->_mTime = configFile.getMTime();

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

	// defaultMemberPrivacy
	const char* defaultMemberPrivacyStr = root->Attribute( DEFAULT_PRIVACY_ATTRIBUTE_NAME );
	config->_package._defaultMemberPrivacy = toAccessPrivacy( defaultMemberPrivacyStr );

	// defaultSerializable
	const char* defaultSerializableStr = root->Attribute( DEFAULT_SERIALIZABLE_ATTRIBUTE_NAME );
	config->_package._defaultSerializable = cr::toSerializableSpecification( defaultSerializableStr );

	// parse class nodes
	XMLElement* classElement = root->FirstChildElement( CLASS_NODE_NAME );
	while ( classElement ) {
		config->_package._classes.push_back( parseClass( classElement ));
		classElement = classElement->NextSiblingElement( CLASS_NODE_NAME );
	}

	return config;
}

// parseClass
Class parseClass( XMLElement* classElement ) {

	Class c;

	const char* className = classElement->Attribute( NAME_ATTRIBUTE_NAME );
	if ( ! className ) {
		throw MissingAttributeException( "Class declaration does not contain a name" );
	}
	c._name = className;

	// defaultMemberPrivacy
	const char* defaultMemberPrivacyStr = classElement->Attribute( DEFAULT_PRIVACY_ATTRIBUTE_NAME );
	c._defaultMemberPrivacy = toAccessPrivacy( defaultMemberPrivacyStr );

	// parse field nodes
	XMLElement* fieldElement = classElement->FirstChildElement( FIELD_NODE_NAME );
	while ( fieldElement ) {

		c._fields.push_back( parseField( fieldElement, c ));
		fieldElement = fieldElement->NextSiblingElement( FIELD_NODE_NAME );

	}

	return c;
}

// parseField
Field parseField( XMLElement* fieldElement, const Class& c ) {

	Field f;
	
	const char* fieldName = fieldElement->Attribute( NAME_ATTRIBUTE_NAME );
	if ( ! fieldName ) {
		throw MissingAttributeException( "Field declaration in class %s does not contain a name", fieldName );
	}
	f._name = fieldName;

	const char* fieldType = fieldElement->Attribute( TYPE_ATTRIBUTE_NAME );
	if ( ! fieldType ) {
		throw MissingAttributeException( "Field declaration in class %s does not contain a type", fieldType );
	}
	try {
		f._dataType = toDataType( fieldType );

		if ( f._dataType == DataType::SERIALIZABLE ) {
			i32 length = strlen( fieldType );

			// also grab type name
			if ( length < 14 ) {
				throw MissingAttributeException( "Invalid serializable type name" );
			} else if ( length > (1024 + 13 - 1) ) {
				throw MissingAttributeException( "serializable type name is too long" );
			}

			Log::i( "Processing serializable: %s", fieldType );

			char buffer[1024];
			strncpy( buffer, (fieldType + 13), (length - 13) );
			buffer[ (length - 13) ] = 0x00;

			f._typeName = buffer;
		}
	} catch ( ... ) {
		Log::w( "Exception caught while parsing class %s. Rethrowing.", c._name.c_str() );
		throw;
	}

	// memberPrivacy
	const char* memberPrivacyStr = fieldElement->Attribute( PRIVACY_ATTRIBUTE_NAME );
	f._memberPrivacy = toAccessPrivacy( memberPrivacyStr );

	return f;
}

}
