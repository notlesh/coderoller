#ifndef __CR_META_H_
#define __CR_META_H_

#include <string>
#include <list>

#include "core/types.h"
#include "core/file.h"

#include "cr_constants.h"

using namespace roller;
using std::string;
using std::list;

namespace cr {

/**
 * Struct representing a field
 */
struct Field {

	string _name;
	AccessPrivacy _memberPrivacy;
	DataType _dataType;
	string _typeName; // for object types, indicates the objects type name
	string _defaultValue;
	SerializableSpecification _serializable;

};

/**
 * Struct representing a class.
 */
struct Class {

	string _name;
	AccessPrivacy _defaultMemberPrivacy;
	list<Field> _fields;
	SerializableSpecification _serializable;

};

/**
 * A package contains a list of classes to be generated.
 */
struct Package {
	AccessPrivacy _defaultMemberPrivacy;
	list<Class> _classes;
	SerializableSpecification _defaultSerializable;
};

/**
 * GenConfig contains the info needed to generate a package.
 */
struct GenConfig {

	Package _package;
	list<Language> _languages;
	File _outputDir;
	ui64 _mTime;
};

};

#endif
